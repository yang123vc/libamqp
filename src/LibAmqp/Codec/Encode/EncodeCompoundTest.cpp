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

#include "Context/TestSupport/BufferHolder.h"
#include "Codec/CodecTestFixture.h"

#include "Codec/Decode/Decode.h"
#include "Codec/Type/Type.h"
#include "Codec/Type/TypePrint.h"

#include <stdio.h>


SUITE(CompoundEncoding)
{
    class EncodeTestFixture : public SuiteCodec::CodecTestFixture,
            public virtual TestSupport::BufferHolder
    {
    public:
        EncodeTestFixture() : result(0) {}
        ~EncodeTestFixture()
        {
            AMQP_FREE(context, result);
        }

    public:
        char *result;
    };
    
    TEST_FIXTURE(EncodeTestFixture, single_element_array)
    {
        amqp_type_t *t;

        type = amqp_encode_array_8(context, buffer);
        ASSERT_INVALID(type);
        CHECK_ARRAY(type);

        t = amqp_encode_short(context, buffer, 10);
        CHECK(amqp_type_is_contained(t));

        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_EQUAL((size_t) 1, (size_t) type->value.array.count);
        CHECK_NOT_NULL(type->value.array.elements);

        CHECK_EQUAL((size_t) 2, type->position.index);
        CHECK_EQUAL((size_t) 4, type->position.size);

        CHECK_EQUAL(0x61, type->value.array.elements[0]->constructor.format_code);

        CHECK(amqp_type_is_encoded(type));
        CHECK(amqp_type_is_encoded(type->value.array.elements[0]));

        CHECK_BUFFERS_MATCH(buffer, test_data::single_element_array);
    }

    TEST_FIXTURE(EncodeTestFixture, short_array)
    {
        type = amqp_encode_array_8(context, buffer);
        CHECK_NOT_NULL(type);
        ASSERT_INVALID(type);
        CHECK_ARRAY(type);

        CHECK_NULL(type->value.array.elements);

        amqp_encode_short(context, buffer, 10);
        amqp_encode_short(context, buffer, 11);
        amqp_encode_short(context, buffer, 11);
        amqp_encode_short(context, buffer, 13);
        amqp_encode_short(context, buffer, 14);

        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_NOT_NULL(type->value.array.elements);
        CHECK_EQUAL((size_t) 5, (size_t) type->value.array.count);
        CHECK_EQUAL((size_t) 2, type->position.index);
        CHECK_EQUAL((size_t) 12, type->position.size);

        CHECK_EQUAL(0x61, type->value.array.elements[0]->constructor.format_code);

        CHECK_BUFFERS_MATCH(buffer, test_data::array_shorts);
    }

    // mixing elements will mark the type as invalid and will leave the buffer in an undefined state.
    TEST_FIXTURE(EncodeTestFixture, invalid_array_with_mixed_types)
    {
        type = amqp_encode_array_8(context, buffer);

        ASSERT_INVALID(type);
        CHECK_ARRAY(type);
        CHECK_NULL(type->value.array.elements);

        amqp_encode_short(context, buffer, 10);
        amqp_encode_string(context, buffer, "hello");

        amqp_complete_type(context, buffer, type);
        ASSERT_INVALID(type);

        CHECK_EQUAL((size_t) 2, (size_t) type->value.array.count);

        CHECK_EQUAL(0x61, type->value.array.elements[0]->constructor.format_code);
        CHECK_EQUAL(0xa1, type->value.array.elements[1]->constructor.format_code);
    }

    // mixing elements will mark the type as invalid and will leave the buffer in an undefined state.
    TEST_FIXTURE(EncodeTestFixture, array_of_strings)
    {
        type = amqp_encode_array_8(context, buffer);
        ASSERT_INVALID(type);
        CHECK_ARRAY(type);

        CHECK_NULL(type->value.array.elements);

        amqp_encode_string(context, buffer, "the");
        amqp_encode_string(context, buffer, "little");
        amqp_encode_string(context, buffer, "brown");
        amqp_encode_string(context, buffer, "fox");

        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_NOT_NULL(type->value.array.elements);
        CHECK_EQUAL((size_t) 4, (size_t) type->value.array.count);

        CHECK_EQUAL(0xa1, type->value.array.elements[0]->constructor.format_code);
        CHECK_EQUAL(0xa1, type->value.array.elements[1]->constructor.format_code);
        CHECK_EQUAL(0xa1, type->value.array.elements[2]->constructor.format_code);
        CHECK_EQUAL(0xa1, type->value.array.elements[3]->constructor.format_code);

        CHECK_BUFFERS_MATCH(buffer, test_data::string_array);
    }

    TEST_FIXTURE(EncodeTestFixture, amqp_encode_list_8_of_shorts)
    {
        type = amqp_encode_list_8(context, buffer);
        ASSERT_INVALID(type);

        CHECK_LIST(type);
        CHECK_NULL(type->value.list.elements);

        amqp_encode_short(context, buffer, 10);
        amqp_encode_short(context, buffer, 11);
        amqp_encode_short(context, buffer, 11);
        amqp_encode_short(context, buffer, 13);
        amqp_encode_short(context, buffer, 14);

        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_EQUAL((size_t) 5, (size_t) type->value.list.count);
        CHECK_EQUAL((size_t) 2, type->position.index);
        CHECK_EQUAL((size_t) 16, type->position.size);

        CHECK_BUFFERS_MATCH(buffer, test_data::list_of_shorts);
    }

    TEST_FIXTURE(EncodeTestFixture, amqp_encode_list_32)
    {
        amqp_type_t *l;

        type = amqp_encode_list_32(context, buffer);
        ASSERT_INVALID(type);
        CHECK_LIST(type);
        CHECK_NULL(type->value.list.elements);

        amqp_encode_symbol(context, buffer, "Foo");
        amqp_encode_double(context, buffer, 123.456);
        amqp_encode_string(context, buffer, "Hello");
        amqp_encode_short(context, buffer, 10);
            l = amqp_encode_array_8(context, buffer);
                amqp_encode_short(context, buffer, 10);
                amqp_encode_short(context, buffer, 11);
            amqp_complete_type(context, buffer, l);

            l = amqp_encode_array_8(context, buffer);
                amqp_encode_short(context, buffer, 12);
            amqp_complete_type(context, buffer, l);
        amqp_complete_type(context, buffer, type);

        ASSERT_VALID(type);
        CHECK_LIST(type);
        CHECK_NOT_NULL(type->value.list.elements);

        CHECK_EQUAL((size_t) 6, (size_t) type->value.list.count);
        CHECK_EQUAL((size_t) 5, type->position.index);
        CHECK_EQUAL((size_t) 42, type->position.size);

        CHECK_BUFFERS_MATCH(buffer, test_data::list);
    }

    TEST_FIXTURE(EncodeTestFixture, amqp_encode_map_8)
    {
        amqp_type_t *l;

        type = amqp_encode_map_8(context, buffer);
        ASSERT_INVALID(type);

            amqp_encode_string(context, buffer, "list:");
            l = amqp_encode_list_8(context, buffer);
                amqp_encode_long(context, buffer, 1);
                amqp_encode_string(context, buffer, "two");
                amqp_encode_double(context, buffer, 3.141593);
                amqp_encode_null(context, buffer);
                amqp_encode_boolean(context, buffer, false);
            amqp_complete_type(context, buffer, l);

            ASSERT_VALID(l);
            CHECK_LIST(l);
            CHECK_EQUAL((size_t) 5, (size_t) l->value.list.count);
            CHECK_EQUAL((size_t) 12, l->position.index);
            CHECK_EQUAL((size_t) 19, l->position.size);


            amqp_encode_null(context, buffer);
            amqp_encode_boolean(context, buffer, true);

            amqp_encode_string(context, buffer, "pi");
            amqp_encode_double(context, buffer, 3.141593);

            amqp_encode_string(context, buffer, "two");
            amqp_encode_long(context, buffer, 2);

            amqp_encode_string(context, buffer, "129");
            amqp_encode_long(context, buffer, 129);

        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_MAP(type);
        CHECK_NOT_NULL(type->value.map.entries);

        CHECK_EQUAL((size_t) 10, (size_t) type->value.map.count);
        CHECK_EQUAL((size_t) 2, type->position.index);
        CHECK_EQUAL((size_t) 65, type->position.size);

        CHECK_BUFFERS_MATCH(buffer, test_data::map);
    }

    TEST_FIXTURE(EncodeTestFixture, empty_map)
    {
        type = amqp_encode_map_8(context, buffer);
        ASSERT_INVALID(type);
        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_MAP(type);
        CHECK_NULL(type->value.map.entries);

        CHECK_EQUAL((size_t) 0, (size_t) type->value.map.count);

        CHECK_BUFFERS_MATCH(buffer, test_data::empty_map);
    }

    TEST_FIXTURE(EncodeTestFixture, empty_list)
    {
        type = amqp_encode_list_8(context, buffer);
        ASSERT_INVALID(type);
        amqp_complete_type(context, buffer, type);
        ASSERT_VALID(type);

        CHECK_LIST(type);
        CHECK_NULL(type->value.list.elements);
        CHECK_EQUAL((size_t) 0, (size_t) type->value.list.count);

        CHECK_BUFFERS_MATCH(buffer, test_data::empty_list_0);
    }

    static amqp_type_t *encode_described_list(amqp_context_t *context, amqp_buffer_t *buffer)
    {
        amqp_type_t *result = amqp_start_encode_described_type(context, buffer);
            amqp_encode_string(context, buffer, "List");
            {
                amqp_type_t *list = amqp_encode_list_8(context, buffer);
                amqp_encode_string(context, buffer, "Pie");
                {
                    amqp_type_t *pi = amqp_start_encode_described_type(context, buffer);
                    amqp_encode_string(context, buffer, "PI");
                    amqp_encode_double(context, buffer, 3.141593);
                    amqp_complete_type(context, buffer, pi);
                }
                amqp_encode_short(context, buffer, 10);
                amqp_complete_type(context, buffer, list);
            }
        amqp_complete_type(context, buffer, result);
        return result;
    }

    TEST_FIXTURE(EncodeTestFixture, described_list)
    {
        type = encode_described_list(context, buffer);

        ASSERT_VALID(type);
        CHECK_BUFFERS_MATCH(buffer, test_data::described_list);
    }

    TEST_FIXTURE(EncodeTestFixture, described_list_structure)
    {
        type = encode_described_list(context, buffer);

        ASSERT_VALID(type);

        CHECK(amqp_type_is_composite(type));
        CHECK(!amqp_type_is_descriptor(type));
        CHECK(!amqp_type_is_described(type));

        CHECK_EQUAL((size_t) 2, type->value.described.count);

        amqp_type_t *descriptor = type->value.described.elements[0];
        amqp_type_t *list = type->value.described.elements[1];

        CHECK(!amqp_type_is_composite(descriptor));
        CHECK(amqp_type_is_descriptor(descriptor));
        CHECK(!amqp_type_is_described(descriptor));

        CHECK(!amqp_type_is_composite(list));
        CHECK(!amqp_type_is_descriptor(list));
        CHECK(amqp_type_is_described(list));

        CHECK_LIST(list);

        CHECK(amqp_type_is_composite(list->value.list.elements[1]));
        CHECK(!amqp_type_is_descriptor(list->value.list.elements[1]));
        CHECK(!amqp_type_is_described(list->value.list.elements[1]));

        CHECK(amqp_type_is_descriptor(list->value.list.elements[1]->value.described.elements[0]));
        CHECK(amqp_type_is_described(list->value.list.elements[1]->value.described.elements[1]));

        CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[0]));
        CHECK(amqp_type_is_contained(list->value.list.elements[1]->value.described.elements[1]));
    }

    static amqp_type_t *encode_list_of_one_short(amqp_context_t *context, amqp_buffer_t *buffer, short value)
    {
        amqp_type_t *result = amqp_encode_list_8(context, buffer);
        amqp_encode_short(context, buffer, value);
        amqp_complete_type(context, buffer, result);
        return result;
    }

    static amqp_type_t *encode_an_empty_list(amqp_context_t *context, amqp_buffer_t *buffer)
    {
        amqp_type_t *result = amqp_encode_list_8(context, buffer);
        amqp_complete_type(context, buffer, result);
        return result;
    }

    TEST_FIXTURE(EncodeTestFixture, array_of_lists)
    {
        type = amqp_encode_array_8(context, buffer);
            encode_list_of_one_short(context, buffer, 7);
            encode_list_of_one_short(context, buffer, 11);
            encode_list_of_one_short(context, buffer, 13);
        amqp_complete_type(context, buffer, type);

        ASSERT_VALID(type);
        CHECK_ARRAY(type);
        CHECK_EQUAL((size_t) 3, (size_t) type->value.array.count);

        CHECK_LIST(type->value.array.elements[0]);
        CHECK_LIST(type->value.array.elements[1]);
        CHECK_LIST(type->value.array.elements[2]);

        CHECK_BUFFERS_MATCH(buffer, test_data::array_of_lists);
    }

    TEST_FIXTURE(EncodeTestFixture, array_of_lists_one_empty)
    {
        type = amqp_encode_array_8(context, buffer);
            encode_list_of_one_short(context, buffer, 7);
            encode_list_of_one_short(context, buffer, 11);
            encode_an_empty_list(context, buffer);
        amqp_complete_type(context, buffer, type);

        ASSERT_VALID(type);
        CHECK_ARRAY(type);
        CHECK_EQUAL((size_t) 3, (size_t) type->value.array.count);

        CHECK_LIST(type->value.array.elements[0]);
        CHECK_LIST(type->value.array.elements[1]);
        CHECK_LIST(type->value.array.elements[2]);
        CHECK(amqp_type_is_empty_list(type->value.array.elements[2]));

        CHECK_BUFFERS_MATCH(buffer, test_data::array_of_lists_one_empty);
    }

    TEST_FIXTURE(EncodeTestFixture, array_of_single_empty_list)
    {
        type = amqp_encode_array_8(context, buffer);
            encode_an_empty_list(context, buffer);
        amqp_complete_type(context, buffer, type);

        ASSERT_VALID(type);
        CHECK_ARRAY(type);
        CHECK_EQUAL((size_t) 1, (size_t) type->value.array.count);

        CHECK_LIST(type->value.array.elements[0]);

        CHECK(amqp_type_is_empty_list(type->value.array.elements[0]));

        CHECK_BUFFERS_MATCH(buffer, test_data::array_of_single_empty_list);
    }
}