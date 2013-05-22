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

#include "ConnectionStateTracker.h"

#include <decaf/lang/Runnable.h>
#include <decaf/util/HashCode.h>
#include <decaf/util/LinkedHashMap.h>
#include <decaf/util/MapEntry.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/util/concurrent/ConcurrentStlMap.h>

#include <activemq/commands/ConsumerControl.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/wireformat/WireFormat.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::state;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace state {


    class MessageCache : public LinkedHashMap<Pointer<MessageId>, Pointer<Command> > {
    protected:

        ConnectionStateTracker* parent;

    public:

        int currentCacheSize;

    public:

        MessageCache(ConnectionStateTracker* parent) :
            LinkedHashMap<Pointer<MessageId>, Pointer<Command> >(), parent(parent), currentCacheSize(0) {
        }

        virtual ~MessageCache() {}

        virtual bool removeEldestEntry(const MapEntry<Pointer<MessageId>, Pointer<Command> >& eldest) {
            bool result = currentCacheSize > parent->getMaxMessageCacheSize();
            if (result) {
                Pointer<Message> message = eldest.getValue().dynamicCast<Message>();
                currentCacheSize -= message->getSize();
            }
            return result;
        }
    };

    class MessagePullCache : public LinkedHashMap<std::string, Pointer<Command> > {
    protected:

        ConnectionStateTracker* parent;

    public:

        MessagePullCache(ConnectionStateTracker* parent) :
            LinkedHashMap<std::string, Pointer<Command> >(), parent(parent) {
        }

        virtual ~MessagePullCache() {}

        virtual bool removeEldestEntry(const MapEntry<std::string, Pointer<Command> >& eldest AMQCPP_UNUSED) {
            return size() > parent->getMaxMessagePullCacheSize();
        }
    };

    class StateTrackerImpl {
    private:

        StateTrackerImpl(const StateTrackerImpl&);
        StateTrackerImpl& operator= (const StateTrackerImpl&);

    public:

        /** Parent ConnectionStateTracker */
        ConnectionStateTracker* parent;

        /** Creates a unique marker for this state tracker */
        const Pointer<Tracked> TRACKED_RESPONSE_MARKER;

        /** Map holding the ConnectionStates, indexed by the ConnectionId */
        ConcurrentStlMap<Pointer<ConnectionId>, Pointer<ConnectionState>, ConnectionId::COMPARATOR> connectionStates;

        /** Store Messages if trackMessages == true */
        MessageCache messageCache;

        /** Store MessagePull commands for replay */
        MessagePullCache messagePullCache;

        StateTrackerImpl(ConnectionStateTracker * parent) : parent(parent),
                                                            TRACKED_RESPONSE_MARKER(new Tracked()),
                                                            connectionStates(),
                                                            messageCache(parent),
                                                            messagePullCache(parent) {
        }

        ~StateTrackerImpl() {
            try {
                connectionStates.clear();
                messageCache.clear();
                messagePullCache.clear();
            }
            AMQ_CATCHALL_NOTHROW()
        }
    };

    class RemoveTransactionAction : public Runnable {
    private:

        Pointer<TransactionInfo> info;
        ConnectionStateTracker* stateTracker;

    private:

        RemoveTransactionAction(const RemoveTransactionAction&);
        RemoveTransactionAction& operator=(const RemoveTransactionAction&);

    public:

        RemoveTransactionAction(ConnectionStateTracker* stateTracker, Pointer<TransactionInfo> info) :
            info(info), stateTracker(stateTracker) {
        }

        virtual ~RemoveTransactionAction() {}

        virtual void run() {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            Pointer<ConnectionState> cs = stateTracker->impl->connectionStates.get(connectionId);
            Pointer<TransactionState> txState = cs->removeTransactionState(info->getTransactionId());
            if (txState != NULL) {
                txState->clear();
            }
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ConnectionStateTracker::ConnectionStateTracker() : impl(new StateTrackerImpl(this)),
                                                   trackTransactions(false),
                                                   restoreSessions(true),
                                                   restoreConsumers(true),
                                                   restoreProducers(true),
                                                   restoreTransaction(true),
                                                   trackMessages(true),
                                                   trackTransactionProducers(true),
                                                   maxMessageCacheSize(128 * 1024),
                                                   maxMessagePullCacheSize(10) {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionStateTracker::~ConnectionStateTracker() {
    try {
        delete impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Tracked> ConnectionStateTracker::track(Pointer<Command> command) {

    try{

        Pointer<Command> result = command->visit(this);
        if (result == NULL) {
            return Pointer<Tracked>();
        } else {
            return result.dynamicCast<Tracked>();
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::trackBack(Pointer<Command> command) {

    try {
        if (command != NULL) {
            if (trackMessages && command->isMessage()) {
                Pointer<Message> message = command.dynamicCast<Message>();
                if (message->getTransactionId() == NULL) {
                    this->impl->messageCache.currentCacheSize += message->getSize();
                }
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::restore(Pointer<transport::Transport> transport) {

    try {

        Pointer<Iterator<Pointer<ConnectionState> > > iterator(
            this->impl->connectionStates.values().iterator());

        while (iterator->hasNext()) {
            Pointer<ConnectionState> state = iterator->next();

            Pointer<ConnectionInfo> info = state->getInfo();
            info->setFailoverReconnect(true);
            transport->oneway(info);

            doRestoreTempDestinations(transport, state);

            if (restoreSessions) {
                doRestoreSessions(transport, state);
            }

            if (restoreTransaction) {
                doRestoreTransactions(transport, state);
            }
        }

        // Now we flush messages
        Pointer<Iterator<Pointer<Command> > > messages(this->impl->messageCache.values().iterator());
        while (messages->hasNext()) {
            transport->oneway(messages->next());
        }

        Pointer<Iterator<Pointer<Command> > > messagePullIter(this->impl->messagePullCache.values().iterator());
        while (messagePullIter->hasNext()) {
            transport->oneway(messagePullIter->next());
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreTransactions(Pointer<transport::Transport> transport, Pointer<ConnectionState> connectionState) {

    try {

        std::vector<Pointer<TransactionInfo> > toRollback;

        // For any completed transactions we don't know if the commit actually made it to the broker
        // or was lost along the way, so they need to be rolled back.
        Pointer<Iterator<Pointer<TransactionState> > > iter(connectionState->getTransactionStates().iterator());
        while (iter->hasNext()) {

            Pointer<TransactionState> txState = iter->next();
            Pointer<Command> lastCommand = txState->getCommands().getLast();
            if (lastCommand->isTransactionInfo()) {
                Pointer<TransactionInfo> transactionInfo = lastCommand.dynamicCast<TransactionInfo>();
                if (transactionInfo->getType() == ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE) {
                    toRollback.push_back(transactionInfo);
                    continue;
                }
            }

            // replay short lived producers that may have been involved in the transaction
            Pointer<Iterator<Pointer<ProducerState> > > state(txState->getProducerStates().iterator());
            while (state->hasNext()) {
                transport->oneway(state->next()->getInfo());
            }

            std::auto_ptr<Iterator<Pointer<Command> > > commands(txState->getCommands().iterator());

            while (commands->hasNext()) {
                transport->oneway(commands->next());
            }

            state.reset(txState->getProducerStates().iterator());
            while (state->hasNext()) {
                transport->oneway(state->next()->getInfo()->createRemoveCommand());
            }
        }

        // Trigger failure of commit for all outstanding completed but in doubt transactions.
        std::vector<Pointer<TransactionInfo> >::const_iterator command = toRollback.begin();
        for (; command != toRollback.end(); ++command) {
            Pointer<ExceptionResponse> response(new ExceptionResponse());
            Pointer<BrokerError> exception(new BrokerError());
            exception->setExceptionClass("TransactionRolledBackException");
            exception->setMessage(
                    std::string("Transaction completion in doubt due to failover. Forcing rollback of ") + (*command)->getTransactionId()->toString());
            response->setException(exception);
            response->setCorrelationId((*command)->getCommandId());
            transport->getTransportListener()->onCommand(response);
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreSessions(Pointer<transport::Transport> transport, Pointer<ConnectionState> connectionState) {

    try {

        Pointer<Iterator<Pointer<SessionState> > > iter(connectionState->getSessionStates().iterator());
        while (iter->hasNext()) {
            Pointer<SessionState> state = iter->next();
            transport->oneway(state->getInfo());

            if (restoreProducers) {
                doRestoreProducers(transport, state);
            }

            if (restoreConsumers) {
                doRestoreConsumers(transport, state);
            }
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreConsumers(Pointer<transport::Transport> transport, Pointer<SessionState> sessionState) {

    try {

        // Restore the session's consumers but possibly in pull only (prefetch 0 state) till recovery complete
        Pointer<ConnectionState> connectionState =
            this->impl->connectionStates.get(sessionState->getInfo()->getSessionId()->getParentId());
        bool connectionInterruptionProcessingComplete = connectionState->isConnectionInterruptProcessingComplete();

        Pointer<Iterator<Pointer<ConsumerState> > > state(sessionState->getConsumerStates().iterator());
        while (state->hasNext()) {

            Pointer<ConsumerInfo> infoToSend = state->next()->getInfo();
            Pointer<wireformat::WireFormat> wireFormat = transport->getWireFormat();

            if (!connectionInterruptionProcessingComplete && infoToSend->getPrefetchSize() > 0 && wireFormat->getVersion() > 5) {

                Pointer<ConsumerInfo> oldInfoToSend = infoToSend;
                infoToSend.reset(oldInfoToSend->cloneDataStructure());
                connectionState->getRecoveringPullConsumers().put(infoToSend->getConsumerId(), oldInfoToSend);
                infoToSend->setPrefetchSize(0);
            }

            transport->oneway(infoToSend);
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreProducers(Pointer<transport::Transport> transport, Pointer<SessionState> sessionState) {

    try {

        // Restore the session's producers
        Pointer<Iterator<Pointer<ProducerState> > > iter(sessionState->getProducerStates().iterator());
        while (iter->hasNext()) {
            Pointer<ProducerState> state = iter->next();
            transport->oneway(state->getInfo());
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreTempDestinations(Pointer<transport::Transport> transport, Pointer<ConnectionState> connectionState) {
    try {
        std::auto_ptr<Iterator<Pointer<DestinationInfo> > > iter(connectionState->getTempDesinations().iterator());

        while (iter->hasNext()) {
            transport->oneway(iter->next());
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processDestinationInfo(DestinationInfo* info) {

    try {
        if (info != NULL) {
            Pointer<ConnectionState> cs = this->impl->connectionStates.get(info->getConnectionId());
            if (cs != NULL && info->getDestination()->isTemporary()) {
                cs->addTempDestination(Pointer<DestinationInfo>(info->cloneDataStructure()));
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveDestination(DestinationInfo* info) {

    try {
        if (info != NULL) {
            Pointer<ConnectionState> cs = this->impl->connectionStates.get(info->getConnectionId());
            if (cs != NULL && info->getDestination()->isTemporary()) {
                cs->removeTempDestination(info->getDestination());
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processProducerInfo(ProducerInfo* info) {

    try {
        if (info != NULL && info->getProducerId() != NULL) {
            Pointer<SessionId> sessionId = info->getProducerId()->getParentId();
            if (sessionId != NULL) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if (connectionId != NULL) {
                    Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                    if (cs != NULL) {
                        Pointer<SessionState> ss = cs->getSessionState(sessionId);
                        if (ss != NULL) {
                            ss->addProducer(Pointer<ProducerInfo>(info->cloneDataStructure()));
                        }
                    }
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveProducer(ProducerId* id) {

    try {
        if (id != NULL) {
            Pointer<SessionId> sessionId = id->getParentId();
            if (sessionId != NULL) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if (connectionId != NULL) {
                    Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                    if (cs != NULL) {
                        Pointer<SessionState> ss = cs->getSessionState(sessionId);
                        if (ss != NULL) {
                            ss->removeProducer(Pointer<ProducerId>(id->cloneDataStructure()));
                        }
                    }
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processConsumerInfo(ConsumerInfo* info) {

    try {

        if (info != NULL) {
            Pointer<SessionId> sessionId = info->getConsumerId()->getParentId();
            if (sessionId != NULL) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if (connectionId != NULL) {
                    Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                    if (cs != NULL) {
                        Pointer<SessionState> ss = cs->getSessionState(sessionId);
                        if (ss != NULL) {
                            ss->addConsumer(Pointer<ConsumerInfo>(info->cloneDataStructure()));
                        }
                    }
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveConsumer(ConsumerId* id) {

    try {
        if (id != NULL) {
            Pointer<SessionId> sessionId = id->getParentId();
            if (sessionId != NULL) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if (connectionId != NULL) {
                    Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                    if (cs != NULL) {
                        Pointer<SessionState> ss = cs->getSessionState(sessionId);
                        if (ss != NULL) {
                            ss->removeConsumer(Pointer<ConsumerId>(id->cloneDataStructure()));
                        }
                    }
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processSessionInfo(SessionInfo* info) {

    try {

        if (info != NULL) {
            Pointer<ConnectionId> connectionId = info->getSessionId()->getParentId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    cs->addSession(Pointer<SessionInfo>(info->cloneDataStructure()));
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveSession(SessionId* id) {

    try {

        if (id != NULL) {
            Pointer<ConnectionId> connectionId = id->getParentId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    cs->removeSession(Pointer<SessionId>(id->cloneDataStructure()));
                }
            }
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processConnectionInfo(ConnectionInfo* info) {

    try {
        if (info != NULL) {
            Pointer<ConnectionInfo> infoCopy(info->cloneDataStructure());
            this->impl->connectionStates.put(
                info->getConnectionId(), Pointer<ConnectionState>(new ConnectionState(infoCopy)));
        }
        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveConnection(ConnectionId* id) {

    try {
        if (id != NULL) {
            this->impl->connectionStates.remove(Pointer<ConnectionId>(id->cloneDataStructure()));
        }

        return this->impl->TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processMessage(Message* message) {

    try {

        if (message != NULL) {
            if (trackTransactions && message->getTransactionId() != NULL) {
                Pointer<ProducerId> producerId = message->getProducerId();
                Pointer<ConnectionId> connectionId = producerId->getParentId()->getParentId();

                if (connectionId != NULL) {
                    Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                    if (cs != NULL) {
                        Pointer<TransactionState> transactionState = cs->getTransactionState(message->getTransactionId());
                        if (transactionState != NULL) {
                            transactionState->addCommand(Pointer<Command>(message->cloneDataStructure()));

                            if (trackTransactionProducers) {
                                // Track the producer in case it is closed before a commit
                                Pointer<SessionState> sessionState = cs->getSessionState(producerId->getParentId());
                                Pointer<ProducerState> producerState = sessionState->getProducerState(producerId);
                                producerState->setTransactionState(transactionState);
                            }
                        }
                    }
                }
                return this->impl->TRACKED_RESPONSE_MARKER;
            } else if (trackMessages) {
                this->impl->messageCache.put(
                    message->getMessageId(), Pointer<Message>(message->cloneDataStructure()));
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processBeginTransaction(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    cs->addTransactionState(info->getTransactionId());
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    transactionState->addCommand(Pointer<Command>(info->cloneDataStructure()));
                }
            }

            return this->impl->TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processPrepareTransaction(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    if (transactionState != NULL) {
                        transactionState->addCommand(Pointer<Command>(info->cloneDataStructure()));
                    }
                }
            }

            return this->impl->TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processCommitTransactionOnePhase(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    if (transactionState != NULL) {
                        Pointer<TransactionInfo> infoCopy(info->cloneDataStructure());
                        transactionState->addCommand(infoCopy);
                        return Pointer<Tracked>(new Tracked(Pointer<Runnable>(new RemoveTransactionAction(this, infoCopy))));
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processCommitTransactionTwoPhase(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    if (transactionState != NULL) {
                        Pointer<TransactionInfo> infoCopy(info->cloneDataStructure());
                        transactionState->addCommand(infoCopy);
                        return Pointer<Tracked>(new Tracked(Pointer<Runnable>(new RemoveTransactionAction(this, infoCopy))));
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRollbackTransaction(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    if (transactionState != NULL) {
                        Pointer<TransactionInfo> infoCopy(info->cloneDataStructure());
                        transactionState->addCommand(infoCopy);
                        return Pointer<Tracked>(new Tracked(Pointer<Runnable>(new RemoveTransactionAction(this, infoCopy))));
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processEndTransaction(TransactionInfo* info) {

    try {

        if (trackTransactions && info != NULL) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if (connectionId != NULL) {
                Pointer<ConnectionState> cs = this->impl->connectionStates.get(connectionId);
                if (cs != NULL) {
                    Pointer<TransactionState> transactionState = cs->getTransactionState(info->getTransactionId());
                    if (transactionState != NULL) {
                        transactionState->addCommand(Pointer<Command>(info->cloneDataStructure()));
                    }
                }
            }

            return this->impl->TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processMessagePull(MessagePull* pull) {

    try {

        if (pull != NULL && pull->getDestination() != NULL && pull->getConsumerId() != NULL) {
            std::string id = pull->getDestination()->toString() + "::" + pull->getConsumerId()->toString();
            this->impl->messagePullCache.put(id, Pointer<Command>(pull->cloneDataStructure()));
        }

        return Pointer<Command>();
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::connectionInterruptProcessingComplete(transport::Transport* transport, Pointer<ConnectionId> connectionId) {

    Pointer<ConnectionState> connectionState = this->impl->connectionStates.get(connectionId);

    if (connectionState != NULL) {

        connectionState->setConnectionInterruptProcessingComplete(true);

        StlMap<Pointer<ConsumerId>, Pointer<ConsumerInfo>, ConsumerId::COMPARATOR> stalledConsumers = connectionState->getRecoveringPullConsumers();

        Pointer<Iterator<Pointer<ConsumerId> > > key(stalledConsumers.keySet().iterator());
        while (key->hasNext()) {
            Pointer<ConsumerControl> control(new ConsumerControl());

            Pointer<ConsumerId> theKey = key->next();

            control->setConsumerId(theKey);
            control->setPrefetch(stalledConsumers.get(theKey)->getPrefetchSize());
            control->setDestination(stalledConsumers.get(theKey)->getDestination());

            try {
                transport->oneway(control);
            } catch (Exception& ex) {
            }
        }

        stalledConsumers.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::transportInterrupted() {

    Pointer<Iterator<Pointer<ConnectionState> > > state(this->impl->connectionStates.values().iterator());
    while (state->hasNext()) {
        state->next()->setConnectionInterruptProcessingComplete(false);
    }
}
