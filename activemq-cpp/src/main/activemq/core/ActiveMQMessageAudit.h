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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDIT_H_
#define _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDIT_H_

#include <activemq/util/Config.h>

#include <activemq/commands/MessageId.h>
#include <activemq/commands/ProducerId.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace core {

    class MessageAuditImpl;

    class AMQCPP_API ActiveMQMessageAudit {
    private:

        MessageAuditImpl* impl;

    public:

        static const int DEFAULT_WINDOW_SIZE;
        static const int MAXIMUM_PRODUCER_COUNT;

    public:

        /**
         * Default Constructor windowSize = 2048, maximumNumberOfProducersToTrack = 64
         */
        ActiveMQMessageAudit();

        /**
         * Construct a MessageAudit
         *
         * @param auditDepth
         *      The range of ids to track.
         * @param maximumNumberOfProducersToTrack
         *      The number of producers expected in the system
         */
        ActiveMQMessageAudit(int auditDepth, int maximumNumberOfProducersToTrack);

        ~ActiveMQMessageAudit();

    public:

        /**
         * Gets the currently configured Audit Depth
         *
         * @returns the current audit depth setting
         */
        int getAuditDepth() const;

        /**
         * Sets a new Audit Depth value.
         *
         * @param value
         *      The range of ids to track.
         */
        void setAuditDepth(int value);

        /**
         * @returns the current number of producers that will be tracked.
         */
        int getMaximumNumberOfProducersToTrack() const;

        /**
         * Sets the number of producers to track
         *
         * @param value
         *      The number of producers expected in the system
         */
        void getMaximumNumberOfProducersToTrack(int value);

        /**
         * checks whether this messageId has been seen before and adds this
         * messageId to the list
         *
         * @param msgId
         *      The string value Message Id.
         *
         * @return true if the message is a duplicate.
         */
        bool isDuplicate(const std::string& msgId) const;

        /**
         * Checks if this messageId has been seen before
         *
         * @param msgId
         *      The target MessageId to check.
         *
         * @return true if the message is a duplicate
         */
        bool isDuplicate(decaf::lang::Pointer<commands::MessageId> msgId) const;

        /**
         * Marks this message as being received.
         *
         * @param msgId
         *      The string value Message Id.
         */
        void rollback(const std::string& msgId);

        /**
         * Marks this message as being received.
         *
         * @param msgId
         *      The target MessageId to check.
         */
        void rollback(decaf::lang::Pointer<commands::MessageId> msgId);

        /**
         * Check the MessageId is in order
         *
         * @param msgId
         *      The string value Message Id.
         *
         * @return true if the MessageId is in order.
         */
        bool isInOrder(const std::string& msgId) const;

        /**
         * Check the MessageId is in order
         *
         * @param msgId
         *      The target MessageId to check.
         *
         * @return true if the MessageId is in order.
         */
        bool isInOrder(decaf::lang::Pointer<commands::MessageId> msgId) const;

        /**
         * @returns the last sequence Id that we've audited for the given producer.
         */
        long long getLastSeqId(decaf::lang::Pointer<commands::ProducerId> id) const;

        /**
         * Clears this Audit.
         */
        void clear();

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDIT_H_ */
