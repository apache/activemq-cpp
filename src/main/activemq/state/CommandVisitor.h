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

#ifndef _ACTIVEMQ_STATE_COMMANDVISITOR_H_
#define _ACTIVEMQ_STATE_COMMANDVISITOR_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace commands {
    class Command;
    class ConnectionInfo;
    class SessionInfo;
    class ProducerInfo;
    class ConnectionId;
    class SessionId;
    class ProducerId;
    class ConsumerId;
    class ConsumerInfo;
    class DestinationInfo;
    class RemoveSubscriptionInfo;
    class Message;
    class MessageAck;
    class MessagePull;
    class TransactionInfo;
    class WireFormatInfo;
    class ProducerAck;
    class MessageDispatch;
    class MessageDispatchNotification;
    class ControlCommand;
    class ConnectionError;
    class ConnectionControl;
    class ConsumerControl;
    class ShutdownInfo;
    class KeepAliveInfo;
    class FlushCommand;
    class BrokerError;
    class BrokerInfo;
    class RemoveInfo;
    class Response;
    class ReplayCommand;
}
namespace state {

    /**
     * Interface for an Object that can visit the various Command Objects that
     * are sent from and to this client.  The Commands themselves implement a
     * <code>visit</code> method that is called with an instance of this interface
     * and each one then call the appropriate <code>processXXX</code> method.
     *
     * @since 3.0
     */
    class AMQCPP_API CommandVisitor {
    public:

        virtual ~CommandVisitor() {}

        virtual decaf::lang::Pointer<commands::Command> processTransactionInfo(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveInfo(
            commands::RemoveInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processConnectionInfo(
            commands::ConnectionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processSessionInfo(
            commands::SessionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processProducerInfo(
            commands::ProducerInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processConsumerInfo(
            commands::ConsumerInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveConnection(
            commands::ConnectionId* id ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveSession(
            commands::SessionId* id ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveProducer(
            commands::ProducerId* id ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveConsumer(
            commands::ConsumerId* id ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processDestinationInfo(
            commands::DestinationInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveDestination(
            commands::DestinationInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRemoveSubscriptionInfo(
            commands::RemoveSubscriptionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processMessage(
            commands::Message* send ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processMessageAck(
            commands::MessageAck* ack ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processMessagePull(
            commands::MessagePull* pull ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processBeginTransaction(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processPrepareTransaction(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processCommitTransactionOnePhase(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processCommitTransactionTwoPhase(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRollbackTransaction(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processWireFormat(
            commands::WireFormatInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processKeepAliveInfo(
            commands::KeepAliveInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processShutdownInfo(
            commands::ShutdownInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processFlushCommand(
            commands::FlushCommand* command ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processBrokerInfo(
            commands::BrokerInfo* info) = 0;

        virtual decaf::lang::Pointer<commands::Command> processRecoverTransactions(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processForgetTransaction(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processEndTransaction(
            commands::TransactionInfo* info ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processMessageDispatchNotification(
            commands::MessageDispatchNotification* notification ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processProducerAck(
            commands::ProducerAck* ack ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processMessageDispatch(
            commands::MessageDispatch* dispatch ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processControlCommand(
            commands::ControlCommand* command ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processConnectionError(
            commands::ConnectionError* error ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processConnectionControl(
            commands::ConnectionControl* control ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processConsumerControl(
            commands::ConsumerControl* control ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processBrokerError(
            commands::BrokerError* error ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processReplayCommand(
            commands::ReplayCommand* replay ) = 0;

        virtual decaf::lang::Pointer<commands::Command> processResponse(
            commands::Response* response ) = 0;

    };

}}

#endif /*_ACTIVEMQ_STATE_COMMANDVISITOR_H_*/
