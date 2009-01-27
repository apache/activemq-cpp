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
#include <activemq/transport/Response.h>
#include <activemq/exceptions/ActiveMQException.h>

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
#include <activemq/commands/ConnectionError.h>
#include <activemq/commands/ConnectionControl.h>
#include <activemq/commands/ConsumerControl.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/KeepAliveInfo.h>
#include <activemq/commands/FlushCommand.h>
#include <activemq/commands/BrokerInfo.h>

namespace activemq {
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

        virtual transport::Response* processAddConnection(
            commands::ConnectionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processAddSession(
            commands::SessionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processAddProducer(
            commands::ProducerInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processAddConsumer(
            commands::ConsumerInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveConnection(
            commands::ConnectionId id ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveSession(
            commands::SessionId id ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveProducer(
            commands::ProducerId id ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveConsumer(
            commands::ConsumerId id ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processAddDestination(
            commands::DestinationInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveDestination(
            commands::DestinationInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRemoveSubscription(
            commands::RemoveSubscriptionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processMessage(
            commands::Message send ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processMessageAck(
            commands::MessageAck ack ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processMessagePull(
            commands::MessagePull pull ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processBeginTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processPrepareTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processCommitTransactionOnePhase(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processCommitTransactionTwoPhase(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRollbackTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processWireFormat(
            commands::WireFormatInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processKeepAlive(
            commands::KeepAliveInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processShutdown(
            commands::ShutdownInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processFlush(
            commands::FlushCommand command ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processBrokerInfo(
            commands::BrokerInfo info) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processRecoverTransactions(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processForgetTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processEndTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processMessageDispatchNotification(
            commands::MessageDispatchNotification notification ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processProducerAck(
            commands::ProducerAck ack ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processMessageDispatch(
            commands::MessageDispatch dispatch ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processControlCommand(
            commands::ControlCommand command ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processConnectionError(
            commands::ConnectionError error ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processConnectionControl(
            commands::ConnectionControl control ) throw ( exceptions::ActiveMQException ) = 0;

        virtual transport::Response* processConsumerControl(
            commands::ConsumerControl control ) throw ( exceptions::ActiveMQException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_STATE_COMMANDVISITOR_H_*/
