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

#ifndef LIBAMQP_TRANSPORT_TRANSPORT_TEST_SUPPORT_H
#define LIBAMQP_TRANSPORT_TRANSPORT_TEST_SUPPORT_H

#include "Context/ContextTestSupport.h"
#include "Transport/Transport.h"
#include "Transport/EndPointStubb.h"

namespace SuiteTransport
{
    class TransportFixture : public SuiteContext::ContextFixture
    {
    public:
        TransportFixture();
        ~TransportFixture();

        amqp_endpoint_t *endpoint;

        static int in_memory;
        static amqp_endpoint_address_t endpoint_address;
        static amqp_endpoint_t *initialize_endpoint(amqp_context_t *context);
        static void opened_callback();;
    private:
    };
}

#endif
