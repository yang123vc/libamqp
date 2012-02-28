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
    TEST_FIXTURE(EncodeTestFixture, EncodeLong)
    {
        type = amqp_encode_long(context, buffer, -129);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::long_8_minus_129);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x08, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, EncodeLongWithSmallValueShouldEncodeSmallLong)
    {
        type = amqp_encode_long(context, buffer, -4);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::long_small);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, EncodeLongWithZeroValueShouldEncodeShortLongWithZeroValue)
    {
        type = amqp_encode_long(context, buffer, 0);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::long_small_zero);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);
     }

    TEST_FIXTURE(EncodeTestFixture, EncodeSmallLong)
    {
        type = amqp_encode_small_long(context, buffer, -4);

        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::long_small);

        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);
    }

    TEST_FIXTURE(EncodeTestFixture, explicit_encode_small_long_with_zero_value_should_encode_small_long)
    {
        type = amqp_encode_small_long(context, buffer, 0U);

        CHECK_NOT_NULL(type);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x01, type->position.size);

        CHECK_BUFFERS_MATCH(buffer, test_data::long_small_zero);
    }
}
