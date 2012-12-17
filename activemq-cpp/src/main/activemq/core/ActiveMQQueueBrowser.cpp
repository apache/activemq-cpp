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

#include "ActiveMQQueueBrowser.h"

#include <cms/MessageListener.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/kernels/ActiveMQConsumerKernel.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/PrefetchPolicy.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class Browser : public ActiveMQConsumerKernel {
    public:

        ActiveMQQueueBrowser* parent;

    private:

        Browser(const Browser&);
        Browser& operator= (const Browser&);

    public:

        Browser(ActiveMQQueueBrowser* parent, ActiveMQSessionKernel* session,
                const Pointer<commands::ConsumerId>& id,
                const Pointer<commands::ActiveMQDestination>& destination,
                const std::string& name, const std::string& selector,
                int prefetch, int maxPendingMessageCount, bool noLocal,
                bool browser, bool dispatchAsync,
                cms::MessageListener* listener ) :
            ActiveMQConsumerKernel(session, id, destination, name, selector, prefetch,
                                   maxPendingMessageCount, noLocal, browser, dispatchAsync,
                                   listener), parent(parent) {

        }

        virtual void dispatch(const Pointer<MessageDispatch>& dispatched) {

            try{

                if (dispatched->getMessage() == NULL) {
                    this->parent->browseDone.set(true);
                } else {
                    ActiveMQConsumerKernel::dispatch(dispatched);
                }

                this->parent->notifyMessageAvailable();
            }
            AMQ_CATCH_RETHROW( ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
            AMQ_CATCHALL_THROW( ActiveMQException )
        }
    };
}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueueBrowser::ActiveMQQueueBrowser(ActiveMQSessionKernel* session,
                                           const Pointer<commands::ConsumerId>& consumerId,
                                           const Pointer<commands::ActiveMQDestination>& destination,
                                           const std::string& selector,
                                           bool dispatchAsync ) : cms::QueueBrowser(),
                                                                  cms::MessageEnumeration(),
                                                                  session(session),
                                                                  consumerId(consumerId),
                                                                  destination(destination),
                                                                  selector(selector),
                                                                  dispatchAsync(dispatchAsync),
                                                                  queue(NULL),
                                                                  closed(false),
                                                                  mutex(),
                                                                  wait(),
                                                                  browseDone(),
                                                                  browser(NULL) {

    if (session == NULL) {
        throw ActiveMQException(__FILE__, __LINE__, "Session instance provided was NULL.");
    }

    if (consumerId == NULL) {
        throw ActiveMQException(__FILE__, __LINE__, "ConsumerId instance provided was NULL.");
    }

    if (destination == NULL || !destination->isQueue()) {
        throw ActiveMQException(__FILE__, __LINE__, "Destination instance provided was NULL or not a Queue.");
    }

    // Cache the Queue instance for faster retreival.
    this->queue = destination.dynamicCast<cms::Queue>().get();
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQQueueBrowser::~ActiveMQQueueBrowser() {
    try {
        this->close();
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
const cms::Queue* ActiveMQQueueBrowser::getQueue() const {
    return this->queue;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQQueueBrowser::getMessageSelector() const {
    return this->selector;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageEnumeration* ActiveMQQueueBrowser::getEnumeration() {

    try {
        checkClosed();
        if (this->browser == NULL) {
            this->browser = createConsumer();
        }
        return this;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueueBrowser::close() {
    try {

        if (this->closed) {
            return;
        }

        synchronized(&mutex) {
            destroyConsumer();
            this->closed = true;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQQueueBrowser::hasMoreMessages() {

    try {

        while (true) {

            synchronized(&mutex) {
                if (this->browser == NULL) {
                    return false;
                }
            }

            if (this->browser->getMessageAvailableCount() > 0) {
                return true;
            }

            if (browseDone.get() || !this->session->isStarted()) {
                destroyConsumer();
                return false;
            }

            waitForMessageAvailable();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQQueueBrowser::nextMessage() {

    try {

        while (true) {

            synchronized(&mutex) {
                if (this->browser == NULL) {
                    return NULL;
                }
            }

            try {

                cms::Message* answer = this->browser->receiveNoWait();
                if (answer != NULL) {
                    return answer;
                }

            } catch (cms::CMSException& e) {
                return NULL;
            }

            if (this->browseDone.get() || !this->session->isStarted()) {
                destroyConsumer();
                return NULL;
            }

            waitForMessageAvailable();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueueBrowser::notifyMessageAvailable() {

    synchronized(&wait) {
        wait.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueueBrowser::waitForMessageAvailable() {
    synchronized(&wait) {
        wait.wait(2000);
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQConsumerKernel> ActiveMQQueueBrowser::createConsumer() {

    this->browseDone.set(false);

    int prefetch = this->session->getConnection()->getPrefetchPolicy()->getQueueBrowserPrefetch();

    Pointer<ActiveMQConsumerKernel> consumer(new Browser(this, session, consumerId, destination, "", selector, prefetch, 0, false, true, dispatchAsync, NULL));

    try {
        this->session->addConsumer(consumer);
        this->session->syncRequest(consumer->getConsumerInfo());
    } catch (Exception& ex) {
        this->session->removeConsumer(consumer);
        throw;
    }

    if (this->session->getConnection()->isStarted()) {
        consumer->start();
    }

    return consumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueueBrowser::destroyConsumer() {

    if (this->browser == NULL) {
        return;
    }

    try {

        if (this->session->isTransacted()) {
            session->commit();
        }

        this->browser->close();
        this->browser.reset(NULL);
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQQueueBrowser::checkClosed() {
    if (closed) {
        throw ActiveMQException(__FILE__, __LINE__, "The QueueBrowser is closed.");
    }
}
