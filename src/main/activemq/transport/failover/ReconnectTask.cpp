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

#include "ReconnectTask.h"
#include "FailoverTransport.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ReconnectTask::ReconnectTask( FailoverTransport* parent ) : parent( parent ) {

    if( this->parent == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Parent FailoverTransport passed was null" );
    }

    this->threadTerminated = false;
    this->pending = false;
    this->shutDown = false;

    this->start();
}

////////////////////////////////////////////////////////////////////////////////
ReconnectTask::~ReconnectTask() {
    try{
        this->shutdown();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ReconnectTask::shutdown( unsigned int timeout ) {

    synchronized( &mutex ) {
        shutDown = true;
        pending = true;
        mutex.notifyAll();

        // Wait till the thread stops ( no need to wait if shutdown
        // is called from thread that is shutting down)
        if( !threadTerminated ) {
            mutex.wait( timeout );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ReconnectTask::wakeup() {

    synchronized( &mutex ) {
        if( shutDown) {
            return;
        }
        pending = true;
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ReconnectTask::iterate() {

    bool result = false;
    bool buildBackup = true;

    synchronized( &( parent->backupMutex ) ) {
        if( parent->connectedTransport == NULL && !parent->closed ) {
            result = parent->doReconnect();
            buildBackup = false;
        }
    }

    if( buildBackup ) {
        parent->buildBackups();
    } else {
        // build backups on the next iteration
        result = true;
        this->wakeup();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void ReconnectTask::run() {

    try {

        while( true ) {

            synchronized( &mutex ) {
                pending = false;
                if( shutDown ) {
                    return;
                }
            }

            if( !this->iterate() ) {

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
