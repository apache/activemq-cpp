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
#include <decaf/internal/util/concurrent/Threading.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalThreadStateException.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/Executors.h>

#include <vector>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::internal::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace lang{

    class ThreadProperties {
    private:

        ThreadProperties( const ThreadProperties& );
        ThreadProperties& operator= ( const ThreadProperties& );

    public:

        Runnable* task;
        ThreadHandle* handle;
        Thread::UncaughtExceptionHandler* exHandler;
        static unsigned int id;
        static Thread::UncaughtExceptionHandler* defaultHandler;

    public:

        ThreadProperties() : task(NULL), handle(NULL), exHandler(NULL) {}

    };

}}

////////////////////////////////////////////////////////////////////////////////
unsigned int ThreadProperties::id = 0;
Thread::UncaughtExceptionHandler* ThreadProperties::defaultHandler = NULL;

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(ThreadHandle* handle) : Runnable(), properties(NULL) {

    this->properties = new ThreadProperties();
    this->properties->handle = handle;
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread() : Runnable(), properties(NULL) {
    this->initializeSelf(NULL, "", -1);
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(Runnable* task) : Runnable(), properties(NULL) {
    this->initializeSelf(task, "", -1);
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(const std::string& name) : Runnable(), properties(NULL) {
    this->initializeSelf(NULL, name, -1);
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(Runnable* task, const std::string& name) : Runnable(), properties( NULL ) {
    this->initializeSelf(task, name, -1);
}

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(Runnable* task, const std::string& name, long long stackSize) : Runnable(), properties( NULL ) {
    this->initializeSelf(task, name, stackSize);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::initializeSelf(Runnable* task, const std::string& name, long long stackSize) {

    std::string threadName = name;

    if( threadName.empty() ) {
        threadName = std::string( "Thread-" ) + Integer::toString( ++ThreadProperties::id );
    } else {
        threadName = name;
    }

    this->properties = new ThreadProperties();
    this->properties->handle =
        Threading::createNewThread(this, threadName.c_str(), stackSize);
    this->properties->task = task;
}

////////////////////////////////////////////////////////////////////////////////
Thread::~Thread() {
    try {
        Threading::destroyThread(this->properties->handle);
        delete this->properties;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void Thread::run() {

    if (this->properties->task != NULL) {
        this->properties->task->run();
    }
}

////////////////////////////////////////////////////////////////////////////////
void Thread::start() {

    try {

        if( Threading::getThreadState(this->properties->handle) > Thread::NEW ) {
            throw IllegalThreadStateException(
                __FILE__, __LINE__,
                "Thread::start - Thread already started");
        }

        Threading::start(this->properties->handle);
     }
     DECAF_CATCH_RETHROW( IllegalThreadStateException )
     DECAF_CATCH_RETHROW( RuntimeException )
     DECAF_CATCH_EXCEPTION_CONVERT( NullPointerException, RuntimeException )
     DECAF_CATCHALL_THROW( RuntimeException )
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join() {
    Threading::join(this->properties->handle, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join(long long millisecs) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs ) - Value given {%d} is less than 0", millisecs );
    }

    Threading::join(this->properties->handle, millisecs, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::join(long long millisecs, int nanos) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs, nanos ) - Value given {%d} is less than 0", millisecs );
    }

    if( nanos < 0 || nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::join( millisecs, nanos ) - Nanoseconds must be in range [0...999999]" );
    }

    Threading::join(this->properties->handle, millisecs, nanos);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep(long long millisecs) {
    Threading::sleep(millisecs, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::sleep(long long millisecs, int nanos) {

    if( millisecs < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::sleep( millisecs, nanos ) - Value given {%d} is less than 0", millisecs );
    }

    if( nanos < 0 || nanos > 999999 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::sleep( millisecs, nanos ) - Nanoseconds must be in range [0...999999]" );
    }


    Threading::sleep(millisecs, nanos);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::yield() {
    Threading::yeild();
}

////////////////////////////////////////////////////////////////////////////////
long long Thread::getId() const {
    return Threading::getThreadId(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setName(const std::string& name) {
    Threading::setThreadName(this->properties->handle, name.c_str());
}

////////////////////////////////////////////////////////////////////////////////
std::string Thread::getName() const {
    return Threading::getThreadName(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setPriority(int value) {

    if (value < Thread::MIN_PRIORITY || value > Thread::MAX_PRIORITY) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Thread::setPriority - Specified value {%d} is out of range", value );
    }

    Threading::setThreadPriority(this->properties->handle, value);
}

////////////////////////////////////////////////////////////////////////////////
int Thread::getPriority() const {
    return Threading::getThreadPriority(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setUncaughtExceptionHandler(UncaughtExceptionHandler* handler) {
    this->properties->exHandler = handler;
}

////////////////////////////////////////////////////////////////////////////////
Thread::UncaughtExceptionHandler* Thread::getUncaughtExceptionHandler() const {
    return this->properties->exHandler;
}

////////////////////////////////////////////////////////////////////////////////
Thread::UncaughtExceptionHandler* Thread::getDefaultUncaughtExceptionHandler() {
    return ThreadProperties::defaultHandler;
}

////////////////////////////////////////////////////////////////////////////////
void Thread::setDefaultUncaughtExceptionHandler(Thread::UncaughtExceptionHandler* handler) {
    ThreadProperties::defaultHandler = handler;
}

////////////////////////////////////////////////////////////////////////////////
std::string Thread::toString() const {

    return std::string(Threading::getThreadName(this->properties->handle)) + ": Priority=" +
           Integer::toString(Threading::getThreadPriority(this->properties->handle)) +
           ", ThreadID=" + Long::toString( Threading::getThreadId(this->properties->handle));
}

////////////////////////////////////////////////////////////////////////////////
bool Thread::isAlive() const {
    return Threading::isThreadAlive(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
Thread::State Thread::getState() const {
    return Threading::getThreadState(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
Thread* Thread::currentThread() {
    return Threading::getCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////
void Thread::interrupt() {
    Threading::interrupt(this->properties->handle);
}

////////////////////////////////////////////////////////////////////////////////
bool Thread::interrupted() {
    return Threading::interrupted();
}

////////////////////////////////////////////////////////////////////////////////
bool Thread::isInterrupted() const {
    return Threading::isInterrupted(this->properties->handle, false);
}

////////////////////////////////////////////////////////////////////////////////
ThreadHandle* Thread::getHandle() const {
    return this->properties->handle;
}
