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

        virtual transport::Response* processAddConnection(
            commands::ConnectionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processAddSession(
            commands::SessionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processAddProducer(
            commands::ProducerInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processAddConsumer(
            commands::ConsumerInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveConnection(
            commands::ConnectionId id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveSession(
            commands::SessionId id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveProducer(
            commands::ProducerId id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveConsumer(
            commands::ConsumerId id ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processAddDestination(
            commands::DestinationInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveDestination(
            commands::DestinationInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRemoveSubscription(
            commands::RemoveSubscriptionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processMessage(
            commands::Message send ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processMessageAck(
            commands::MessageAck ack ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processMessagePull(
            commands::MessagePull pull ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processBeginTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processPrepareTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processCommitTransactionOnePhase(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processCommitTransactionTwoPhase(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRollbackTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processWireFormat(
            commands::WireFormatInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processKeepAlive(
            commands::KeepAliveInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processShutdown(
            commands::ShutdownInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processFlush(
            commands::FlushCommand command ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processBrokerInfo(
            commands::BrokerInfo info) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processRecoverTransactions(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processForgetTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processEndTransaction(
            commands::TransactionInfo info ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processMessageDispatchNotification(
            commands::MessageDispatchNotification notification ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processProducerAck(
            commands::ProducerAck ack ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processMessageDispatch(
            commands::MessageDispatch dispatch ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processControlCommand(
            commands::ControlCommand command ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processConnectionError(
            commands::ConnectionError error ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processConnectionControl(
            commands::ConnectionControl control ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

        virtual transport::Response* processConsumerControl(
            commands::ConsumerControl control ) throw ( exceptions::ActiveMQException ) {

            return NULL;
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_COMMANDVISITORADAPTER_H_*/
