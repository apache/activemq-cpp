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

#include "PropertiesBenchmark.h"

#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
PropertiesBenchmark::PropertiesBenchmark() {
}

////////////////////////////////////////////////////////////////////////////////
void PropertiesBenchmark::run() {

    int numRuns = 250;
    std::string result = "";
    bool hasIt = false;
    Properties copy;

    for( int i = 0; i < numRuns; ++i ) {
        properties.setProperty( "test", "value" );
        hasIt = properties.hasProperty( "test" );
        result = properties.getProperty( "test" );
        properties.remove( "test" );
    }

    std::vector< std::pair< std::string, std::string > > array;

    std::string prefix = "test";
    for( int i = 0; i < numRuns; ++i ) {
        properties.setProperty(
            prefix + Integer::toString(i), prefix + Integer::toString(i) );
    }

    for( int i = 0; i < numRuns; ++i ) {
        array = properties.toArray();
    }

    for( int i = 0; i < numRuns; ++i ) {
        copy.copy( &properties );
        copy.clear();
    }

}
