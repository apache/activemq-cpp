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
#include "ActiveMQTransactionContext.h"

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/commands/TransactionInfo.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Iterator.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransactionContext::ActiveMQTransactionContext( ActiveMQSession* session,
                                                        const Properties& properties ) {
    try {

        if( session == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::ActiveMQTransactionContext - "
                "Initialized with a NULL session data");
        }

        // Store State Data
        this->session = session;
        this->connection = session->getConnection();

        // convert from property Strings to int.
        this->maxRedeliveries = Integer::parseInt(
            properties.getProperty( "transaction.maxRedeliveryCount", "5" ) );

        // Start a new Transaction
        this->startTransaction();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransactionContext::~ActiveMQTransactionContext() {

    try{

        // TODO
        // Inform the connector we are rolling back before we close so that
        // the provider knows we didn't complete this transaction
//        connection->getConnectionData()->getConnector()->
//            rollback( transactionInfo, session->getSessionInfo() );

    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::addSynchronization( Synchronization* sync ) {

    synchronized( &this->synchronizations ) {
        this->synchronizations.add( sync );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::removeSynchronization( Synchronization* sync ) {

    synchronized( &this->synchronizations ) {
        this->synchronizations.remove( sync );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::commit()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionInfo.get() == NULL ||
            this->transactionInfo->getTransactionId() == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::commit - "
                "Commit called before transaction was started.");
        }

        // Stop any deliveries
        this->session->stop();

        // Notify each registered Synchronization that we are committing this Transaction.
        synchronized( &this->synchronizations ) {

            std::auto_ptr< decaf::util::Iterator<Synchronization*> > iter(
                this->synchronizations.iterator() );

            while( iter->hasNext() ) {
                iter->next()->beforeCommit();
            }
        }

        // Commit the current Transaction
        this->transactionInfo->setType( ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE );
        this->connection->oneway( this->transactionInfo );

        // Notify each registered Synchronization that we have committed this Transaction.
        synchronized( &this->synchronizations ) {

            std::auto_ptr<decaf::util::Iterator<Synchronization*> > iter(
                this->synchronizations.iterator() );

            while( iter->hasNext() ) {
                iter->next()->afterCommit();
            }
        }

        // Clear all the Synchronizations.
        this->clearSynchronizations();

        // Start a new Transaction
        this->startTransaction();

        // Stop any deliveries
        this->session->start();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::rollback()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionInfo.get() == NULL ||
            this->transactionInfo->getTransactionId() == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::rollback - "
                "Rollback called before transaction was started.");
        }

        // Stop any Deliveries
        this->session->stop();

        // Rollback the Transaction
        this->transactionInfo->setType( ActiveMQConstants::TRANSACTION_STATE_ROLLBACK );
        this->connection->oneway( this->transactionInfo );

        // Notify each registered Synchronization that we are committing this Transaction.
        synchronized( &this->synchronizations ) {

            std::auto_ptr<decaf::util::Iterator<Synchronization*> > iter(
                this->synchronizations.iterator() );

            while( iter->hasNext() ) {
                iter->next()->beforeCommit();
            }
        }

        // Clear all the Synchronizations.
        this->clearSynchronizations();

        // Start a new Transaction
        this->startTransaction();

        // Start Deliveries
        this->session->start();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::startTransaction() throw( activemq::exceptions::ActiveMQException ) {

    try{

        this->transactionInfo.reset( new TransactionInfo() );

        // Create the Id
        Pointer<LocalTransactionId> id( new LocalTransactionId() );
        id->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        id->setValue( this->connection->getNextTransactionId() );

        // Populate the Info Command.
        this->transactionInfo->setConnectionId( id->getConnectionId() );
        this->transactionInfo->setTransactionId( id );
        this->transactionInfo->setType( ActiveMQConstants::TRANSACTION_STATE_BEGIN );

        this->connection->oneway( this->transactionInfo );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::clearSynchronizations()
    throw( activemq::exceptions::ActiveMQException ) {


    try{

        // delete each of the Synchronizations and then clear the Set.
        synchronized( &this->synchronizations ) {

            std::auto_ptr<decaf::util::Iterator<Synchronization*> > iter(
                this->synchronizations.iterator() );

            while( iter->hasNext() ) {
                delete iter->next();
            }

            this->synchronizations.clear();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
