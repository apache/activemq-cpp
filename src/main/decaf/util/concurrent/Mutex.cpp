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
#include <decaf/lang/Integer.h>

#include <list>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class MutexProperties {
    private:

        MutexProperties( const MutexProperties& );
        MutexProperties& operator= ( const MutexProperties& );

    public:

        MutexProperties( const std::string& name ) : mutex( NULL ), condition( NULL ), name( name ) {
            if( this->name.empty() ) {
                this->name = std::string( "Mutex-" ) + Integer::toString( ++id );
            }
        }

        // The Platform Mutex object and an associated Condition Object
        // for use in the wait / notify pattern.
        MutexHandle* mutex;
        ConditionHandle* condition;
        std::string name;

        static unsigned int id;

    };

    unsigned int MutexProperties::id = 0;

}}}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() : Synchronizable(), properties( NULL ) {

    this->properties = new MutexProperties("");

    // Allocate the OS Mutex Implementation.
    this->properties->mutex = MutexImpl::create();
    this->properties->condition = ConditionImpl::create( this->properties->mutex );
}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex( const std::string& name ) : Synchronizable(), properties( NULL ) {

    this->properties = new MutexProperties( name );

    // Allocate the OS Mutex Implementation.
    this->properties->mutex = MutexImpl::create();
    this->properties->condition = ConditionImpl::create( this->properties->mutex );
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex() {

    unlock();

    ConditionImpl::destroy( this->properties->condition );
    MutexImpl::destroy( this->properties->mutex );

    delete this->properties;
}

////////////////////////////////////////////////////////////////////////////////
std::string Mutex::getName() const {
    return this->properties->name;
}

////////////////////////////////////////////////////////////////////////////////
std::string Mutex::toString() const {
    return this->properties->name;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() {
    MutexImpl::lock( this->properties->mutex );
}

////////////////////////////////////////////////////////////////////////////////
bool Mutex::tryLock() {
    return MutexImpl::trylock( this->properties->mutex );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() {
    MutexImpl::unlock( this->properties->mutex );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() {
    ConditionImpl::wait( this->properties->condition );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs ) {
    wait( millisecs, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs, int nanos ) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Milliseconds value cannot be negative." );
    }

    if( nanos < 0 || nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Nanoseconds value must be in the range [0..999999]." );
    }

    ConditionImpl::wait( this->properties->condition, millisecs, nanos );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() {
    ConditionImpl::notify( this->properties->condition );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() {
    ConditionImpl::notifyAll( this->properties->condition );
}
