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
#include <cms/XAResource.h>
#include <cms/CMSException.h>
#include <cms/XAException.h>

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

    class LocalTransactionEventListener;
    class ActiveMQSession;
    class ActiveMQConnection;
    class TxContextData;

    /**
     * Transaction Management class, hold messages that are to be redelivered
     * upon a request to roll-back.  The Transaction represents an always
     * running transaction, when it is committed or rolled back it silently
     * creates a new transaction for the next set of messages.  The only
     * way to permanently end this transaction is to delete it.
     *
     * @since 2.0
     */
    class AMQCPP_API ActiveMQTransactionContext : public cms::XAResource {
    private:

        // Internal structure to hold all class TX data.
        TxContextData* context;

        // Session this Transaction is associated with
        ActiveMQSession* session;

        // The Connection that is the parent of the Session.
        ActiveMQConnection* connection;

        // List of Registered Synchronizations
        decaf::util::StlSet< Pointer<Synchronization> > synchronizations;

    private:

        ActiveMQTransactionContext( const ActiveMQTransactionContext& );
        ActiveMQTransactionContext& operator= ( const ActiveMQTransactionContext& );

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
        virtual void begin();

        /**
         * Commit the current Transaction
         * @throw ActiveMQException
         */
        virtual void commit();

        /**
         * Rollback the current Transaction
         * @throw ActiveMQException
         */
        virtual void rollback();

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
         * Checks to see if there is currently an Local Transaction in progess, returns
         * false if not, true otherwise.
         *
         * @returns true if an Local Transaction is in progress.
         */
        virtual bool isInLocalTransaction() const;

        /**
         * Checks to see if there is currently an XA Transaction in progess, returns
         * false if not, true otherwise.
         *
         * @returns true if an XA Transaction is in progress.
         */
        virtual bool isInXATransaction() const;

    public:  // XAResource implementation.

        virtual void commit( const cms::Xid* xid, bool onePhase );

        virtual void end( const cms::Xid* xid, int flags );

        virtual void forget( const cms::Xid* xid );

        virtual int getTransactionTimeout() const;

        virtual bool isSameRM( const cms::XAResource* theXAResource );

        virtual int prepare( const cms::Xid* xid );

        virtual int recover(int flag, cms::Xid** recovered );

        virtual void rollback( const cms::Xid* xid );

        virtual bool setTransactionTimeout( int seconds );

        virtual void start( const cms::Xid* xid, int flags );

    private:

        std::string getResourceManagerId() const;
        void setXid( const cms::Xid* xid );
        bool equals( const cms::Xid* local, const cms::Xid* remote );
        cms::XAException toXAException( cms::CMSException& ex );
        cms::XAException toXAException( decaf::lang::Exception& ex );

        void beforeEnd();
        void afterCommit();
        void afterRollback();

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQTRANSACTIONCONTEXT_H_*/
