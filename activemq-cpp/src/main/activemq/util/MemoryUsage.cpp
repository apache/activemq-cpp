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

#include "MemoryUsage.h"
#include <decaf/util/concurrent/Concurrent.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
MemoryUsage::MemoryUsage() {
    this->limit = 0;
    this->usage = 0;
}

////////////////////////////////////////////////////////////////////////////////
MemoryUsage::MemoryUsage( unsigned long long limit ) {
    this->limit = limit;
    this->usage = 0;
}

////////////////////////////////////////////////////////////////////////////////
MemoryUsage::~MemoryUsage() {
}

////////////////////////////////////////////////////////////////////////////////
void MemoryUsage::waitForSpace() {

    synchronized( &mutex ) {
        while( this->isFull() ) {
            mutex.wait();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void MemoryUsage::waitForSpace( unsigned int timeout ) {

    if( this->isFull() ) {
        synchronized( &mutex ) {
            mutex.wait( timeout );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void MemoryUsage::increaseUsage( unsigned long long value ) {

    if( value == 0 ) {
        return;
    }

    synchronized( &mutex ) {
        this->usage += value;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MemoryUsage::decreaseUsage( unsigned long long value ) {

    if( value == 0 ) {
        return;
    }

    synchronized( &mutex ) {
        value > this->usage ? this->usage = 0 : this->usage -= value;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool MemoryUsage::isFull() const {
    bool result = false;

    synchronized( &mutex ) {
        result = this->usage >= this->limit;
    }

    return result;
}
