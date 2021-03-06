/*
   Copyright 2011-2012 StormMQ Limited

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distribtransition_to_uted under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <errno.h>
#ifndef _MSC_VER
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif

#include "Transport/Connection/ConnectionStateMachine.h"

DECLARE_TRANSITION_FUNCTION(initialized);
DECLARE_TRANSITION_FUNCTION(enabled);
DECLARE_TRANSITION_FUNCTION(reading);
DECLARE_TRANSITION_FUNCTION(failed);
DECLARE_TRANSITION_FUNCTION(eof);
DECLARE_TRANSITION_FUNCTION(eof_alerted);
DECLARE_TRANSITION_FUNCTION(stopped);

static void cleanup_reader(amqp_connection_t *connection);

void amqp_connection_reader_initialize(amqp_connection_t *connection)
{
    connection->state.reader.name = "created";
    transition_to_state(connection, initialized);
}
void amqp_connection_reader_cleanup(amqp_connection_t *connection)
{
    cleanup_reader(connection);
}

int amqp_connection_reader_is_state(const amqp_connection_t *connection, const char *state_name)
{
    if (connection->state.reader.name == 0) return false;

    return amqp_connection_is(connection, AMQP_CONNECTION_SOCKET_CONNECTED) && strcmp(connection->state.reader.name, state_name) == 0;
}

static void cleanup_reader(amqp_connection_t *connection)
{
    if (connection->io.read.watcher)
    {
        amqp_io_event_watcher_stop(connection->io.read.watcher);
        amqp_io_event_watcher_destroy(connection->context, connection->io.read.watcher);
        connection->io.read.watcher = 0;
    }
}

static void call_read_callback(amqp_connection_t *connection)
{
    amqp_connection_read_callback_f read_callback = connection->io.read.read_callback;
    amqp_buffer_t *buffer = connection->io.read.buffer;
    size_t satisfied = connection->io.read.satisfied;

    connection->io.read.read_callback = 0;
    connection->io.read.buffer = 0;
    connection->io.read.satisfied  = 0;

    connection->io.read.last_read_time = ev_now(connection->context->thread_event_loop);

    read_callback(connection, buffer, satisfied);
}
static int is_partial_read(amqp_connection_t *connection)
{
    return connection->io.read.satisfied > 0 && connection->io.read.satisfied < connection->io.read.n_required;
}

static void read_eof(amqp_connection_t *connection)
{
    amqp_connection_failure_flag_set(connection, AMQP_CONNECTION_READ_EOF);

    if (is_partial_read(connection))
    {
        amqp_io_event_watcher_stop(connection->io.read.watcher);
        cleanup_reader(connection);
        transition_to_state(connection, failed);
        amqp_error(connection->context, AMQP_ERROR_PARTIAL_READ,
                "Partial read while reading from %s:%d. Required %lu bytes but only received %lu before EOF.",
                connection->socket.hostname, connection->socket.port_number,
                connection->io.read.n_required, connection->io.read.satisfied
        );
        amqp_connection_failure_flag_set(connection, AMQP_CONNECTION_READ_ERROR);
        connection->state.connection.fail(connection);
        return;
    }

    transition_to_state(connection, eof);
    call_read_callback(connection);
}

static int read_what_is_buffered(amqp_connection_t *connection)
{
    int n = 0;
    struct iovec *io_vec;
    int needed = connection->io.read.n_required - connection->io.read.satisfied;

    assert(needed > 0);

    while (needed > 0)
    {
        int iov_count;
        io_vec = amqp_buffer_write_io_vec(connection->io.read.buffer, needed, &iov_count);
        n = readv(connection->socket.fd, io_vec, iov_count);
        if (n < 0)
        {
            break;
        }
        if (n == 0)
        {
            read_eof(connection);
            return 0;
        }
        amqp_buffer_advance_write_point(connection->io.read.buffer, n);
        needed -= n;
        connection->io.read.satisfied += n;
    }

    if (n == -1)
    {
        switch (errno)
        {
        case EAGAIN:
            amqp_io_event_watcher_start(connection->io.read.watcher);
            break;

        case ECONNRESET:
            // treat as EOF
            read_eof(connection);
            break;

        default:
            connection->state.reader.fail(connection, errno);
            break;
        }
        return 0;
    }

    call_read_callback(connection);
    return 1;
}

static int continue_read_operation(amqp_connection_t *connection)
{
    return read_what_is_buffered(connection);
}

static void queue_read_operation(amqp_connection_t *connection, amqp_buffer_t *buffer, size_t required, amqp_connection_read_callback_f read_callback)
{
    connection->io.read.read_callback = read_callback;
    connection->io.read.buffer = buffer;
    connection->io.read.n_required = required;
    connection->io.read.satisfied = 0;
}

static void read_event_handler(amqp_event_loop_t* loop, ev_io *io, const int revents)
{
    amqp_io_event_watcher_t *watcher = (amqp_io_event_watcher_t *) io;
    amqp_connection_t *connection = watcher->data.connection;

    assert(revents == EV_READ);
    connection->state.reader.continue_read(connection);
}

static void reader_stop(amqp_connection_t *connection)
{
    amqp_io_event_watcher_stop(connection->io.read.watcher);
    cleanup_reader(connection);

    transition_to_state(connection, stopped);
}
static void reader_fail(amqp_connection_t *connection, int error_code)
{
    amqp_io_event_watcher_stop(connection->io.read.watcher);
    cleanup_reader(connection);
    transition_to_state(connection, failed);
    amqp_io_error_code(connection->context, error_code, "Failure while reading from %s:%d", connection->socket.hostname, connection->socket.port_number);

    connection->socket.errno_error_code = error_code;
    amqp_connection_failure_flag_set(connection, AMQP_CONNECTION_READ_ERROR);
    connection->state.connection.fail(connection);
}

static void enable_while_initialized(amqp_connection_t *connection)
{
    assert(connection->io.read.watcher == 0);

    connection->io.read.watcher = amqp_io_event_watcher_initialize(connection->context, connection->context->thread_event_loop, read_event_handler, connection->socket.fd, EV_READ);
    connection->io.read.watcher->data.connection = connection;
    transition_to_state(connection, enabled);
}
static void stop_while_initialized(amqp_connection_t *connection)
{
    transition_to_state(connection, stopped);
}
static void reset_while_initialized(amqp_connection_t *connection)
{
    enable_while_initialized(connection);
}
DEFINE_TRANSITION_TO_STATE(initialized)
{
    connection->state.reader.enable = enable_while_initialized;
    connection->state.reader.stop = stop_while_initialized;
    connection->state.reader.reset = reset_while_initialized;
}

static void commence_read_while_enabled(amqp_connection_t *connection, amqp_buffer_t *buffer, size_t required, amqp_connection_read_callback_f read_callback)
{
    assert(buffer != 0 && read_callback != 0);

    transition_to_state(connection, reading);
    queue_read_operation(connection, buffer, required, read_callback);
    if (continue_read_operation(connection))
    {
        connection->state.reader.delayed_read(connection);
    }
}
static void next_read_while_enabled(amqp_connection_t *connection)
{
//    just gobble spurious event
    amqp_io_event_watcher_stop(connection->io.read.watcher);
}
DEFINE_TRANSITION_TO_STATE(enabled)
{
    connection->state.reader.commence_read = commence_read_while_enabled;
    connection->state.reader.continue_read = next_read_while_enabled;
    connection->state.reader.stop = reader_stop;
    connection->state.reader.fail = reader_fail;
}

static void commence_read_while_reading(amqp_connection_t *connection, amqp_buffer_t *buffer, size_t required, amqp_connection_read_callback_f read_callback)
{
    assert(buffer != 0 && read_callback != 0);
    assert(connection->io.read.read_callback == 0 && connection->io.read.buffer == 0 );

    queue_read_operation(connection, buffer, required, read_callback);
}
static void next_read_while_reading(amqp_connection_t *connection)
{
    amqp_io_event_watcher_stop(connection->io.read.watcher);
    if (continue_read_operation(connection))
    {
        connection->state.reader.delayed_read(connection);
    }
}
static void reset_while_reading(amqp_connection_t *connection)
{
    amqp_io_event_watcher_stop(connection->io.read.watcher);
    transition_to_state(connection, enabled);
}
static inline int is_read_queued(amqp_connection_t *connection)
{
    return connection->io.read.buffer != 0;
}
// TODO - rename ?
static void delayed_read_while_reading(amqp_connection_t *connection)
{
    int read_complete;
    while (is_read_queued(connection))
    {
        transition_to_state(connection, reading);
        read_complete = continue_read_operation(connection);
        if (!read_complete)
        {
            return;
        }
    }
    transition_to_state(connection, enabled);
}
DEFINE_TRANSITION_TO_STATE(reading)
{
    connection->state.reader.commence_read = commence_read_while_reading;
    connection->state.reader.continue_read = next_read_while_reading;
    connection->state.reader.reset = reset_while_reading;
    connection->state.reader.delayed_read = delayed_read_while_reading;
    connection->state.reader.stop = reader_stop;
    connection->state.reader.fail = reader_fail;
}

static void commence_read_while_eof(amqp_connection_t *connection, amqp_buffer_t *buffer, size_t required, amqp_connection_read_callback_f read_callback)
{
    assert(buffer != 0 && read_callback != 0);

    transition_to_state(connection, eof_alerted);
    connection->io.read.satisfied = 0;
    connection->io.read.read_callback = read_callback;
    call_read_callback(connection);
}
DEFINE_TRANSITION_TO_STATE(eof)
{
    connection->state.reader.commence_read = commence_read_while_eof;
}

static void reset_while_eof_alerted(amqp_connection_t *connection)
{
    // Allow one more read for drain operation
    transition_to_state(connection, eof);
}
DEFINE_TRANSITION_TO_STATE(eof_alerted)
{
    // Here to cause a failure if application code ignores the first notification of EOF
    connection->state.reader.reset = reset_while_eof_alerted;
}

DEFINE_TRANSITION_TO_STATE(stopped)
{
}
DEFINE_TRANSITION_TO_STATE(failed)
{
}

/* Default reader states */
static void illegal_read_state(amqp_connection_t *connection, const char *event)
{
    amqp_error(connection->context, AMQP_ERROR_ILLEGAL_STATE,
            "Connection reader does not support \"%s\" while \"%s\", socket is \"%s\" and connection is \"%s\".",
            event, connection->state.reader.name, connection->state.socket.name, connection->state.connection.name);
    amqp_fatal_program_error("Connection reader state error");
}

