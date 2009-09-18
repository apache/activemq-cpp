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

#include <decaf/util/concurrent/Mutex.h>

#include <decaf/internal/util/concurrent/MutexImpl.h>
#include <decaf/internal/util/concurrent/ConditionImpl.h>

#include <list>
#include <memory>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class MutexProperties {
    public:

        MutexProperties() {}

        // The Platform Mutex object and an associated Condition Object
        // for use in the wait / notify pattern.
        std::auto_ptr<MutexHandle> mutex;
        std::auto_ptr<ConditionHandle> condition;

    };

}}}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() {

    this->properties.reset( new MutexProperties );

    // Allocate the OS Mutex Implementation.
    this->properties->mutex.reset( MutexImpl::create() );
    this->properties->condition.reset( ConditionImpl::create( this->properties->mutex.get() ) );
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex() {
    unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() throw( decaf::lang::exceptions::RuntimeException ) {

    MutexImpl::lock( this->properties->mutex.get() );
}

////////////////////////////////////////////////////////////////////////////////
bool Mutex::tryLock() throw( decaf::lang::exceptions::RuntimeException ) {

    return MutexImpl::trylock( this->properties->mutex.get() );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() throw( decaf::lang::exceptions::RuntimeException ) {

    MutexImpl::unlock( this->properties->mutex.get() );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() throw( decaf::lang::exceptions::RuntimeException,
                          decaf::lang::exceptions::IllegalMonitorStateException,
                          decaf::lang::exceptions::InterruptedException ) {

    ConditionImpl::wait( this->properties->condition.get() );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs )
    throw( decaf::lang::exceptions::RuntimeException,
           decaf::lang::exceptions::IllegalMonitorStateException,
           decaf::lang::exceptions::InterruptedException ) {

    wait( millisecs, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs, int nanos )
    throw( decaf::lang::exceptions::RuntimeException,
           decaf::lang::exceptions::IllegalArgumentException,
           decaf::lang::exceptions::IllegalMonitorStateException,
           decaf::lang::exceptions::InterruptedException ) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Milliseconds value cannot be negative." );
    }

    if( nanos < 0 || nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Nanoseconds value must be in the range [0..999999]." );
    }

    ConditionImpl::wait( this->properties->condition.get(), millisecs, nanos );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() throw( decaf::lang::exceptions::RuntimeException,
                            decaf::lang::exceptions::IllegalMonitorStateException ) {

    ConditionImpl::notify( this->properties->condition.get() );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                               decaf::lang::exceptions::IllegalMonitorStateException ) {

    ConditionImpl::notifyAll( this->properties->condition.get() );
}
