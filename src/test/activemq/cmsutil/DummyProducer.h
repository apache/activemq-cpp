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
        
    public:

        DummyProducer(MessageContext* messageContext, const cms::Destination* dest) {
            deliveryMode = 1;
            disableMessageId = false;
            disableMessageTimestamp = false;
            priority = 4;
            ttl = 0L;
            this->dest = dest;
            this->messageContext = messageContext;
        }
        virtual ~DummyProducer() {}
              
        virtual void close() throw( cms::CMSException ){}
        
        /**
         * Sends the message to the default producer destination, but does
         * not take ownership of the message, caller must still destroy it.
         * Uses default values for deliveryMode, priority, and time to live.
         * 
         * @param message
         *      The message to be sent.
         * @throws cms::CMSException
         */
        virtual void send( cms::Message* message ) throw ( cms::CMSException ){
            send(message, deliveryMode, priority, ttl);
        }

        /**
         * Sends the message to the default producer destination, but does
         * not take ownership of the message, caller must still destroy it.
         * 
         * @param message
         *      The message to be sent.
         * @param deliveryMode 
         *      The delivery mode to be used.
         * @param priority 
         *      The priority for this message.
         * @param timeToLive 
         *      The time to live value for this message in milliseconds.
         * @throws cms::CMSException
         */
        virtual void send( cms::Message* message, int deliveryMode, int priority, 
            long long timeToLive) throw ( cms::CMSException ){
            
            send(dest, message, deliveryMode, priority, timeToLive);
        }
            
        /**
         * Sends the message to the designated destination, but does
         * not take ownership of the message, caller must still destroy it.
         * Uses default values for deliveryMode, priority, and time to live.
         * 
         * @param destination
         *      The destination on which to send the message
         * @param message
         *      the message to be sent.
         * @throws cms::CMSException
         */
        virtual void send( const cms::Destination* destination,
                           cms::Message* message ) throw ( cms::CMSException ){
            send(dest, message, deliveryMode, priority, ttl);
        }
                           
        /**
         * Sends the message to the designated destination, but does
         * not take ownership of the message, caller must still destroy it.
         * 
         * @param destination
         *      The destination on which to send the message
         * @param message
         *      The message to be sent.
         * @param deliveryMode
         *      The delivery mode to be used.
         * @param priority 
         *      The priority for this message.
         * @param timeToLive 
         *      The time to live value for this message in milliseconds.
         * @throws cms::CMSException
         */     
        virtual void send( const cms::Destination* destination,
            cms::Message* message, int deliveryMode, int priority, 
            long long timeToLive) throw ( cms::CMSException ){
            
            messageContext->send(destination, message, deliveryMode, priority, timeToLive);
        }
            
        /** 
         * Sets the delivery mode for this Producer
         * 
         * @param mode
         *      The DeliveryMode
         */
        virtual void setDeliveryMode( int mode ) {
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
        virtual void setDisableMessageID( bool value ) {
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
        virtual void setDisableMessageTimeStamp( bool value ) {
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
        virtual void setPriority( int priority ) {
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
        virtual void setTimeToLive( long long time ) {
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
        
    };
    
}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYPRODUCER_H_*/
