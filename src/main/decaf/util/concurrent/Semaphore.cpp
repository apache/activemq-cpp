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
Semaphore::Semaphore( int permits ) {

    this->handle.reset( new SemaphoreHandle( permits ) );
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::Semaphore( int permits, bool fair ) {

    this->handle.reset( new SemaphoreHandle( permits, fair ) );
}

////////////////////////////////////////////////////////////////////////////////
Semaphore::~Semaphore() {
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire() throw( decaf::lang::exceptions::InterruptedException,
                                 decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly() throw ( decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire() throw ( decaf::lang::exceptions::RuntimeException ) {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( long long timeout, const TimeUnit& unit )
    throw( decaf::lang::exceptions::InterruptedException,
           decaf::lang::exceptions::RuntimeException ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release() throw ( decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquire( int permits ) throw( decaf::lang::exceptions::InterruptedException,
                                              decaf::lang::exceptions::IllegalArgumentException,
                                              decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::acquireUninterruptibly( int permits )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( int permits )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::RuntimeException ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Semaphore::tryAcquire( int permits, long long timeout, const TimeUnit& unit )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::RuntimeException ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Semaphore::release( int permits )
    throw( decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::RuntimeException ) {

}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::availablePermits() const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int Semaphore::drainPermits() throw( decaf::lang::exceptions::RuntimeException ) {
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
