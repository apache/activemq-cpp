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

#include "Semaphore.h"

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {
namespace concurrent {

    class SemaphoreHandle {
    public:

        int permits;
        bool fair;

    public:

        SemaphoreHandle( int permits ) : permits( permits ), fair( false ) {
        }

        SemaphoreHandle( int permits, bool fair ) : permits( permits ), fair( fair ) {
        }

        ~SemaphoreHandle() {
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
Semaphore::Semaphore( int permits ) : handle(new SemaphoreHandle( permits )) {
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::Semaphore( int permits, bool fair ) : handle(new SemaphoreHandle( permits, fair )) {
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::~Semaphore() {
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire() {
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly() {
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire() {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( long long timeout DECAF_UNUSED, const TimeUnit& unit DECAF_UNUSED ) {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release() {

}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire( int permits DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly( int permits DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( int permits DECAF_UNUSED ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( int permits DECAF_UNUSED, long long timeout DECAF_UNUSED, const TimeUnit& unit DECAF_UNUSED ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release( int permits DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::availablePermits() const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::drainPermits() {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::isFair() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
std::string Semaphore::toString() const {
    return "";
}
