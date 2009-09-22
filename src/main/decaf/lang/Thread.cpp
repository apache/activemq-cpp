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

#include <decaf/internal/DecafRuntime.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
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
#ifdef HAVE_PROCESS_H
#include <process.h>
#endif

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util::concurrent;

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
         * The Platform Specific Thread Handle.
         */
        #ifdef HAVE_PTHREAD_H
            pthread_t handle;
            pthread_attr_t attributes;
        #else
            HANDLE handle;
        #endif

        /**
         * Current state of this thread.
         */
        Thread::State state;

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

        ThreadProperties() {

            #ifdef HAVE_PTHREAD_H
                pthread_attr_init( &attributes );
            #endif
        }

        ~ThreadProperties() {

            #ifdef HAVE_PTHREAD_H
                pthread_attr_destroy( &attributes );
            #endif
        }

    public:

        static void runCallback( ThreadProperties* properties ) {

            properties->state = Thread::RUNNABLE;

            // Invoke run on the task.
            try{
                properties->task->run();
            } catch( decaf::lang::Throwable& error ){

                if( properties->exHandler != NULL ) {
                    properties->exHandler->uncaughtException( properties->parent, error );
                }
            } catch( std::exception& stdEx ) {

                RuntimeException error( __FILE__, __LINE__, stdEx.what() );

                if( properties->exHandler != NULL ) {
                    properties->exHandler->uncaughtException( properties->parent, error );
                }
            } catch( ... ) {

                RuntimeException error(
                    __FILE__, __LINE__,
                    "Uncaught exception bubbled up to Thread::run, Thread Terminating.");

                if( properties->exHandler != NULL ) {
                    properties->exHandler->uncaughtException( properties->parent, error );
                }
            }

            // Indicate we are done.
            properties->state = Thread::TERMINATED;
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace{

    #ifdef HAVE_PTHREAD_H

        pthread_key_t currentThreadKey;

        void* threadWorker( void* arg ) {

            ThreadProperties* properties = (ThreadProperties*)arg;

            pthread_setspecific( currentThreadKey, (void*)properties->parent );

            ThreadProperties::runCallback( properties );

            pthread_setspecific( currentThreadKey, NULL );
            pthread_exit(0);

            return NULL;
        }
    #else

        DWORD currentThreadKey;

        unsigned int __stdcall threadWorker( void* arg ) {
            ThreadProperties* properties = (ThreadProperties*)arg;

            ::TlsSetValue( currentThreadKey, (void*)properties->parent );

            ThreadProperties::runCallback( properties );

            ::TlsSetValue( currentThreadKey, NULL );

            #ifndef _WIN32_WCE
                _endthreadex( 0 );
            #else
                ExitThread( 0 );
            #endif

            ::CloseHandle( properties->handle );

            return NULL;
        }

    #endif
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ThreadProperties::id = 0;

////////////////////////////////////////////////////////////////////////////////
void Thread::initThreading() {

    Thread* mainThread = new Thread( "Main Thread" );

    mainThread->properties->state = Thread::RUNNABLE;
    mainThread->properties->priority = Thread::NORM_PRIORITY;
    mainThread->properties->parent = mainThread;

    #ifdef HAVE_PTHREAD_H

        mainThread->properties->handle = pthread_self();

        // Create the Key used to store the Current Thread data
        pthread_key_create( &currentThreadKey, NULL );
        pthread_setspecific( currentThreadKey, mainThread );

    #else

        mainThread->properties->handle = ::GetCurrentThread();

        // Create the key used to store the Current Thread data
        currentThreadKey = ::TlsAlloc();
        ::TlsSetValue( currentThreadKey, mainThread );

    #endif
}

////////////////////////////////////////////////////////////////////////////////
void Thread::shutdownThreading() {

    #ifdef HAVE_PTHREAD_H

        // Get the Main Thread and Destroy it
        Thread* mainThread = (Thread*) pthread_getspecific( currentThreadKey );

        delete mainThread;

        // Destroy the current Thread key now, no longer needed.
        pthread_key_delete( currentThreadKey );

    #else

        Thread* mainThread = (Thread*) ::TlsGetValue( currentThreadKey );

        delete mainThread;

        // Destroy our TLS resources before we shutdown.
        ::TlsFree( currentThreadKey );

    #endif
}

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

    this->properties->state = Thread::NEW;
    this->properties->priority = Thread::NORM_PRIORITY;
    this->properties->task = task;
    this->properties->exHandler = NULL;
    this->properties->parent = this;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread() {
}

////////////////////////////////////////////////////////////////////////////////
void Thread::run() {
    // No work to do as yet.
}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() throw ( decaf::lang::exceptions::IllegalThreadStateException,
                             decaf::lang::exceptions::RuntimeException ) {

    try {

        if( this->properties->state > Thread::NEW ) {
            throw IllegalThreadStateException(
                __FILE__, __LINE__,
                "Thread::start - Thread already started");
        }

        #ifdef HAVE_PTHREAD_H
            int result = pthread_create( &( properties->handle ),
                                         &( properties->attributes ),
                                         threadWorker, properties.get() );

            if( result != 0 ) {
                throw RuntimeException(
                    __FILE__, __LINE__, "Failed to create new Thread." );
            }

        #else

            unsigned int threadId = 0;

            #ifndef _WIN32_WCE

                properties->handle = (HANDLE)_beginthreadex(
                     NULL, (DWORD)0, threadWorker, properties.get(), 0, &threadId );

            #else

                properties->hanlde = CreateThread( NULL, 0, threadWorker, properties.get(), 0, &threadId ) );

                if( properties->handle == 0 ) {
                    throw RuntimeException(
                        __FILE__, __LINE__, "Failed to create new Thread." );
                }

            #endif

        #endif

        // Only try and set this if its not the default value.
        if( properties->priority != Thread::NORM_PRIORITY ) {
            setPriority( properties->priority );
        }

        properties->state = Thread::RUNNABLE;
     }
     DECAF_CATCH_RETHROW( IllegalThreadStateException )
     DECAF_CATCH_RETHROW( RuntimeException )
     DECAF_CATCH_EXCEPTION_CONVERT( NullPointerException, RuntimeException )
     DECAF_CATCHALL_THROW( RuntimeException )
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( decaf::lang::exceptions::InterruptedException )
{
    if( this->properties->state < Thread::RUNNABLE ) {
        throw Exception( __FILE__, __LINE__,
            "Thread::join() called without having called Thread::start()");
    }

    if( this->properties->state != Thread::TERMINATED ) {

        #ifdef HAVE_PTHREAD_H
            void* theReturn = 0;
            pthread_join( properties->handle, &theReturn );
        #else
            Thread::join( INFINITE, 0 );
        #endif
    }
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join( long long millisecs )
    throw ( decaf::lang::exceptions::IllegalArgumentException,
            decaf::lang::exceptions::InterruptedException ) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs ) - Value given {%d} is less than 0", millisecs );
    }

    this->join( millisecs, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join( long long millisecs, unsigned int nanos )
    throw ( decaf::lang::exceptions::IllegalArgumentException,
            decaf::lang::exceptions::InterruptedException ) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs, nanos ) - Value given {%d} is less than 0", millisecs );
    }

    if( nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs, nanos ) - Nanoseconds must be in range [0...999999]" );
    }

    #ifdef HAVE_PTHREAD_H

        void* theReturn = NULL;

        // TODO - Still need a way to do this if the non-posix method doesn't exist.
        #if HAVE_PTHREAD_TIMEDJOIN_NP

            long long totalTime = TimeUnit::MILLISECONDS.toNanos( millisecs ) + nanos;

            timespec time;
            time.tv_nsec = totalTime % 1000000000;
            time.tv_sec = totalTime / 1000000000;

            pthread_timedjoin_np( properties->handle, &theReturn, &time );

        #endif
    #else
        unsigned int rv = WaitForSingleObject( properties->handle, (DWORD)millisecs );
    #endif
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( long long millisecs )
    throw( decaf::lang::exceptions::InterruptedException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    Thread::sleep( millisecs, 0 );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep( long long millisecs, unsigned int nanos )
    throw( decaf::lang::exceptions::InterruptedException,
           decaf::lang::exceptions::IllegalArgumentException ) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::sleep( millisecs, nanos ) - Value given {%d} is less than 0", millisecs );
    }

    if( nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::sleep( millisecs, nanos ) - Nanoseconds must be in range [0...999999]" );
    }

    #ifdef HAVE_PTHREAD_H
        long long usecs = TimeUnit::MILLISECONDS.toMicros( millisecs ) +
                          TimeUnit::NANOSECONDS.toMicros( nanos );

        struct timeval tv;
        tv.tv_usec = usecs % 1000000;
        tv.tv_sec = usecs / 1000000;
        select( 0, NULL, NULL, NULL, &tv );
    #else
        ::Sleep( (DWORD)millisecs );
    #endif
}

