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
#include <activemq/util/MemoryUsage.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/ProducerAck.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <memory>

namespace activemq{
namespace core{

    class ActiveMQSession;

    class AMQCPP_API ActiveMQProducer : public cms::MessageProducer {
    private:

        // Disable sending timestamps
        bool disableTimestamps;

        // Disable adding a Message Id
        bool disableMessageId;

        // The default delivery Mode of this Producer
        int defaultDeliveryMode;

        // The default priority Level to send at
        int defaultPriority;

        // The default time to live value for messages in milliseconds
        long long defaultTimeToLive;

        // The default Send Timeout for this Producer.
        long long sendTimeout;

        // Session that this producer sends to.
        ActiveMQSession* session;

        // This Producers protocol specific info object
        std::auto_ptr<commands::ProducerInfo> producerInfo;

        // Boolean that indicates if the consumer has been closed
        bool closed;

        // Memory Usage Class, created only if the Producer is tracking its usage.
        std::auto_ptr<util::MemoryUsage> memoryUsage;

        // The Destination assigned at creation, NULL if not assigned.
        std::auto_ptr<cms::Destination> destination;

    public:

        /**
         * Constructor, creates an instance of an ActiveMQProducer
         *
         * @param producerInfo
         *        Pointer to a ProducerInfo command which identifies this producer.
         * @param destination
         *        The assigned Destination this Producer sends to, or null if not set.
         *        The Producer does not own the Pointer passed.
         * @param session
         *        The Session which is the parent of this Producer.
         */
        ActiveMQProducer( commands::ProducerInfo* producerInfo,
                          const cms::Destination* destination,
                          ActiveMQSession* session );

        virtual ~ActiveMQProducer();

        /**
         * Closes the Consumer.  This will return all allocated resources
         * and purge any outstanding messages.  This method will block if
         * there is a call to receive in progress, or a dispatch to a
         * MessageListener in place
         * @throws CMSException
         */
        virtual void close() throw ( cms::CMSException );

        /**
         * Sends the message to the default producer destination.
         * @param a Message Object Pointer
         * @throws CMSException
         */
        virtual void send( cms::Message* message ) throw ( cms::CMSException );

        /**
         * Sends the message to the default producer destination, but does
         * not take ownership of the message, caller must still destroy it.
         * @param message - a Message Object Pointer
         * @param deliverMode The delivery mode to be used.
         * @param priority The priority for this message.
         * @param timeToLive The time to live value for this message in
         * milliseconds.
         * @throws CMSException
         */
        virtual void send( cms::Message* message, int deliveryMode,
                           int priority, long long timeToLive )
                               throw ( cms::CMSException );

        /**
         * Sends the message to the designated destination.
         * @param a Message Object Pointer
         * @throws CMSException
         */
        virtual void send( const cms::Destination* destination,
                           cms::Message* message ) throw ( cms::CMSException );

        /**
         * Sends the message to the designated destination, but does
         * not take ownership of the message, caller must still destroy it.
         * @param destination - a Message Object Pointer
         * @param message - a Message Object Pointer
         * @param deliverMode The delivery mode to be used.
         * @param priority The priority for this message.
         * @param timeToLive The time to live value for this message in
         * milliseconds.
         * @throws CMSException
         */
        virtual void send( const cms::Destination* destination,
                           cms::Message* message, int deliveryMode,
                           int priority, long long timeToLive )
                                throw ( cms::CMSException );

        /**
         * Sets the delivery mode for this Producer
         * @param The DeliveryMode
         */
        virtual void setDeliveryMode( int mode ) {
            this->defaultDeliveryMode = mode;
        }

        /**
         * Gets the delivery mode for this Producer
         * @return The DeliveryMode
         */
        virtual int getDeliveryMode() const {
            return this->defaultDeliveryMode;
        }

        /**
         * Sets if Message Ids are disabled for this Producer
         * @param boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageID( bool value ) {
            this->disableMessageId = value;
        }

        /**
         * Sets if Message Ids are disabled for this Producer
         * @param boolean indicating enable / disable (true / false)
         */
        virtual bool getDisableMessageID() const {
            return this->disableMessageId;
        }

        /**
         * Sets if Message Time Stamps are disabled for this Producer
         * @param boolean indicating enable / disable (true / false)
         */
        virtual void setDisableMessageTimeStamp( bool value ) {
            this->disableTimestamps = value;
        }

        /**
         * Sets if Message Time Stamps are disabled for this Producer
         * @param boolean indicating enable / disable (true / false)
         */
        virtual bool getDisableMessageTimeStamp() const {
            return this->disableTimestamps;
        }

        /**
         * Sets the Priority that this Producers sends messages at
         * @param int value for Priority level
         */
        virtual void setPriority( int priority ) {
            this->defaultPriority = priority;
        }

        /**
         * Gets the Priority level that this producer sends messages at
         * @return int based priority level
         */
        virtual int getPriority() const {
            return this->defaultPriority;
        }

        /**
         * Sets the Time to Live that this Producers sends messages with
         * @param time The new default time to live value in milliseconds.
         */
        virtual void setTimeToLive( long long time ) {
            this->defaultTimeToLive = time;
        }

        /**
         * Gets the Time to Live that this producer sends messages with
         * @return The default time to live value in milliseconds.
         */
        virtual long long getTimeToLive() const {
            return this->defaultTimeToLive;
        }

        /**
         * Sets the Send Timeout that this Producers sends messages with
         * @param time The new default send timeout value in milliseconds.
         */
        virtual void setSendTimeout( long long time ) {
            this->sendTimeout = time;
        }

        /**
         * Gets the Send Timeout that this producer sends messages with
         * @return The default send timeout value in milliseconds.
         */
        virtual long long getSendTimeout() const {
            return this->sendTimeout;
        }

    public:

        /**
         * Retries this object ProducerInfo pointer
         * @return ProducerInfo pointer
         */
        virtual commands::ProducerInfo* getProducerInfo(){
            return this->producerInfo.get();
        }

        /**
         * Handles the work of Processing a ProducerAck Command from the Broker.
         * @param ack - The ProducerAck message received from the Broker.
         */
        virtual void onProducerAck( const commands::ProducerAck& ack );

   private:

       // Checks for the closed state and throws if so.
       void checkClosed() throw( exceptions::ActiveMQException );

   };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQPRODUCER_H_*/
