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
#include "Context/ErrorHandling.h"

#include "Codec/Encode/EncodeTestFixture.h"

SUITE(CodecEncode)
{
    TEST_FIXTURE(EncodeTestFixture, EncodeUShort)
    {
        type = amqp_encode_ushort(context, buffer, 65534);

        CHECK_NOT_NULL(type);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x02, type->position.size);
        CHECK_BUFFERS_MATCH(buffer, test_data::ushort_2);
    }

    TEST_FIXTURE(EncodeTestFixture, EncodeShort)
    {
        type = amqp_encode_short(context, buffer, -2);

        CHECK_NOT_NULL(type);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x02, type->position.size);
        CHECK_BUFFERS_MATCH(buffer, test_data::short_2);
    }

    TEST_FIXTURE(EncodeTestFixture, EncodeUByte)
    {
        type = amqp_encode_ubyte(context, buffer, 254);

        CHECK_NOT_NULL(type);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);
        CHECK_BUFFERS_MATCH(buffer, test_data::ubyte_1);
    }

    TEST_FIXTURE(EncodeTestFixture, EncodeByte)
    {
        type = amqp_encode_byte(context, buffer, -2);

        CHECK_NOT_NULL(type);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);
        CHECK_BUFFERS_MATCH(buffer, test_data::byte_1);
    }


    TEST_FIXTURE(EncodeTestFixture, TimeStamp)
    {
        type = amqp_encode_timestamp(context, buffer, 1291654800000LL);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::timestamp_8);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x08, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, TimeStamp2)
    {
        type = amqp_encode_timestamp(context, buffer, -1864105200000LL);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::timestamp_before_epoc_8);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x08, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, Float)
    {
        type = amqp_encode_float(context, buffer, 123.456f);

        CHECK_NOT_NULL(type);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x04, type->position.size);

        result = amqp_decode(context, buffer);
        CHECK_NOT_NULL(result);
        ASSERT_VALID(result);
        CHECK_CLOSE(123.456f, amqp_type_to_float(result), 0.00001);
    }

    TEST_FIXTURE(EncodeTestFixture, Double)
    {
        type = amqp_encode_double(context, buffer, 123.456);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::double_8);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x08, type->position.size);

        result = amqp_decode(context, buffer);
        CHECK_NOT_NULL(result);
        ASSERT_VALID(result);
        CHECK_CLOSE(123.456, amqp_type_to_double(result), 0.00001);
    }

    TEST_FIXTURE(EncodeTestFixture, uuid)
    {
        amqp_uuid_t uuid = {0xf8, 0x1d, 0x4f, 0xae, 0x7d, 0xec, 0x11, 0xd0, 0xa7, 0x65, 0x00, 0xa0, 0xc9, 0x1e, 0x6b, 0xf6};
        type = amqp_encode_uuid(context, buffer, &uuid);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::uuid_16);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x10, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, uuid_t_width)
    {
        CHECK_EQUAL((size_t) 16, sizeof(amqp_uuid_t));
    }

}
