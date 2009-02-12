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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQTRANSACTIONCONTEXT_H_
#define _ACTIVEMQ_CORE_ACTIVEMQTRANSACTIONCONTEXT_H_

#include <memory>

#include <cms/Message.h>
#include <cms/CMSException.h>

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/Synchronization.h>

#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/StlSet.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq{
namespace core{

    class ActiveMQSession;
    class ActiveMQConnection;

    /**
     * Transaction Management class, hold messages that are to be redelivered
     * upon a request to roll-back.  The Transaction represents an always
     * running transaction, when it is committed or rolled back it silently
     * creates a new transaction for the next set of messages.  The only
     * way to permanently end this transaction is to delete it.
     *
     * Configuration options
     *
     * transaction.maxRedeliveryCount
     *   Max number of times a message can be re-delivered, if the session is
     *   rolled back more than this many time, the message is dropped.
     */
    class AMQCPP_API ActiveMQTransactionContext {
    private:

        // Session this Transaction is associated with
        ActiveMQSession* session;

        // The Connection that is the parent of the Session.
        ActiveMQConnection* connection;

        // Transaction Info for the current Transaction
        std::auto_ptr<commands::TransactionInfo> transactionInfo;

        // List of Registered Synchronizations
        decaf::util::StlSet<Synchronization*> synchronizations;

        // Lock object to protect the rollback Map
        decaf::util::concurrent::Mutex mutex;

        // Max number of redeliveries per message
        unsigned int maxRedeliveries;

    public:

        /**
         * Constructor
         * @param session - the session that contains this transaction
         * @param properties - configuration parameters for this object
         */
        ActiveMQTransactionContext( ActiveMQSession* session,
                                    const decaf::util::Properties& properties );

        virtual ~ActiveMQTransactionContext();

        /**
         * Adds a Synchronization to this Transaction.
         * @param sync - The Synchronization instance to add.
         */
        virtual void addSynchronization( Synchronization* sync );

        /**
         * Removes a Synchronization to this Transaction.
         * @param sync - The Synchronization instance to add.
         */
        virtual void removeSynchronization( Synchronization* sync );

        /**
         * Commit the current Transaction
         * @throw CMSException
         */
        virtual void commit() throw ( exceptions::ActiveMQException );

        /**
         * Rollback the current Transaction
         * @throw CMSException
         */
        virtual void rollback() throw ( exceptions::ActiveMQException );

        /**
         * Get the Transaction Information object for the current
         * Transaction, returns NULL if no transaction is running
         * @return TransactionInfo
         */
        virtual const commands::TransactionInfo* getTransactionInfo() const {
            return transactionInfo.get();
        }

        /**
         * Get the Transaction Id object for the current
         * Transaction, returns NULL if no transaction is running
         * @return TransactionInfo
         */
        virtual const decaf::lang::Pointer<commands::TransactionId>& getTransactionId() const {
            if( this->transactionInfo.get() == NULL ) {
                throw decaf::lang::exceptions::InvalidStateException(
                    __FILE__, __LINE__, "Transaction Not Started." );
            }

            return transactionInfo->getTransactionId();
        }

    private:

        // Remove and Delete all Synchronizations from the Set of registered
        // Synchronizations in this Transaction.
        void clearSynchronizations() throw( exceptions::ActiveMQException );

        // Starts a new Transaction.
        void startTransaction() throw( exceptions::ActiveMQException );

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQTRANSACTIONCONTEXT_H_*/
