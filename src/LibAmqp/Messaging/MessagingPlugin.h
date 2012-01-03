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

/*
  WARNING: This file was generated by scripts/generate. DO NOT HAND EDIT.
     scripts/generate uses:
  	scripts/generate-amqp-types.rb to generate AMQP typedefs
  	scripts/generate-decode-methods.rb to generate composite type decode methods
 */
#ifndef LIBAMQP_MESSAGING_MESSAGING_PLUGIN_H
#define LIBAMQP_MESSAGING_MESSAGING_PLUGIN_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBAMQP_AMQP_CONTEXT_TYPE_T
#define LIBAMQP_AMQP_CONTEXT_TYPE_T
typedef struct amqp_context_t amqp_context_t;
#endif

#ifndef LIBAMQP_AMQP_TYPE_T
#define LIBAMQP_AMQP_TYPE_T
    typedef struct amqp_type_t amqp_type_t;
#endif

#ifndef LIBAMQP_AMQP_WILDCARD_T
#define LIBAMQP_AMQP_WILDCARD_T
    typedef struct amqp_wildcard_t amqp_wildcard_t;
#endif

#include "AmqpTypes/AmqpMessagingType.h"

typedef struct amqp_messaging_methods_t
{
    struct {
        amqp_messaging_type_decoder_t source;
        amqp_messaging_type_decoder_t target;
        amqp_messaging_type_decoder_t delivery_state;
    } decode;
} amqp_messaging_methods_t;

void amqp_initialize_default_messaging_methods(amqp_messaging_methods_t *methods);

#ifdef __cplusplus
}
#endif
#endif
