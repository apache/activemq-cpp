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
#include <decaf/util/NoSuchElementException.h>

#include <activemq/commands/ConsumerControl.h>
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

    class RemoveTransactionAction : public Runnable {
    private:

        const Pointer<TransactionInfo> info;
        ConnectionStateTracker* stateTracker;

    private:

        RemoveTransactionAction( const RemoveTransactionAction& );
        RemoveTransactionAction& operator= ( const RemoveTransactionAction& );

    public:

        RemoveTransactionAction( ConnectionStateTracker* stateTracker,
                                 const Pointer<TransactionInfo>& info ) :
            info( info ), stateTracker( stateTracker ) {}

        virtual ~RemoveTransactionAction() {}

        virtual void run() {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            Pointer<ConnectionState> cs = stateTracker->connectionStates.get( connectionId );
            cs->removeTransactionState( info->getTransactionId() );
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ConnectionStateTracker::ConnectionStateTracker() : TRACKED_RESPONSE_MARKER( new Tracked() ),
                                                   connectionStates(),
                                                   messageCache(),
                                                   trackTransactions(false),
                                                   restoreSessions(true),
                                                   restoreConsumers(true),
                                                   restoreProducers(true),
                                                   restoreTransaction(true),
                                                   trackMessages(true),
                                                   trackTransactionProducers(true),
                                                   maxCacheSize(128 * 1024),
                                                   currentCacheSize(0) {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionStateTracker::~ConnectionStateTracker() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Tracked> ConnectionStateTracker::track( const Pointer<Command>& command ) {

    try{

        Pointer<Command> result = command->visit( this );
        if( result == NULL ) {
            return Pointer<Tracked>();
        } else {
            return result.dynamicCast<Tracked>();
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::trackBack( const Pointer<Command>& command ) {

    try{
        if( trackMessages && command != NULL && command->isMessage() ) {
            Pointer<Message> message =
                command.dynamicCast<Message>();
            if( message->getTransactionId() == NULL ) {
                currentCacheSize = currentCacheSize + message->getSize();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::restore( const Pointer<transport::Transport>& transport ) {

    try{

        std::vector< Pointer<ConnectionState> > connectionStates = this->connectionStates.values();
        std::vector< Pointer<ConnectionState> >::const_iterator iter = connectionStates.begin();

        for( ; iter != connectionStates.end(); ++iter ) {
            Pointer<ConnectionState> state = *iter;

            Pointer<ConnectionInfo> info = state->getInfo();
            info->setFailoverReconnect( true );
            transport->oneway( info );

            doRestoreTempDestinations( transport, state );

            if( restoreSessions ) {
                doRestoreSessions( transport, state );
            }

            if( restoreTransaction ) {
                doRestoreTransactions( transport, state );
            }
        }

        // Now we flush messages
        std::vector< Pointer<Message> > messages = messageCache.values();
        std::vector< Pointer<Message> >::const_iterator messageIter = messages.begin();

        for( ; messageIter != messages.end(); ++messageIter ) {
            transport->oneway( *messageIter );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreTransactions( const Pointer<transport::Transport>& transport,
                                                    const Pointer<ConnectionState>& connectionState ) {

    try{

        std::vector< Pointer<Command> > toIgnore;

        // Restore the session's transaction state
        std::vector< Pointer<TransactionState> > transactionStates =
            connectionState->getTransactionStates();

        std::vector< Pointer<TransactionState> >::const_iterator iter = transactionStates.begin();

        for( ; iter != transactionStates.end(); ++iter ) {

            // ignore any empty (ack) transaction
            if( (*iter)->getCommands().size() == 2 ) {
                Pointer<Command> lastCommand = (*iter)->getCommands().get(1);
                if( lastCommand->isTransactionInfo() ) {
                    Pointer<TransactionInfo> transactionInfo = lastCommand.dynamicCast<TransactionInfo>();

                    if( transactionInfo->getType() == ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE ) {
                        toIgnore.push_back(lastCommand);
                        continue;
                    }
                }
            }

            // replay short lived producers that may have been involved in the transaction
            std::vector< Pointer<ProducerState> > producerStates = (*iter)->getProducerStates();
            std::vector< Pointer<ProducerState> >::const_iterator state = producerStates.begin();

            for( ; state != producerStates.end(); ++state ) {
                transport->oneway( (*state)->getInfo() );
            }

            std::auto_ptr< Iterator< Pointer<Command> > > commands(
                (*iter)->getCommands().iterator() );

            while( commands->hasNext() ) {
                transport->oneway( commands->next() );
            }

            state = producerStates.begin();
            for( ; state != producerStates.end(); ++state ) {
                transport->oneway( (*state)->getInfo()->createRemoveCommand() );
            }
        }

        std::vector< Pointer<Command> >::const_iterator command = toIgnore.begin();
        for( ; command != toIgnore.end(); ++command ) {
            // respond to the outstanding commit
            Pointer<Response> response( new Response() );
            response->setCorrelationId( (*command)->getCommandId() );
            transport->getTransportListener()->onCommand( response );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreSessions( const Pointer<transport::Transport>& transport,
                                                const Pointer<ConnectionState>& connectionState ) {

    try{

        std::vector< Pointer<SessionState> > sessionStates = connectionState->getSessionStates();

        std::vector< Pointer<SessionState> >::const_iterator iter = sessionStates.begin();

        // Restore the Session State
        for( ; iter != sessionStates.end(); ++iter ) {
            Pointer<SessionState> state = *iter;
            transport->oneway( state->getInfo() );

            if( restoreProducers ) {
                doRestoreProducers( transport, state );
            }

            if( restoreConsumers ) {
                doRestoreConsumers( transport, state );
            }
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreConsumers( const Pointer<transport::Transport>& transport,
                                                 const Pointer<SessionState>& sessionState ) {

    try{

        // Restore the session's consumers but possibly in pull only (prefetch 0 state) till recovery complete
        Pointer<ConnectionState> connectionState =
            connectionStates.get( sessionState->getInfo()->getSessionId()->getParentId() );
        bool connectionInterruptionProcessingComplete =
            connectionState->isConnectionInterruptProcessingComplete();

        std::vector< Pointer<ConsumerState> > consumerStates = sessionState->getConsumerStates();
        std::vector< Pointer<ConsumerState> >::const_iterator state = consumerStates.begin();

        for( ; state != consumerStates.end(); ++state ) {

            Pointer<ConsumerInfo> infoToSend = (*state)->getInfo();
            Pointer<wireformat::WireFormat> wireFormat = transport->getWireFormat();

            if( !connectionInterruptionProcessingComplete && infoToSend->getPrefetchSize() > 0 && wireFormat->getVersion() > 5) {

                infoToSend.reset( (*state)->getInfo()->cloneDataStructure() );
                connectionState->getRecoveringPullConsumers().put( infoToSend->getConsumerId(), (*state)->getInfo() );
                infoToSend->setPrefetchSize(0);
            }

            transport->oneway( infoToSend );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreProducers( const Pointer<transport::Transport>& transport,
                                                 const Pointer<SessionState>& sessionState ) {

    try{

        // Restore the session's producers
        std::vector< Pointer<ProducerState> > producerStates = sessionState->getProducerStates();

        std::vector< Pointer<ProducerState> >::const_iterator iter = producerStates.begin();

        for( ; iter != producerStates.end(); ++iter ) {
            Pointer<ProducerState> state = *iter;
            transport->oneway( state->getInfo() );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreTempDestinations( const Pointer<transport::Transport>& transport,
                                                        const Pointer<ConnectionState>& connectionState ) {

    try{

        std::auto_ptr< Iterator< Pointer<DestinationInfo> > > iter(
            connectionState->getTempDesinations().iterator() );

        while( iter->hasNext() ) {
            transport->oneway( iter->next() );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processDestinationInfo( DestinationInfo* info ) {

    try{
        if( info != NULL ) {
            Pointer<ConnectionState> cs = connectionStates.get( info->getConnectionId() );
            if( cs != NULL && info->getDestination()->isTemporary() ) {
                cs->addTempDestination( Pointer<DestinationInfo>( info->cloneDataStructure() ) );
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveDestination( DestinationInfo* info ) {

    try{
        if( info != NULL ) {
            Pointer<ConnectionState> cs = connectionStates.get( info->getConnectionId() );
            if( cs != NULL && info->getDestination()->isTemporary() ) {
                cs->removeTempDestination( info->getDestination() );
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processProducerInfo( ProducerInfo* info ) {

    try{

        if( info != NULL && info->getProducerId() != NULL ) {
            Pointer<SessionId> sessionId = info->getProducerId()->getParentId();
            if( sessionId != NULL ) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<SessionState> ss = cs->getSessionState( sessionId );
                        if( ss != NULL ) {
                            ss->addProducer(
                                Pointer<ProducerInfo>( info->cloneDataStructure() ) );
                        }
                    }
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveProducer( ProducerId* id ) {

    try{

        if( id != NULL ) {
            Pointer<SessionId> sessionId = id->getParentId();
            if( sessionId != NULL ) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<SessionState> ss = cs->getSessionState( sessionId );
                        if( ss != NULL ) {
                            ss->removeProducer( Pointer<ProducerId>( id->cloneDataStructure() ) );
                        }
                    }
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processConsumerInfo( ConsumerInfo* info ) {

    try{

        if( info != NULL ) {
            Pointer<SessionId> sessionId = info->getConsumerId()->getParentId();
            if( sessionId != NULL ) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<SessionState> ss = cs->getSessionState( sessionId );
                        if( ss != NULL ) {
                            ss->addConsumer(
                                Pointer<ConsumerInfo>( info->cloneDataStructure() ) );
                        }
                    }
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveConsumer( ConsumerId* id ) {

    try{
        if( id != NULL ) {
            Pointer<SessionId> sessionId = id->getParentId();
            if( sessionId != NULL ) {
                Pointer<ConnectionId> connectionId = sessionId->getParentId();
                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<SessionState> ss = cs->getSessionState( sessionId );
                        if( ss != NULL ) {
                            ss->removeConsumer( Pointer<ConsumerId>( id->cloneDataStructure() ) );
                        }
                    }
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processSessionInfo( SessionInfo* info ) {

    try{

        if( info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getSessionId()->getParentId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    cs->addSession( Pointer<SessionInfo>( info->cloneDataStructure() ) );
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveSession( SessionId* id ) {

    try{

        if( id != NULL ) {
            Pointer<ConnectionId> connectionId = id->getParentId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    cs->removeSession( Pointer<SessionId>( id->cloneDataStructure() ) );
                }
            }
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processConnectionInfo( ConnectionInfo* info ) {

    try{

        if( info != NULL ) {
            Pointer<ConnectionInfo> infoCopy( info->cloneDataStructure() );
            connectionStates.put( info->getConnectionId(),
                                  Pointer<ConnectionState>( new ConnectionState( infoCopy ) ) );
        }
        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRemoveConnection( ConnectionId* id ) {

    try{

        if( id != NULL ) {
            connectionStates.remove( Pointer<ConnectionId>( id->cloneDataStructure() ) );
        }

        return TRACKED_RESPONSE_MARKER;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processMessage( Message* message ) {

    try{

        if( message != NULL ) {
            if( trackTransactions && message->getTransactionId() != NULL ) {
                Pointer<ProducerId> producerId = message->getProducerId();
                Pointer<ConnectionId> connectionId = producerId->getParentId()->getParentId();

                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<TransactionState> transactionState =
                            cs->getTransactionState( message->getTransactionId() );
                        if( transactionState != NULL ) {
                            transactionState->addCommand(
                                Pointer<Command>( message->cloneDataStructure() ) );

                            if( trackTransactionProducers ) {
                                // Track the producer in case it is closed before a commit
                                Pointer<SessionState> sessionState = cs->getSessionState( producerId->getParentId() );
                                Pointer<ProducerState> producerState = sessionState->getProducerState(producerId);
                                producerState->setTransactionState(transactionState);
                            }
                        }
                    }
                }
                return TRACKED_RESPONSE_MARKER;
            }else if( trackMessages ) {
                messageCache.put( message->getMessageId(),
                                  Pointer<Message>( message->cloneDataStructure() ) );
            }
        }
        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processMessageAck( MessageAck* ack ) {

    try{

        if( trackTransactions && ack != NULL && ack->getTransactionId() != NULL) {
            Pointer<ConnectionId> connectionId;// =
                ack->getConsumerId()->getParentId()->getParentId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( ack->getTransactionId() );
                    if( transactionState != NULL ) {
                        transactionState->addCommand(
                            Pointer<Command>( ack->cloneDataStructure() ) );
                    }
                }
            }
            return TRACKED_RESPONSE_MARKER;
        }
        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processBeginTransaction( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    cs->addTransactionState( info->getTransactionId() );
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    transactionState->addCommand(
                        Pointer<Command>( info->cloneDataStructure() ) );
                }
            }

            return TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processPrepareTransaction( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    if( transactionState != NULL ) {
                        transactionState->addCommand(
                            Pointer<Command>( info->cloneDataStructure() ) );
                    }
                }
            }

            return TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processCommitTransactionOnePhase( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    if( transactionState != NULL ) {
                        Pointer<TransactionInfo> infoCopy =
                            Pointer<TransactionInfo>( info->cloneDataStructure() );
                        transactionState->addCommand( infoCopy );
                        return Pointer<Tracked>( new Tracked(
                            Pointer<Runnable>( new RemoveTransactionAction( this, infoCopy ) ) ) );
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processCommitTransactionTwoPhase( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    if( transactionState != NULL ) {
                        Pointer<TransactionInfo> infoCopy =
                            Pointer<TransactionInfo>( info->cloneDataStructure() );
                        transactionState->addCommand( infoCopy );
                        return Pointer<Tracked>( new Tracked(
                            Pointer<Runnable>( new RemoveTransactionAction( this, infoCopy ) ) ) );
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processRollbackTransaction( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    if( transactionState != NULL ) {
                        Pointer<TransactionInfo> infoCopy =
                            Pointer<TransactionInfo>( info->cloneDataStructure() );
                        transactionState->addCommand( infoCopy );
                        return Pointer<Tracked>( new Tracked(
                            Pointer<Runnable>( new RemoveTransactionAction( this, infoCopy ) ) ) );
                    }
                }
            }
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> ConnectionStateTracker::processEndTransaction( TransactionInfo* info ) {

    try{

        if( trackTransactions && info != NULL ) {
            Pointer<ConnectionId> connectionId = info->getConnectionId();
            if( connectionId != NULL ) {
                Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                if( cs != NULL ) {
                    Pointer<TransactionState> transactionState =
                        cs->getTransactionState( info->getTransactionId() );
                    if( transactionState != NULL ) {
                        transactionState->addCommand(
                            Pointer<Command>( info->cloneDataStructure() ) );
                    }
                }
            }

            return TRACKED_RESPONSE_MARKER;
        }

        return Pointer<Response>();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::connectionInterruptProcessingComplete(
    transport::Transport* transport, const Pointer<ConnectionId>& connectionId ) {

    Pointer<ConnectionState> connectionState = connectionStates.get( connectionId );

    if( connectionState != NULL ) {

        connectionState->setConnectionInterruptProcessingComplete( true );

        StlMap< Pointer<ConsumerId>, Pointer<ConsumerInfo>, ConsumerId::COMPARATOR > stalledConsumers =
            connectionState->getRecoveringPullConsumers();

        std::vector< Pointer<ConsumerId> > keySet = stalledConsumers.keySet();
        std::vector< Pointer<ConsumerId> >::const_iterator key = keySet.begin();

        for( ; key != keySet.end(); ++key ) {
            Pointer<ConsumerControl> control( new ConsumerControl() );

            control->setConsumerId( *key );
            control->setPrefetch( stalledConsumers.get( *key )->getPrefetchSize() );
            control->setDestination( stalledConsumers.get( *key )->getDestination() );

            try {
                transport->oneway( control );
            } catch( Exception& ex ) {
            }
        }

        stalledConsumers.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::transportInterrupted() {

    std::vector< Pointer<ConnectionState> > connectionStatesVec = this->connectionStates.values();
    std::vector< Pointer<ConnectionState> >::const_iterator state = connectionStatesVec.begin();

    for( ; state != connectionStatesVec.end(); ++state ) {
        (*state)->setConnectionInterruptProcessingComplete( false );
    }
}