////////////////////////////////////////////////////////////////////////////////
void Thread::yield() {

    #ifdef HAVE_PTHREAD_H
        #ifdef HAVE_PTHREAD_YIELD
            pthread_yield();
        #else
            #ifdef HAVE_SCHED_YIELD
                sched_yield();
            #endif
        #endif
    #else
        #ifndef _WIN32_WCE
            SwitchToThread();
        #endif
    #endif
}

////////////////////////////////////////////////////////////////////////////////
long long Thread::getId() {

    #ifdef HAVE_PTHREAD_H
        return (long long)pthread_self();
    #else
        return (long long)::GetCurrentThreadId();
    #endif
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

    //ThreadImpl::setPriority( this->properties->threadHandle.get(), value );
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

////////////////////////////////////////////////////////////////////////////////
bool Thread::isAlive() const {
    return this->properties->state != Thread::NEW &&
           this->properties->state != Thread::TERMINATED;
}

////////////////////////////////////////////////////////////////////////////////
Thread::State Thread::getState() const {
    return this->properties->state;
}

////////////////////////////////////////////////////////////////////////////////
Thread* Thread::currentThread() {

    void* result = NULL;

    #ifdef HAVE_PTHREAD_H

        result = pthread_getspecific( currentThreadKey );

    #else

        result = ::TlsGetValue( currentThreadKey );

    #endif

    if( result == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "Failed to find the Current Thread pointer in the TLS." );
    }

    return (Thread*)result;
}
