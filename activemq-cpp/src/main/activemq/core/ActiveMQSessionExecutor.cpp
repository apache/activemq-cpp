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
 
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
using namespace activemq::exceptions;

//////////////////////////////////////////////////////////////////////////////// 
ActiveMQSessionExecutor::ActiveMQSessionExecutor( ActiveMQSession* session ) {
    this->session = session;
    this->started = false;
    this->thread = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionExecutor::~ActiveMQSessionExecutor() {
    
    // Stop the thread if it's running.
    stop();
    
    // Empty the message queue and destroy any remaining messages.
    clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::execute( DispatchData& data ) {
        
    // Add the data to the queue.
    synchronized( &messageQueue ) {
        messageQueue.push(data);
        wakeup();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::executeFirst( DispatchData& data ) {
        
    // Add the data to the front of the queue.
    synchronized( &messageQueue ) {
        messageQueue.enqueueFront(data);
        wakeup();
    }
}
    
////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::start() {
    
    synchronized( &messageQueue ) {
        started = true;
        
        // Don't create the thread unless we need to.
        if( thread == NULL ) {
            thread = new Thread( this );
            thread->start();
        }
        
        wakeup();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::stop() {
    
    synchronized( &messageQueue ) {
        
        started = false;
        wakeup();
    }
    
    if( thread != NULL ) {
        thread->join();
        delete thread;
        thread = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::clear() {
    
    synchronized( &messageQueue ) {
        
        while( !messageQueue.empty() ) {
            DispatchData data = messageQueue.pop();
            delete data.getMessage();
        }
        
        wakeup();
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
    } catch( std::exception& ex ) {
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
        
        while( started ) {
            
            // Dispatch all currently available messages.
            dispatchAll();
            
            synchronized( &messageQueue ) {
                
                if( messageQueue.empty() && started ) {
            
                    // Wait for more data or to be woken up.
                    messageQueue.wait();
                }
            }
        }
        
    } catch( ActiveMQException& ex ) {
        ex.setMark(__FILE__, __LINE__ );
        session->fire( ex );
    } catch( std::exception& stdex ) {
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
    std::vector<DispatchData> dataList;
    synchronized( &messageQueue ) {
        
        dataList = messageQueue.toArray();
        messageQueue.clear();
    }
    
    // Dispatch all currently available messages.
    for( unsigned int ix=0; ix<dataList.size(); ++ix ) {
        DispatchData& data = dataList[ix];
        dispatch( data );
    }
}
 
////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionExecutor::wakeup() {
 
    synchronized( &messageQueue ) {
        messageQueue.notifyAll();
    }
}

