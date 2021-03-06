/*
   Copyright 2011-2012 StormMQ Limited

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <TestHarness.h>

#include "Buffer/BufferTestSupport.h"
#include "Context/TestSupport/ContextHolder.h"
#include "Transport/Connection/TestSupport/ConnectionHolder.h"
#include "Transport/Connection/TestSupport/WriteInterceptor.h"
#include "Transport/Connection/TestSupport/ReadInterceptor.h"

#include "Transport/Frame/Frame.h"
#include "TestData/TestFrames.h"
#include "Codec/Type/TypePrint.h"

#include "debug_helper.h"

SUITE(ConnectionAmqp)
{
    class ConnectionAmqpFixture :
            public virtual TestSupport::ClientConnectionHolder,
            public virtual TestSupport::ContextHolder,
            public TestSupport::WriteInterceptor,
            public TestSupport::ReadInterceptor
    {
    public:
        ConnectionAmqpFixture();
        ~ConnectionAmqpFixture();
        static void do_nothing(amqp_connection_t *connection);
    public:
        amqp_frame_t *frame;
    };

    ConnectionAmqpFixture::ConnectionAmqpFixture() : frame(0)
    {
        connection->socket.hostname = "localhost.localdomain";
        connection->state.connection.shutdown = do_nothing;
    }

    ConnectionAmqpFixture::~ConnectionAmqpFixture()
    {
        connection->socket.hostname = 0;
        amqp_frame_cleanup(context, frame);
    }

    void ConnectionAmqpFixture::do_nothing(amqp_connection_t *connection)
    {
    }

    TEST_FIXTURE(ConnectionAmqpFixture, fixture_should_balance_allocations)
    {
        CHECK(connection->state.frame.name != 0);
        CHECK_EQUAL("initialized", connection->state.amqp.name);
    }

    TEST_FIXTURE(ConnectionAmqpFixture, amqp_send_open)
    {
        connection->state.amqp.send_open(connection);

        CHECK_EQUAL("open_sent", connection->state.amqp.name);
//        t::amqp_buffer_dump(context, intercepted_write());

        frame = amqp_decode_amqp_frame(context, intercepted_write());
        ASSERT(frame != 0);
//        amqp_type_dump(context, 1, frame->type);
        CHECK_EQUAL(0x10U, frame->descriptor.id);
    }

//    TEST_FIXTURE(ConnectionAmqpFixture, amqp_send_open_reply)
//    {
//        static test_data::TestData *test_frames[] =
//        {
//            &test_data::broker_open_frame,
//            &test_data::close_confirm_frame,
//            0
//        };
//
//        set_test_data_for_read(test_frames);
//
//        connection->state.amqp.send_open(connection);
//
//        CHECK_EQUAL("open_sent", connection->state.amqp.name);

//        frame = amqp_decode_amqp_frame(context, intercepted_write());
//        ASSERT(frame != 0);
//        amqp_type_dump(context, 1, frame->type);
//        CHECK_EQUAL(0x10U, frame->descriptor.id);
//        CHECK(0);
//    }

//    TEST_FIXTURE(ConnectionAmqpFixture, sasl_server_side_handshake)
//    {
//        static test_data::TestData *test_frames[] =
//        {
//            &test_data::sasl_init_frame,
//            0
//        };
//
//        amqp_connection_accepted_socket_initialize(connection, -1, 0, 0);
//        connection->state.negotiator.send = call_done_callback;
//        set_test_data_for_read(test_frames);
//        connection->state.sasl.tunnel.accept(connection, AMQP_SASL_PREFERRED_VERSION);
//        CHECK_EQUAL("negotiated", connection->state.sasl.name);
//    }
}
