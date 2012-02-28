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

#ifndef LIBAMQP_TRANSPORT_LOW_LEVEL_TEST_SUPPORT_TIMER_HOLDER_H
#define LIBAMQP_TRANSPORT_LOW_LEVEL_TEST_SUPPORT_TIMER_HOLDER_H

#include "libamqp_common.h"
#include "Transport/LowLevel/Timer.h"
#include "Context/TestSupport/ContextHolder.h"

namespace TestSupport
{
    class TimerHolder :
            public virtual TestSupport::ContextHolder
    {
    public:
        TimerHolder();
        ~TimerHolder();

    public:
        amqp_timer_t *timer;
        static void expiry_handler(amqp_context_t *context, amqp_timer_t *timer);
    };
}
#endif
