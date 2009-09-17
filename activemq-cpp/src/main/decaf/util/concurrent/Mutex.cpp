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

//#include <apr_errno.h>
//#include <apr_time.h>
//#include <apr_thread_mutex.h>
//#include <apr_thread_cond.h>
//
//#include <decaf/internal/AprPool.h>

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

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace concurrent{

    class MutexProperties {
    public:

        MutexProperties() {
//            this->mutex = NULL;
//            this->lock_owner = 0;
//            this->lock_count = 0;
        }

        // Our one and only APR Pool
//        internal::AprPool aprPool;

        // The mutex object.
        std::auto_ptr<MutexHandle> mutex;
        std::auto_ptr<ConditionHandle> condition;

        // List of waiting threads
//        std::list<apr_thread_cond_t*> eventQ;
//
//        // Lock Status Members
//        volatile long long lock_owner;
//        volatile long long lock_count;

    };

}}}

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() {

    this->properties.reset( new MutexProperties );

    // Allocate the OS Mutex Implementation.
    this->properties->mutex.reset( MutexImpl::create() );
    this->properties->condition.reset( ConditionImpl::create( this->properties->mutex.get() ) );
//    apr_thread_mutex_create( &( properties->mutex ),
//                             APR_THREAD_MUTEX_NESTED,
//                             properties->aprPool.getAprPool() );
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex() {

    // Unlock the mutex, the destruction of the AprPool will take care
    // of cleaning up the apr_thread_mutex_t allocated in the ctor.
    //unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() throw( lang::Exception ) {

    MutexImpl::lock( this->properties->mutex.get() );
//    long long threadId = lang::Thread::getId();
//
//    if( threadId == properties->lock_owner ) {
//        properties->lock_count++;
//    } else {
//        apr_thread_mutex_lock( properties->mutex );
//        properties->lock_owner = threadId;
//        properties->lock_count = 1;
//    }
}

////////////////////////////////////////////////////////////////////////////////
bool Mutex::tryLock() throw( lang::Exception ) {

    return MutexImpl::trylock( this->properties->mutex.get() );

//    long long threadId = lang::Thread::getId();
//
//    if( threadId == properties->lock_owner ) {
//        properties->lock_count++;
//    } else {
//
//        if( apr_thread_mutex_trylock( properties->mutex ) == APR_SUCCESS ) {
//            properties->lock_owner = threadId;
//            properties->lock_count = 1;
//        } else {
//            return false;
//        }
//    }
//
//    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() throw( lang::Exception ) {

    MutexImpl::unlock( this->properties->mutex.get() );

//    if( properties->lock_owner == 0 ) {
//        return;
//    }
//
//    if( !isLockOwner() ) {
//        throw lang::Exception(
//            __FILE__, __LINE__,
//            "Mutex::unlock - Failed, not Lock Owner!" );
//    }
//
//    properties->lock_count--;
//
//    if( properties->lock_count == 0 ) {
//        properties->lock_owner = 0;
//        apr_thread_mutex_unlock( properties->mutex );
//    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() throw( lang::Exception ) {

    // Delegate to the timed version
    // wait( WAIT_INFINITE, 0 );
    ConditionImpl::wait( this->properties->condition.get() );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs ) throw( lang::Exception ) {

    wait( millisecs, 0 );

//    if( !isLockOwner() ) {
//        throw lang::Exception(
//            __FILE__, __LINE__,
//            "Mutex::wait - Failed, not Lock Owner!");
//    }
//
//    // Save the current owner as we are going to unlock and release for
//    // someone else to lock on potentially.  When we come back and
//    // re-lock we want to restore to the state we were in before.
//    long long lock_owner = this->properties->lock_owner;
//    long long lock_count = this->properties->lock_count;
//
//    this->properties->lock_owner = 0;
//    this->properties->lock_count = 0;
//
//    // Create this threads wait event
//    apr_thread_cond_t* waitEvent = NULL;
//    apr_pool_t *subPool = NULL;
//    apr_pool_create_ex( &subPool, properties->aprPool.getAprPool(), NULL, NULL );
//    apr_thread_cond_create( &waitEvent, subPool );
//
//    // Store the event in the queue so that a notify can
//    // call it and wake up the thread.
//    properties->eventQ.push_back( waitEvent );
//
//    if( millisecs != WAIT_INFINITE ) {
//        apr_interval_time_t wait = millisecs * 1000;
//        apr_thread_cond_timedwait( waitEvent, properties->mutex, wait );
//    } else {
//        apr_thread_cond_wait( waitEvent, properties->mutex );
//    }
//
//    // Be Sure that the event is now removed
//    properties->eventQ.remove( waitEvent );
//
//    // Destroy our wait event now, the notify method will have removed it
//    // from the event queue.
//    apr_thread_cond_destroy( waitEvent );
//    apr_pool_destroy( subPool );
//
//    // restore the owner
//    this->properties->lock_owner = lock_owner;
//    this->properties->lock_count = lock_count;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( long long millisecs, int nanos DECAF_UNUSED ) throw( lang::Exception ) {

    // For now delegate to the single arg version.
    // this->wait( millisecs );

    ConditionImpl::wait( this->properties->condition.get(), millisecs, nanos );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() throw( lang::Exception )
{
    ConditionImpl::notify( this->properties->condition.get() );

//    if( !isLockOwner() ) {
//        throw lang::Exception(
//            __FILE__, __LINE__,
//            "Mutex::Notify - Failed, not Lock Owner!" );
//    }
//
//    if( !properties->eventQ.empty() ) {
//        apr_thread_cond_t* event = properties->eventQ.front();
//        properties->eventQ.remove( event );
//        apr_thread_cond_signal( event );
//    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() throw( lang::Exception )
{
    ConditionImpl::notifyAll( this->properties->condition.get() );

//    if( !isLockOwner() ) {
//        throw lang::Exception(
//            __FILE__, __LINE__,
//            "Mutex::NotifyAll - Failed, not Lock Owner!" );
//    }
//
//    while( !properties->eventQ.empty() ) {
//        apr_thread_cond_t* event = properties->eventQ.front();
//        properties->eventQ.remove( event );
//        apr_thread_cond_signal( event );
//    }
}
