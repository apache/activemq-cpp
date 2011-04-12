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

#include <decaf/util/concurrent/ConcurrentStlMap.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace state {

    class RemoveTransactionAction;
    using decaf::lang::Pointer;
    using decaf::util::concurrent::ConcurrentStlMap;

    class AMQCPP_API ConnectionStateTracker : public CommandVisitorAdapter {
    private:

        /** Creates a unique marker for this state tracker */
        const Pointer<Tracked> TRACKED_RESPONSE_MARKER;

        /** Map holding the ConnectionStates, indexed by the ConnectionId */
        ConcurrentStlMap< Pointer<ConnectionId>, Pointer<ConnectionState>,
                          ConnectionId::COMPARATOR > connectionStates;

        // TODO - The Map doesn't have a way to automatically remove the eldest Entry
        //        Either we need to implement something similar to LinkedHashMap or find
        //        some other way of tracking the eldest entry into the map and removing it
        //        if the cache size is exceeded.
        ConcurrentStlMap< Pointer<MessageId>, Pointer<Message>,
                          MessageId::COMPARATOR > messageCache;

        bool trackTransactions;
        bool restoreSessions;
        bool restoreConsumers;
        bool restoreProducers;
        bool restoreTransaction;
        bool trackMessages;
        bool trackTransactionProducers;
        int maxCacheSize;
        int currentCacheSize;

        friend class RemoveTransactionAction;

    public:

        ConnectionStateTracker();

        virtual ~ConnectionStateTracker();

        Pointer<Tracked> track( const Pointer<Command>& command );

        void trackBack( const Pointer<Command>& command );

        void restore( const Pointer<transport::Transport>& transport );

        void connectionInterruptProcessingComplete(
            transport::Transport* transport, const Pointer<ConnectionId>& connectionId );

        void transportInterrupted();

        virtual Pointer<Command> processDestinationInfo( DestinationInfo* info );

        virtual Pointer<Command> processRemoveDestination( DestinationInfo* info );

        virtual Pointer<Command> processProducerInfo( ProducerInfo* info );

        virtual Pointer<Command> processRemoveProducer( ProducerId* id );

        virtual Pointer<Command> processConsumerInfo( ConsumerInfo* info );

        virtual Pointer<Command> processRemoveConsumer( ConsumerId* id );

        virtual Pointer<Command> processSessionInfo( SessionInfo* info );

        virtual Pointer<Command> processRemoveSession( SessionId* id );

        virtual Pointer<Command> processConnectionInfo( ConnectionInfo* info );

        virtual Pointer<Command> processRemoveConnection( ConnectionId* id );

        virtual Pointer<Command> processMessage( Message* message );

        virtual Pointer<Command> processMessageAck( MessageAck* ack );

        virtual Pointer<Command> processBeginTransaction( TransactionInfo* info );

        virtual Pointer<Command> processPrepareTransaction( TransactionInfo* info );

        virtual Pointer<Command> processCommitTransactionOnePhase( TransactionInfo* info );

        virtual Pointer<Command> processCommitTransactionTwoPhase( TransactionInfo* info );

        virtual Pointer<Command> processRollbackTransaction( TransactionInfo* info );

        virtual Pointer<Command> processEndTransaction( TransactionInfo* info );

        bool isRestoreConsumers() const {
            return this->restoreConsumers;
        }

        void setRestoreConsumers( bool restoreConsumers ) {
            this->restoreConsumers = restoreConsumers;
        }

        bool isRestoreProducers() const {
            return this->restoreProducers;
        }

        void setRestoreProducers( bool restoreProducers ) {
            this->restoreProducers = restoreProducers;
        }

        bool isRestoreSessions() const {
            return this->restoreSessions;
        }

        void setRestoreSessions( bool restoreSessions ) {
            this->restoreSessions = restoreSessions;
        }

        bool isTrackTransactions() const {
            return this->trackTransactions;
        }

        void setTrackTransactions( bool trackTransactions ) {
            this->trackTransactions = trackTransactions;
        }

        bool isRestoreTransaction() const {
            return this->restoreTransaction;
        }

        void setRestoreTransaction( bool restoreTransaction ) {
            this->restoreTransaction = restoreTransaction;
        }

        bool isTrackMessages() const {
            return this->trackMessages;
        }

        void setTrackMessages( bool trackMessages ) {
            this->trackMessages = trackMessages;
        }

        int getMaxCacheSize() const {
            return this->maxCacheSize;
        }

        void setMaxCacheSize( int maxCacheSize ) {
            this->maxCacheSize = maxCacheSize;
        }

        bool isTrackTransactionProducers() const {
            return this->trackTransactionProducers;
        }

        void setTrackTransactionProducers( bool trackTransactionProducers ) {
            this->trackTransactionProducers = trackTransactionProducers;
        }

    private:

        void doRestoreTransactions( const Pointer<transport::Transport>& transport,
                                    const Pointer<ConnectionState>& connectionState );

        void doRestoreSessions( const Pointer<transport::Transport>& transport,
                                const Pointer<ConnectionState>& connectionState );

        void doRestoreConsumers( const Pointer<transport::Transport>& transport,
                                 const Pointer<SessionState>& sessionState );

        void doRestoreProducers( const Pointer<transport::Transport>& transport,
                                 const Pointer<SessionState>& sessionState );

        void doRestoreTempDestinations( const Pointer<transport::Transport>& transport,
                                        const Pointer<ConnectionState>& connectionState );

    };

}}

#endif /*_ACTIVEMQ_STATE_CONNECTIONSTATETRACKER_H_*/
