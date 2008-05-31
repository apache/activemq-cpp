/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_UTIL_PRIMITIVEVALUETYPES_H_
#define _ACTIVEMQ_UTIL_PRIMITIVEVALUETYPES_H_

#include <string>
#include <vector>

namespace activemq{
namespace util{

    class PrimitiveMap;
    class PrimitiveList;

    /**
     * Enumeration for the various primitive types.
     */
    enum PrimitiveValueTypeEnum{
        NULL_TYPE          = 0,
        BOOLEAN_TYPE       = 1,
        BYTE_TYPE          = 2,
        CHAR_TYPE          = 3,
        SHORT_TYPE         = 4,
        INTEGER_TYPE       = 5,
        LONG_TYPE          = 6,
        DOUBLE_TYPE        = 7,
        FLOAT_TYPE         = 8,
        STRING_TYPE        = 9,
        BYTE_ARRAY_TYPE    = 10,
        MAP_TYPE           = 11,
        LIST_TYPE          = 12,
        BIG_STRING_TYPE    = 13
    };

}}

#endif /*_ACTIVEMQ_UTIL_PRIMITIVEVALUETYPES_H_*/
