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

#ifndef _ACTIVEMQ_STATE_COMMANDVISITORADAPTER_H_
#define _ACTIVEMQ_STATE_COMMANDVISITORADAPTER_H_

#include <activemq/util/Config.h>

#include <activemq/state/CommandVisitor.h>
#include <activemq/core/ActiveMQConstants.h>

#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/commands/SessionId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/ProducerAck.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/commands/MessageDispatchNotification.h>
#include <activemq/commands/ControlCommand.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/ConnectionError.h>
#include <activemq/commands/ConnectionControl.h>
#include <activemq/commands/ConsumerControl.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/KeepAliveInfo.h>
#include <activemq/commands/FlushCommand.h>
#include <activemq/commands/BrokerError.h>
#include <activemq/commands/BrokerInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ReplayCommand.h>
#include <activemq/commands/Response.h>

namespace activemq {
namespace state {

    /**
     * Default Implementation of a CommandVisitor that returns NULL for all calls.
     *
     * @since 3.0
     */
    class AMQCPP_API CommandVisitorAdapter : public CommandVisitor {
    public:

        virtual ~CommandVisitorAdapter() {}

        virtual decaf::lang::Pointer<commands::Command> processRemoveConnection(
            commands::ConnectionId* id AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveSession(
            commands::SessionId* id AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveProducer(
            commands::ProducerId* id AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveConsumer(
            commands::ConsumerId* id AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processDestinationInfo(
            commands::DestinationInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveDestination(
            commands::DestinationInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveSubscriptionInfo(
            commands::RemoveSubscriptionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processMessage(
            commands::Message* send AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processMessageAck(
            commands::MessageAck* ack AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processMessagePull(
            commands::MessagePull* pull AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processBeginTransaction(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processPrepareTransaction(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processCommitTransactionOnePhase(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processCommitTransactionTwoPhase(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRollbackTransaction(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processWireFormat(
            commands::WireFormatInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processKeepAliveInfo(
            commands::KeepAliveInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processShutdownInfo(
            commands::ShutdownInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processFlushCommand(
            commands::FlushCommand* command AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processBrokerInfo(
            commands::BrokerInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRecoverTransactions(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processForgetTransaction(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processEndTransaction(
            commands::TransactionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processMessageDispatchNotification(
            commands::MessageDispatchNotification* notification AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processProducerAck(
            commands::ProducerAck* ack AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processMessageDispatch(
            commands::MessageDispatch* dispatch AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processControlCommand(
            commands::ControlCommand* command AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processConnectionError(
            commands::ConnectionError* error AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processConnectionControl(
            commands::ConnectionControl* control AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processConsumerControl(
            commands::ConsumerControl* control AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processBrokerError(
            commands::BrokerError* error AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processReplayCommand(
            commands::ReplayCommand* replay AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processResponse(
            commands::Response* response AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processConnectionInfo(
            commands::ConnectionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processSessionInfo(
            commands::SessionInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processProducerInfo(
            commands::ProducerInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processConsumerInfo(
            commands::ConsumerInfo* info AMQCPP_UNUSED ) {

            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processTransactionInfo(
            commands::TransactionInfo* info ) {

            if( info != decaf::lang::Pointer<commands::Command>() ) {
                switch( info->getType() ) {
                    case core::ActiveMQConstants::TRANSACTION_STATE_BEGIN:
                        return this->processBeginTransaction( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE:
                        return this->processCommitTransactionOnePhase( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_COMMITTWOPHASE:
                        return this->processCommitTransactionTwoPhase( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_END:
                        return this->processEndTransaction( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_FORGET:
                        return this->processForgetTransaction( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_PREPARE:
                        return this->processPrepareTransaction( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_RECOVER:
                        return this->processRecoverTransactions( info );
                    case core::ActiveMQConstants::TRANSACTION_STATE_ROLLBACK:
                        return this->processRollbackTransaction( info );
                    default:
                        throw exceptions::ActiveMQException(
                            __FILE__, __LINE__, "Unknown Transaction Info Type.");
                }
            }
            return decaf::lang::Pointer<commands::Command>();
        }

        virtual decaf::lang::Pointer<commands::Command> processRemoveInfo(
            commands::RemoveInfo* info ) {

            if( info != decaf::lang::Pointer<commands::Command>() ) {
                switch( info->getObjectId()->getDataStructureType() ) {
                    case commands::ConnectionId::ID_CONNECTIONID:
                        return this->processRemoveConnection(
                            dynamic_cast<commands::ConnectionId*>( info->getObjectId().get() ) );
                    case commands::SessionId::ID_SESSIONID:
                        return this->processRemoveSession(
                            dynamic_cast<commands::SessionId*>( info->getObjectId().get() ) );
                    case commands::ConsumerId::ID_CONSUMERID:
                        return this->processRemoveConsumer(
                            dynamic_cast<commands::ConsumerId*>( info->getObjectId().get() ) );
                    case commands::ProducerId::ID_PRODUCERID:
                        return this->processRemoveProducer(
                            dynamic_cast<commands::ProducerId*>( info->getObjectId().get() ) );
                    default:
                        throw exceptions::ActiveMQException(
                            __FILE__, __LINE__, "Unknown Remove Info Type.");
                }
            }
            return decaf::lang::Pointer<commands::Command>();
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_COMMANDVISITORADAPTER_H_*/
