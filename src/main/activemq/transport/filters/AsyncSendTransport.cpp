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

#include "AsyncSendTransport.h"

using namespace std;
using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AsyncSendTransport::AsyncSendTransport( Transport* next, bool own )
 : TransportFilter( next, own ) {

    this->closed = true;
    this->asyncThread = NULL;
    this->maxBacklog = 0;
}

////////////////////////////////////////////////////////////////////////////////
AsyncSendTransport::AsyncSendTransport( Transport* next, unsigned int maxBacklog, bool own )
 : TransportFilter( next, own ) {

    this->closed = true;
    this->asyncThread = NULL;
    this->maxBacklog = maxBacklog;
}

////////////////////////////////////////////////////////////////////////////////
AsyncSendTransport::~AsyncSendTransport() {
    try {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::oneway( Command* command )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    try{

        // Put it in the send queue, thread will dispatch it.  We clone it
        // in case the client deletes their copy before we get a chance to
        // send it.
        synchronized( &msgQueue ) {

            while( this->maxBacklog != 0 && msgQueue.size() >= this->maxBacklog ) {
                msgQueue.wait();
            }

            msgQueue.push( command->cloneCommand() );
            msgQueue.notifyAll();
        }
    }
    AMQ_CATCH_RETHROW( CommandIOException )
    AMQ_CATCH_RETHROW( UnsupportedOperationException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, CommandIOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, CommandIOException )
    AMQ_CATCHALL_THROW( CommandIOException )
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::start() throw( cms::CMSException ) {

    try {

        this->closed = false;

        // Kill the thread
        this->startThread();

        next->start();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::close() throw( cms::CMSException ) {

    try {

        this->closed = true;

        // Kill the thread
        this->stopThread();

        next->close();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::run() {

    try{

        while( !closed ) {

            Command* command = NULL;

            synchronized( &msgQueue ) {

                // Gaurd against spurious wakeup or race to sync lock
                // also if the listner has been unregistered we don't
                // have anyone to notify, so we wait till a new one is
                // registered, and then we will deliver the backlog
                while( msgQueue.empty() ) {

                    if( closed ) {
                        break;
                    }
                    msgQueue.wait();
                }

                // don't want to process messages if we are shutting down.
                if( closed ) {
                    return;
                }

                // get the data
                command = msgQueue.pop();

                // Notify the callers that we now have room for at least one more
                // message to send.
                msgQueue.notifyAll();
            }

            // Dispatch the message
            next->oneway( command );

            // Destroy Our copy of the message
            delete command;
        }
    } catch(...) {
        this->fire( ActiveMQException(
            __FILE__, __LINE__,
            "AsyncSendTransport::run - "
            "Connector threw an unknown Exception, recovering..." ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::startThread() throw ( ActiveMQException ) {

    try{

        // Start the thread, if it's not already started.
        if( asyncThread == NULL ) {
            asyncThread = new Thread( this );
            asyncThread->start();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::stopThread() throw ( ActiveMQException ) {

    try{

        // if the thread is running signal it to quit and then
        // wait for run to return so thread can die
        if( asyncThread != NULL ) {

            synchronized( &msgQueue ) {
                // Force a wakeup if run is in a wait.
                msgQueue.notifyAll();
            }

            // Wait for it to die and then delete it.
            asyncThread->join();
            delete asyncThread;
            asyncThread = NULL;
        }

        // Clean all the messages up
        purgeMessages();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void AsyncSendTransport::purgeMessages() throw ( ActiveMQException ) {

    try{

        synchronized( &msgQueue ) {

            while( !msgQueue.empty() ) {

                // destroy these messages if this is not a transacted
                // session, if it is then the tranasction will clean
                // the messages up.
                delete msgQueue.pop();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
