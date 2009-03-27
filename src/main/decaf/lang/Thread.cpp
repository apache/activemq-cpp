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

#include "Thread.h"

#ifdef _WIN32
#pragma warning( disable: 4311 )
#endif

#include <apr_time.h>
#include <apr_portable.h>

#include <decaf/internal/DecafRuntime.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/RuntimeException.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Thread::Thread() {
    this->task = this;
    this->started = false;
    this->joined = false;
    this->threadHandle = NULL;
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task ) {
    this->task = task;
    this->started = false;
    this->joined = false;
    this->threadHandle = NULL;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread(){
}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() throw ( Exception )
{
    if( this->started ) {
        throw Exception(
            __FILE__, __LINE__,
            "Thread::start - Thread already started");
    }

    DecafRuntime* runtime = dynamic_cast<DecafRuntime*>( Runtime::getRuntime() );

    apr_status_t err = apr_thread_create(
        &this->threadHandle,
        NULL,
        runCallback,
        this,
        runtime->getGlobalPool() );

    if( err != APR_SUCCESS ) {
        throw Exception(
            __FILE__, __LINE__,
            "Thread::start - Could not start thread");
    }

    // Mark the thread as started.
    started = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( Exception )
{
    if( !this->started ) {
        throw Exception( __FILE__, __LINE__,
            "Thread::join() called without having called Thread::start()");
    }

    if( !this->joined ) {
        apr_status_t threadReturn;
        if( apr_thread_join( &threadReturn, this->threadHandle ) != APR_SUCCESS ) {
            throw Exception( __FILE__, __LINE__,
                "Thread::join() - Failed to Join the Thread");
        }
    }
    this->joined = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( int millisecs ) {
    apr_sleep( (apr_interval_time_t)(millisecs * 1000) );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::yield() {
    apr_thread_yield();
}

////////////////////////////////////////////////////////////////////////////////
unsigned long Thread::getId() {
    return (unsigned long)( apr_os_thread_current() );
}

////////////////////////////////////////////////////////////////////////////////
void* APR_THREAD_FUNC Thread::runCallback( apr_thread_t* self, void* param ) {

    // Get the instance.
    Thread* thread = (Thread*)param;

    // Invoke run on the task.
    try{
        thread->task->run();
    } catch( ... ){
        RuntimeException ex(
            __FILE__, __LINE__,
            "unhandled exception bubbled up to Thread::run");
        ex.printStackTrace();
    }

    // Indicate we are done.
    apr_thread_exit( self, APR_SUCCESS );
    return NULL;
}
