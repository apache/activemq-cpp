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
#include <decaf/util/concurrent/Mutex.h>

#include <vector>

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
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class ThreadProperties {
    public:

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

        volatile Thread::State state;
        std::string name;
        int priority;
        bool interrupted;
        bool unparked;
        bool parked;
        decaf::util::concurrent::Mutex mutex;

        static unsigned int id;

        Thread::UncaughtExceptionHandler* exHandler;
        Thread* parent;

    public:

        ThreadProperties() {

            this->priority = Thread::NORM_PRIORITY;
            this->state = Thread::NEW;
            this->interrupted = false;
            this->parked = false;
            this->unparked = false;
            this->parent = NULL;

            #ifdef HAVE_PTHREAD_H
                pthread_attr_init( &attributes );
            #endif
        }

        ~ThreadProperties() {

            #ifdef HAVE_PTHREAD_H
                pthread_attr_destroy( &attributes );
            #else
                ::CloseHandle( handle );
            #endif
        }

    public:

        static void runCallback( ThreadProperties* properties ) {

            synchronized( &properties->mutex ) {
                properties->state = Thread::RUNNABLE;
            }

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

            synchronized( &properties->mutex ) {

                // Indicate we are done.
                properties->state = Thread::TERMINATED;

                // Signal any joined threads.
                properties->mutex.notifyAll();
            }
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace{

    Thread* mainThread = NULL;
    std::vector<Thread*> foreignThreads;

    #ifdef HAVE_PTHREAD_H

        pthread_key_t currentThreadKey;

        void* threadWorker( void* arg ) {

            ThreadProperties* properties = (ThreadProperties*)arg;

            pthread_setspecific( currentThreadKey, (void*)properties->parent );

            ThreadProperties::runCallback( properties );

            pthread_setspecific( currentThreadKey, NULL );
            pthread_detach( properties->handle );

            properties->state = Thread::TERMINATED;

            return NULL;
        }
    #else

        DWORD currentThreadKey;

        unsigned int __stdcall threadWorker( void* arg ) {
            ThreadProperties* properties = (ThreadProperties*)arg;

            ::TlsSetValue( currentThreadKey, (void*)properties->parent );

            ThreadProperties::runCallback( properties );

            ::TlsSetValue( currentThreadKey, NULL );

            properties->state = Thread::TERMINATED;

            #ifndef _WIN32_WCE
                _endthreadex( 0 );
            #else
                ExitThread( 0 );
            #endif
        }

    #endif
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ThreadProperties::id = 0;

////////////////////////////////////////////////////////////////////////////////
void Thread::initThreading() {

    // We mark the thread where Decaf's Init routine is called from as our Main Thread.
    mainThread = Thread::createForeignThreadInstance( "Main Thread" );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::shutdownThreading() {

    // Clear the Main Thread instance pointer, this indicates we are Shutdown.
    mainThread = NULL;

    // Destroy any Foreign Thread Facades that were created during runtime.
    std::vector<Thread*>::iterator iter = foreignThreads.begin();
    for( ; iter != foreignThreads.end(); ++iter ) {
        delete *iter;
    }
    foreignThreads.clear();

    #ifdef HAVE_PTHREAD_H

        // Destroy the current Thread key now, no longer needed.
        pthread_key_delete( currentThreadKey );

    #else

        // Destroy our TLS resources before we shutdown.
        ::TlsFree( currentThreadKey );

    #endif
}

////////////////////////////////////////////////////////////////////////////////
Thread* Thread::createForeignThreadInstance( const std::string& name ) {

    Thread* foreignThread = new Thread( name );

    foreignThread->properties->state = Thread::RUNNABLE;
    foreignThread->properties->priority = Thread::NORM_PRIORITY;
    foreignThread->properties->parent = NULL;

    #ifdef HAVE_PTHREAD_H

        foreignThread->properties->handle = pthread_self();

        // Create the Key used to store the Current Thread data
        pthread_key_create( &currentThreadKey, NULL );
        pthread_setspecific( currentThreadKey, foreignThread );

    #else

        foreignThread->properties->handle = ::GetCurrentThread();

        // Create the key used to store the Current Thread data
        currentThreadKey = ::TlsAlloc();
        ::TlsSetValue( currentThreadKey, foreignThread );

    #endif

    // Store the Facade for later deletion
    foreignThreads.push_back( foreignThread );

    return foreignThread;
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread() : Runnable(), properties( NULL ) {
    this->initialize( this, "" );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task ): Runnable(), properties( NULL ) {
    this->initialize( task, "" );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( const std::string& name ): Runnable(), properties( NULL ) {
    this->initialize( this, name );
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread( Runnable* task, const std::string& name ): Runnable(), properties( NULL ) {
    this->initialize( task, name );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::initialize( Runnable* task, const std::string& name ) {

    this->properties = new ThreadProperties();

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
    try{
        delete this->properties;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
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

        // The lock here acts as a gate to the newly created thread, it won't begin execution
        // until this method completes which ensures that if the main thread calls start and
        // then join but the child thread has already exited there won't be a deadlock on the
        // wait handle.  Also the new thread won't be able to update any internal data until
        // creation is complete which ensure that there is no state corruption.
        synchronized( &this->properties->mutex ) {

            #ifdef HAVE_PTHREAD_H
                int result = pthread_create( &( properties->handle ),
                                             &( properties->attributes ),
                                             threadWorker, properties );

                if( result != 0 ) {
                    throw RuntimeException(
                        __FILE__, __LINE__, "Failed to create new Thread." );
                }

            #else

                unsigned int threadId = 0;

                #ifndef _WIN32_WCE

                    properties->handle = (HANDLE)_beginthreadex(
                         NULL, (DWORD)0, threadWorker, properties, 0, &threadId );

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
     }
     DECAF_CATCH_RETHROW( IllegalThreadStateException )
     DECAF_CATCH_RETHROW( RuntimeException )
     DECAF_CATCH_EXCEPTION_CONVERT( NullPointerException, RuntimeException )
     DECAF_CATCHALL_THROW( RuntimeException )
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() throw( decaf::lang::exceptions::InterruptedException ) {

    if( this->properties->state < Thread::RUNNABLE ) {
        return;
    }

    if( this->properties->state != Thread::TERMINATED ) {

        #ifdef HAVE_PTHREAD_H
            void* theReturn = 0;
            pthread_join( properties->handle, &theReturn );
        #else
            ::WaitForSingleObject( properties->handle, INFINITE );
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

    if( this->properties->state < Thread::RUNNABLE ) {
        return;
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

    synchronized( &this->properties->mutex ) {

        if( this->properties->state < Thread::RUNNABLE ) {
            return;
        }

        this->properties->mutex.wait( millisecs, nanos );
    }
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

    if( mainThread == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "The Decaf Threading API is in a Shutdown State." );
    }

    void* result = NULL;

    #ifdef HAVE_PTHREAD_H

        result = pthread_getspecific( currentThreadKey );

    #else

        result = ::TlsGetValue( currentThreadKey );

    #endif

    // This is a foreign Thread (Not Created By Decaf) lets create a stand in Thread
    // instance so that other threads in Decaf can join it and wait on it.
    if( result == NULL ) {
        result = Thread::createForeignThreadInstance(
            std::string( "ForeignThread-" ) + Long::toString( Thread::getId() ) );
    }

    return (Thread*)result;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::park( Thread* thread ) {

    if( thread == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Null Thread Pointer Passed." );
    }

    Thread::park( thread, 0LL, 0LL );
}

////////////////////////////////////////////////////////////////////////////////
void Thread::park( Thread* thread, long long mills, long long nanos ) {

    if( thread == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Null Thread Pointer Passed." );
    }

    synchronized( &thread->properties->mutex ) {

        if( thread->properties->unparked ) {
            thread->properties->unparked = false;
            return;
        }

        thread->properties->parked = true;

        if( mills == 0 && nanos == 0 ) {
            thread->properties->mutex.wait();
        } else {
            thread->properties->mutex.wait( mills, (int)nanos );
        }

        thread->properties->parked = false;

        // consume the unparked token.
        thread->properties->unparked = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Thread::unpark( Thread* thread ) {

    if( thread == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Null Thread Pointer Passed." );
    }

    synchronized( &thread->properties->mutex ) {

        // Set the unparked token, if the thread is parked it will consume
        // it when it resumes, otherwise the next call to park will consume
        // it without needing to actually wait.
        thread->properties->unparked = true;

        // If the thread is actually parked then we send it a signal so
        // that it will resume.
        if( thread->properties->parked ) {
            thread->properties->mutex.notifyAll();
        }
    }
}
