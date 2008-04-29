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

#include "SetBenchmark.h"

#include <decaf/lang/Integer.h>
#include <decaf/util/Iterator.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
SetBenchmark::SetBenchmark(){
}

////////////////////////////////////////////////////////////////////////////////
void SetBenchmark::run(){

    int numRuns = 500;
    std::string test = "test";
    std::string resultStr = "";
    Set<std::string> stringCopy;
    Set<int> intCopy;

    for( int i = 0; i < numRuns; ++i ) {
        stringSet.add( test + Integer::toString(i) );
        intSet.add( 100 + i );
        stringSet.contains( test + Integer::toString(i) );
        intSet.contains( 100 + i );
    }

    for( int i = 0; i < numRuns; ++i ) {
        stringSet.remove( test + Integer::toString(i) );
        intSet.remove( 100 + i );
        stringSet.contains( test + Integer::toString(i) );
        intSet.contains( 100 + i );
    }

    for( int i = 0; i < numRuns; ++i ) {
        stringSet.add( test + Integer::toString(i) );
        intSet.add( 100 + i );
    }

    std::vector<std::string> stringVec;
    std::vector<int> intVec;

    for( int i = 0; i < numRuns / 2; ++i ) {
        stringVec = stringSet.toArray();
        intVec = intSet.toArray();
    }

    std::string tempStr = "";
    int tempInt = 0;

    for( int i = 0; i < numRuns / 2; ++i ) {

        Iterator<std::string>* strIter = stringSet.iterator();
        Iterator<int>* intIter = intSet.iterator();

        while( strIter->hasNext() ){
            tempStr = strIter->next();
        }

        while( intIter->hasNext() ){
            tempInt = intIter->next();
        }

        delete strIter;
        delete intIter;
    }

    for( int i = 0; i < numRuns / 2; ++i ) {
        stringCopy.copy( stringSet );
        stringCopy.clear();
        intCopy.copy( intSet );
        intCopy.clear();
    }

}
