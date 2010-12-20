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
#include <decaf/internal/util/concurrent/unix/ConditionHandle.h>
#include <decaf/internal/util/concurrent/unix/MutexHandle.h>

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/concurrent/TimeUnit.h>

#include <list>
#include <memory>

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_SCHED_H
#include <sched.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif

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

    if( pthread_cond_init( &handle->condition, NULL ) != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to initialize OS Condition object." );
    }

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::destroy( decaf::util::concurrent::ConditionHandle* handle ) {

    if( handle == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Handle to Condition was NULL." );
    }

    pthread_cond_destroy( &handle->condition );
    delete handle;
}

///////////////////////////////////////////////////////////////////////////////
void ConditionImpl::wait( ConditionHandle* condition ) {

    // Save the current owner as we are going to unlock and release for
    // someone else to lock on potentially.  When we come back and
    // re-lock we want to restore to the state we were in before.
    long long lock_owner = condition->mutex->lock_owner;
    long long lock_count = condition->mutex->lock_count;

    condition->mutex->lock_owner = 0;
    condition->mutex->lock_count = 0;

    if( pthread_cond_wait( &condition->condition, &condition->mutex->mutex ) != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to wait on OS Condition object." );
    }

    // restore the owner
    condition->mutex->lock_owner = lock_owner;
    condition->mutex->lock_count = lock_count;
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::wait( ConditionHandle* condition, long long mills, long long nanos ) {

    // Save the current owner as we are going to unlock and release for
    // someone else to lock on potentially.  When we come back and
    // re-lock we want to restore to the state we were in before.
    long long lock_owner = condition->mutex->lock_owner;
    long long lock_count = condition->mutex->lock_count;

    condition->mutex->lock_owner = 0;
    condition->mutex->lock_count = 0;

    // Get time now as nanoseconds.
    struct timeval tv;
    gettimeofday( &tv, NULL );
    long long timeNow = TimeUnit::SECONDS.toNanos( tv.tv_sec ) +
                        TimeUnit::MICROSECONDS.toNanos( tv.tv_usec );

    // Convert delay to nanoseconds and add it to now.
    long long delay = TimeUnit::MILLISECONDS.toNanos( mills ) + nanos + timeNow;

    struct timespec abstime;
    abstime.tv_sec = TimeUnit::NANOSECONDS.toSeconds( delay );
    abstime.tv_nsec = delay % 1000000000;

    unsigned int result = pthread_cond_timedwait( &condition->condition, &condition->mutex->mutex, &abstime );

    // restore the owner
    condition->mutex->lock_owner = lock_owner;
    condition->mutex->lock_count = lock_count;

    if( result != 0 && result != ETIMEDOUT ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to wait on OS Condition object." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::notify( ConditionHandle* condition ) {

    if( pthread_cond_signal( &condition->condition ) ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to signal OS Condition object." );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConditionImpl::notifyAll( ConditionHandle* condition ) {

    if( pthread_cond_broadcast( &condition->condition ) ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to broadcast signal OS Condition object." );
    }
}
