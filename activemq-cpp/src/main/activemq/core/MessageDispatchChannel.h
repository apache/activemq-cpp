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

#ifndef _ACTIVEMQ_CORE_MESSAGEDISPATCHCHANNEL_H_
#define _ACTIVEMQ_CORE_MESSAGEDISPATCHCHANNEL_H_

#include <activemq/util/Config.h>
#include <activemq/commands/MessageDispatch.h>

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;
    using activemq::commands::MessageDispatch;

    class AMQCPP_API MessageDispatchChannel : public decaf::util::concurrent::Synchronizable {
    public:

        virtual ~MessageDispatchChannel() {}

        /**
         * Add a Message to the Channel behind all pending message.
         *
         * @param message - The message to add to the Channel.
         */
        virtual void enqueue( const Pointer<MessageDispatch>& message ) = 0;

        /**
         * Add a message to the front of the Channel.
         *
         * @param message - The Message to add to the front of the Channel.
         */
        virtual void enqueueFirst( const Pointer<MessageDispatch>& message ) = 0;

        /**
         * @return true if there are no messages in the Channel.
         */
        virtual bool isEmpty() const = 0;

        /**
         * @return has the Queue been closed.
         */
        virtual bool isClosed() const = 0;

        /**
         * @return true if the Channel currently running and will dequeue message.
         */
        virtual bool isRunning() const = 0;

        /**
         * Used to get an enqueued message. The amount of time this method blocks is
         * based on the timeout value. - if timeout==-1 then it blocks until a
         * message is received. - if timeout==0 then it it tries to not block at
         * all, it returns a message if it is available - if timeout>0 then it
         * blocks up to timeout amount of time. Expired messages will consumed by
         * this method.
         *
         * @return null if we timeout or if the consumer is closed.
         * @throws ActiveMQException
         */
        virtual Pointer<MessageDispatch> dequeue( long long timeout ) = 0;

        /**
         * Used to get an enqueued message if there is one queued right now.  If there is
         * no waiting message than this method returns Null.
         *
         * @return a message if there is one in the queue.
         */
        virtual Pointer<MessageDispatch> dequeueNoWait() = 0;

        /**
         * Peek in the Queue and return the first message in the Channel without removing
         * it from the channel.
         *
         * @return a message if there is one in the queue.
         */
        virtual Pointer<MessageDispatch> peek() const = 0;

        /**
         * Starts dispatch of messages from the Channel.
         */
        virtual void start() = 0;

        /**
         * Stops dispatch of message from the Channel.
         */
        virtual void stop() = 0;

        /**
         * Close this channel no messages will be dispatched after this method is called.
         */
        virtual void close() = 0;

        /**
         * Clear the Channel, all pending messages are removed.
         */
        virtual void clear() = 0;

        /**
         * @return the number of Messages currently in the Channel.
         */
        virtual int size() const = 0;

        /**
         * Remove all messages that are currently in the Channel and return them as
         * a list of Messages.
         *
         * @return a list of Messages that was previously in the Channel.
         */
        virtual std::vector< Pointer<MessageDispatch> > removeAll() = 0;

    };

}}

#endif /* _ACTIVEMQ_CORE_MESSAGEDISPATCHCHANNEL_H_ */
