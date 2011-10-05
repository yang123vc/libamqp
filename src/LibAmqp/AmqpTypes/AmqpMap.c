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

#include <assert.h>
#include <string.h>

#include "Misc/Bits.h"
#include "AmqpTypes/AmqpMap.h"
#include "debug_helper.h"

void amqp_map_initialize(amqp_context_t *context, amqp_map_t *map, int initial_capacity)
{
    map->capacity = amqp_next_power_two(initial_capacity);
    assert(map->capacity > 0);

    map->buckets = AMQP_MALLOC_ARRAY(context, void *, map->capacity);
    map->count = 0;
}

void amqp_map_cleanup(amqp_context_t *context, amqp_map_t *map)
{
    AMQP_FREE(context, map->buckets);
}

