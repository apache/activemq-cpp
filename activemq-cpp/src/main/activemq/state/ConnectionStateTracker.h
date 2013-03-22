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

#ifndef _ACTIVEMQ_STATE_CONNECTIONSTATETRACKER_H_
#define _ACTIVEMQ_STATE_CONNECTIONSTATETRACKER_H_

#include <activemq/util/Config.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitorAdapter.h>
#include <activemq/state/ConnectionState.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/ProducerState.h>
#include <activemq/state/SessionState.h>
#include <activemq/state/TransactionState.h>
#include <activemq/state/Tracked.h>
#include <activemq/transport/Transport.h>

#include <decaf/lang/Pointer.h>

namespace activemq {
namespace state {

    class RemoveTransactionAction;
    class StateTrackerImpl;

    class AMQCPP_API ConnectionStateTracker: public CommandVisitorAdapter {
    private:

        StateTrackerImpl* impl;

        bool trackTransactions;
        bool restoreSessions;
        bool restoreConsumers;
        bool restoreProducers;
        bool restoreTransaction;
        bool trackMessages;
        bool trackTransactionProducers;
        int maxMessageCacheSize;
        int maxMessagePullCacheSize;

        friend class RemoveTransactionAction;

    public:

        ConnectionStateTracker();

        virtual ~ConnectionStateTracker();

        Pointer<Tracked> track(Pointer<Command> command);

        void trackBack(decaf::lang::Pointer<Command> command);

        void restore(decaf::lang::Pointer<transport::Transport> transport);

        void connectionInterruptProcessingComplete(
            transport::Transport* transport, decaf::lang::Pointer<ConnectionId> connectionId);

        void transportInterrupted();

        virtual decaf::lang::Pointer<Command> processDestinationInfo(DestinationInfo* info);

        virtual decaf::lang::Pointer<Command> processRemoveDestination(DestinationInfo* info);

        virtual decaf::lang::Pointer<Command> processProducerInfo(ProducerInfo* info);

        virtual decaf::lang::Pointer<Command> processRemoveProducer(ProducerId* id);

        virtual decaf::lang::Pointer<Command> processConsumerInfo(ConsumerInfo* info);

        virtual decaf::lang::Pointer<Command> processRemoveConsumer(ConsumerId* id);

        virtual decaf::lang::Pointer<Command> processSessionInfo(SessionInfo* info);

        virtual decaf::lang::Pointer<Command> processRemoveSession(SessionId* id);

        virtual decaf::lang::Pointer<Command> processConnectionInfo(ConnectionInfo* info);

        virtual decaf::lang::Pointer<Command> processRemoveConnection(ConnectionId* id);

        virtual decaf::lang::Pointer<Command> processMessage(Message* message);

        virtual decaf::lang::Pointer<Command> processBeginTransaction(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processPrepareTransaction(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processCommitTransactionOnePhase(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processCommitTransactionTwoPhase(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processRollbackTransaction(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processEndTransaction(TransactionInfo* info);

        virtual decaf::lang::Pointer<Command> processMessagePull(MessagePull* pull);

        bool isRestoreConsumers() const {
            return this->restoreConsumers;
        }

        void setRestoreConsumers(bool restoreConsumers) {
            this->restoreConsumers = restoreConsumers;
        }

        bool isRestoreProducers() const {
            return this->restoreProducers;
        }

        void setRestoreProducers(bool restoreProducers) {
            this->restoreProducers = restoreProducers;
        }

        bool isRestoreSessions() const {
            return this->restoreSessions;
        }

        void setRestoreSessions(bool restoreSessions) {
            this->restoreSessions = restoreSessions;
        }

        bool isTrackTransactions() const {
            return this->trackTransactions;
        }

        void setTrackTransactions(bool trackTransactions) {
            this->trackTransactions = trackTransactions;
        }

        bool isRestoreTransaction() const {
            return this->restoreTransaction;
        }

        void setRestoreTransaction(bool restoreTransaction) {
            this->restoreTransaction = restoreTransaction;
        }

        bool isTrackMessages() const {
            return this->trackMessages;
        }

        void setTrackMessages(bool trackMessages) {
            this->trackMessages = trackMessages;
        }

        int getMaxMessageCacheSize() const {
            return this->maxMessageCacheSize;
        }

        void setMaxMessageCacheSize(int maxMessageCacheSize) {
            this->maxMessageCacheSize = maxMessageCacheSize;
        }

        int getMaxMessagePullCacheSize() const {
            return this->maxMessagePullCacheSize;
        }

        void setMaxMessagePullCacheSize(int maxMessagePullCacheSize) {
            this->maxMessagePullCacheSize = maxMessagePullCacheSize;
        }

        bool isTrackTransactionProducers() const {
            return this->trackTransactionProducers;
        }

        void setTrackTransactionProducers(bool trackTransactionProducers) {
            this->trackTransactionProducers = trackTransactionProducers;
        }

    private:

        void doRestoreTransactions(decaf::lang::Pointer<transport::Transport> transport,
                                   decaf::lang::Pointer<ConnectionState> connectionState);

        void doRestoreSessions(decaf::lang::Pointer<transport::Transport> transport,
                               decaf::lang::Pointer<ConnectionState> connectionState);

        void doRestoreConsumers(decaf::lang::Pointer<transport::Transport> transport,
                                decaf::lang::Pointer<SessionState> sessionState);

        void doRestoreProducers(decaf::lang::Pointer<transport::Transport> transport,
                                decaf::lang::Pointer<SessionState> sessionState);

        void doRestoreTempDestinations(decaf::lang::Pointer<transport::Transport> transport,
                                       decaf::lang::Pointer<ConnectionState> connectionState);

    };

}}

#endif /*_ACTIVEMQ_STATE_CONNECTIONSTATETRACKER_H_*/
