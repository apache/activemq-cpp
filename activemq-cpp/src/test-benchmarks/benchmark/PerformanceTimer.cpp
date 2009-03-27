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

#include "PerformanceTimer.h"

#include <decaf/util/Date.h>

using namespace std;
using namespace benchmark;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
PerformanceTimer::PerformanceTimer(){
    this->numberOfRuns = 0;
}

////////////////////////////////////////////////////////////////////////////////
PerformanceTimer::~PerformanceTimer(){
}

////////////////////////////////////////////////////////////////////////////////
void PerformanceTimer::start(){
    this->startTime = Date::getCurrentTimeMilliseconds();
}

////////////////////////////////////////////////////////////////////////////////
void PerformanceTimer::stop(){

    this->endTime = Date::getCurrentTimeMilliseconds();
    times.push_back( endTime - startTime );
    numberOfRuns++;
}

////////////////////////////////////////////////////////////////////////////////
void PerformanceTimer::reset(){
    this->startTime = 0;
    this->endTime = 0;
    this->times.clear();
}

////////////////////////////////////////////////////////////////////////////////
long long PerformanceTimer::getAverageTime() const{

    std::vector<long long>::const_iterator iter = times.begin();
    long long totalTime = 0;

    do{
        totalTime += *iter;
    } while( ++iter != times.end() );

    if( totalTime == 0 || numberOfRuns == 0 ) {
        return 0;
    }

    return totalTime / numberOfRuns;
}
