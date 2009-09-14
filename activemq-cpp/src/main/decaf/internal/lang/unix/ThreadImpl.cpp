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

#include <decaf/internal/lang/ThreadImpl.h>

#include <decaf/lang/Math.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/concurrent/TimeUnit.h>

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

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class ThreadHandle {
    public:

        typedef void (*threadEntry)( decaf::lang::ThreadHandle* self, void* data );

        ThreadHandle() {
            pthread_attr_init( &attributes );
            running = false;
            returnStatus = false;
            userArg = NULL;
            entryFunctionPtr = NULL;
        }

        ~ThreadHandle() {
            pthread_attr_destroy( &attributes );
        }

        pthread_t thread;
        pthread_attr_t attributes;
        bool returnStatus;
        threadEntry entryFunctionPtr;
        void* userArg;
        bool running;

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace{

    void* threadWorker( void* arg ) {
        ThreadHandle* handle = (ThreadHandle*)arg;
        handle->running = true;
        handle->entryFunctionPtr( handle, handle->userArg );
        handle->running = false;
        pthread_exit(0);
        return NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
ThreadHandle* ThreadImpl::create( decaf::lang::Thread* parent,
                                  void (*threadEntry)( decaf::lang::ThreadHandle* self, void* data ),
                                  void* userArg ) {

    if( parent == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Pointer to parent Thread object is null." );
    }

    if( threadEntry == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Pointer to thread entry function is null." );
    }

    std::auto_ptr<ThreadHandle> handle( new ThreadHandle );
    handle->entryFunctionPtr = threadEntry;
    handle->userArg = userArg;

    int result = pthread_create( &( handle->thread ), &( handle->attributes ), threadWorker, handle.get() );

    // Only try and set this if its not the default value.
    if( parent->getPriority() != Thread::NORM_PRIORITY ) {
        setPriority( handle.get(), parent->getPriority() );
    }

    if( result != 0 ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to create new Thread." );
    }

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::yeild() {

    #ifdef HAVE_PTHREAD_YIELD
        pthread_yield();
    #else
        #ifdef HAVE_SCHED_YIELD
            sched_yield();
        #endif
    #endif
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::sleep( long long mills, long long nanos ) {

    long long usecs = TimeUnit::MILLISECONDS.toMicros( mills ) +
                      TimeUnit::NANOSECONDS.toMicros( nanos );

    struct timeval tv;
    tv.tv_usec = usecs % 1000000;
    tv.tv_sec = usecs / 1000000;
    select( 0, NULL, NULL, NULL, &tv );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::join( decaf::lang::ThreadHandle* handle ) {

    void* theReturn = 0;
    pthread_join( handle->thread, &theReturn );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::join( decaf::lang::ThreadHandle* handle, long long mills, long long nanos ) {

    void* theReturn = NULL;

#if HAVE_PTHREAD_TIMEDJOIN_NP

    long long totalTime = TimeUnit::MILLISECONDS.toNanos( mills ) + nanos;

    timespec time;
    time.tv_nsec = totalTime % 1000000000;
    time.tv_sec = totalTime / 1000000000;

    pthread_timedjoin_np( handle->thread, &theReturn, &time );

#endif

    // TODO - Figure out something for platforms without the non-posix functions.
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::setPriority( decaf::lang::ThreadHandle* handle, unsigned int priority ) {

    // TODO
    //    int MAX_PRIORITY = sched_get_priority_max( SCHED_RR );
    //    int MIN_PRIORITY = sched_get_priority_min( SCHED_RR );
    //    int MID_PRIORITY = ( MAX_PRIORITY + MIN_PRIORITY ) / 2;

}

////////////////////////////////////////////////////////////////////////////////
long long ThreadImpl::getThreadId() {
    return (long long)pthread_self();
}
