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

#include "TestHarness.h"
#include "ErrorHandling/ErrorHandling.h"

#include "Codec/CodecTestSupport.h"

namespace
{
    t::Context *c;
    amqp_type_t *type;
    char *result;


    void SetUp()
    {
        c = new t::Context();
        type = 0;
        result = 0;
    }

    void TearDown()
    {
        AMQP_FREE(result);
        AMQP_DEALLOC(amqp_type_t, c->context(), type);
        delete c;
    }
}

TEST(EncodeZeroByteTypeTest, Null)
{
    type = amqp_encode_null(c->context());
    ASSERT_NOT_NULL(type);
    ASSERT_BUFFERS_MATCH(c->context()->encode.buffer, test_data::null_0);

    CHECK_EQUAL(0x40, type->format_code);
    CHECK_EQUAL((size_t) 0x01, type->position.index);
    CHECK_EQUAL((size_t) 0x00, type->position.size);
    CHECK_EQUAL(1U, type->flags.is_null);
}

TEST(EncodeZeroByteTypeTest, True)
{
    type = amqp_encode_boolean(c->context(), 1);

    ASSERT_NOT_NULL(type);
    ASSERT_BUFFERS_MATCH(c->context()->encode.buffer, test_data::true_0);

    CHECK_EQUAL(0x41, type->format_code);
    CHECK_EQUAL((size_t) 0x01, type->position.index);
    CHECK_EQUAL((size_t) 0x00, type->position.size);
    CHECK_EQUAL(0U, type->flags.is_null);
}

TEST(EncodeZeroByteTypeTest, False)
{
    type = amqp_encode_boolean(c->context(), 0);

    ASSERT_NOT_NULL(type);
    ASSERT_BUFFERS_MATCH(c->context()->encode.buffer, test_data::false_0);

    CHECK_EQUAL(0x42, type->format_code);
    CHECK_EQUAL((size_t) 0x01, type->position.index);
    CHECK_EQUAL((size_t) 0x00, type->position.size);
    CHECK_EQUAL(0U, type->flags.is_null);
}

