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
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const Pointer<Tracked> ConnectionStateTracker::TRACKED_RESPONSE_MARKER( new Tracked() );

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
ConnectionStateTracker::ConnectionStateTracker() {

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

