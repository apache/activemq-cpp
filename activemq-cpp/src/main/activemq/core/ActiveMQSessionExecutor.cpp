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
#include "ActiveMQSession.h"
#include "ActiveMQMessage.h"
#include "ActiveMQConsumer.h"
#include <activemq/connector/ConsumerInfo.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionExecutor::ActiveMQSessionExecutor( ActiveMQSession* session ) {

    this->session = session;
    this->closed = false;
    this->started = false;
    this->thread = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionExecutor::~ActiveMQSessionExecutor() {

    try {

        // Terminate the thread.
        close();
        
        // Empty the message queue and destroy any remaining messages.
        clear();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::close() {
    
    synchronized( &mutex ) {

        closed = true;
        mutex.notifyAll();
    }

    if( thread != NULL ) {
        thread->join();
        delete thread;
        thread = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::execute( DispatchData& data ) {

    // Add the data to the queue.
    synchronized( &mutex ) {
        messageQueue.push_back( data );
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::executeFirst( DispatchData& data ) {

    // Add the data to the front of the queue.
    synchronized( &mutex ) {
        messageQueue.push_front( data );
        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
vector<ActiveMQMessage*> ActiveMQSessionExecutor::purgeConsumerMessages( 
    ActiveMQConsumer* consumer )
{
    vector<ActiveMQMessage*> retVal;
    
    const connector::ConsumerInfo* consumerInfo = consumer->getConsumerInfo();
    
    synchronized( &mutex ) {
        
        list<DispatchData>::iterator iter = messageQueue.begin();
        while( iter != messageQueue.end() ) {
            list<DispatchData>::iterator currentIter = iter;
            DispatchData& dispatchData = *iter++;
            if( consumerInfo == dispatchData.getConsumer() ||
                consumerInfo->getConsumerId() == dispatchData.getConsumer()->getConsumerId() ) 
            {
                retVal.push_back( dispatchData.getMessage() );
                messageQueue.erase(currentIter);
            }
        }
    }
    
    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::start() {
    
    synchronized( &mutex ) {
        
        if( closed || started ) { 
            return;
        }
    
        started = true;

        // Don't create the thread unless we need to.
        if( thread == NULL ) {
            thread = new Thread( this );
            thread->start();
        }

        mutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::stop() {
    
    synchronized( &mutex ) {
        
        if( closed || !started ) {
            return;
        }

        // Set the state to stopped.
        started = false;
        
        // Wakeup the thread so that it can acknowledge the stop request.
        mutex.notifyAll();
        
        // Wait for the thread to notify us that it has acknowledged
        // the stop request.
        mutex.wait();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::clear() {

    synchronized( &mutex ) {

        list<DispatchData>::iterator iter = messageQueue.begin();
        while( iter != messageQueue.end() ) {
            DispatchData data = *iter++;
            delete data.getMessage();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::dispatch( DispatchData& data ) {

    try {

        ActiveMQConsumer* consumer = NULL;
        util::Map<long long, ActiveMQConsumer*>& consumers = session->getConsumers();

        synchronized(&consumers) {
            consumer = consumers.getValue( data.getConsumer()->getConsumerId() );
        }

        // If the consumer is not available, just delete the message.
        // Otherwise, dispatch the message to the consumer.
        if( consumer == NULL ) {
            delete data.getMessage();
        } else {
            consumer->dispatch( data );
        }

    } catch( ActiveMQException& ex ) {
        ex.setMark(__FILE__, __LINE__ );
        ex.printStackTrace();
    } catch( exception& ex ) {
        ActiveMQException amqex( __FILE__, __LINE__, ex.what() );
        amqex.printStackTrace();
    } catch( ... ) {
        ActiveMQException amqex( __FILE__, __LINE__, "caught unknown exception" );
        amqex.printStackTrace();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::run() {

    try {

        while( true ) {

            // Dispatch all currently available messages.
            dispatchAll();

            synchronized( &mutex ) {
                
                // If we're closing down, exit the thread.
                if( closed ) {
                    return;
                }
                
                // When told to stop, the calling thread will wait for a
                // responding notification, indicating that we have acknowledged
                // the stop command.
                if( !started ) {
                    mutex.notifyAll();
                }

                if( messageQueue.empty() || !started ) {

                    // Wait for more data or to be woken up.
                    mutex.wait();
                }
            }
        }

    } catch( ActiveMQException& ex ) {
        ex.setMark(__FILE__, __LINE__ );
        session->fire( ex );
    } catch( exception& stdex ) {
        ActiveMQException ex( __FILE__, __LINE__, stdex.what() );
        session->fire( ex );
    } catch( ... ) {
        ActiveMQException ex(__FILE__, __LINE__, "caught unknown exception" );
        session->fire( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::dispatchAll() {
    
    // Take out all of the dispatch data currently in the array.
    list<DispatchData> dataList;
    synchronized( &mutex ) {
        
        // When told to stop, the calling thread will wait for a
        // responding notification, indicating that we have acknowledged
        // the stop command.
        if( !started ) {
            mutex.notifyAll();
        }
                
        if( !started || closed ) {
            return;
        }

        dataList = messageQueue;
        messageQueue.clear();
    }
    
    // Dispatch all currently available messages.
    list<DispatchData>::iterator iter = dataList.begin();
    while( iter != dataList.end() ) {
        DispatchData& data = *iter++;
        dispatch( data );
    }
}


