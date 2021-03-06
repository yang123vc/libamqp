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

#include "Codec/Decode/DecodeTestFixture.h"

#include "Codec/Decode/Decode.h"
#include "Codec/Type/Type.h"

SUITE(CodecDecode)
{
    TEST_FIXTURE(DecodeTestFixture, Float)
    {
        load_decode_buffer(test_data::float_4);

        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);
        CHECK(amqp_type_is_float(type));
        CHECK_CLOSE(123.456f, amqp_type_to_float(type), 0.00005);
    }

    TEST_FIXTURE(DecodeTestFixture, Double)
    {
        load_decode_buffer(test_data::double_8);

        type = amqp_decode(context, decode_buffer);

        ASSERT_VALID(type);
        CHECK(amqp_type_is_double(type));
        CHECK_CLOSE(123.456, amqp_type_to_double(type), 0.00005);
    }
}