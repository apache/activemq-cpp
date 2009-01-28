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

        virtual commands::Command* processTransactionInfo(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processConnectionInfo(
            commands::ConnectionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processSessionInfo(
            commands::SessionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processProducerInfo(
            commands::ProducerInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processConsumerInfo(
            commands::ConsumerInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveConnection(
            commands::ConnectionId* id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveSession(
            commands::SessionId* id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveProducer(
            commands::ProducerId* id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveConsumer(
            commands::ConsumerId* id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processDestinationInfo(
            commands::DestinationInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveDestination(
            commands::DestinationInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveSubscriptionInfo(
            commands::RemoveSubscriptionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processMessage(
            commands::Message* send ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processMessageAck(
            commands::MessageAck* ack ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processMessagePull(
            commands::MessagePull* pull ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processBeginTransaction(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processPrepareTransaction(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processCommitTransactionOnePhase(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processCommitTransactionTwoPhase(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRollbackTransaction(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processWireFormat(
            commands::WireFormatInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processKeepAliveInfo(
            commands::KeepAliveInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processShutdownInfo(
            commands::ShutdownInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processFlushCommand(
            commands::FlushCommand* command ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processBrokerInfo(
            commands::BrokerInfo* info) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRecoverTransactions(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processForgetTransaction(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processEndTransaction(
            commands::TransactionInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processMessageDispatchNotification(
            commands::MessageDispatchNotification* notification ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processProducerAck(
            commands::ProducerAck* ack ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processMessageDispatch(
            commands::MessageDispatch* dispatch ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processControlCommand(
            commands::ControlCommand* command ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processConnectionError(
            commands::ConnectionError* error ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processConnectionControl(
            commands::ConnectionControl* control ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processConsumerControl(
            commands::ConsumerControl* control ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processBrokerError(
            commands::BrokerError* error ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processRemoveInfo(
            commands::RemoveInfo* info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processReplayCommand(
            commands::ReplayCommand* replay ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual commands::Command* processResponse(
            commands::Response* response ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_COMMANDVISITORADAPTER_H_*/
