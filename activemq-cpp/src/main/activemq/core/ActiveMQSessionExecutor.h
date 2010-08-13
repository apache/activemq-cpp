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

#include <activemq/util/Config.h>
#include <activemq/core/MessageDispatchChannel.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/threads/Task.h>
#include <activemq/threads/TaskRunner.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using activemq::commands::MessageDispatch;

    class ActiveMQSession;
    class ActiveMQConsumer;

    /**
     * Delegate dispatcher for a single session.  Contains a thread
     * to provide for asynchronous dispatching.
     */
    class AMQCPP_API ActiveMQSessionExecutor : activemq::threads::Task {
    private:

        /** Session that is this executors parent. */
        ActiveMQSession* session;

        /** The Channel that holds the waiting Messages for Dispatching. */
        Pointer<MessageDispatchChannel> messageQueue;

        /** The Dispatcher TaskRunner */
        Pointer<activemq::threads::TaskRunner> taskRunner;

    private:

        ActiveMQSessionExecutor( const ActiveMQSessionExecutor& );
        ActiveMQSessionExecutor& operator= ( const ActiveMQSessionExecutor& );

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
        virtual void execute( const Pointer<MessageDispatch>& data );

        /**
         * Executes the dispatch.  Adds the given data to the
         * beginning of the queue.
         * @param data - the data to be dispatched.
         */
        virtual void executeFirst( const Pointer<MessageDispatch>& data );

        /**
         * Removes all messages in the Dispatch Channel so that non are delivered.
         */
        virtual void clearMessagesInProgress() {
            this->messageQueue->clear();
        }

        /**
         * @return true if there are any pending messages in the dispatch channel.
         */
        virtual bool hasUncomsumedMessages() const {
            return !messageQueue->isClosed() && messageQueue->isRunning() && !messageQueue->isEmpty();
        }

        /**
         * wakeup this executer and dispatch any pending messages.
         */
        virtual void wakeup();

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
        virtual void close() {
            this->messageQueue->close();
        }

        /**
         * @return true indicates if the executor is started
         */
        virtual bool isRunning() const {
            return this->messageQueue->isRunning();
        }

        /**
         * @return true if there are no messages in the Dispatch Channel.
         */
        virtual bool isEmpty() {
            return messageQueue->isEmpty();
        }

        /**
         * Removes all queued messages and destroys them.
         */
        virtual void clear() {
            this->messageQueue->clear();
        }

        /**
         * Iterates on the MessageDispatchChannel sending all pending messages
         * to the Consumers they are destined for.
         *
         * @return false if there are no more messages to dispatch.
         */
        virtual bool iterate();

        /**
         * @returns a vector containing all the unconsumed messages, this clears the
         *          Message Dispatch Channel when called.
         */
        std::vector< Pointer<MessageDispatch> > getUnconsumedMessages() {
            return messageQueue->removeAll();
        }

    private:

        /**
         * Dispatches a message to a particular consumer.
         * @param data - The message to be dispatched.
         */
        virtual void dispatch( const Pointer<MessageDispatch>& data );

    };

}}

#endif /*ACTIVEMQ_CORE_ACTIVEMQSESSIONEXECUTOR_*/
