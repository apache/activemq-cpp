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

#ifndef _ACTIVEMQ_CORE_FIFOMESSAGEDISPATCHCHANNEL_H_
#define _ACTIVEMQ_CORE_FIFOMESSAGEDISPATCHCHANNEL_H_

#include <activemq/util/Config.h>
#include <activemq/core/MessageDispatchChannel.h>

#include <decaf/util/LinkedList.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace core {

    class AMQCPP_API FifoMessageDispatchChannel : public MessageDispatchChannel {
    private:

        bool closed;
        bool running;

        mutable decaf::util::LinkedList< Pointer<MessageDispatch> > channel;

    private:

        FifoMessageDispatchChannel(const FifoMessageDispatchChannel&);
        FifoMessageDispatchChannel& operator=(const FifoMessageDispatchChannel&);

    public:

        FifoMessageDispatchChannel();

        virtual ~FifoMessageDispatchChannel();

        virtual void enqueue(const Pointer<MessageDispatch>& message);

        virtual void enqueueFirst(const Pointer<MessageDispatch>& message);

        virtual bool isEmpty() const;

        virtual bool isClosed() const {
            return this->closed;
        }

        virtual bool isRunning() const {
            return this->running;
        }

        virtual Pointer<MessageDispatch> dequeue(long long timeout);

        virtual Pointer<MessageDispatch> dequeueNoWait();

        virtual Pointer<MessageDispatch> peek() const;

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual void clear();

        virtual int size() const;

        virtual std::vector<Pointer<MessageDispatch> > removeAll();

    public:

        virtual void lock() {
            channel.lock();
        }

        virtual bool tryLock() {
            return channel.tryLock();
        }

        virtual void unlock() {
            channel.unlock();
        }

        virtual void wait() {
            channel.wait();
        }

        virtual void wait(long long millisecs) {
            channel.wait(millisecs);
        }

        virtual void wait(long long millisecs, int nanos) {
            channel.wait(millisecs, nanos);
        }

        virtual void notify() {
            channel.notify();
        }

        virtual void notifyAll() {
            channel.notifyAll();
        }

    };

}}

#endif /* _ACTIVEMQ_CORE_FIFOMESSAGEDISPATCHCHANNEL_H_ */
