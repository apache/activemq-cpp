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

#ifndef ACTIVEMQ_CORE_ACTIVEMQSESSIONEXECUTOR_
#define ACTIVEMQ_CORE_ACTIVEMQSESSIONEXECUTOR_

#include <activemq/core/Dispatcher.h>
#include <activemq/concurrent/Runnable.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Mutex.h>
#include <vector>
#include <list>

namespace activemq{
namespace core{
  
    class ActiveMQSession;
    class ActiveMQConsumer;
  
    /**
     * Delegate dispatcher for a single session.  Contains a thread
     * to provide for asynchronous dispatching.
     */
    class ActiveMQSessionExecutor : 
        public Dispatcher, 
        public concurrent::Runnable 
    {   
    private:
        
        ActiveMQSession* session;
        std::list<DispatchData> messageQueue;        
        concurrent::Thread* thread;
        concurrent::Mutex mutex;
        bool started;
        bool closed;
        
    public:
    
        /**
         * Creates an un-started executor for the given session.
         */
        ActiveMQSessionExecutor( ActiveMQSession* session );
        
        /**
         * Calls stop() then clear().
         */
        virtual ~ActiveMQSessionExecutor();
        
        /**
         * Executes the dispatch.  Adds the given data to the
         * end of the queue.
         * @param data - the data to be dispatched.
         */
        virtual void execute( DispatchData& data );
        
        /**
         * Executes the dispatch.  Adds the given data to the
         * beginning of the queue.
         * @param data - the data to be dispatched.
         */
        virtual void executeFirst( DispatchData& data );
            
        /**
         * Removes all messages for the given consumer from the queue and
         * returns them.
         * @param consumer the subject consmer
         * @return all messages that were queued for the consumer.
         */
        virtual std::vector<ActiveMQMessage*> purgeConsumerMessages( ActiveMQConsumer* consumer );
        
        /**
         * Starts the dispatching.
         */
        virtual void start();
        
        /**
         * Stops dispatching.
         */
        virtual void stop();
        
        /**
         * Terminates the dispatching thread.  Once this is called, the executor is no longer
         * usable.
         */         
        virtual void close();
        
        /**
         * Indicates if the executor is started
         */
        virtual bool isStarted() const {
            return started;
        }
        
        /**
         * Removes all queued messgaes and destroys them.
         */
        virtual void clear();
            
        /**
         * Dispatches a message to a particular consumer.
         * @param consumer - The consumer to dispatch to.
         * @param msg - The message to be dispatched.
         */
        virtual void dispatch( DispatchData& data );
            
        /**
         * Run method - called by the Thread class in the context
         * of the thread.
         */
        virtual void run();
        
    private:
    
        /**
         * Dispatches all messages currently in the queue.
         */
        void dispatchAll();
    };
    
}}

#endif /*ACTIVEMQ_CORE_ACTIVEMQSESSIONEXECUTOR_*/
