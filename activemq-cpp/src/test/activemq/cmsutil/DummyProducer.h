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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYPRODUCER_H_
#define ACTIVEMQ_CMSUTIL_DUMMYPRODUCER_H_

#include <cms/MessageProducer.h>
#include <activemq/cmsutil/MessageContext.h>

namespace activemq {
namespace cmsutil {

    class DummyProducer : public cms::MessageProducer {
    private:

        const cms::Destination* dest;
        int deliveryMode;
        bool disableMessageId;
        bool disableMessageTimestamp;
        int priority;
        long long ttl;
        MessageContext* messageContext;
        cms::MessageTransformer* transformer;

    private:

        DummyProducer(const DummyProducer&);
        DummyProducer& operator= (const DummyProducer&);

    public:

        DummyProducer(MessageContext* messageContext, const cms::Destination* dest) :
            dest(dest), deliveryMode(1), disableMessageId(false), disableMessageTimestamp(false),
            priority(4), ttl(0LL), messageContext(messageContext), transformer(NULL) {
        }

        virtual ~DummyProducer() {}

        virtual void close() {}

        virtual void send(cms::Message* message) {
            send(message, deliveryMode, priority, ttl);
        }

        virtual void send(cms::Message* message, cms::AsyncCallback* onComplate) {
            send(message, deliveryMode, priority, ttl, onComplate);
        }

        virtual void send(cms::Message* message, int deliveryMode, int priority, long long timeToLive) {
            send(dest, message, deliveryMode, priority, timeToLive);
        }

        virtual void send(cms::Message* message, int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* onComplete) {
            send(dest, message, deliveryMode, priority, timeToLive, onComplete);
        }

        virtual void send(const cms::Destination* destination, cms::Message* message) {
            send(dest, message, deliveryMode, priority, ttl);
        }

        virtual void send(const cms::Destination* destination, cms::Message* message, cms::AsyncCallback* onComplete) {
            send(dest, message, deliveryMode, priority, ttl, onComplete);
        }

        virtual void send(const cms::Destination* destination, cms::Message* message, int deliveryMode, int priority, long long timeToLive) {
            messageContext->send(destination, message, deliveryMode, priority, timeToLive);
        }

        virtual void send(const cms::Destination* destination, cms::Message* message, int deliveryMode, int priority, long long timeToLive, cms::AsyncCallback* onComplete) {
            messageContext->send(destination, message, deliveryMode, priority, timeToLive);
        }

        /**
         * Sets the delivery mode for this Producer
         *
         * @param mode
         *      The DeliveryMode
         */
        virtual void setDeliveryMode(int mode) {
            this->deliveryMode = mode;
        }

        /**
         * Gets the delivery mode for this Producer
         *
         * @return The DeliveryMode
         */
        virtual int getDeliveryMode() const {
            return deliveryMode;
        }

        /**
         * Sets if Message Ids are disbled for this Producer
         *
         * @param value
         *      boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageID(bool value) {
            disableMessageId = value;
        }

        /**
         * Gets if Message Ids are disbled for this Producer
         *
         * @return boolean indicating enable / disable (true / false)
         */
        virtual bool getDisableMessageID() const {
            return disableMessageId;
        }

        /**
         * Sets if Message Time Stamps are disbled for this Producer
         * @param value - boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageTimeStamp(bool value) {
            disableMessageTimestamp = value;
        }

        /**
         * Gets if Message Time Stamps are disbled for this Producer
         *
         * @return boolean indicating enable / disable (true / false)
         */
        virtual bool getDisableMessageTimeStamp() const {
            return disableMessageTimestamp;
        }

        /**
         * Sets the Priority that this Producers sends messages at
         *
         * @param priority
         *      int value for Priority level
         */
        virtual void setPriority(int priority) {
            this->priority = priority;
        }

        /**
         * Gets the Priority level that this producer sends messages at
         *
         * @return int based priority level
         */
        virtual int getPriority() const {
            return priority;
        }

        /**
         * Sets the Time to Live that this Producers sends messages with.  This
         * value will be used if the time to live is not specified via the
         * send method.
         *
         * @param time
         *      default time to live value in milliseconds
         */
        virtual void setTimeToLive(long long time) {
            ttl = time;
        }

        /**
         * Gets the Time to Live that this producer sends messages with
         *
         * @return Time to live value in milliseconds
         */
        virtual long long getTimeToLive() const {
            return ttl;
        }

        virtual cms::MessageTransformer* getMessageTransformer() const {
            return transformer;
        }

        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            this->transformer = transformer;
        }
    };

}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYPRODUCER_H_*/
