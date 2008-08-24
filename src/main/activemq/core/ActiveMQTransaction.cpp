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
#include "ActiveMQTransaction.h"

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQMessage.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace decaf::lang::exceptions;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransaction::ActiveMQTransaction( ActiveMQConnection* connection,
                                          ActiveMQSession* session,
                                          const Properties& properties ) {
    try {

        if( connection == NULL || session == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ActiveMQTransaction::ActiveMQTransaction - "
                "Initialized with a NULL connection data");
        }

        // Store State Data
        this->connection = connection;
        this->session = session;

        // convert from property Strings to int.
        maxRedeliveries = Integer::parseInt(
            properties.getProperty( "transaction.maxRedeliveryCount", "5" ) );

        // Start a new Transaction
        transactionInfo = connection->getConnectionData()->
            getConnector()->startTransaction( session->getSessionInfo() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransaction::~ActiveMQTransaction() {

    try{

        // Inform the connector we are rolling back before we close so that
        // the provider knows we didn't complete this transaction
        connection->getConnectionData()->getConnector()->
            rollback( transactionInfo, session->getSessionInfo() );

        // Clean up
        clearTransaction();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::clearTransaction() {

    try{

        if( transactionInfo != NULL ) {
            // Dispose of the ProducerInfo
            transactionInfo->close();
            delete transactionInfo;
        }

        synchronized( &rollbackLock ) {

            // If there are any messages that are being transacted, then
            // they die once and for all here.
            RollbackMap::iterator itr = rollbackMap.begin();

            for( ; itr != rollbackMap.end(); ++itr ) {

                MessageList::iterator msgItr = itr->second.begin();

                for( ; msgItr != itr->second.end(); ++msgItr ) {
                   delete *msgItr;
                }
            }

            rollbackMap.clear();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::addToTransaction( ActiveMQMessage* message,
                                            ActiveMQConsumer* consumer ) {

    synchronized( &rollbackLock ) {
        // Store in the Multi Map
        rollbackMap[consumer].push_back( message );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::addToTransaction( cms::Message* message,
                                            ActiveMQConsumer* consumer ) {

    synchronized( &rollbackLock ) {

        ActiveMQMessage* coreMessage = dynamic_cast<ActiveMQMessage*>( message );

        if( coreMessage == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Message is not a core::ActiveMQMessage derivation" );
        }

        // Store in the Multi Map
        rollbackMap[consumer].push_back( coreMessage );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::removeFromTransaction( ActiveMQConsumer* consumer ) {

    try{

        // Delete all the messages, then remove the consumer's entry from
        // the Rollback Map.
        synchronized( &rollbackLock ) {
            RollbackMap::iterator rb_itr = rollbackMap.find( consumer );

            if( rb_itr == rollbackMap.end() ) {
                return;
            }

            MessageList::iterator itr = rb_itr->second.begin();

            for( ; itr != rollbackMap[consumer].end(); ++itr ) {
               delete *itr;
            }

            // Erase the entry from the map
            rollbackMap.erase( consumer );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::removeFromTransaction( long long consumerId ) {

    try {

        // Delete all the messages, then remove the consumer's entry from
        // the Rollback Map.
        synchronized( &rollbackLock ) {

            RollbackMap::iterator iter = rollbackMap.begin();

            for( ; iter != rollbackMap.end(); ++iter ) {

                long long id = iter->first->getConsumerInfo()->getConsumerId();

                if( id == consumerId ) {
                    removeFromTransaction( iter->first );
                    return;
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::commit()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionInfo == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransaction::begin - "
                "Commit called before transaction was started.");
        }

        // Stop any deliveries
        session->stop();

        // Now that the session is stopped, ack all messages we've
        // delivered to the clients and placed in the Rollback map.
        synchronized( &rollbackLock ) {

            RollbackMap::iterator itr = rollbackMap.begin();

            for(; itr != rollbackMap.end(); ++itr) {
                ackMessages( itr->first, itr->second );
            }
        }

        // Commit the current Transaction
        connection->getConnectionData()->getConnector()->
            commit( transactionInfo, session->getSessionInfo() );

        // Clean out the Transaction
        clearTransaction();

        // Start a new Transaction
        transactionInfo = connection->getConnectionData()->
            getConnector()->startTransaction( session->getSessionInfo() );

        // Stop any deliveries
        session->start();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::rollback()
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        if( this->transactionInfo == NULL ) {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQTransaction::rollback - "
                "Rollback called before transaction was started.");
        }

        // Stop any Deliveries
        session->stop();

        // Rollback the Transaction
        connection->getConnectionData()->getConnector()->
            rollback( transactionInfo, session->getSessionInfo() );

        // Dispose of the TransactionInfo
        transactionInfo->close();
        delete transactionInfo;

        // Start a new Transaction
        transactionInfo = connection->getConnectionData()->
            getConnector()->startTransaction( session->getSessionInfo() );

        // Start Deliveries
        session->start();

        // Roolback the messages to the Session, since we have the lock on the
        // rollbackLock, then no message will added to the transaction until we
        // are done processing all the messages that we to re-deliver and the map
        // is cleared.
        synchronized( &rollbackLock ) {

            RollbackMap::iterator itr = rollbackMap.begin();

            for(; itr != rollbackMap.end(); ++itr) {
                redeliverMessages( itr->first, itr->second );
            }

            // Clear the map.  Ownership of the messages is now handed off
            // to the rollback tasks.
            rollbackMap.clear();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::redeliverMessages( ActiveMQConsumer* consumer,
                                             MessageList& messages )
    throw ( activemq::exceptions::ActiveMQException ) {

    try {

        MessageList::iterator itr = messages.begin();

        for( ; itr != messages.end(); ++itr ) {

            ActiveMQMessage* message = *itr;
            message->setRedeliveryCount( message->getRedeliveryCount() + 1 );

            if( message->getRedeliveryCount() >= maxRedeliveries ) {

                // Poison Ack the Message, we give up processing this one
                connection->getConnectionData()->getConnector()->
                    acknowledge(
                        session->getSessionInfo(),
                        consumer->getConsumerInfo(),
                        dynamic_cast<Message*>( message ),
                        Connector::ACK_TYPE_POISON );

                // Won't be re-delivered so it must be destroyed here.
                delete message;

                continue;
            }

            DispatchData data( consumer->getConsumerInfo(), message );
            session->dispatch( data );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransaction::ackMessages( ActiveMQConsumer* consumer,
                                       MessageList& messages )
    throw ( activemq::exceptions::ActiveMQException ) {

    try {

        std::list<const cms::Message*> cmsMessages;

        MessageList::iterator iter = messages.begin();
        for( ; iter != messages.end(); ++iter ) {
            cmsMessages.insert( cmsMessages.end(),
                dynamic_cast<const cms::Message*>( *iter ) );
        }

        // Acknowledge the Messages let the connector do it in the most
        // efficient manner it can for large message block acks.
        connection->getConnectionData()->getConnector()->
            acknowledge(
                session->getSessionInfo(),
                consumer->getConsumerInfo(),
                cmsMessages,
                Connector::ACK_TYPE_CONSUMED );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
