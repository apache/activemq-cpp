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

#include "ActiveMQSessionExecutor.h"

#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/FifoMessageDispatchChannel.h>
#include <activemq/core/SimplePriorityMessageDispatchChannel.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/threads/DedicatedTaskRunner.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::threads;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionExecutor::ActiveMQSessionExecutor( ActiveMQSession* session ) :
            session( session ), messageQueue(), taskRunner() {

    if( this->session->getConnection()->isMessagePrioritySupported() ) {
        this->messageQueue.reset( new SimplePriorityMessageDispatchChannel() );
    } else {
        this->messageQueue.reset( new FifoMessageDispatchChannel() );
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionExecutor::~ActiveMQSessionExecutor() {

    try {

        // Terminate the thread.
        stop();

        // Close out the Message Channel.
        close();

        // Empty the message queue and destroy any remaining messages.
        clear();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::execute( const Pointer<MessageDispatch>& dispatch ) {

    // Add the data to the queue.
    this->messageQueue->enqueue( dispatch );
    this->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::executeFirst( const Pointer<MessageDispatch>& dispatch ) {

    // Add the data to the queue.
    this->messageQueue->enqueueFirst( dispatch );
    this->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::wakeup() {

    Pointer<TaskRunner> taskRunner = this->taskRunner;
    synchronized( messageQueue.get() ) {
        if( this->taskRunner == NULL ) {
            this->taskRunner.reset( new DedicatedTaskRunner( this ) );
        }

        taskRunner = this->taskRunner;
    }

    taskRunner->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::start() {

    if( !messageQueue->isRunning() ) {

        messageQueue->start();
        if( hasUncomsumedMessages() ) {
            this->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::stop() {

    if( messageQueue->isRunning() ) {
        messageQueue->stop();
        Pointer<TaskRunner> taskRunner = this->taskRunner;
        if( taskRunner != NULL ) {
            this->taskRunner.reset( NULL );
            taskRunner->shutdown();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::dispatch( const Pointer<MessageDispatch>& dispatch ) {

    try {

        ActiveMQConsumer* consumer = NULL;

        synchronized( &( this->session->consumers ) ) {
            if( this->session->consumers.containsKey( dispatch->getConsumerId() ) ) {
                consumer = this->session->consumers.get( dispatch->getConsumerId() );
            }
        }

        // If the consumer is not available, just ignore the message.
        // Otherwise, dispatch the message to the consumer.
        if( consumer != NULL ) {
            consumer->dispatch( dispatch );
        }

    } catch( decaf::lang::Exception& ex ) {
        ex.setMark(__FILE__, __LINE__ );
        ex.printStackTrace();
    } catch( std::exception& ex ) {
        ActiveMQException amqex( __FILE__, __LINE__, ex.what() );
        amqex.printStackTrace();
    } catch( ... ) {
        ActiveMQException amqex( __FILE__, __LINE__, "caught unknown exception" );
        amqex.printStackTrace();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSessionExecutor::iterate() {

    try {

        synchronized( &( this->session->consumers ) ) {

            std::vector<ActiveMQConsumer*> consumers = this->session->consumers.values();
            std::vector<ActiveMQConsumer*>::iterator iter = consumers.begin();

            // Deliver any messages queued on the consumer to their listeners.
            for( ; iter != consumers.end(); ++iter ) {
                if( (*iter)->iterate() ) {
                    return true;
                }
            }
        }

        // No messages left queued on the listeners.. so now dispatch messages
        // queued on the session
        Pointer<MessageDispatch> message = messageQueue->dequeueNoWait();
        if( message != NULL ) {
            dispatch( message );
            return !messageQueue->isEmpty();
        }

        return false;

    } catch( decaf::lang::Exception& ex ) {
        ex.setMark(__FILE__, __LINE__ );
        session->fire( ex );
        return true;
    } catch( std::exception& stdex ) {
        ActiveMQException ex( __FILE__, __LINE__, stdex.what() );
        session->fire( ex );
        return true;
    } catch( ... ) {
        ActiveMQException ex(__FILE__, __LINE__, "caught unknown exception" );
        session->fire( ex );
        return true;
    }
}
