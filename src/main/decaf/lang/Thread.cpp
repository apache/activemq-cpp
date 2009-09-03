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

#include <apr.h>
#include <apr_time.h>
#include <apr_portable.h>
#include <apr_thread_proc.h>

#include <decaf/internal/DecafRuntime.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class ThreadProperties {
    public:

        /**
         * The task to be run by this thread, defaults to
         * this thread object.
         */
        Runnable* task;

        /**
         * APR Thread Handle
         */
        apr_thread_t* threadHandle;

        /**
         * Started state of this thread.
         */
        bool started;

        /**
         * Indicates whether the thread has already been
         * joined.
         */
        bool joined;

        /**
         * The Assigned name of this thread.
         */
        std::string name;

        /**
         * The currently assigned priority
         */
        int priority;

        /**
         * static value that holds the incrementing Thread ID for unnamed threads.
         */
        static unsigned int id;

        /**
         * The handler to invoke if the thread terminates due to an exception that
         * was not caught in the user's run method.
         */
        Thread::UncaughtExceptionHandler* exHandler;

        /**
         * The Thread that created this Object.
         */
        Thread* parent;

    public:

        static void* APR_THREAD_FUNC runCallback( apr_thread_t* self, void* param ) {

            // Get the instance.
            ThreadProperties* properties = (ThreadProperties*)param;

            // Invoke run on the task.
            try{
                properties->task->run();
            } catch( decaf::lang::Throwable& error ){

                if( properties->exHandler != NULL ) {
                    properties->exHandler->uncaughtException( properties->parent, error );
                }

                error.printStackTrace();

            } catch( ... ) {
                RuntimeException error(
                    __FILE__, __LINE__,
                    "unhandled exception bubbled up to Thread::run");

                if( properties->exHandler != NULL ) {
                    properties->exHandler->uncaughtException( properties->parent, error );
                }

                error.printStackTrace();
            }

            // Indicate we are done.
            apr_thread_exit( self, APR_SUCCESS );
            return NULL;
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
unsigned int ThreadProperties::id = 0;

////////////////////////////////////////////////////////////////////////////////
Thread::Thread() {
    this->initialize( this, "" );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task ) {
    this->initialize( task, "" );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( const std::string& name ) {
    this->initialize( this, name );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task, const std::string& name ) {
    this->initialize( task, name );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::initialize( Runnable* task, const std::string& name ) {

    this->properties.reset( new ThreadProperties() );

    if( name == "" ) {
        this->properties->name = std::string( "Thread-" ) + Integer::toString( ++ThreadProperties::id );
    } else {
        this->properties->name = name;
    }

    this->properties->joined = false;
    this->properties->started = false;
    this->properties->priority = Thread::NORM_PRIORITY;
    this->properties->task = task;
    this->properties->threadHandle = NULL;
    this->properties->exHandler = NULL;
    this->properties->parent = this;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread() {
}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() throw ( Exception )
{
    if( this->properties->started ) {
        throw Exception(
            __FILE__, __LINE__,
            "Thread::start - Thread already started");
    }

    DecafRuntime* runtime = dynamic_cast<DecafRuntime*>( Runtime::getRuntime() );

    apr_status_t err = apr_thread_create(
        &this->properties->threadHandle,
        NULL,
        this->properties->runCallback,
        this->properties.get(),
        runtime->getGlobalPool() );

    if( err != APR_SUCCESS ) {
        throw Exception(
            __FILE__, __LINE__,
            "Thread::start - Could not start thread");
    }

    // Mark the thread as started.
    this->properties->started = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( Exception )
{
    if( !this->properties->started ) {
        throw Exception( __FILE__, __LINE__,
            "Thread::join() called without having called Thread::start()");
    }

    if( !this->properties->joined ) {
        apr_status_t threadReturn;
        if( apr_thread_join( &threadReturn, this->properties->threadHandle ) != APR_SUCCESS ) {
            throw Exception( __FILE__, __LINE__,
                "Thread::join() - Failed to Join the Thread");
        }
    }

    this->properties->joined = true;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( long long millisecs )
    throw( lang::exceptions::InterruptedException,
           lang::exceptions::IllegalArgumentException ) {

    Thread::sleep( millisecs, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( long long millisecs, unsigned int nanos )
    throw( lang::exceptions::InterruptedException,
           lang::exceptions::IllegalArgumentException ) {

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
void Thread::setName( const std::string& name ) {
    this->properties->name = name;
}

////////////////////////////////////////////////////////////////////////////////
std::string Thread::getName() const {
    return this->properties->name;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setPriority( int value ) throw( decaf::lang::exceptions::IllegalArgumentException ) {

    if( value < Thread::MIN_PRIORITY || value > Thread::MAX_PRIORITY ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::setPriority - Specified value {%d} is out of range", value );
    }

    this->properties->priority = value;

    // TODO - Alter Threads actual priority.
}

////////////////////////////////////////////////////////////////////////////////
int Thread::getPriority() const {
    return this->properties->priority;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setUncaughtExceptionHandler( UncaughtExceptionHandler* handler ) {
    this->properties->exHandler = handler;
}

////////////////////////////////////////////////////////////////////////////////
const Thread::UncaughtExceptionHandler* Thread::getUncaughtExceptionHandler() const {
    return this->properties->exHandler;
}

////////////////////////////////////////////////////////////////////////////////
std::string Thread::toString() const {

    return this->properties->name + ": Priority=" +
           Integer::toString( this->properties->priority ) + ", ThreadID=" +
           Long::toString( Thread::getId() );
}
