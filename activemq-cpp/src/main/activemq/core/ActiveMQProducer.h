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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQPRODUCER_H_
#define _ACTIVEMQ_CORE_ACTIVEMQPRODUCER_H_

#include <cms/MessageProducer.h>
#include <cms/Message.h>
#include <cms/Destination.h>
#include <cms/DeliveryMode.h>

#include <activemq/util/Config.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/core/kernels/ActiveMQProducerKernel.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;

    class ActiveMQSession;

    class AMQCPP_API ActiveMQProducer : public cms::MessageProducer {
    private:

        Pointer<activemq::core::kernels::ActiveMQProducerKernel> kernel;

    private:

        ActiveMQProducer(const ActiveMQProducer&);
        ActiveMQProducer& operator=(const ActiveMQProducer&);

    public:

        /**
         * Constructor, creates an instance of an ActiveMQProducer to wrap the
         * provided ActiveMQProducerKernel.
         *
         * @param kernel
         *        The Producer kernel pointer that implements the producers functionality.
         */
        ActiveMQProducer(Pointer<activemq::core::kernels::ActiveMQProducerKernel> kernel);

        virtual ~ActiveMQProducer();

    public:  // cms::MessageProducer methods.

        virtual void close();

        virtual void send(cms::Message* message);

        virtual void send(cms::Message* message, cms::AsyncCallback* callback);

        virtual void send(cms::Message* message, int deliveryMode, int priority, long long timeToLive);

        virtual void send(cms::Message* message, int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* callback);

        virtual void send(const cms::Destination* destination, cms::Message* message);

        virtual void send(const cms::Destination* destination, cms::Message* message, cms::AsyncCallback* callback);

        virtual void send(const cms::Destination* destination, cms::Message* message,
                          int deliveryMode, int priority, long long timeToLive);

        virtual void send(const cms::Destination* destination, cms::Message* message,
                          int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* callback);

        /**
         * Sets the delivery mode for this Producer
         * @param mode - The DeliveryMode to use for Message sends.
         */
        virtual void setDeliveryMode(int mode) {
            this->kernel->setDeliveryMode(mode);
        }

        /**
         * Gets the delivery mode for this Producer
         * @return The DeliveryMode
         */
        virtual int getDeliveryMode() const {
            return this->kernel->getDeliveryMode();
        }

        /**
         * Sets if Message Ids are disabled for this Producer
         * @param value - boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageID(bool value) {
            this->kernel->setDisableMessageID(value);
        }

        /**
         * Gets if Message Ids are disabled for this Producer
         * @return a boolean indicating state enable / disable (true / false) for MessageIds.
         */
        virtual bool getDisableMessageID() const {
            return this->kernel->getDisableMessageID();
        }

        /**
         * Sets if Message Time Stamps are disabled for this Producer
         * @param value - boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageTimeStamp(bool value) {
            this->kernel->setDisableMessageTimeStamp(value);
        }

        /**
         * Gets if Message Time Stamps are disabled for this Producer
         * @returns boolean indicating state of enable / disable (true / false)
         */
        virtual bool getDisableMessageTimeStamp() const {
            return this->kernel->getDisableMessageTimeStamp();
        }

        /**
         * Sets the Priority that this Producers sends messages at
         * @param priority int value for Priority level
         */
        virtual void setPriority(int priority) {
            this->kernel->setPriority(priority);
        }

        /**
         * Gets the Priority level that this producer sends messages at
         * @return int based priority level
         */
        virtual int getPriority() const {
            return this->kernel->getPriority();
        }

        /**
         * Sets the Time to Live that this Producers sends messages with
         * @param time The new default time to live value in milliseconds.
         */
        virtual void setTimeToLive(long long time) {
            this->kernel->setTimeToLive(time);
        }

        /**
         * Gets the Time to Live that this producer sends messages with
         * @return The default time to live value in milliseconds.
         */
        virtual long long getTimeToLive() const {
            return this->kernel->getTimeToLive();
        }

        /**
         * Sets the Send Timeout that this Producers sends messages with
         * @param time The new default send timeout value in milliseconds.
         */
        virtual void setSendTimeout(long long time) {
            this->kernel->setSendTimeout(time);
        }

        /**
         * Gets the Send Timeout that this producer sends messages with
         * @return The default send timeout value in milliseconds.
         */
        virtual long long getSendTimeout() const {
            return this->kernel->getSendTimeout();
        }

        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            this->kernel->setMessageTransformer(transformer);
        }

        virtual cms::MessageTransformer* getMessageTransformer() const {
            return this->kernel->getMessageTransformer();
        }

    public:

        /**
         * @returns true if this Producer has been closed.
         */
        bool isClosed() const {
            return this->kernel->isClosed();
        }

        /**
         * Retries this object ProducerInfo pointer
         * @return ProducerInfo Reference
         */
        const Pointer<commands::ProducerInfo>& getProducerInfo() const {
            return this->kernel->getProducerInfo();
        }

        /**
         * Retries this object ProducerId or NULL if closed.
         * @return ProducerId Reference
         */
        const Pointer<commands::ProducerId>& getProducerId() const {
            return this->kernel->getProducerId();
        }
   };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQPRODUCER_H_*/
