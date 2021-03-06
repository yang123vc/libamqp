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
    TEST_FIXTURE(EncodeTestFixture, EncodeNullValue)
    {
        type = amqp_encode_null(context, buffer);
        CHECK_NOT_NULL(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::null_0);

        CHECK_EQUAL(0x40, type->constructor.format_code);
        CHECK_EQUAL((size_t) 0x01, type->position.index);
        CHECK_EQUAL((size_t) 0x00, type->position.size);
        CHECK(amqp_type_is_null(type));
    }
}
