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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQMESSAGE_H_
#define _ACTIVEMQ_CORE_ACTIVEMQMESSAGE_H_

#include <cms/Message.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace core{

    class ActiveMQAckHandler;

    /**
     * Interface for all ActiveMQ Messages that will pass through the core
     * API layer.  This interface defines a method that the API uses to set
     * an Acknowledgment handler that will be called by the message when
     * a user calls the <code>acknowledge</code> method of the Message
     * interface.  This is only done when the Session that this message
     * passes through is in Client Acknowledge mode.
     */
    class AMQCPP_API ActiveMQMessage {
    public:

        virtual ~ActiveMQMessage() {}

        /**
         * Sets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @param handler ActiveMQAckHandler to call
         */
        virtual void setAckHandler( ActiveMQAckHandler* handler ) = 0;

        /**
         * Gets the number of times this message has been redelivered.
         * @return redelivery count
         */
        virtual int getRedeliveryCount() const = 0;

        /**
         * Sets the count of the number of times this message has been
         * redelivered
         * @param count the redelivery count
         */
        virtual void setRedeliveryCount( int count ) = 0;

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const = 0;

        /**
         * Returns if the Message Body is Read Only.
         * @returns true if the Message Body is Read Only.
         */
        virtual bool isReadOnlyBody() const = 0;

        /**
         * Sets the Read Only status of a Message Body
         * @param value - true if the Message Body is Read Only.
         */
        virtual void setReadOnlyBody( bool value ) = 0;

        /**
         * Returns if the Message Properties are Read Only.
         * @returns true if the Message properties are Read Only.
         */
        virtual bool isReadOnlyProperties() const = 0;

        /**
         * Sets the Read Only status of a Message's Properties
         * @param value - true if the Message Properties are Read Only.
         */
        virtual void setReadOnlyProperties( bool value ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQMESSAGE_H_*/
