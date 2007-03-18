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
#include <activemq/util/Queue.h>

namespace activemq{
namespace core{
  
    class ActiveMQSession;
  
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
        util::Queue<DispatchData> messageQueue;
        bool started;
        concurrent::Thread* thread;
        
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
         * Starts the dispatching.
         */
        virtual void start();
        
        /**
         * Stops dispatching.
         */
        virtual void stop();
        
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
         * Depending on whether or not the session is async,
         * notifies the thread or simply dispatches all available
         * messages synchronously.
         */
        virtual void wakeup();
            
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
