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

#ifndef LIBAMQP_TRANSPORT_ACCEPT_HANDLER_H
#define LIBAMQP_TRANSPORT_ACCEPT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libamqp_common.h"
#include "Transport/AcceptHandler.h"

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
typedef struct amqp_context_t amqp_context_t;
#endif

#ifndef LIBAMQP_AMQP_CONNECTION_TEST_HOOK_TYPE_T
#define LIBAMQP_AMQP_CONNECTION_TEST_HOOK_TYPE_T
typedef struct amqp_connection_test_hook_t amqp_connection_test_hook_t;
#endif

#ifndef LIBAMQP_AMQP_CONNECTIONS_TYPE_T
#define LIBAMQP_AMQP_CONNECTIONS_TYPE_T
typedef struct amqp_connections_t amqp_connections_t;
#endif

#ifndef LIBAMQP_AMQP_ACCEPT_HANDLER_ARGUMENTS_TYPE_T
#define LIBAMQP_AMQP_ACCEPT_HANDLER_ARGUMENTS_TYPE_T
typedef struct amqp_accept_handler_arguments_t amqp_accept_handler_arguments_t;
#endif

struct amqp_accept_handler_arguments_t
{
    amqp_connections_t *connections;
    amqp_connection_test_hook_t *test_hooks;
    void *user_data;
};

extern amqp_accept_handler_arguments_t *amqp_accept_handler_arguments_initialize(amqp_context_t *context, amqp_connection_test_hook_t *test_hooks, void *user_data);
extern void amqp_accept_handler_arguments_destroy(amqp_context_t *context, amqp_accept_handler_arguments_t *arguments);

#ifdef __cplusplus
}
#endif
#endif
