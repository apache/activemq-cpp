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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_MARSHAL_STOMPHELPER_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_MARSHAL_STOMPHELPER_H_

#include <activemq/util/Config.h>
#include <activemq/util/LongSequenceGenerator.h>
#include <activemq/wireformat/stomp/StompFrame.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessageId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace wireformat {
namespace stomp {

    using decaf::lang::Pointer;
    using activemq::commands::Message;
    using activemq::commands::MessageId;
    using activemq::commands::ProducerId;
    using activemq::commands::ConsumerId;
    using activemq::commands::TransactionId;
    using activemq::commands::ActiveMQDestination;

    /**
     * Utility Methods used when marshaling to and from StompFrame's.
     *
     * @since 3.0
     */
    class StompHelper {
    private:

        activemq::util::LongSequenceGenerator messageIdGenerator;

    public:

        StompHelper();

        virtual ~StompHelper();

    public:

        /**
         * Converts the Headers in a Stomp Frame into Headers in the given Message
         * Command.
         *
         * @param frame - The frame to extract headers from.
         * @param message - The message to move the Headers to.
         */
        void convertProperties( const Pointer<StompFrame>& frame, const Pointer<Message>& message );

        /**
         * Converts the Properties in a Message Command to Valid Headers and Properties
         * in the StompFrame.
         *
         * @param message - The message to move the Headers to.
         * @param frame - The frame to extract headers from.
         */
        void convertProperties( const Pointer<Message>& message, const Pointer<StompFrame>& frame );

        /**
         * Converts from a Stomp Destination to an ActiveMQDestination
         *
         * @param destination - The Stomp Destination name string.
         * @returns Pointer to a new ActiveMQDestination.
         */
        Pointer<ActiveMQDestination> convertDestination( const std::string& destination );

        /**
         * Converts from a ActiveMQDestination to a Stomp Destination Name
         *
         * @param destination - The ActiveMQDestination to Convert
         * @return the Stomp String name that defines the destination.
         */
        std::string convertDestination( const Pointer<ActiveMQDestination>& destination );

        /**
         * Converts a MessageId instance to a Stomp MessageId String.
         *
         * @param messageId - the MessageId instance to convert.
         * @return a Stomp Message Id String.
         */
        std::string convertMessageId( const Pointer<MessageId>& messageId );

        /**
         * Converts a Stomp MessageId string to a MessageId
         *
         * @param messageId - the String message Id to convert.
         * @return Pointer to a new MessageId.
         */
        Pointer<MessageId> convertMessageId( const std::string& messageId );

        /**
         * Converts a ConsumerId instance to a Stomp ConsumerId String.
         *
         * @param consumerId - the Consumer instance to convert.
         * @return a Stomp Consumer Id String.
         */
        std::string convertConsumerId( const Pointer<ConsumerId>& consumerId );

        /**
         * Converts a Stomp ConsumerId string to a ConsumerId
         *
         * @param consumerId - the String Consumer Id to convert.
         * @return Pointer to a new ConsumerId.
         */
        Pointer<ConsumerId> convertConsumerId( const std::string& consumerId );

        /**
         * Converts a ProducerId instance to a Stomp ProducerId String.
         *
         * @param producerId - the Producer instance to convert.
         * @return a Stomp Producer Id String.
         */
        std::string convertProducerId( const Pointer<ProducerId>& producerId );

        /**
         * Converts a Stomp ProducerId string to a ProducerId
         *
         * @param producerId - the String Producer Id to convert.
         * @return Pointer to a new ProducerId.
         */
        Pointer<ProducerId> convertProducerId( const std::string& producerId );

        /**
         * Converts a TransactionId instance to a Stomp TransactionId String.
         *
         * @param transactionId - the Transaction instance to convert.
         * @return a Stomp Transaction Id String.
         */
        std::string convertTransactionId( const Pointer<TransactionId>& transactionId );

        /**
         * Converts a Stomp TransactionId string to a TransactionId
         *
         * @param transactionId - the String Transaction Id to convert.
         * @return Pointer to a new TransactionId.
         */
        Pointer<TransactionId> convertTransactionId( const std::string& transactionId );

    };

}}}

#endif /* _ACTIVEMQ_WIREFORMAT_STOMP_MARSHAL_STOMPHELPER_H_ */
