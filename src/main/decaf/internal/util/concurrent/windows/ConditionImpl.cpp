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

#include <decaf/internal/util/concurrent/ConditionImpl.h>
#include <decaf/internal/util/concurrent/MutexImpl.h>
#include <decaf/internal/util/concurrent/windows/ConditionHandle.h>
#include <decaf/internal/util/concurrent/windows/MutexHandle.h>

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/concurrent/TimeUnit.h>

#include <list>
#include <memory>

#include <limits.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ConditionHandle* ConditionImpl::create( MutexHandle* mutex ) {

    std::auto_ptr<ConditionHandle> handle( new ConditionHandle );

    handle->mutex = mutex;

    handle->semaphore = CreateSemaphore( NULL, 0, LONG_MAX, NULL );
    if( handle->semaphore == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to initialize OS Condition object." );
    }

    try{
        InitializeCriticalSection( &handle->criticalSection );
    } catch(...) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to initialize OS Condition object." );
    }

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::destroy( decaf::util::concurrent::ConditionHandle* handle ) {

    if( handle == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Handle to Condition Object was Null." );
    }

    CloseHandle( handle->semaphore );
    ::DeleteCriticalSection( &handle->criticalSection );

    delete handle;
}

///////////////////////////////////////////////////////////////////////////////
void ConditionImpl::wait( ConditionHandle* condition ) {

    ConditionImpl::wait( condition, INFINITE, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::wait( ConditionHandle* condition, long long mills, long long nanos ) {

    DWORD res = 0;
    bool doWake = false;
    unsigned int generation = 0;

    // Store the state of the condition before waiting.
    EnterCriticalSection( &condition->criticalSection );

    condition->numWaiting++;
    generation = condition->generation;

    LeaveCriticalSection( &condition->criticalSection );

    // Save the current owner as we are going to unlock and release for
    // someone else to lock on potentially.  When we come back and
    // re-lock we want to restore to the state we were in before.  These
    // values are on the calling threads stack now so when it re-acquires
    // the lock its essentially restoring its own state data back to the
    // loack object.
    long long lock_owner = condition->mutex->lock_owner;
    long long lock_count = condition->mutex->lock_count;

    condition->mutex->lock_owner = 0;
    condition->mutex->lock_count = 0;

    // Circumvent the MutexImpl Implementation and unlock the Critical Section ourself.
    // We have the state preserved here so we can restore it once the condition has ]
    // been signaled, the default Mutex unlock behavior is of a recursive lock so
    // if we don't do this here it might not unlock if this thread had locked it more
    // than once.
    LeaveCriticalSection( &condition->mutex->mutex );

    do {

        res = WaitForSingleObject( condition->semaphore, (DWORD)mills );

        EnterCriticalSection( &condition->criticalSection );

        if( condition->numWake ) {

            if( condition->generation != generation ) {
                condition->numWake--;
                condition->numWaiting--;
                break;
            } else {
                doWake = true;
            }
        }
        else if( res != WAIT_OBJECT_0 ) {
            condition->numWaiting--;
            break;
        }

        LeaveCriticalSection( &condition->criticalSection );

        if( doWake ) {
            doWake = false;
            ReleaseSemaphore( condition->semaphore, 1, NULL );
        }

    } while( true );

    LeaveCriticalSection( &condition->criticalSection );

    // Perform the reciprocal of the Mutex unlock done above.
    EnterCriticalSection( &condition->mutex->mutex );

    // restore the owner now that its been re-locked.
    condition->mutex->lock_owner = lock_owner;
    condition->mutex->lock_count = lock_count;
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::notify( ConditionHandle* condition ) {

    try {

        bool doWake = false;

        EnterCriticalSection( &condition->criticalSection );

        if( condition->numWaiting > condition->numWake ) {
            doWake = true;
            condition->numWake++;
            condition->generation++;
        }

        LeaveCriticalSection( &condition->criticalSection );

        if( doWake ) {
            ReleaseSemaphore( condition->semaphore, 1, NULL );
        }

    } catch(...) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to signal OS Condition object." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::notifyAll( ConditionHandle* condition ) {

    try {
        unsigned int numWake = 0;

        EnterCriticalSection( &condition->criticalSection );

        if( condition->numWaiting > condition->numWake ) {
            numWake = condition->numWaiting - condition->numWake;
            condition->numWake = condition->numWaiting;
            condition->generation++;
        }

        LeaveCriticalSection( &condition->criticalSection );

        if( numWake ) {
            ReleaseSemaphore( condition->semaphore, numWake, NULL );
        }
    } catch(...) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to broadcast signal OS Condition object." );
    }
}
