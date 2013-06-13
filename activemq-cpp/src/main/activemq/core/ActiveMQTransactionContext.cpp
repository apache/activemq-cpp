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

#include <cms/Xid.h>
#include <cms/XAException.h>
#include <cms/TransactionInProgressException.h>
#include <cms/TransactionRolledBackException.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/IntegerResponse.h>
#include <activemq/commands/DataArrayResponse.h>
#include <activemq/commands/LocalTransactionId.h>
#include <activemq/commands/XATransactionId.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/concurrent/ConcurrentStlMap.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class TxContextData {
    private:

        TxContextData(const TxContextData&);
        TxContextData& operator=(const TxContextData&);

    public:

        // Tracks local transactions
        Pointer<commands::TransactionId> transactionId;

        // To track XA transactions.
        Pointer<Xid> associatedXid;
        int beforeEndIndex;

        TxContextData() : transactionId(), associatedXid(), beforeEndIndex() {
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Finally {
    private:

        Finally(const Finally&);
        Finally& operator=(const Finally&);

    private:

        decaf::util::StlSet< Pointer<Synchronization> >* syncs;

    public:

        Finally(decaf::util::StlSet<Pointer<Synchronization> >* syncs) : syncs(syncs) {
        }

        ~Finally() {
            if (this->syncs != NULL) {
                this->syncs->clear();
            }
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransactionContext::ActiveMQTransactionContext(ActiveMQSessionKernel* session, const Properties& properties AMQCPP_UNUSED) :
    context(new TxContextData()), session(session), connection(), synchronizations() {

    try {

        if (session == NULL) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "ActiveMQTransactionContext::ActiveMQTransactionContext - "
                "Initialized with a NULL session data");
        }

        this->connection = session->getConnection();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTransactionContext::~ActiveMQTransactionContext() {
    try {
        delete this->context;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::addSynchronization(const Pointer<Synchronization>& sync) {

    synchronized(&this->synchronizations) {
        this->synchronizations.add(sync);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::removeSynchronization( const Pointer<Synchronization>& sync ) {

    synchronized(&this->synchronizations) {
        this->synchronizations.remove(sync);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::begin() {

    try{

        if (isInXATransaction()) {
            throw cms::TransactionInProgressException(
                "Cannot start a local transaction while an XA Transaction is in progress.");
        }

        if (!isInTransaction()) {

            synchronized(&this->synchronizations) {
                this->synchronizations.clear();
            }

            Pointer<LocalTransactionId> id(new LocalTransactionId());
            id->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
            id->setValue(this->connection->getNextLocalTransactionId());

            Pointer<TransactionInfo> transactionInfo(new TransactionInfo());
            transactionInfo->setConnectionId(id->getConnectionId());
            transactionInfo->setTransactionId(id);
            transactionInfo->setType(ActiveMQConstants::TRANSACTION_STATE_BEGIN);

            this->connection->oneway(transactionInfo);
            this->context->transactionId = id.dynamicCast<TransactionId>();
        }
    }
    AMQ_CATCH_RETHROW(cms::CMSException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::commit() {

    try{

        if (isInXATransaction()) {
            throw cms::TransactionInProgressException("Cannot Commit a local transaction while an XA Transaction is in progress.");
        }

        try {
            this->beforeEnd();
        } catch (cms::CMSException& ex) {
            rollback();
            throw;
        }

        if (isInTransaction()) {
            Pointer<TransactionInfo> info(new TransactionInfo());
            info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
            info->setTransactionId(this->context->transactionId);
            info->setType(ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE);

            // Before we send the command NULL the id in case of an exception.
            this->context->transactionId.reset(NULL);

            try {
                this->connection->syncRequest(info);
                this->afterCommit();
            } catch(cms::CMSException& ex) {
                this->afterRollback();
                throw;
            }
        }
    }
    AMQ_CATCH_RETHROW(cms::CMSException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::rollback() {

    try{

        if (isInXATransaction()) {
            throw cms::TransactionInProgressException("Cannot Rollback a local transaction while an XA Transaction is in progress.");
        }

        try {
            this->beforeEnd();
        } catch (cms::TransactionRolledBackException& ex) {
            // Ignore, can occur on failover if the last command was commit.
        }

        if (isInTransaction()) {

            Pointer<TransactionInfo> info(new TransactionInfo());
            info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
            info->setTransactionId(this->context->transactionId);
            info->setType(ActiveMQConstants::TRANSACTION_STATE_ROLLBACK);

            // Before we send the command NULL the id in case of an exception.
            this->context->transactionId.reset(NULL);

            this->connection->syncRequest(info);
            this->afterRollback();
        }
    }
    AMQ_CATCH_RETHROW(cms::CMSException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::beforeEnd() {

    // Notify each registered Synchronization that we are ending this Transaction.
    synchronized(&this->synchronizations) {

        std::auto_ptr<decaf::util::Iterator< Pointer<Synchronization> > > iter(
            this->synchronizations.iterator());

        while (iter->hasNext()) {
            iter->next()->beforeEnd();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::afterCommit() {

    // Notify each registered Synchronization that we committed this Transaction.
    synchronized(&this->synchronizations) {

        Finally finalizer(&this->synchronizations);

        std::auto_ptr<decaf::util::Iterator<Pointer<Synchronization> > > iter(
            this->synchronizations.iterator());

        while (iter->hasNext()) {
            iter->next()->afterCommit();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::afterRollback() {

    // Notify each registered Synchronization that we rolled back this Transaction.
    synchronized(&this->synchronizations) {

        Finally finalizer( &this->synchronizations );

        std::auto_ptr<decaf::util::Iterator< Pointer<Synchronization> > > iter(
            this->synchronizations.iterator() );

        while( iter->hasNext() ) {
            iter->next()->afterRollback();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<TransactionId>& ActiveMQTransactionContext::getTransactionId() const {
    return this->context->transactionId;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::isInTransaction() const {
    return this->context->transactionId != NULL;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::isInLocalTransaction() const {
    return this->context->transactionId != NULL && this->context->transactionId->isLocalTransactionId();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::isInXATransaction() const {
    return this->context->transactionId != NULL && this->context->transactionId->isXATransactionId();
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQTransactionContext::getTransactionTimeout() const {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::setTransactionTimeout( int seconds AMQCPP_UNUSED ) {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQTransactionContext::recover(int flag AMQCPP_UNUSED, Xid** recovered) {

    Pointer<TransactionInfo> info(new TransactionInfo());
    info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
    info->setType(ActiveMQConstants::TRANSACTION_STATE_RECOVER);

    try {

        this->connection->checkClosedOrFailed();
        this->connection->ensureConnectionInfoSent();

        Pointer<Response> response = this->connection->syncRequest(info);
        Pointer<DataArrayResponse> arrayResponse = response.dynamicCast<DataArrayResponse>();

        std::vector<Pointer<DataStructure> > array = arrayResponse->getData();

        int size = (int) array.size();

        if (size > 0) {

            // Allocate space for all the recovered Xid's, if client passed us an existing
            // array then this would leak, but they were warned, so just go with it.
            recovered = new Xid*[array.size()];

            // We need to clone each Xid and then add it to the array, the client is now
            // responsible for freeing this memory.
            for (int i = 0; i < size; ++i) {
                Pointer<XATransactionId> xid = array[i].dynamicCast<XATransactionId>();
                recovered[i] = xid->clone();
            }
        }

        return size;
    } catch (Exception& e) {
        throw toXAException(e);
    } catch (CMSException& e) {
        throw toXAException(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::start(const Xid* xid, int flags) {

    if (this->isInLocalTransaction()) {
        throw XAException(XAException::XAER_PROTO);
    }

    // Are we already associated?
    if (this->context->associatedXid != NULL) {
        throw new XAException(XAException::XAER_PROTO);
    }

    const char* txSuspendResumeNotSupportMsg = "The suspend/resume of a transaction "
            "is not supported. Instead it is recommended "
            "that a new JMS session be created.";

    if ((flags & TMJOIN) == TMJOIN) {
        throw XAException(txSuspendResumeNotSupportMsg);
    }
    if ((flags & TMRESUME) == TMRESUME) {
        throw XAException(txSuspendResumeNotSupportMsg);
    }

    // prepare for a new association
    this->synchronizations.clear();
    this->context->beforeEndIndex = 0;

    this->setXid(xid);
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQTransactionContext::prepare(const Xid* xid) {

    // We allow interleaving multiple transactions, so we don't limit prepare to the associated xid.
    Pointer<XATransactionId> x;

    // THIS SHOULD NEVER HAPPEN because end(xid, TMSUCCESS) should have been called first
    if (xid == NULL || equals(this->context->associatedXid.get(), xid)) {
        throw XAException(XAException::XAER_PROTO);
    } else {
        x.reset(new XATransactionId(xid));
    }

    try {

        Pointer<TransactionInfo> info(new TransactionInfo());
        info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        info->setTransactionId(x);
        info->setType(ActiveMQConstants::TRANSACTION_STATE_PREPARE);

        // Find out if the server wants to commit or rollback.
        Pointer<Response> response = this->connection->syncRequest(info);

        Pointer<IntegerResponse> intResponse = response.dynamicCast<IntegerResponse>();

        if (XAResource::XA_RDONLY == intResponse->getResult()) {

            // transaction stops now, may be syncs that need a callback
            this->afterCommit();
        }

        return intResponse->getResult();

    } catch (Exception& e) {

        try {
            this->afterRollback();
        } catch (...) {
        }

        throw toXAException(e);

    } catch (CMSException& e) {

        try {
            this->afterRollback();
        } catch (...) {
        }

        throw toXAException(e);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::commit(const Xid* xid, bool onePhase) {

    // We allow interleaving multiple transactions, so we don't limit prepare to the associated xid.
    Pointer<XATransactionId> x;

    // THIS SHOULD NEVER HAPPEN because end(xid, TMSUCCESS) should have been called first
    if (xid == NULL || equals(this->context->associatedXid.get(), xid)) {
        throw XAException(XAException::XAER_PROTO);
    } else {
        x.reset(new XATransactionId(xid));
    }

    try {

        this->connection->checkClosedOrFailed();
        this->connection->ensureConnectionInfoSent();

        // Let the server know that the tx is rollback.
        Pointer<TransactionInfo> info(new TransactionInfo());
        info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        info->setTransactionId(x);
        info->setType(onePhase ? ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE : ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE);

        this->connection->syncRequest(info);

        this->afterCommit();

    } catch (Exception& ex) {

        try {
            this->afterRollback();
        } catch (...) {
        }

        throw toXAException(ex);

    } catch (CMSException& e) {

        try {
            this->afterRollback();
        } catch (...) {
        }

        throw toXAException(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::rollback(const Xid* xid) {

    // We allow interleaving multiple transactions, so we don't limit prepare to the associated xid.
    Pointer<XATransactionId> x;

    if (xid == NULL) {
        throw XAException(XAException::XAER_PROTO);
    }

    if (equals(this->context->associatedXid.get(), xid)) {
        x = this->context->transactionId.dynamicCast<XATransactionId>();
    } else {
        x.reset(new XATransactionId(xid));
    }

    try {

        this->connection->checkClosedOrFailed();
        this->connection->ensureConnectionInfoSent();

        // Let the server know that the tx is rollback.
        Pointer<TransactionInfo> info(new TransactionInfo());
        info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        info->setTransactionId(x);
        info->setType(ActiveMQConstants::TRANSACTION_STATE_ROLLBACK);

        this->connection->syncRequest(info);

        this->afterRollback();

    } catch (Exception& ex) {
        throw toXAException(ex);
    } catch (CMSException& e) {
        throw toXAException(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::end(const Xid* xid, int flags) {

    if (isInLocalTransaction()) {
        throw XAException(XAException::XAER_PROTO);
    }

    if ((flags & (TMSUSPEND | TMFAIL)) != 0) {

        // You can only suspend the associated xid.
        if (!equals(this->context->associatedXid.get(), xid)) {
            throw XAException(XAException::XAER_PROTO);
        }

        try {
            this->beforeEnd();
        } catch (Exception& e) {
            throw toXAException(e);
        } catch (CMSException& e) {
            throw toXAException(e);
        }

        setXid(NULL);

    } else if ((flags & TMSUCCESS) == TMSUCCESS) {

        // set to NULL if this is the current xid.
        // otherwise this could be an asynchronous success call
        if (equals(this->context->associatedXid.get(), xid)) {

            try {
                beforeEnd();
            } catch (Exception& ex) {
                throw toXAException(ex);
            } catch (CMSException& e) {
                throw toXAException(e);
            }

            setXid(NULL);
        }

    } else {
        throw XAException(XAException::XAER_INVAL);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::forget(const Xid* xid) {

    // We allow interleaving multiple transactions, so we don't limit prepare to the associated xid.
    Pointer<XATransactionId> x;

    if (xid == NULL) {
        throw XAException(XAException::XAER_PROTO);
    }

    if (equals(this->context->associatedXid.get(), xid)) {
        x = this->context->transactionId.dynamicCast<XATransactionId>();
    } else {
        x.reset(new XATransactionId(xid));
    }

    // Let the server know that the tx is rollback.
    Pointer<TransactionInfo> info(new TransactionInfo());
    info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
    info->setTransactionId(x);
    info->setType(ActiveMQConstants::TRANSACTION_STATE_FORGET);

    try {
        this->connection->syncRequest(info);
    } catch (Exception& ex) {
        throw toXAException(ex);
    } catch (CMSException& e) {
        throw toXAException(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::isSameRM(const XAResource* resource) {

    if (resource == NULL) {
        return false;
    }

    const ActiveMQTransactionContext* cntx = dynamic_cast<const ActiveMQTransactionContext*>(resource);

    if (cntx == NULL) {
        return false;
    }

    try {
        return getResourceManagerId() == cntx->getResourceManagerId();
    } catch (Exception& ex) {
        throw toXAException(ex);
    } catch (CMSException& ex) {
        throw XAException("Could not get the Resource Manager Id.", ex.clone());
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTransactionContext::setXid(const Xid* xid) {

    try {
        this->connection->checkClosedOrFailed();
        this->connection->ensureConnectionInfoSent();
    } catch (Exception& e) {
        throw toXAException(e);
    } catch (CMSException& e) {
        throw toXAException(e);
    }

    if (xid != NULL) {

        // Associate this new Xid with this Transaction as the root of the TX.
        this->context->associatedXid.reset(xid->clone());
        this->context->transactionId.reset(new XATransactionId(xid));

        Pointer<TransactionInfo> info(new TransactionInfo());
        info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        info->setTransactionId(this->context->transactionId);
        info->setType(ActiveMQConstants::TRANSACTION_STATE_BEGIN);

        try {
            this->connection->oneway(info);
        } catch (Exception& e) {
            throw toXAException(e);
        } catch (CMSException& e) {
            throw toXAException(e);
        }

    } else {

        if (this->context->transactionId != NULL) {

            Pointer<TransactionInfo> info(new TransactionInfo());
            info->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
            info->setTransactionId(this->context->transactionId);
            info->setType(ActiveMQConstants::TRANSACTION_STATE_END);

            try {
                this->connection->syncRequest(info);
            } catch (CMSException& e) {
                throw toXAException(e);
            }
        }

        // remove the association currently in place.
        this->context->associatedXid.reset(NULL);
        this->context->transactionId.reset(NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTransactionContext::equals(const cms::Xid* local, const cms::Xid* remote) {

    if (local == remote) {
        return true;
    }

    if ((local == NULL) ^ (remote == NULL)) {
        return false;
    }

    if (local->getFormatId() != remote->getFormatId()) {
        return false;
    } else {

        std::vector<unsigned char> localBQual(Xid::MAXBQUALSIZE);
        std::vector<unsigned char> remoteBQual(Xid::MAXBQUALSIZE);

        local->getBranchQualifier(&localBQual[0], Xid::MAXBQUALSIZE);
        remote->getBranchQualifier(&remoteBQual[0], Xid::MAXBQUALSIZE);

        if (localBQual != remoteBQual) {
            return false;
        }

        std::vector<unsigned char> localGTXID(Xid::MAXBQUALSIZE);
        std::vector<unsigned char> remoteGTXID(Xid::MAXBQUALSIZE);

        local->getGlobalTransactionId(&localGTXID[0], Xid::MAXGTRIDSIZE);
        remote->getGlobalTransactionId(&remoteGTXID[0], Xid::MAXGTRIDSIZE);

        if (localGTXID != remoteGTXID) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTransactionContext::getResourceManagerId() const {
    return this->connection->getResourceManagerId();
}

////////////////////////////////////////////////////////////////////////////////
XAException ActiveMQTransactionContext::toXAException(decaf::lang::Exception& ex) {
    CMSException cmsEx = CMSExceptionSupport::create(ex);
    XAException xae(ex.getMessage(), &cmsEx);
    xae.setErrorCode(XAException::XAER_RMFAIL);
    return xae;
}

////////////////////////////////////////////////////////////////////////////////
XAException ActiveMQTransactionContext::toXAException(cms::CMSException& ex) {
    XAException xae(ex.getMessage(), ex.clone());
    xae.setErrorCode(XAException::XAER_RMFAIL);
    return xae;
}
