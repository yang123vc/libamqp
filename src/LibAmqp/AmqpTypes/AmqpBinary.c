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

#include <assert.h>
#include <string.h>

#include "AmqpTypes/AmqpTypesInternal.h"
#include "AmqpTypes/AmqpBinary.h"
#include "Codec/Type/TypeExtension.h"
#include "AmqpTypes/AmqpVariableInternal.h"

#include "debug_helper.h"


void amqp_binary_initialize_as_null(amqp_context_t *context, amqp_binary_t *binary)
{
    binary->leader.fn_table = 0;
    amqp_variable_initialize_as_null(&binary->v);
}

static void create_dtor(amqp_context_t *context, amqp_amqp_type_t *type)
{
    amqp_binary_t *binary = (amqp_binary_t *) type;
    if (binary->v.data)
    {
        AMQP_FREE(context, binary->v.data);
    }

    AMQP_FREE(context, binary);
}
static void initialize_dtor(amqp_context_t *context, amqp_amqp_type_t *type)
{
    amqp_binary_t *binary = (amqp_binary_t *) type;
    if (binary->v.data)
    {
        AMQP_FREE(context, binary->v.data);
    }
}

void amqp_binary_initialize_from_type(amqp_context_t *context, amqp_binary_t *binary, amqp_type_t *type)
{
    static amqp_fn_table_t table = {
        .dtor = initialize_dtor
    };
    binary->leader.fn_table = &table;

    assert(amqp_type_is_binary(type));
    amqp_variable_initialize_from_type(&binary->v, type);
}

amqp_binary_t *amqp_binary_create_from_type(amqp_context_t *context, amqp_type_t *type)
{
    static amqp_fn_table_t table = {
        .dtor = create_dtor
    };
    amqp_binary_t *result = AMQP_MALLOC(context, amqp_binary_t);
    assert(amqp_type_is_binary(type));

    result->leader.fn_table = &table;
    amqp_variable_initialize_from_type(&result->v, type);
    return result;
}

void amqp_binary_initialize(amqp_context_t *context, amqp_binary_t *binary, const uint8_t *data, size_t size)
{
    static amqp_fn_table_t table = {
        .dtor = initialize_dtor
    };
    binary->leader.fn_table = &table;
    amqp_variable_initialize(&binary->v, (uint8_t *) amqp_duplicate(context, (const char *) data, size), size);
}

static amqp_binary_t *binary_create(amqp_context_t *context, const uint8_t *data, size_t size)
{
    static amqp_fn_table_t table = {
        .dtor = create_dtor
    };

    amqp_binary_t *result = AMQP_MALLOC(context, amqp_binary_t);
    result->leader.fn_table = &table;
    amqp_variable_initialize(&result->v, data, size);
    return result;
}

amqp_binary_t *amqp_binary_create(amqp_context_t *context, const uint8_t *data, size_t size)
{
    return binary_create(context, (uint8_t *) amqp_duplicate(context, (const char *) data, size), size);
}

amqp_binary_t *amqp_binary_clone(amqp_context_t *context, amqp_binary_t *source)
{
    return binary_create(context, amqp_variable_clone_data(context, &source->v), source->v.size);
}

int amqp_binary_to_bytes(amqp_binary_t *binary, uint8_t *buffer, size_t buffer_size)
{
    return amqp_variable_to_bytes(&binary->v, buffer, buffer_size);
}

int amqp_binary_compare(amqp_binary_t *lhs, amqp_binary_t *rhs)
{
    return amqp_variable_compare(&lhs->v, &rhs->v);
}

int amqp_binary_compare_with_bytes(amqp_binary_t *lhs, const uint8_t *rhs, size_t size)
{
    return amqp_variable_compare_with_bytes(&lhs->v, rhs, size);
}

uint32_t amqp_binary_hash(amqp_binary_t *binary)
{
    return amqp_variable_hash(&binary->v);
}

int amqp_binary_index_of(amqp_binary_t *binary, int c, int starting_position)
{
    while (starting_position < binary->v.size)
    {
        if (amqp_binary_byte_get_at(binary, starting_position) == c)
        {
            return starting_position;
        }
        starting_position++;
    }
    return -1;
}

uint8_t *amqp_binary_sub_sequence(amqp_context_t *context, amqp_binary_t *binary, size_t start, size_t length)
{
    uint8_t *result = amqp_malloc(context, length + 1);
    // TODO - use block copies
    int i;
    for (i = 0; i < length; i++)
    {
        result[i] = amqp_binary_byte_get_at(binary, start + i);
    }
    return result;
}
