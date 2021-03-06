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

#ifndef LIBAMQP_TRANSPORT_DUMMY_BROKER_ECHO_SERVER_H
#define LIBAMQP_TRANSPORT_DUMMY_BROKER_ECHO_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libamqp_common.h"
#include "Transport/LowLevel/ListenerThread.h"

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
typedef struct amqp_context_t amqp_context_t;
#endif

extern amqp_listener_thread_t *amqp_echo_server_initialize(amqp_context_t *context, int listen_port_number);
extern void amqp_echo_server_destroy(amqp_context_t *context, amqp_listener_thread_t *echo_server_thread);

#ifdef __cplusplus
}
#endif
#endif
