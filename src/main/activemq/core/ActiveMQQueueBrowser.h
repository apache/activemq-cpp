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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQQUEUEBROWSER_H_
#define _ACTIVEMQ_CORE_ACTIVEMQQUEUEBROWSER_H_

#include <activemq/util/Config.h>

#include <cms/Queue.h>
#include <cms/QueueBrowser.h>
#include <cms/MessageEnumeration.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

#include <string>

namespace activemq {
namespace core {

    class ActiveMQConsumer;
    class ActiveMQSession;
    class Browser;

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQQueueBrowser : public cms::QueueBrowser,
                                            public cms::MessageEnumeration {
    private:

        friend class Browser;

        ActiveMQSession* session;
        Pointer<commands::ConsumerId> consumerId;
        Pointer<commands::ActiveMQDestination> destination;
        std::string selector;
        bool dispatchAsync;
        cms::Queue* queue;
        volatile bool closed;

        mutable decaf::util::concurrent::Mutex mutex;
        mutable decaf::util::concurrent::Mutex wait;
        decaf::util::concurrent::atomic::AtomicBoolean browseDone;

        mutable ActiveMQConsumer* browser;

    private:

        ActiveMQQueueBrowser( const ActiveMQQueueBrowser& );
        ActiveMQQueueBrowser& operator= ( const ActiveMQQueueBrowser& );

    public:

        ActiveMQQueueBrowser( ActiveMQSession* session,
                              const Pointer<commands::ConsumerId>& consumerId,
                              const Pointer<commands::ActiveMQDestination>& destination,
                              const std::string& selector,
                              bool dispatchAsync );

        virtual ~ActiveMQQueueBrowser() throw();

    public:

        virtual const cms::Queue* getQueue() const;

        virtual std::string getMessageSelector() const;

        virtual cms::MessageEnumeration* getEnumeration();

        virtual void close();

        virtual bool hasMoreMessages();

        virtual cms::Message* nextMessage();

    private:

        void checkClosed();
        void notifyMessageAvailable();
        void waitForMessageAvailable();

        ActiveMQConsumer* createConsumer();
        void destroyConsumer();

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQQUEUEBROWSER_H_ */
