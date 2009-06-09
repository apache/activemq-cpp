/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "UUIDTest.h"

#include <decaf/util/UUID.h>

using namespace decaf;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
UUIDTest::UUIDTest() {
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::test() {

    std::string bytes = "ASDASFSADGSDGSDASFA";

    UUID randId = UUID::randomUUID();
    UUID bytesId = UUID::nameUUIDFromBytes( bytes.c_str(), bytes.size() );

    CPPUNIT_ASSERT( !( randId == bytesId ) );
    CPPUNIT_ASSERT( !( randId.equals( bytesId ) ) );
    CPPUNIT_ASSERT( randId.compareTo( bytesId ) != 0 );

    std::string uuidStr = randId.toString();
    UUID strId = UUID::fromString( uuidStr );

    CPPUNIT_ASSERT( randId == strId );
    CPPUNIT_ASSERT( randId.variant() == 2 );

    UUID fromBits( randId.getMostSignificantBits(),
                   randId.getLeastSignificantBits() );

    CPPUNIT_ASSERT( randId.compareTo( fromBits) == 0 );
}
