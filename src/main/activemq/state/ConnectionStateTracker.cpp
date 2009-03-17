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
#include <decaf/lang/exceptions/NoSuchElementException.h>

using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::io;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace state {

    class RemoveTransactionAction : public Runnable {
    private:

        const Pointer<TransactionInfo> info;
        ConnectionStateTracker* stateTracker;

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
ConnectionStateTracker::ConnectionStateTracker() : TRACKED_RESPONSE_MARKER( new Tracked() ) {

    this->trackTransactions = false;
    this->restoreSessions = true;
    this->restoreConsumers = true;
    this->restoreProducers = true;
    this->restoreTransaction = true;
    this->trackMessages = true;
    this->maxCacheSize = 128 * 1024;
    this->currentCacheSize = 0;
}

////////////////////////////////////////////////////////////////////////////////
ConnectionStateTracker::~ConnectionStateTracker() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Tracked> ConnectionStateTracker::track( const Pointer<Command>& command )
    throw( decaf::io::IOException ) {

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
void ConnectionStateTracker::restore( const Pointer<transport::Transport>& transport )
    throw( decaf::io::IOException ) {

    try{

        std::vector< Pointer<ConnectionState> > connectionStates = this->connectionStates.values();
        std::vector< Pointer<ConnectionState> >::const_iterator iter = connectionStates.begin();

        for( ; iter != connectionStates.end(); ++iter ) {
            Pointer<ConnectionState> state = *iter;
            transport->oneway( state->getInfo() );
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
                                                    const Pointer<ConnectionState>& connectionState )
    throw( decaf::io::IOException ) {

    try{

        // Restore the session's transaction state
        std::vector< Pointer<TransactionState> > transactionStates =
            connectionState->getTransactionStates();

        std::vector< Pointer<TransactionState> >::const_iterator iter = transactionStates.begin();

        for( ; iter != transactionStates.end(); ++iter ) {
            Pointer<TransactionState> state = *iter;

            std::auto_ptr< Iterator< Pointer<Command> > > commands(
                state->getCommands().iterator() );

            while( commands->hasNext() ) {
                transport->oneway( commands->next() );
            }
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreSessions( const Pointer<transport::Transport>& transport,
                                                const Pointer<ConnectionState>& connectionState )
    throw( decaf::io::IOException ) {

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
                                                 const Pointer<SessionState>& sessionState )
    throw( decaf::io::IOException ) {

    try{

        // Restore the session's consumers
        std::vector< Pointer<ConsumerState> > consumerStates = sessionState->getConsumerStates();

        std::vector< Pointer<ConsumerState> >::const_iterator iter = consumerStates.begin();

        for( ; iter != consumerStates.end(); ++iter ) {
            Pointer<ConsumerState> state = *iter;
            transport->oneway( state->getInfo() );
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTracker::doRestoreProducers( const Pointer<transport::Transport>& transport,
                                                 const Pointer<SessionState>& sessionState )
    throw( decaf::io::IOException ) {

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
                                                        const Pointer<ConnectionState>& connectionState )
    throw( decaf::io::IOException ) {

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
Pointer<Command> ConnectionStateTracker::processDestinationInfo( DestinationInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRemoveDestination( DestinationInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processProducerInfo( ProducerInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRemoveProducer( ProducerId* id )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processConsumerInfo( ConsumerInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRemoveConsumer( ConsumerId* id )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processSessionInfo( SessionInfo* info)
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRemoveSession( SessionId* id)
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processConnectionInfo( ConnectionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRemoveConnection( ConnectionId* id )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processMessage( Message* message )
    throw ( exceptions::ActiveMQException ) {

    try{
        if( message != NULL ) {
            if( trackTransactions && message->getTransactionId() != NULL ) {
                Pointer<ConnectionId> connectionId =
                    message->getProducerId()->getParentId()->getParentId();
                if( connectionId != NULL ) {
                    Pointer<ConnectionState> cs = connectionStates.get( connectionId );
                    if( cs != NULL ) {
                        Pointer<TransactionState> transactionState =
                            cs->getTransactionState( message->getTransactionId() );
                        if( transactionState != NULL ) {
                            transactionState->addCommand(
                                Pointer<Command>( message->cloneDataStructure() ) );
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
Pointer<Command> ConnectionStateTracker::processMessageAck( MessageAck* ack )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processBeginTransaction( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processPrepareTransaction( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processCommitTransactionOnePhase( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processCommitTransactionTwoPhase( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processRollbackTransaction( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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
Pointer<Command> ConnectionStateTracker::processEndTransaction( TransactionInfo* info )
    throw ( exceptions::ActiveMQException ) {

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

