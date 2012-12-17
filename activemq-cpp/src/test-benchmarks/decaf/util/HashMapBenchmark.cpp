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

#include "HashMapBenchmark.h"

#include <decaf/lang/Integer.h>
#include <decaf/util/StlMap.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
HashMapBenchmark::HashMapBenchmark() : stringMap(), intMap() {
}

////////////////////////////////////////////////////////////////////////////////
HashMapBenchmark::~HashMapBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void HashMapBenchmark::run() {

    int numRuns = 500;
    std::string test = "test";
    std::string resultStr = "";
    StlMap<std::string, std::string> stringCopy;
    StlMap<int, int> intCopy;

    for( int i = 0; i < numRuns; ++i ) {
        stringMap.put( test + Integer::toString(i), test + Integer::toString(i) );
        intMap.put( 100 + i, 100 + i );
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
        stringMap.put( test + Integer::toString(i), test + Integer::toString(i) );
        intMap.put( 100 + i, 100 + i );
    }

    for( int i = 0; i < numRuns / 2; ++i ) {
        Set<std::string>& stringSet = stringMap.keySet();
        stringSet.size();
        Collection<std::string>& stringCol = stringMap.values();
        stringCol.size();
        Set<int>& intSet = intMap.keySet();
        intSet.size();
        Collection<int>& intCol = intMap.values();
        intCol.size();
    }

    for( int i = 0; i < numRuns / 2; ++i ) {
        stringCopy.copy( stringMap );
        stringCopy.clear();
        intCopy.copy( intMap );
        intCopy.clear();
    }

}
