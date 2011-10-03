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

#include <stdlib.h>
#include <string.h>

#include "Buffer/Buffer.h"
#include "TestData/TestFrames.h"

namespace test_data
{
    test_data_def(minimal_frame_header,
        0, 0, 0, 8,
        2, 0, 0, 1
    );

    test_data_def(sasl_mechanisms_frame,
        0x00, 0x00, 0x00, 0x23,
        0x02, 0x00, 0x00, 0x00,
        0x00,
            0x53, 0x40,
            0xd0,
                0x00, 0x00, 0x00, 0x13,
                0x00, 0x00, 0x00, 0x02,
                0x40,
                0x00, 0x41, 0xa1, 0x0a, 0x53, 0x43, 0x52, 0x41, 0x4d, 0x5f, 0x53, 0x54, 0x41, 0x52
    );
}

