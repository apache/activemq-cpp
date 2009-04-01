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
#include <decaf/lang/Long.h>
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

        maximumRedeliveries = Integer::parseInt(
            properties.getProperty( "transaction.maxRedeliveryCount", "5" ) );
        redeliveryDelay = Long::parseLong(
            properties.getProperty( "transaction.redeliveryDelay", "0" ) );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransactionContext::~ActiveMQTransactionContext() {
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::addSynchronization( const Pointer<Synchronization>& sync ) {

    synchronized( &this->synchronizations ) {
        this->synchronizations.add( sync );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::removeSynchronization( const Pointer<Synchronization>& sync ) {

    synchronized( &this->synchronizations ) {
        this->synchronizations.remove( sync );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::begin()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{
        if( !isInTransaction() ) {

            this->synchronizations.clear();

            // Create the Id
            Pointer<LocalTransactionId> id( new LocalTransactionId() );
            id->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
            id->setValue( this->getNextTransactionId() );

            // Create and Populate the Info Command.
            Pointer<TransactionInfo> transactionInfo( new TransactionInfo() );
            transactionInfo->setConnectionId( id->getConnectionId() );
            transactionInfo->setTransactionId( id );
            transactionInfo->setType( ActiveMQConstants::TRANSACTION_STATE_BEGIN );

            this->connection->oneway( transactionInfo );

            this->transactionId = id.dynamicCast<TransactionId>();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::commit()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionId.get() == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::commit - "
                "Commit called before transaction was started.");
        }

        this->beforeEnd();

        // Create and Populate the Info Command.
        Pointer<TransactionInfo> info( new TransactionInfo() );
        info->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        info->setTransactionId( this->transactionId );
        info->setType( ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE );

        // Before we send the command null the id in case of an exception.
        this->transactionId.reset( NULL );

        // Commit the current Transaction
        this->connection->oneway( info );

        this->afterCommit();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::rollback()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionId == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::rollback - "
                "Rollback called before transaction was started.");
        }

        this->beforeEnd();

        // Create and Populate the Info Command.
        Pointer<TransactionInfo> info( new TransactionInfo() );
        info->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        info->setTransactionId( this->transactionId );
        info->setType( ActiveMQConstants::TRANSACTION_STATE_ROLLBACK );

        // Before we send the command null the id in case of an exception.
        this->transactionId.reset( NULL );

        // Roll back the current Transaction
        this->connection->oneway( info );

        this->afterRollback();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::beforeEnd() {

    // Notify each registered Synchronization that we are ending this Transaction.
    synchronized( &this->synchronizations ) {

        std::auto_ptr<decaf::util::Iterator< Pointer<Synchronization> > > iter(
            this->synchronizations.iterator() );

        while( iter->hasNext() ) {
            iter->next()->beforeEnd();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::afterCommit() {

    // Notify each registered Synchronization that we committed this Transaction.
    synchronized( &this->synchronizations ) {

        std::auto_ptr<decaf::util::Iterator< Pointer<Synchronization> > > iter(
            this->synchronizations.iterator() );

        while( iter->hasNext() ) {
            iter->next()->afterCommit();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::afterRollback() {

    // Notify each registered Synchronization that we rolled back this Transaction.
    synchronized( &this->synchronizations ) {

        std::auto_ptr<decaf::util::Iterator< Pointer<Synchronization> > > iter(
            this->synchronizations.iterator() );

        while( iter->hasNext() ) {
            iter->next()->afterRollback();
        }
    }
}
