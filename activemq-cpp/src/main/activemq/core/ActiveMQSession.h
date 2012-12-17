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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQSESSION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQSESSION_H_

#include <cms/Session.h>
#include <cms/ExceptionListener.h>

#include <activemq/util/Config.h>
#include <activemq/core/kernels/ActiveMQConsumerKernel.h>
#include <activemq/core/kernels/ActiveMQProducerKernel.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <activemq/commands/SessionInfo.h>

#include <decaf/lang/Pointer.h>

#include <string>
#include <memory>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;
    using decaf::util::concurrent::atomic::AtomicBoolean;

    class ActiveMQConnection;

    class AMQCPP_API ActiveMQSession : public virtual cms::Session {
    protected:

        Pointer<activemq::core::kernels::ActiveMQSessionKernel> kernel;

    private:

        ActiveMQSession(const ActiveMQSession&);
        ActiveMQSession& operator=(const ActiveMQSession&);

    public:

        ActiveMQSession(Pointer<activemq::core::kernels::ActiveMQSessionKernel> kernel);

        virtual ~ActiveMQSession();

        /**
         * Stops asynchronous message delivery.
         */
        virtual void start();

        /**
         * Starts asynchronous message delivery.
         */
        virtual void stop();

        /**
         * Indicates whether or not the session is currently in the started
         * state.
         */
        bool isStarted() const {
            return this->kernel->isStarted();
        }

    public:   // Implements Methods

        virtual void close();

        virtual void commit();

        virtual void rollback();

        virtual void recover();

        virtual cms::MessageConsumer* createConsumer(const cms::Destination* destination);

        virtual cms::MessageConsumer* createConsumer(const cms::Destination* destination,
                                                     const std::string& selector);

        virtual cms::MessageConsumer* createConsumer(const cms::Destination* destination,
                                                     const std::string& selector,
                                                     bool noLocal);

        virtual cms::MessageConsumer* createDurableConsumer(const cms::Topic* destination,
                                                            const std::string& name,
                                                            const std::string& selector,
                                                            bool noLocal = false);

        virtual cms::MessageProducer* createProducer(const cms::Destination* destination);

        virtual cms::QueueBrowser* createBrowser(const cms::Queue* queue);

        virtual cms::QueueBrowser* createBrowser(const cms::Queue* queue, const std::string& selector);

        virtual cms::Queue* createQueue(const std::string& queueName);

        virtual cms::Topic* createTopic(const std::string& topicName);

        virtual cms::TemporaryQueue* createTemporaryQueue();

        virtual cms::TemporaryTopic* createTemporaryTopic();

        virtual cms::Message* createMessage();

        virtual cms::BytesMessage* createBytesMessage();

        virtual cms::BytesMessage* createBytesMessage(const unsigned char* bytes, int bytesSize);

        virtual cms::StreamMessage* createStreamMessage();

        virtual cms::TextMessage* createTextMessage();

        virtual cms::TextMessage* createTextMessage(const std::string& text);

        virtual cms::MapMessage* createMapMessage();

        virtual cms::Session::AcknowledgeMode getAcknowledgeMode() const {
            return this->kernel->getAcknowledgeMode();
        }

        virtual bool isTransacted() const {
            return this->kernel->isTransacted();
        }

        virtual void unsubscribe(const std::string& name);

   public:  // ActiveMQSession specific Methods

        /**
         * This method gets any registered exception listener of this sessions
         * connection and returns it.  Mainly intended for use by the objects
         * that this session creates so that they can notify the client of
         * exceptions that occur in the context of another thread.
         * @returns cms::ExceptionListener pointer or NULL
         */
        cms::ExceptionListener* getExceptionListener() {
            return this->kernel->getExceptionListener();
        }

        /**
         * Set an MessageTransformer instance that is passed on to all MessageProducer and MessageConsumer
         * objects created from this Session.
         *
         * @param transformer
         *      Pointer to the cms::MessageTransformer to set on all MessageConsumers and MessageProducers.
         */
        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            this->kernel->setMessageTransformer(transformer);
        }

        /**
         * Gets the currently configured MessageTransformer for this Session.
         *
         * @returns the pointer to the currently set cms::MessageTransformer.
         */
        virtual cms::MessageTransformer* getMessageTransformer() const {
            return this->kernel->getMessageTransformer();
        }

        /**
         * Gets the Session Information object for this session, if the
         * session is closed than this method throws an exception.
         * @return SessionInfo Reference
         */
        const commands::SessionInfo& getSessionInfo() const {
            return this->kernel->getSessionInfo();
        }

        /**
         * Gets the Session Id object for this session, if the session
         * is closed than this method throws an exception.
         * @return SessionId Reference
         */
        const commands::SessionId& getSessionId() const {
            return this->kernel->getSessionId();
        }

        /**
         * Gets the ActiveMQConnection that is associated with this session.
         */
        ActiveMQConnection* getConnection() const {
            return this->kernel->getConnection();
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQSESSION_H_*/
