/*
   Copyright 2011 StormMQ Limited

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

#ifndef LIBAMQP_DECODE_TEST_SUPPORT_H
#define LIBAMQP_DECODE_TEST_SUPPORT_H

#include <string.h>

#include <UnitTest++.h>
#include "ExtraChecks.h"

#include "Codec/Decode/Decode.h"
#include "Codec/Encode/Encode.h"
#include "Codec/Type/Type.h"

#include "Context/ContextTestSupport.h"
#include "TestData/TestData.h"

namespace SuiteCodec
{
    class CodecFixture : public SuiteContext::ContextFixture
    {
    public:
        CodecFixture() : type(0) {}
        ~CodecFixture()
        {
            deallocate_type(type);
        }
        
        amqp_memory_pool_t *type_pool()
        {
            return &context->pools.amqp_type_t_pool;
        }
        
        amqp_type_t *allocate_type()
        {
            return (amqp_type_t *) amqp_allocate(type_pool());
        }
        
        void deallocate_type(amqp_type_t *type)
        {
            amqp_deallocate(type_pool(), type);
        }

    public:
        amqp_type_t *type;
    };
}

#define CHECK_FLAG_SET(flag)   CHECK_EQUAL(1U, flag)
#define CHECK_FLAG_CLEAR(flag)   CHECK_EQUAL(0U, flag)

namespace t
{
    void dump_type(amqp_type_t *type);
    void dump_type_buffer(amqp_type_t *type);

    int compare_buffers(const unsigned char *lhs, size_t lhs_size, const unsigned char *rhs, size_t rhs_size);
}



#define ASSERT_VALID(type)  \
    CHECK_NOT_NULL(type); \
    CHECK(amqp_type_is_valid(type))

#define ASSERT_INVALID(type)  \
    CHECK_NOT_NULL(type); \
    CHECK(!amqp_type_is_valid(type))

#define ASSERT_BUFFERS_MATCH(context, byte_array) \
     CHECK(t::compare_buffers(byte_array.bytes(), byte_array.size(), context->bytes, context->limit.size))

#define CHECK_ARRAY(type) \
    CHECK(amqp_type_is_compound(type)); \
    CHECK(amqp_type_is_array(type)); \
    CHECK(!amqp_type_is_map(type)); \
    CHECK(!amqp_type_is_list(type))

#define CHECK_LIST(type) \
    CHECK(amqp_type_is_compound(type)); \
    CHECK(!amqp_type_is_array(type)); \
    CHECK(!amqp_type_is_map(type)); \
    CHECK(amqp_type_is_list(type))

#define CHECK_MAP(type) \
    CHECK(amqp_type_is_compound(type)); \
    CHECK(!amqp_type_is_array(type)); \
    CHECK(amqp_type_is_map(type)); \
    CHECK(!amqp_type_is_list(type))

#endif