static void default_read_enable(amqp_connection_t *connection)
{
    illegal_read_state(connection, "Enable");
}

static void default_commence_read(amqp_connection_t *connection, amqp_buffer_t *buffer, size_t required, amqp_connection_read_callback_f done_callback)
{
    illegal_read_state(connection, "Commence read");
}

static void default_read_next(amqp_connection_t *connection)
{
    illegal_read_state(connection, "Continue read");
}

static void default_reset(amqp_connection_t *connection)
{
}

static void default_read_stop(amqp_connection_t *connection)
{
}

static void default_read_fail(amqp_connection_t *connection, int error_code)
{
    illegal_read_state(connection, "Fail");
}

static void default_delayed_read(amqp_connection_t *connection)
{
}

static void default_state_initialization(amqp_connection_t *connection, const char *state_name, void (*action_initializer)(amqp_connection_t *connection) LIBAMQP_TRACE_STATE_ARGS_DECL)
{
    save_old_state(connection->state.reader.name);

    assert(action_initializer);

    connection->state.reader.enable = default_read_enable;
    connection->state.reader.commence_read = default_commence_read;
    connection->state.reader.continue_read = default_read_next;
    connection->state.reader.reset = default_reset;
    connection->state.reader.stop = default_read_stop;
    connection->state.reader.fail = default_read_fail;
    connection->state.reader.delayed_read = default_delayed_read;

    connection->state.reader.name = state_name;

    action_initializer(connection);
    
    trace_state_transition(AMQP_TRACE_CONNECTION_READER, connection->state.reader.name);
}

