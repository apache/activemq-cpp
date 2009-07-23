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

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/LocalTransactionId.h>
#include <activemq/core/Synchronization.h>
#include <activemq/util/LongSequenceGenerator.h>

#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/util/StlSet.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>

namespace activemq{
namespace core{

    using decaf::lang::Pointer;

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
     *
     * transaction.redeliveryDelay
     *   Time in Milliseconds between message redelivery for rolled back
     *   transactions.
     *
     * @since 2.0
     */
    class AMQCPP_API ActiveMQTransactionContext {
    private:

        // Session this Transaction is associated with
        ActiveMQSession* session;

        // The Connection that is the parent of the Session.
        ActiveMQConnection* connection;

        // Transaction Info for the current Transaction
        Pointer<commands::TransactionId> transactionId;

        // List of Registered Synchronizations
        decaf::util::StlSet< Pointer<Synchronization> > synchronizations;

        // Maximum number of time to redeliver a message when a Transaction is
        // rolled back.
        int maximumRedeliveries;

        // Time to wait before starting delivery again.
        long long redeliveryDelay;

    public:

        /**
         * Constructor
         *
         * @param session
         *      The session that contains this transaction
         * @param properties
         *      Configuration parameters for this object
         */
        ActiveMQTransactionContext( ActiveMQSession* session,
                                    const decaf::util::Properties& properties );

        virtual ~ActiveMQTransactionContext();

        /**
         * Adds a Synchronization to this Transaction.
         * @param sync - The Synchronization instance to add.
         */
        virtual void addSynchronization( const Pointer<Synchronization>& sync );

        /**
         * Removes a Synchronization to this Transaction.
         * @param sync - The Synchronization instance to add.
         */
        virtual void removeSynchronization( const Pointer<Synchronization>& sync );

        /**
         * Begins a new transaction if one is not currently in progress.
         * @throw ActiveMQException
         */
        virtual void begin() throw ( exceptions::ActiveMQException );

        /**
         * Commit the current Transaction
         * @throw ActiveMQException
         */
        virtual void commit() throw ( exceptions::ActiveMQException );

        /**
         * Rollback the current Transaction
         * @throw ActiveMQException
         */
        virtual void rollback() throw ( exceptions::ActiveMQException );

        /**
         * Get the Transaction Id object for the current
         * Transaction, returns NULL if no transaction is running
         * @return TransactionInfo
         * @throw InvalidStateException if a Transaction is not in progress.
         */
        virtual const decaf::lang::Pointer<commands::TransactionId>& getTransactionId() const;

        /**
         * Checks to see if there is currently a Transaction in progress returns
         * false if not, true otherwise.
         *
         * @return true if a transaction is in progress.
         */
        virtual bool isInTransaction() const;

        /**
         * @returns The Maximum number of time the client will attempt to redeliver a
         * message from a rolled back transaction before marking the message as not
         * consumed by this client.
         */
        virtual int getMaximumRedeliveries() const;

        /**
         * @returns The time in Milliseconds that this client is configured to wait in
         * between redelivery attempts for a Message in a rolled back transaction.
         */
        virtual long long getRedeliveryDelay() const;

    private:

        void beforeEnd();
        void afterCommit();
        void afterRollback();

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQTRANSACTIONCONTEXT_H_*/
