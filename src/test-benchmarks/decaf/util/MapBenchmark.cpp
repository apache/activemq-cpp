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

#include "MapBenchmark.h"
#include <decaf/lang/Integer.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
MapBenchmark::MapBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void MapBenchmark::run() {

    int numRuns = 500;
    std::string test = "test";
    std::string resultStr = "";
    Map<std::string, std::string> stringCopy;
    Map<int, int> intCopy;

    for( int i = 0; i < numRuns; ++i ) {
        stringMap.setValue( test + Integer::toString(i), test + Integer::toString(i) );
        intMap.setValue( 100 + i, 100 + i );
        stringMap.containsKey( test + Integer::toString(i) );
        intMap.containsKey( 100 + i );
        stringMap.containsValue( test + Integer::toString(i) );
        intMap.containsValue( 100 + i );
    }

    for( int i = 0; i < numRuns; ++i ) {
        stringMap.remove( test + Integer::toString(i) );
        intMap.remove( 100 + i );
        stringMap.containsKey( test + Integer::toString(i) );
        intMap.containsKey( 100 + i );
    }

    for( int i = 0; i < numRuns; ++i ) {
        stringMap.setValue( test + Integer::toString(i), test + Integer::toString(i) );
        intMap.setValue( 100 + i, 100 + i );
    }

    std::vector<std::string> stringVec;
    std::vector<int> intVec;

    for( int i = 0; i < numRuns / 2; ++i ) {
        stringVec = stringMap.getKeys();
        stringVec = stringMap.getValues();
        intVec = intMap.getKeys();
        intVec = intMap.getValues();
    }

    for( int i = 0; i < numRuns / 2; ++i ) {
        stringCopy.copy( stringMap );
        stringCopy.clear();
        intCopy.copy( intMap );
        intCopy.clear();
    }

}
