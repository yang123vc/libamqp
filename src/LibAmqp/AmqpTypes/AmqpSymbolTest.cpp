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

#include "AmqpTypes/AmqpTypesTestFixture.h"

#include "AmqpTypes/AmqpSymbol.h"

SUITE(AmqpTypesSymbol)
{
    class AmqpSymbolTestFixture  : public SuiteAmqpTypes::AmqpTypesTestFixture
    {
    public:
        AmqpSymbolTestFixture();
        ~AmqpSymbolTestFixture();

        static const char *a;
        static const char *b;
        static const char *c;
        static const char *d;
        static const char *e;

        amqp_type_t *load();
        void load_array();

    public:
        amqp_symbol_t ref;
        amqp_symbol_t *symbol_b;
        amqp_array_t array;
    };

    const char *AmqpSymbolTestFixture::a = "ABC";
    const char *AmqpSymbolTestFixture::b = "ABC";
    const char *AmqpSymbolTestFixture::c = "DEFGH";
    const char *AmqpSymbolTestFixture::d = "IJKLM";
    const char *AmqpSymbolTestFixture::e = "DEFGHIJ";

    AmqpSymbolTestFixture::AmqpSymbolTestFixture() :  symbol_b(0)
    {
        memset(&ref, '\0', sizeof(amqp_symbol_t));
        memset(&array, '\0', sizeof(amqp_array_t));
    }

    AmqpSymbolTestFixture::~AmqpSymbolTestFixture()
    {
        amqp_symbol_cleanup(context, &ref);
        amqp_symbol_cleanup(context, symbol_b);
            amqp_array_cleanup(context, &array);
    }
        
    amqp_type_t *AmqpSymbolTestFixture::load()
    {
        test_data::array_of_symbols.transfer_to(buffer);
        return amqp_decode(context, buffer);
    }
    
    void AmqpSymbolTestFixture::load_array()
    {
        type = load();
        amqp_array_initialize_from_type(context, &array, type);
    }
    
    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_equality)
    {
        amqp_symbol_initialize(context, &ref, a, strlen(a));
        symbol_b = amqp_symbol_create(context, b, strlen(b));
        CHECK(amqp_symbol_compare(&ref, symbol_b) == 0);
        CHECK(amqp_symbol_compare(symbol_b, &ref) == 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_d)
    {
        amqp_symbol_initialize(context, &ref, c, strlen(c));
        symbol_b = amqp_symbol_create(context, d, strlen(d));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_d_gt_c)
    {
        amqp_symbol_initialize(context, &ref, d, strlen(d));
        symbol_b = amqp_symbol_create(context, c, strlen(c));

        CHECK(amqp_symbol_compare(&ref, symbol_b) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_e)
    {
        amqp_symbol_initialize(context, &ref, c, strlen(c));
        symbol_b = amqp_symbol_create(context, e, strlen(e));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_e_gt_c)
    {
        amqp_symbol_initialize(context, &ref, c, strlen(c));
        symbol_b = amqp_symbol_create(context, e, strlen(e));
        CHECK(amqp_symbol_compare(symbol_b, &ref) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_hash)
    {
        amqp_symbol_initialize(context, &ref, a, strlen(a));
        symbol_b = amqp_symbol_create(context, b, strlen(b));
        CHECK_EQUAL(amqp_symbol_hash(&ref), amqp_symbol_hash(symbol_b));
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_array_decode)
    {
        type = load();
        CHECK(amqp_type_is_array(type));
        amqp_array_initialize_from_type(context, &array, type);
        CHECK_EQUAL(5U, amqp_array_count(&array));
        for (int i  = 0; i < 5; i++)
        {
            CHECK(amqp_type_is_symbol(amqp_array_get_type(&array, i)));
        }
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_equality_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 0));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 1));
        CHECK(amqp_symbol_compare(&ref, symbol_b) == 0);
        CHECK(amqp_symbol_compare(symbol_b, &ref) == 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_d_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 3));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_d_gt_c_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 3));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 2));
        CHECK(amqp_symbol_compare(&ref, symbol_b) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_e_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 4));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_e_gt_c_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 4));
        CHECK(amqp_symbol_compare(symbol_b, &ref) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_hash_type)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 0));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 1));
        CHECK_EQUAL(amqp_symbol_hash(&ref), amqp_symbol_hash(symbol_b));
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_equality_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 0));
        symbol_b = amqp_symbol_create(context, b, strlen(b));
        CHECK(amqp_symbol_compare(&ref, symbol_b) == 0);
        CHECK(amqp_symbol_compare(symbol_b, &ref) == 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_d_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create(context, d, strlen(d));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_d_gt_c_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 3));
        symbol_b = amqp_symbol_create(context, c, strlen(c));
        CHECK(amqp_symbol_compare(&ref, symbol_b) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_c_lt_e_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create_from_type(context, amqp_array_get_type(&array, 4));
        CHECK(amqp_symbol_compare(&ref, symbol_b) < 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_e_gt_c_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 2));
        symbol_b = amqp_symbol_create(context, e, strlen(e));
        CHECK(amqp_symbol_compare(symbol_b, &ref) > 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_hash_type_and_symbol)
    {
        load_array();
        amqp_symbol_initialize_from_type(context, &ref, amqp_array_get_type(&array, 0));
        symbol_b = amqp_symbol_create(context, b, strlen(b));
        CHECK_EQUAL(amqp_symbol_hash(&ref), amqp_symbol_hash(symbol_b));
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_equality_with_cstr)
    {
        symbol_b = amqp_symbol_create(context, b, strlen(b));
        CHECK(amqp_symbol_compare_with_cstr(symbol_b, "ABC") == 0);
    }

    TEST_FIXTURE(AmqpSymbolTestFixture, symbol_clone)
    {
        const char *value = "symbol";
        amqp_symbol_initialize(context, &ref, value, strlen(value));
        CHECK(amqp_symbol_compare_with_cstr(&ref, value) == 0);

        symbol_b = amqp_symbol_clone(context, &ref);
        CHECK(amqp_symbol_compare_with_cstr(symbol_b, value) == 0);
    }
}
