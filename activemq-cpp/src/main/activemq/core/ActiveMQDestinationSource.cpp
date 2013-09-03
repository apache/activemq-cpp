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

#include "ActiveMQDestinationSource.h"

#include <cms/DestinationListener.h>
#include <cms/MessageListener.h>
#include <cms/Session.h>
#include <cms/Message.h>
#include <cms/MessageConsumer.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/HashSet.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>

#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQDestinationEvent.h>
#include <activemq/util/AdvisorySupport.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/DestinationInfo.h>

using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {

    class DestinationSourceImpl : public cms::MessageListener {
    private:

        DestinationSourceImpl(const DestinationSourceImpl&);
        DestinationSourceImpl& operator= (const DestinationSourceImpl&);

    public:

        AtomicBoolean started;
        ActiveMQConnection* connection;
        Pointer<Session> session;

        Pointer<MessageConsumer> topicConsumer;
        Pointer<MessageConsumer> queueConsumer;
        Pointer<MessageConsumer> tempTopicConsumer;
        Pointer<MessageConsumer> tempQueueConsumer;

        HashSet< Pointer<ActiveMQDestination> > queues;
        HashSet< Pointer<ActiveMQDestination> > topics;
        HashSet< Pointer<ActiveMQDestination> > tempQueues;
        HashSet< Pointer<ActiveMQDestination> > tempTopics;

        DestinationListener* listener;
        Mutex listenerLock;

    public:

        DestinationSourceImpl() : started(false),
                                  connection(),
                                  session(),
                                  topicConsumer(),
                                  queueConsumer(),
                                  tempTopicConsumer(),
                                  tempQueueConsumer(),
                                  listener(),
                                  listenerLock() {

        }

        virtual ~DestinationSourceImpl() {
            try {
                stop();
            }
            DECAF_CATCHALL_NOTHROW()
        }

        void start() {
            if (started.compareAndSet(false, true)) {
                session.reset(connection->createSession(Session::AUTO_ACKNOWLEDGE));

                Pointer<ActiveMQDestination> queueAdvisories(AdvisorySupport::getQueueAdvisoryTopic());
                Pointer<ActiveMQDestination> topicAdvisories(AdvisorySupport::getTopicAdvisoryTopic());
                Pointer<ActiveMQDestination> tempQueueAdvisories(AdvisorySupport::getTempQueueAdvisoryTopic());
                Pointer<ActiveMQDestination> tempTopicAdvisories(AdvisorySupport::getTempTopicAdvisoryTopic());

                queueConsumer.reset(session->createConsumer(queueAdvisories->getCMSDestination()));
                queueConsumer->setMessageListener(this);

                topicConsumer.reset(session->createConsumer(topicAdvisories->getCMSDestination()));
                topicConsumer->setMessageListener(this);

                tempQueueConsumer.reset(session->createConsumer(tempQueueAdvisories->getCMSDestination()));
                tempQueueConsumer->setMessageListener(this);

                tempTopicConsumer.reset(session->createConsumer(tempTopicAdvisories->getCMSDestination()));
                tempTopicConsumer->setMessageListener(this);
            }
        }

        void stop() {
            if (started.compareAndSet(true, false)) {
                if (session != NULL) {
                    session->close();

                    queueConsumer.reset(NULL);
                    topicConsumer.reset(NULL);
                    tempQueueConsumer.reset(NULL);
                    tempTopicConsumer.reset(NULL);

                    session.reset(NULL);
                }
            }
        }

        virtual void onMessage(const cms::Message* message) {

            if (!this->started.get()) {
                return;
            }

            const commands::Message* amqMessage = dynamic_cast<const commands::Message*>(message);
            if (amqMessage != NULL) {
                Pointer<DataStructure> payload = amqMessage->getDataStructure();
                try {
                    Pointer<DestinationInfo> destinationInfo = payload.dynamicCast<DestinationInfo>();
                    ActiveMQDestinationEvent event(destinationInfo);
                    handleDestinationEvent(event);
                } catch(ClassCastException& ex) {
                }
            }
        }

        void handleDestinationEvent(ActiveMQDestinationEvent& event) {

            Pointer<ActiveMQDestination> dest = event.getDestinationInfo()->getDestination();

            if (dest->isTopic()) {
                if (dest->isTemporary()) {
                    synchronized(&tempTopics) {
                        if (event.isAddOperation()) {
                            tempTopics.add(dest);
                        } else {
                            tempTopics.remove(dest);
                        }
                    }
                } else {
                    synchronized(&topics) {
                        if (event.isAddOperation()) {
                            topics.add(dest);
                        } else {
                            topics.remove(dest);
                        }
                    }
                }
            } else {
                if (dest->isTemporary()) {
                    synchronized(&tempQueues) {
                        if (event.isAddOperation()) {
                            tempQueues.add(dest);
                        } else {
                            tempQueues.remove(dest);
                        }
                    }
                } else {
                    synchronized(&queues) {
                        if (event.isAddOperation()) {
                            queues.add(dest);
                        } else {
                            queues.remove(dest);
                        }
                    }
                }
            }

            synchronized(&listenerLock) {
                if (listener != NULL) {
                    listener->onDestinationEvent(&event);
                }
            }
        }

        std::vector<cms::Queue*> getQueues() {

            std::vector<cms::Queue*> result;

            synchronized(&queues) {
                Pointer< Iterator<Pointer<ActiveMQDestination> > > iter(queues.iterator());
                while (iter->hasNext()) {
                    cms::Destination* copy = iter->next()->getCMSDestination()->clone();
                    result.push_back(dynamic_cast<cms::Queue*>(copy));
                }
            }

            return result;
        }

        std::vector<cms::Topic*> getTopics() {
            std::vector<cms::Topic*> result;

            synchronized(&topics) {
                Pointer< Iterator<Pointer<ActiveMQDestination> > > iter(topics.iterator());
                while (iter->hasNext()) {
                    cms::Destination* copy = iter->next()->getCMSDestination()->clone();
                    result.push_back(dynamic_cast<cms::Topic*>(copy));
                }
            }

            return result;
        }

        std::vector<cms::TemporaryQueue*> getTemporaryQueues() {
            std::vector<cms::TemporaryQueue*> result;

            synchronized(&tempQueues) {
                Pointer< Iterator<Pointer<ActiveMQDestination> > > iter(tempQueues.iterator());
                while (iter->hasNext()) {
                    cms::Destination* copy = iter->next()->getCMSDestination()->clone();
                    result.push_back(dynamic_cast<cms::TemporaryQueue*>(copy));
                }
            }

            return result;
        }

        std::vector<cms::TemporaryTopic*> getTemporaryTopics() {
            std::vector<cms::TemporaryTopic*> result;

            synchronized(&tempTopics) {
                Pointer< Iterator<Pointer<ActiveMQDestination> > > iter(tempTopics.iterator());
                while (iter->hasNext()) {
                    cms::Destination* copy = iter->next()->getCMSDestination()->clone();
                    result.push_back(dynamic_cast<cms::TemporaryTopic*>(copy));
                }
            }

            return result;
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestinationSource::ActiveMQDestinationSource(ActiveMQConnection* connection) :
    DestinationSource(), impl(new DestinationSourceImpl()) {

    if (connection == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Connection passed was NULL");
    }

    this->impl->connection = connection;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestinationSource::~ActiveMQDestinationSource() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestinationSource::setListener(cms::DestinationListener* listener) {
    synchronized(&this->impl->listenerLock) {
        this->impl->listener = listener;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::DestinationListener* ActiveMQDestinationSource::getListener() const {
    return this->impl->listener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestinationSource::start() {
    this->impl->start();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestinationSource::stop() {
    this->impl->stop();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<cms::Queue*> ActiveMQDestinationSource::getQueues() const {
    return this->impl->getQueues();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<cms::Topic*> ActiveMQDestinationSource::getTopics() const {
    return this->impl->getTopics();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<cms::TemporaryQueue*> ActiveMQDestinationSource::getTemporaryQueues() const {
    return this->impl->getTemporaryQueues();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<cms::TemporaryTopic*> ActiveMQDestinationSource::getTemporaryTopics() const {
    return this->impl->getTemporaryTopics();
}
