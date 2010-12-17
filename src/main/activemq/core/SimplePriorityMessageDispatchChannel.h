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

#ifndef _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNEL_H_
#define _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNEL_H_

#include <activemq/util/Config.h>
#include <activemq/core/MessageDispatchChannel.h>

#include <decaf/util/LinkedList.h>
#include <decaf/lang/ArrayPointer.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq {
namespace core {

    using decaf::lang::ArrayPointer;

    class AMQCPP_API SimplePriorityMessageDispatchChannel : public MessageDispatchChannel {
    private:

        static const int MAX_PRIORITIES;

        bool closed;
        bool running;

        mutable decaf::util::concurrent::Mutex mutex;

        mutable ArrayPointer< decaf::util::LinkedList< Pointer<MessageDispatch> > > channels;

        int enqueued;

    private:

        SimplePriorityMessageDispatchChannel( const SimplePriorityMessageDispatchChannel& );
        SimplePriorityMessageDispatchChannel& operator= ( const SimplePriorityMessageDispatchChannel& );

    public:

        SimplePriorityMessageDispatchChannel();
        virtual ~SimplePriorityMessageDispatchChannel();

        virtual void enqueue( const Pointer<MessageDispatch>& message );

        virtual void enqueueFirst( const Pointer<MessageDispatch>& message );

        virtual bool isEmpty() const;

        virtual bool isClosed() const {
            return this->closed;
        }

        virtual bool isRunning() const {
            return this->running;
        }

        virtual Pointer<MessageDispatch> dequeue( long long timeout );

        virtual Pointer<MessageDispatch> dequeueNoWait();

        virtual Pointer<MessageDispatch> peek() const;

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual void clear();

        virtual int size() const;

        virtual std::vector< Pointer<MessageDispatch> > removeAll();

    public:

        virtual void lock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.lock();
        }

        virtual bool tryLock() throw( decaf::lang::exceptions::RuntimeException ) {
            return mutex.tryLock();
        }

        virtual void unlock() throw( decaf::lang::exceptions::RuntimeException ) {
            mutex.unlock();
        }

        virtual void wait() throw( decaf::lang::exceptions::RuntimeException,
                                   decaf::lang::exceptions::IllegalMonitorStateException,
                                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait();
        }

        virtual void wait( long long millisecs )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs );
        }

        virtual void wait( long long millisecs, int nanos )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalArgumentException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {

            mutex.wait( millisecs, nanos );
        }

        virtual void notify() throw( decaf::lang::exceptions::RuntimeException,
                                     decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notify();
        }

        virtual void notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                                        decaf::lang::exceptions::IllegalMonitorStateException ) {

            mutex.notifyAll();
        }

    private:

        decaf::util::LinkedList< Pointer<MessageDispatch> >& getChannel( const Pointer<MessageDispatch>& dispatch );

        Pointer<MessageDispatch> removeFirst();

        Pointer<MessageDispatch> getFirst() const;

    };

}}

#endif /* _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNEL_H_ */
