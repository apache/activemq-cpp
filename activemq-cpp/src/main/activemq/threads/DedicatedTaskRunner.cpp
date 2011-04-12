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

#include "DedicatedTaskRunner.h"

#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DedicatedTaskRunner::DedicatedTaskRunner( Task* task ) :
    mutex(), thread(), threadTerminated(false), pending(false), shutDown(false), task(task) {

    if( this->task == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Task passed was null" );
    }

    this->thread.reset( new Thread( this ) );
    this->thread->start();
}

////////////////////////////////////////////////////////////////////////////////
DedicatedTaskRunner::~DedicatedTaskRunner() {
    try{
        this->shutdown();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::shutdown( unsigned int timeout ) {

    synchronized( &mutex ) {
        shutDown = true;
        pending = true;
        mutex.notifyAll();

        // Wait till the thread stops ( no need to wait if shutdown
        // is called from thread that is shutting down)
        if( Thread::currentThread() != this->thread.get() && !threadTerminated ) {
            mutex.wait( timeout );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::shutdown() {

    synchronized( &mutex ) {
        shutDown = true;
        pending = true;
        mutex.notifyAll();
    }

    // Wait till the thread stops ( no need to wait if shutdown
    // is called from thread that is shutting down)
    if( Thread::currentThread() != this->thread.get() && !threadTerminated ) {
        this->thread->join();
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::wakeup() {

    synchronized( &mutex ) {
        if( shutDown) {
            return;
        }
        pending = true;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void DedicatedTaskRunner::run() {

    try {

        while( true ) {

            synchronized( &mutex ) {
                pending = false;
                if( shutDown ) {
                    return;
                }
            }

            if( !this->task->iterate() ) {

                // wait to be notified.
                synchronized( &mutex ) {
                    if( shutDown ) {
                        return;
                    }
                    while( !pending ) {
                        mutex.wait();
                    }
                }
            }

        }
    }
    AMQ_CATCH_NOTHROW( Exception )
    AMQ_CATCHALL_NOTHROW()

    // Make sure we notify any waiting threads that thread
    // has terminated.
    synchronized( &mutex ) {
        threadTerminated = true;
        mutex.notifyAll();
    }
}
