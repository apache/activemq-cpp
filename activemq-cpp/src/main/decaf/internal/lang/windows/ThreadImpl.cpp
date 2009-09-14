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

#ifdef HAVE_PROCESS_H
#include <process.h>
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
            running = false;
            returnStatus = false;
            userArg = NULL;
            entryFunctionPtr = NULL;
            handle = NULL;
        }

        ~ThreadHandle() {
        }

        HANDLE handle;
        bool returnStatus;
        threadEntry entryFunctionPtr;
        void* userArg;
        bool running;

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace{

    unsigned int __stdcall threadWorker( void* arg ) {
        ThreadHandle* handle = (ThreadHandle*)arg;
        handle->running = true;
        handle->entryFunctionPtr( handle, handle->userArg );
        handle->running = false;
#ifndef _WIN32_WCE
        _endthreadex( 0 );
#else
        ExitThread( 0 );
#endif
        ::CloseHandle( handle->handle );

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

    unsigned int threadId = 0;

    std::auto_ptr<ThreadHandle> handle( new ThreadHandle );
    handle->entryFunctionPtr = threadEntry;
    handle->userArg = userArg;

#ifndef _WIN32_WCE

    handle->handle = (HANDLE)_beginthreadex( NULL, (DWORD)0, threadWorker, handle.get(), 0, &threadId );

#else

    handle->hanlde = CreateThread( NULL, 0, threadWorker, handle.get(), 0, &threadId ) );

#endif

   if( handle->handle == 0 ) {
       throw RuntimeException(
           __FILE__, __LINE__, "Failed to create new Thread." );
   }

   // Only try and set this if its not the default value.
    if( parent->getPriority() != Thread::NORM_PRIORITY ) {
        setPriority( handle.get(), parent->getPriority() );
    }

    return handle.release();
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::yeild() {

#ifndef _WIN32_WCE
    SwitchToThread();
#endif
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::sleep( long long mills, long long nanos ) {

    ::Sleep( (DWORD)mills );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::join( decaf::lang::ThreadHandle* handle ) {

    ThreadImpl::join( handle, INFINITE, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void ThreadImpl::join( decaf::lang::ThreadHandle* handle, long long mills, long long nanos ) {

    unsigned int rv = WaitForSingleObject( handle->handle, (DWORD)mills );
    ::CloseHandle( handle->handle );
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
    return (long long)::GetCurrentThreadId();
}
