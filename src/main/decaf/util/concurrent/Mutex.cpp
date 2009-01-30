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

#include <apr_errno.h>
#include <apr_time.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() {

    apr_thread_mutex_create( &mutex, APR_THREAD_MUTEX_NESTED, aprPool.getAprPool() );
    this->lock_owner = 0;
    this->lock_count = 0;
}

////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex() {

    // Unlock the mutex, the destruction of the AprPool will take care
    // of cleaning up the apr_thread_mutex_t allocated in the ctor.
    unlock();
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::lock() throw( lang::Exception ) {

    unsigned long threadId = lang::Thread::getId();

    if( threadId == lock_owner ) {
        lock_count++;
    } else {
        apr_thread_mutex_lock( mutex );
        lock_owner = threadId;
        lock_count = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::unlock() throw( lang::Exception ) {

    if( lock_owner == 0 ) {
        return;
    }

    if( !isLockOwner() ) {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::unlock - Failed, not Lock Owner!" );
    }

    lock_count--;

    if(lock_count == 0) {
        lock_owner = 0;
        apr_thread_mutex_unlock( mutex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait() throw( lang::Exception ) {
    // Delegate to the timed version
    wait( WAIT_INFINITE );
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::wait( unsigned long millisecs )
    throw( lang::Exception ) {

    if( !isLockOwner() ) {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::wait - Failed, not Lock Owner!");
    }

    // Save the current owner as we are going to unlock and release for
    // someone else to lock on potentially.  When we come back and
    // re-lock we want to restore to the state we were in before.
    unsigned long lock_owner = this->lock_owner;
    unsigned long lock_count = this->lock_count;

    this->lock_owner = 0;
    this->lock_count = 0;

    // Create this threads wait event
    apr_thread_cond_t* waitEvent = NULL;
    apr_pool_t *subPool = NULL;
    apr_pool_create_ex( &subPool, aprPool.getAprPool(), NULL, NULL );
    apr_thread_cond_create( &waitEvent, subPool );

    // Store the event in the queue so that a notify can
    // call it and wake up the thread.
    eventQ.push_back( waitEvent );

    if( millisecs != WAIT_INFINITE ) {
        apr_interval_time_t wait = millisecs * 1000;
        apr_thread_cond_timedwait( waitEvent, mutex, wait );
    } else {
        apr_thread_cond_wait( waitEvent, mutex );
    }

    // Be Sure that the event is now removed
    eventQ.remove( waitEvent );

    // Destroy our wait event now, the notify method will have removed it
    // from the event queue.
    apr_thread_cond_destroy( waitEvent );
    apr_pool_destroy( subPool );

    // restore the owner
    this->lock_owner = lock_owner;
    this->lock_count = lock_count;
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notify() throw( lang::Exception )
{
    if( !isLockOwner() ) {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::Notify - Failed, not Lock Owner!" );
    }

    if( !eventQ.empty() ) {
        apr_thread_cond_t* event = eventQ.front();
        eventQ.remove( event );
        apr_thread_cond_signal( event );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Mutex::notifyAll() throw( lang::Exception )
{
    if( !isLockOwner() ) {
        throw lang::Exception(
            __FILE__, __LINE__,
            "Mutex::NotifyAll - Failed, not Lock Owner!" );
    }

    while( !eventQ.empty() ) {
        apr_thread_cond_t* event = eventQ.front();
        eventQ.remove( event );
        apr_thread_cond_signal( event );
    }
}
