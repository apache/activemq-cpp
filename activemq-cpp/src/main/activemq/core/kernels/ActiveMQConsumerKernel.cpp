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

#include "ActiveMQConsumerKernel.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/Collections.h>
#include <decaf/util/concurrent/ExecutorService.h>
#include <decaf/util/concurrent/Executors.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <activemq/util/Config.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/util/ActiveMQMessageTransformation.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/core/FifoMessageDispatchChannel.h>
#include <activemq/core/SimplePriorityMessageDispatchChannel.h>
#include <activemq/core/RedeliveryPolicy.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <activemq/threads/Scheduler.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageTransformer.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::threads;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace core {
namespace kernels {

    class PreviouslyDeliveredMap : public HashMap<Pointer<MessageId>, bool> {
    public:

        Pointer<TransactionId> transactionId;

        PreviouslyDeliveredMap(Pointer<TransactionId> transactionId) :
            transactionId(transactionId) {
        }

        virtual ~PreviouslyDeliveredMap() {}
    };

    class ActiveMQConsumerKernelConfig {
    private:

        ActiveMQConsumerKernelConfig(const ActiveMQConsumerKernelConfig&);
        ActiveMQConsumerKernelConfig& operator=(const ActiveMQConsumerKernelConfig&);

    public:

        cms::MessageListener* listener;
        cms::MessageAvailableListener* messageAvailableListener;
        cms::MessageTransformer* transformer;
        decaf::util::concurrent::Mutex listenerMutex;
        AtomicBoolean deliveringAcks;
        AtomicBoolean started;
        AtomicBoolean closeSyncRegistered;
        Pointer<MessageDispatchChannel> unconsumedMessages;
        decaf::util::LinkedList< decaf::lang::Pointer<commands::MessageDispatch> > deliveredMessages;
        long long lastDeliveredSequenceId;
        Pointer<commands::MessageAck> pendingAck;
        int deliveredCounter;
        int additionalWindowSize;
        volatile bool synchronizationRegistered;
        volatile bool isClearDeliveredList;
        AtomicInteger inProgressClearRequiredFlag;
        long long redeliveryDelay;
        Pointer<RedeliveryPolicy> redeliveryPolicy;
        Pointer<Exception> failureError;
        Pointer<Scheduler> scheduler;
        int hashCode;
        Pointer<PreviouslyDeliveredMap> previouslyDeliveredMessages;
        long long failoverRedeliveryWaitPeriod;
        bool transactedIndividualAck;
        bool nonBlockingRedelivery;
        bool consumerExpiryCheckEnabled;
        bool optimizeAcknowledge;
        long long optimizeAckTimestamp;
        long long optimizeAcknowledgeTimeOut;
        long long optimizedAckScheduledAckInterval;
        Runnable* optimizedAckTask;
        int ackCounter;
        int dispatchedCount;
        Pointer<ExecutorService> executor;
        ActiveMQSessionKernel* session;
        ActiveMQConsumerKernel* parent;
        Pointer<ConsumerInfo> info;

        ActiveMQConsumerKernelConfig() : listener(NULL),
                                         messageAvailableListener(NULL),
                                         transformer(NULL),
                                         listenerMutex(),
                                         deliveringAcks(),
                                         started(),
                                         closeSyncRegistered(),
                                         unconsumedMessages(),
                                         deliveredMessages(),
                                         lastDeliveredSequenceId(-1),
                                         pendingAck(),
                                         deliveredCounter(0),
                                         additionalWindowSize(0),
                                         synchronizationRegistered(false),
                                         isClearDeliveredList(false),
                                         inProgressClearRequiredFlag(0),
                                         redeliveryDelay(0),
                                         redeliveryPolicy(),
                                         failureError(),
                                         scheduler(),
                                         hashCode(),
                                         previouslyDeliveredMessages(),
                                         failoverRedeliveryWaitPeriod(0),
                                         transactedIndividualAck(false),
                                         nonBlockingRedelivery(false),
                                         consumerExpiryCheckEnabled(true),
                                         optimizeAcknowledge(false),
                                         optimizeAckTimestamp(System::currentTimeMillis()),
                                         optimizeAcknowledgeTimeOut(),
                                         optimizedAckScheduledAckInterval(),
                                         optimizedAckTask(),
                                         ackCounter(),
                                         dispatchedCount(),
                                         executor(),
                                         session(),
                                         parent(),
                                         info() {
        }

        bool isTimeForOptimizedAck(int prefetchSize) const {
            if (ackCounter + deliveredCounter >= (prefetchSize * 0.65)) {
                return true;
            }

            long long nextAckTime = optimizeAckTimestamp + optimizeAcknowledgeTimeOut;

            if (optimizeAcknowledgeTimeOut > 0 && System::currentTimeMillis() >= nextAckTime) {
                return true;
            }

            return false;
        }

        void clearDeliveredList() {
            if (isClearDeliveredList) {
                synchronized (&this->deliveredMessages) {
                    if (isClearDeliveredList) {
                        if (!deliveredMessages.isEmpty()) {
                            if (session->isTransacted()) {
                                if (previouslyDeliveredMessages == NULL) {
                                    previouslyDeliveredMessages.reset(new PreviouslyDeliveredMap(
                                        session->getTransactionContext()->getTransactionId()));
                                }

                                Pointer<Iterator<Pointer<MessageDispatch> > > iter(deliveredMessages.iterator());

                                while (iter->hasNext()) {
                                    Pointer<MessageDispatch> dispatch = iter->next();
                                    previouslyDeliveredMessages->put(dispatch->getMessage()->getMessageId(), false);
                                }
                            } else {
                                if (session->isClientAcknowledge() || session->isIndividualAcknowledge()) {
                                    if (!info->isBrowser()) {
                                        Pointer<Iterator<Pointer<MessageDispatch> > > iter(deliveredMessages.iterator());

                                        // allow redelivery
                                        while (iter->hasNext()) {
                                            Pointer<MessageDispatch> dispatch = iter->next();
                                            session->getConnection()->rollbackDuplicate(parent, dispatch->getMessage());
                                        }
                                    }
                                }
                                deliveredMessages.clear();
                                pendingAck.reset(NULL);
                            }
                        }
                        isClearDeliveredList = false;
                    }
                }
            }
        }

        void clearPreviouslyDelivered() {
            if (previouslyDeliveredMessages != NULL) {
                previouslyDeliveredMessages->clear();
                previouslyDeliveredMessages.reset(NULL);
            }
        }

        // called with deliveredMessages locked
        void removeFromDeliveredMessages(Pointer<MessageId> key) {
            Pointer< Iterator< Pointer<MessageDispatch> > > iter(this->deliveredMessages.iterator());
            while (iter->hasNext()) {
                Pointer<MessageDispatch> candidate = iter->next();
                if (key->equals(candidate->getMessage()->getMessageId().get())) {
                    session->getConnection()->rollbackDuplicate(this->parent, candidate->getMessage());
                    iter->remove();
                    break;
                }
            }
        }

        // called with unconsumedMessages && deliveredMessages locked remove any message
        // not re-delivered as they can't be replayed to this consumer on rollback
        void rollbackPreviouslyDeliveredAndNotRedelivered() {
            if (previouslyDeliveredMessages != NULL) {
                Set<MapEntry<Pointer<MessageId>, bool> >& entries = previouslyDeliveredMessages->entrySet();
                Pointer<Iterator<MapEntry<Pointer<MessageId>, bool> > > iter(entries.iterator());
                while (iter->hasNext()) {
                    MapEntry<Pointer<MessageId>, bool> entry = iter->next();
                    if (!entry.getValue()) {
                        removeFromDeliveredMessages(entry.getKey());
                    }
                }

                clearPreviouslyDelivered();
            }
        }

        void rollbackOnFailedRecoveryRedelivery() {
            if (previouslyDeliveredMessages != NULL) {
                // if any previously delivered messages was not re-delivered, transaction is invalid
                // and must roll back as messages have been dispatched elsewhere.
                int numberNotReplayed = 0;
                Set<MapEntry<Pointer<MessageId>, bool> >& entries = previouslyDeliveredMessages->entrySet();
                Pointer<Iterator<MapEntry<Pointer<MessageId>, bool> > > iter(entries.iterator());
                while (iter->hasNext()) {
                    MapEntry<Pointer<MessageId>, bool> entry = iter->next();
                    if (!entry.getValue()) {
                        numberNotReplayed++;
                    }
                }
                if (numberNotReplayed > 0) {
                    std::string txId = previouslyDeliveredMessages->transactionId != NULL ?
                        previouslyDeliveredMessages->transactionId->toString() : "<None>";
                    std::string message = std::string("rolling back transaction (") + txId +
                        ") post failover recovery. " + Integer::toString(numberNotReplayed) +
                        " previously delivered message(s) not replayed to consumer: " +
                        info->getConsumerId()->toString();
                    throw cms::TransactionRolledBackException(message);
                }
            }
        }

        void waitForRedeliveries() {
            if (failoverRedeliveryWaitPeriod > 0 && previouslyDeliveredMessages != NULL) {
                long long expiry = System::currentTimeMillis() + failoverRedeliveryWaitPeriod;
                int numberNotReplayed;
                do {
                    numberNotReplayed = 0;
                    synchronized (&this->deliveredMessages) {
                        if (previouslyDeliveredMessages != NULL) {
                            Set<MapEntry<Pointer<MessageId>, bool> >& entries = previouslyDeliveredMessages->entrySet();
                            Pointer<Iterator<MapEntry<Pointer<MessageId>, bool> > > iter(entries.iterator());
                            while (iter->hasNext()) {
                                MapEntry<Pointer<MessageId>, bool> entry = iter->next();
                                if (!entry.getValue()) {
                                    numberNotReplayed++;
                                }
                            }
                        }
                    }
                    if (numberNotReplayed > 0) {
                        try {
                            Thread::sleep(Math::max(500LL, failoverRedeliveryWaitPeriod/4));
                        } catch (InterruptedException& ex) {
                            break;
                        }
                    }
                } while (numberNotReplayed > 0 && expiry < System::currentTimeMillis());
            }
        }

        bool redeliveryExpectedInCurrentTransaction(Pointer<MessageDispatch> dispatch, bool markReceipt) {
            if (session->isTransacted()) {
                synchronized (&this->deliveredMessages) {
                    if (previouslyDeliveredMessages != NULL) {
                        if (previouslyDeliveredMessages->containsKey(dispatch->getMessage()->getMessageId())) {
                            if (markReceipt) {
                                previouslyDeliveredMessages->put(dispatch->getMessage()->getMessageId(), true);
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool redeliveryPendingInCompetingTransaction(Pointer<MessageDispatch> dispatch) {
            ArrayList< Pointer<ActiveMQSessionKernel> > sessions = session->getConnection()->getSessions();

            Pointer<Iterator<Pointer<ActiveMQSessionKernel> > > sessionIter(sessions.iterator());
            while (sessionIter->hasNext()) {
                Pointer<ActiveMQSessionKernel> session = sessionIter->next();
                ArrayList< Pointer<ActiveMQConsumerKernel> > consumers = session->getConsumers();
                Pointer<Iterator<Pointer<ActiveMQConsumerKernel> > > consumersIter(consumers.iterator());

                while (consumersIter->hasNext()) {
                    Pointer<ActiveMQConsumerKernel> consumer = consumersIter->next();
                    return consumer->isRedeliveryExpectedInCurrentTransaction(dispatch);
                }
            }

            return false;
        }

        bool consumeExpiredMessage(const Pointer<MessageDispatch> dispatch) {
            if (dispatch->getMessage()->isExpired()) {
                return !info->isBrowser() && consumerExpiryCheckEnabled;
            }

            return false;
        }

        bool redeliveryExceeded(Pointer<MessageDispatch> dispatch) {
            try {
                return session->isTransacted() && redeliveryPolicy != NULL &&
                       redeliveryPolicy->getMaximumRedeliveries() != RedeliveryPolicy::NO_MAXIMUM_REDELIVERIES &&
                       dispatch->getRedeliveryCounter() > redeliveryPolicy->getMaximumRedeliveries() &&
                        // redeliveryCounter > x expected after resend via brokerRedeliveryPlugin
                       !dispatch->getMessage()->getMessageProperties().containsKey("redeliveryDelay");
            } catch (Exception& ignored) {
                return false;
            }
        }

        void posionAck(Pointer<MessageDispatch> dispatch, const std::string& cause) {
            Pointer<MessageAck> poisonAck(new MessageAck(dispatch, ActiveMQConstants::ACK_TYPE_POISON, 1));
            poisonAck->setFirstMessageId(dispatch->getMessage()->getMessageId());
            poisonAck->setPoisonCause(createBrokerError(cause));
            session->sendAck(poisonAck);
        }

        Pointer<BrokerError> createBrokerError(const std::string& message) {
            Pointer<BrokerError> cause(new BrokerError());
            cause->setExceptionClass("javax.jms.JMSException");
            cause->setMessage(message);
            return cause;
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
namespace {

    /**
     * Class used to deal with consumers in an active transaction.  This
     * class calls back into the consumer when the transaction is Committed or
     * Rolled Back to process that event.
     */
    class TransactionSynhcronization : public Synchronization {
    private:

        Pointer<ActiveMQConsumerKernel> consumer;
        ActiveMQConsumerKernelConfig* impl;

    private:

        TransactionSynhcronization(const TransactionSynhcronization&);
        TransactionSynhcronization& operator=(const TransactionSynhcronization&);

    public:

        TransactionSynhcronization(Pointer<ActiveMQConsumerKernel> consumer, ActiveMQConsumerKernelConfig* impl) :
            Synchronization(), consumer(consumer), impl(impl) {

            if (consumer == NULL) {
                throw NullPointerException(__FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~TransactionSynhcronization() {}

        virtual void beforeEnd() {
            if (impl->transactedIndividualAck) {
                impl->clearDeliveredList();
                impl->waitForRedeliveries();
                synchronized(&impl->deliveredMessages) {
                    impl->rollbackOnFailedRecoveryRedelivery();
                }
            } else {
                consumer->acknowledge();
            }
            consumer->setSynchronizationRegistered(false);
        }

        virtual void afterCommit() {
            consumer->commit();
            consumer->setSynchronizationRegistered(false);
            consumer.reset(NULL);
        }

        virtual void afterRollback() {
            consumer->rollback();
            consumer->setSynchronizationRegistered(false);
            consumer.reset(NULL);
        }
    };

    /**
     * Class used to Hook a consumer that has been closed into the Transaction
     * it is currently a part of.  Once the Transaction has been Committed or
     * Rolled back this Synchronization can finish the Close of the consumer.
     */
    class CloseSynhcronization : public Synchronization {
    private:

        Pointer<ActiveMQConsumerKernel> consumer;

    private:

        CloseSynhcronization(const CloseSynhcronization&);
        CloseSynhcronization& operator=(const CloseSynhcronization&);

    public:

        CloseSynhcronization(Pointer<ActiveMQConsumerKernel> consumer) : consumer(consumer) {
            if (consumer == NULL) {
                throw NullPointerException(__FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~CloseSynhcronization() {}

        virtual void beforeEnd() {
        }

        virtual void afterCommit() {
            consumer->doClose();
            consumer.reset(NULL);
        }

        virtual void afterRollback() {
            consumer->doClose();
            consumer.reset(NULL);
        }
    };

    /**
     * ActiveMQAckHandler used to support Managed Acknowledge modes.
     */
    class NoOpAckHandler : public ActiveMQAckHandler {
    private:

        NoOpAckHandler(const NoOpAckHandler&);
        NoOpAckHandler& operator=(const NoOpAckHandler&);

    public:

        NoOpAckHandler() {
        }

        void acknowledgeMessage(const commands::Message* message AMQCPP_UNUSED) {
        }
    };

    /**
     * ActiveMQAckHandler used to support Client Acknowledge mode.
     */
    class ClientAckHandler : public ActiveMQAckHandler {
    private:

        ActiveMQSessionKernel* session;

    private:

        ClientAckHandler(const ClientAckHandler&);
        ClientAckHandler& operator=(const ClientAckHandler&);

    public:

        ClientAckHandler(ActiveMQSessionKernel* session) : session(session) {
            if (session == NULL) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Ack Handler Created with NULL Session.");
            }
        }

        void acknowledgeMessage(const commands::Message* message AMQCPP_UNUSED) {
            try {
                this->session->acknowledge();
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }
    };

    /**
     * ActiveMQAckHandler used to enable the Individual Acknowledge mode.
     */
    class IndividualAckHandler : public ActiveMQAckHandler {
    private:

        ActiveMQConsumerKernel* consumer;
        Pointer<commands::MessageDispatch> dispatch;

    private:

        IndividualAckHandler(const IndividualAckHandler&);
        IndividualAckHandler& operator=(const IndividualAckHandler&);

    public:

        IndividualAckHandler(ActiveMQConsumerKernel* consumer, const Pointer<MessageDispatch>& dispatch) :
            consumer(consumer), dispatch(dispatch) {

            if (consumer == NULL) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Ack Handler Created with NULL consumer.");
            }
        }

        void acknowledgeMessage(const commands::Message* message AMQCPP_UNUSED) {
            try {
                if (this->dispatch != NULL) {
                    this->consumer->acknowledge(this->dispatch);
                    this->dispatch.reset(NULL);
                }
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }
    };

    /**
     * Class used to Start a Consumer's dispatch queue asynchronously from the
     * configured Scheduler.
     */
    class StartConsumerTask : public Runnable {
    private:

        Pointer<ActiveMQConsumerKernel> consumer;
        ActiveMQSessionKernel* session;

    private:

        StartConsumerTask(const StartConsumerTask&);
        StartConsumerTask& operator=(const StartConsumerTask&);

    public:

        StartConsumerTask(Pointer<ActiveMQConsumerKernel> consumer, ActiveMQSessionKernel* session) :
            Runnable(), consumer(consumer), session(session) {

            if (consumer == NULL) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~StartConsumerTask() {}

        virtual void run() {
            try {
                if (!this->consumer->isClosed()) {
                    this->consumer->start();
                }
            } catch(cms::CMSException& ex) {
                Exception wrapper(ex.clone());
                this->session->getConnection()->onAsyncException(wrapper);
            }

            this->consumer.reset(NULL);
        }
    };

    class AsyncMessageAckTask : public Runnable {
    private:

        Pointer<MessageAck> ack;
        ActiveMQSessionKernel* session;
        ActiveMQConsumerKernelConfig* impl;

    private:

        AsyncMessageAckTask(const AsyncMessageAckTask&);
        AsyncMessageAckTask& operator=(const AsyncMessageAckTask&);

    public:

        AsyncMessageAckTask(Pointer<MessageAck> ack, ActiveMQSessionKernel* session, ActiveMQConsumerKernelConfig* impl) :
            Runnable(), ack(ack), session(session), impl(impl) {}
        virtual ~AsyncMessageAckTask() {}

        virtual void run() {
            try {
                this->session->sendAck(ack, true);
                this->impl->deliveringAcks.set(false);
            } catch(Exception& ex) {
                this->impl->deliveringAcks.set(false);
            } catch(cms::CMSException& ex) {
                this->impl->deliveringAcks.set(false);
            }
        }
    };

    class OptimizedAckTask : public Runnable {
    private:

        Pointer<ActiveMQConsumerKernel> consumer;
        ActiveMQConsumerKernelConfig* impl;

    private:

        OptimizedAckTask(const OptimizedAckTask&);
        OptimizedAckTask& operator=(const OptimizedAckTask&);

    public:

        OptimizedAckTask(Pointer<ActiveMQConsumerKernel> consumer, ActiveMQConsumerKernelConfig* impl) :
            Runnable(), consumer(consumer), impl(impl) {}
        virtual ~OptimizedAckTask() {}

        virtual void run() {
            try {
                if (impl->optimizeAcknowledge && !impl->unconsumedMessages->isClosed()) {
                    this->consumer->deliverAcks();
                }

            } catch(Exception& ex) {
                impl->session->getConnection()->onAsyncException(ex);
            }
            this->consumer.reset(NULL);
        }
    };

    class NonBlockingRedeliveryTask : public Runnable {
    private:

        ActiveMQSessionKernel* session;
        Pointer<ActiveMQConsumerKernel> consumer;
        ActiveMQConsumerKernelConfig* impl;
        ArrayList<Pointer<MessageDispatch> > redeliveries;

    private:

        NonBlockingRedeliveryTask(const NonBlockingRedeliveryTask&);
        NonBlockingRedeliveryTask& operator=(const NonBlockingRedeliveryTask&);

    public:

        NonBlockingRedeliveryTask(ActiveMQSessionKernel* session, Pointer<ActiveMQConsumerKernel> consumer, ActiveMQConsumerKernelConfig* impl) :
            Runnable(), session(session), consumer(consumer), impl(impl), redeliveries() {

            this->redeliveries.copy(impl->deliveredMessages);
            Collections::reverse(this->redeliveries);
        }
        virtual ~NonBlockingRedeliveryTask() {}

        virtual void run() {
            try {
                if (!impl->unconsumedMessages->isClosed()) {
                    Pointer<Iterator<Pointer<MessageDispatch> > > iter(redeliveries.iterator());
                    while (iter->hasNext() && !impl->unconsumedMessages->isClosed()) {
                        Pointer<MessageDispatch> dispatch = iter->next();
                        session->dispatch(dispatch);
                    }
                }
            } catch (Exception& e) {
                session->getConnection()->onAsyncException(e);
            }

            this->consumer.reset(NULL);
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumerKernel::ActiveMQConsumerKernel(ActiveMQSessionKernel* session,
                                               const Pointer<ConsumerId>& id,
                                               const Pointer<ActiveMQDestination>& destination,
                                               const std::string& name,
                                               const std::string& selector,
                                               int prefetch,
                                               int maxPendingMessageCount,
                                               bool noLocal,
                                               bool browser,
                                               bool dispatchAsync,
                                               cms::MessageListener* listener) : internal(NULL),
                                                                                 session(NULL),
                                                                                 consumerInfo() {

    if (session == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Consumer created with NULL Session");
    }

    if (destination == NULL) {
        throw cms::InvalidDestinationException("Consumer created with NULL Destination");
    } else if (destination->getPhysicalName() == "") {
        throw cms::InvalidDestinationException("Destination given has no Physical Name.");
    } else if (destination->isTemporary()) {

        std::string physicalName = destination->getPhysicalName();
        std::string connectionId = session->getConnection()->getConnectionInfo().getConnectionId()->getValue();

        if (physicalName.find(connectionId) == std::string::npos) {
            throw cms::InvalidDestinationException(
                "Cannot use a Temporary destination from another Connection");
        }

        Pointer<ActiveMQTempDestination> tempDest = destination.dynamicCast<ActiveMQTempDestination>();

        if (session->getConnection()->isDeleted(tempDest)) {
            throw cms::InvalidDestinationException(
                "Cannot use a Temporary destination that has been deleted");
        }
    }

    if (prefetch < 0) {
        throw cms::CMSException("Cannot have a prefetch size less than zero");
    }

    this->internal = new ActiveMQConsumerKernelConfig();

    Pointer<ConsumerInfo> consumerInfo(new ConsumerInfo());

    consumerInfo->setConsumerId(id);
    consumerInfo->setClientId(session->getConnection()->getClientID());
    consumerInfo->setDestination(destination);
    consumerInfo->setSubscriptionName(name);
    consumerInfo->setSelector(selector);
    consumerInfo->setPrefetchSize(prefetch);
    consumerInfo->setCurrentPrefetchSize(prefetch);
    consumerInfo->setMaximumPendingMessageLimit(maxPendingMessageCount);
    consumerInfo->setBrowser(browser);
    consumerInfo->setDispatchAsync(dispatchAsync);
    consumerInfo->setNoLocal(noLocal);
    consumerInfo->setExclusive(session->getConnection()->isExclusiveConsumer());
    consumerInfo->setRetroactive(session->getConnection()->isUseRetroactiveConsumer());

    // Initialize Consumer Data
    this->session = session;
    this->consumerInfo = consumerInfo;
    this->internal->session = session;
    this->internal->parent = this;
    this->internal->info = consumerInfo;
    this->internal->redeliveryPolicy.reset(this->session->getConnection()->getRedeliveryPolicy()->clone());
    this->internal->scheduler = this->session->getScheduler();

    if (this->session->getConnection()->isMessagePrioritySupported()) {
        this->internal->unconsumedMessages.reset(new SimplePriorityMessageDispatchChannel());
    } else {
        this->internal->unconsumedMessages.reset(new FifoMessageDispatchChannel());
    }

    if (listener != NULL) {
        this->setMessageListener(listener);
    }

    applyDestinationOptions(this->consumerInfo);

    if (session->getConnection()->isOptimizeAcknowledge() && session->isAutoAcknowledge() && !consumerInfo->isBrowser()) {
        this->internal->optimizeAcknowledge = true;
    }

    if (this->internal->optimizeAcknowledge) {
        this->internal->optimizeAcknowledgeTimeOut = session->getConnection()->getOptimizeAcknowledgeTimeOut();
        this->setOptimizedAckScheduledAckInterval(
            session->getConnection()->getOptimizedAckScheduledAckInterval());
    }

    consumerInfo->setOptimizedAcknowledge(this->internal->optimizeAcknowledge);
    this->internal->failoverRedeliveryWaitPeriod =
        session->getConnection()->getConsumerFailoverRedeliveryWaitPeriod();
    this->internal->nonBlockingRedelivery = session->getConnection()->isNonBlockingRedelivery();
    this->internal->transactedIndividualAck =
        session->getConnection()->isTransactedIndividualAck() ||
        this->internal->nonBlockingRedelivery ||
        this->session->getConnection()->isMessagePrioritySupported();
    this->internal->consumerExpiryCheckEnabled =
        this->session->getConnection()->isConsumerExpiryCheckEnabled();

    if (this->consumerInfo->getPrefetchSize() < 0) {
        delete this->internal;
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Cannot create a consumer with a negative prefetch");
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumerKernel::~ActiveMQConsumerKernel() {
    try {
        this->close();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->internal;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::start() {

    if (this->internal->unconsumedMessages->isClosed()) {
        return;
    }

    this->internal->started.set(true);
    this->internal->unconsumedMessages->start();
    this->session->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::stop() {
    this->internal->started.set(false);
    this->internal->unconsumedMessages->stop();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isClosed() const {
    return this->internal->unconsumedMessages->isClosed();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::close() {

    try {
        if (!this->isClosed()) {

            if (!this->internal->deliveredMessages.isEmpty() &&
                this->session->getTransactionContext() != NULL &&
                this->session->getTransactionContext()->isInTransaction() &&
                this->internal->closeSyncRegistered.compareAndSet(false, true)) {

                Pointer<ActiveMQConsumerKernel> self =
                    this->session->lookupConsumerKernel(this->consumerInfo->getConsumerId());
                Pointer<Synchronization> sync(new CloseSynhcronization(self));
                this->session->getTransactionContext()->addSynchronization(sync);
            } else {
                doClose();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::doClose() {

    try {
        // Store interrupted state and clear so that Transport operations don't
        // throw InterruptedException and we ensure that resources are clened up.
        bool interrupted = Thread::interrupted();

        dispose();
        // Remove at the Broker Side, consumer has been removed from the local
        // Session and Connection objects so if the remote call to remove throws
        // it is okay to propagate to the client.
        Pointer<RemoveInfo> info(new RemoveInfo);
        info->setObjectId(this->consumerInfo->getConsumerId());
        info->setLastDeliveredSequenceId(this->internal->lastDeliveredSequenceId);
        this->session->oneway(info);
        if (interrupted) {
            Thread::currentThread()->interrupt();
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::dispose() {

    try {

        if (!this->isClosed()) {

            if (!session->isTransacted()) {
                deliverAcks();
                if (isAutoAcknowledgeBatch()) {
                    acknowledge();
                }
            }

            this->internal->started.set(false);

            if (this->internal->executor != NULL) {
                this->internal->executor->shutdown();
                this->internal->executor->awaitTermination(60, TimeUnit::SECONDS);
                this->internal->executor.reset(NULL);
            }

            if (this->internal->optimizedAckTask != NULL) {
                this->session->getScheduler()->cancel(this->internal->optimizedAckTask);
                this->internal->optimizedAckTask = NULL;
            }

            if (session->isClientAcknowledge() || session->isIndividualAcknowledge()) {
                if (!this->consumerInfo->isBrowser()) {
                    // roll back duplicates that aren't acknowledged
                    ArrayList< Pointer<MessageDispatch> > tmp;
                    synchronized(&this->internal->deliveredMessages) {
                        tmp.copy(this->internal->deliveredMessages);
                    }
                    Pointer< Iterator<Pointer<MessageDispatch> > > iter(tmp.iterator());
                    while (iter->hasNext()) {
                        Pointer<MessageDispatch> msg = iter->next();
                        this->session->getConnection()->rollbackDuplicate(this, msg->getMessage());
                    }
                    tmp.clear();
                }
            }

            // Identifies any errors encountered during shutdown.
            bool haveException = false;
            ActiveMQException error;

            if (!this->internal->session->isTransacted()) {
                // For IndividualAck Mode we need to unlink the ack handler to remove a
                // cyclic reference to the MessageDispatch that brought the message to us.
                synchronized(&internal->deliveredMessages) {
                    if (this->session->isIndividualAcknowledge()) {
                        std::auto_ptr<Iterator<Pointer<MessageDispatch> > > iter(this->internal->deliveredMessages.iterator());
                        while (iter->hasNext()) {
                            iter->next()->getMessage()->setAckHandler(Pointer<ActiveMQAckHandler>());
                        }
                    }
                    this->internal->deliveredMessages.clear();
                }
            }

            // Stop and Wakeup all sync consumers.
            this->internal->unconsumedMessages->close();

            // Remove this Consumer from the Connections set of Dispatchers
            Pointer<ActiveMQConsumerKernel> consumer(this);
            try {
                this->session->removeConsumer(consumer);
            } catch (Exception& e) {
                consumer.release();
                throw;
            }
            consumer.release();

            // Ensure these are filtered as duplicates.
            std::vector< Pointer<MessageDispatch> > list = this->internal->unconsumedMessages->removeAll();
            if (!this->consumerInfo->isBrowser()) {
                std::vector< Pointer<MessageDispatch> >::const_iterator iter = list.begin();

                for (; iter != list.end(); ++iter) {
                    Pointer<MessageDispatch> md = *iter;
                    this->session->getConnection()->rollbackDuplicate(this, md->getMessage());
                }
            }

            // If we encountered an error, propagate it.
            if (haveException) {
                error.setMark(__FILE__, __LINE__);
                throw error;
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConsumerKernel::getMessageSelector() const {
    try {
        checkClosed();
        return this->consumerInfo->getSelector();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageDispatch> ActiveMQConsumerKernel::dequeue(long long timeout) {

    try {

        // Calculate the deadline
        long long deadline = 0;
        if (timeout > 0) {
            deadline = System::currentTimeMillis() + timeout;
        }

        // Loop until the time is up or we get a non-expired message
        while (true) {
            Pointer<MessageDispatch> dispatch = this->internal->unconsumedMessages->dequeue(timeout);
            if (dispatch == NULL) {
                if (timeout > 0 && !this->internal->unconsumedMessages->isClosed()) {
                    timeout = Math::max(deadline - System::currentTimeMillis(), 0LL);
                } else {
                    if (this->internal->failureError != NULL) {
                        throw CMSExceptionSupport::create(*this->internal->failureError);
                    } else {
                        return Pointer<MessageDispatch>();
                    }
                }
            } else if (dispatch->getMessage() == NULL) {
                return Pointer<MessageDispatch> ();
            } else if (internal->consumeExpiredMessage(dispatch)) {
                beforeMessageIsConsumed(dispatch);
                afterMessageIsConsumed(dispatch, true);
                if (timeout > 0) {
                    timeout = Math::max(deadline - System::currentTimeMillis(), 0LL);
                }

                sendPullRequest(timeout);
            } else if (internal->redeliveryExceeded(dispatch)) {
                internal->posionAck(dispatch,
                                    "dispatch to " + getConsumerId()->toString() +
                                    " exceeds RedeliveryPolicy limit: " +
                                    Integer::toString(internal->redeliveryPolicy->getMaximumRedeliveries()));
                if (timeout > 0) {
                    timeout = Math::max(deadline - System::currentTimeMillis(), 0LL);
                }

                sendPullRequest(timeout);
            } else {
                return dispatch;
            }
        }

        return Pointer<MessageDispatch>();
    } catch (InterruptedException& ex) {
        Thread::currentThread()->interrupt();
        throw CMSExceptionSupport::create(ex);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumerKernel::receive() {

    try {

        this->checkClosed();
        this->checkMessageListener();

        // Send a request for a new message if needed
        this->sendPullRequest(0);

        // Wait for the next message.
        Pointer<MessageDispatch> message = dequeue(-1);
        if (message == NULL) {
            return NULL;
        }

        beforeMessageIsConsumed(message);
        afterMessageIsConsumed(message, false);

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message, createCMSMessage will do this for us.
        return createCMSMessage(message).release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumerKernel::receive(int timeout) {

    try {

        this->checkClosed();
        this->checkMessageListener();
        if (timeout == 0) {
            return this->receive();
        }

        // Send a request for a new message if needed
        this->sendPullRequest(timeout);

        // Get the next available message, if there is one.
        Pointer<MessageDispatch> message;
        if (internal->info->getPrefetchSize() == 0) {
            message = dequeue(-1);  // Broker will signal if no message.
        } else {
            message = dequeue(timeout);
        }

        if (message == NULL) {
            return NULL;
        }

        beforeMessageIsConsumed(message);
        afterMessageIsConsumed(message, false);

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message, createCMSMessage will do this for us.
        return createCMSMessage(message).release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumerKernel::receiveNoWait() {

    try {

        this->checkClosed();
        this->checkMessageListener();

        // Send a request for a new message if needed
        this->sendPullRequest(-1);

        // Get the next available message, if there is one.
        Pointer<MessageDispatch> message;
        if (internal->info->getPrefetchSize() == 0) {
            message = dequeue(-1);  // Broker will signal if no message.
        } else {
            message = dequeue(0);
        }

        if (message == NULL) {
            return NULL;
        }

        beforeMessageIsConsumed(message);
        afterMessageIsConsumed(message, false);

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message, createCMSMessage will do this for us.
        return createCMSMessage(message).release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setMessageListener(cms::MessageListener* listener) {

    try {

        this->checkClosed();

        if (this->consumerInfo->getPrefetchSize() == 0 && listener != NULL) {
            throw ActiveMQException(__FILE__, __LINE__,
                "Cannot deliver async when Prefetch is Zero, set Prefecth to at least One.");
        }

        if (listener != NULL) {

            // Now that we have a valid message listener, redispatch all the messages that it missed.
            bool wasStarted = session->isStarted();
            if (wasStarted) {
                session->stop();
            }

            synchronized(&(this->internal->listenerMutex)) {
                this->internal->listener = listener;
            }

            this->session->redispatch(*(this->internal->unconsumedMessages));

            if (wasStarted) {
                this->session->start();
            }
        } else {
            synchronized(&(this->internal->listenerMutex)) {
                this->internal->listener = NULL;
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::beforeMessageIsConsumed(Pointer<MessageDispatch> dispatch) {
    this->internal->lastDeliveredSequenceId = dispatch->getMessage()->getMessageId()->getBrokerSequenceId();

    if (!isAutoAcknowledgeBatch()) {

        // When not in an Auto
        synchronized(&this->internal->deliveredMessages) {
            this->internal->deliveredMessages.addFirst(dispatch);
        }

        if (this->session->isTransacted()) {
            if (this->internal->transactedIndividualAck) {
                immediateIndividualTransactedAck(dispatch);
            } else {
                ackLater(dispatch, ActiveMQConstants::ACK_TYPE_DELIVERED);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::immediateIndividualTransactedAck(Pointer<MessageDispatch> dispatch) {
    // acks accumulate on the broker pending transaction completion to indicate delivery status
    registerSync();

    Pointer<MessageAck> ack(new MessageAck(dispatch, ActiveMQConstants::ACK_TYPE_INDIVIDUAL, 1));
    ack->setTransactionId(this->session->getTransactionContext()->getTransactionId());
    this->session->syncRequest(ack);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::registerSync() {
    this->session->doStartTransaction();
    if (!this->internal->synchronizationRegistered) {
        this->internal->synchronizationRegistered = true;
        Pointer<ActiveMQConsumerKernel> self =
            this->session->lookupConsumerKernel(this->consumerInfo->getConsumerId());
        Pointer<Synchronization> sync(new TransactionSynhcronization(self, this->internal));
        this->session->getTransactionContext()->addSynchronization(sync);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::afterMessageIsConsumed(Pointer<MessageDispatch> message, bool messageExpired ) {

    try {

        if (this->internal->unconsumedMessages->isClosed()) {
            return;
        } else if (messageExpired) {
            acknowledge(message, ActiveMQConstants::ACK_TYPE_EXPIRED);
            return;
        } else if (session->isTransacted()) {
            return;
        }

        if (isAutoAcknowledgeEach()) {
            if (this->internal->deliveringAcks.compareAndSet(false, true)) {
                synchronized(&this->internal->deliveredMessages) {
                    if (!this->internal->deliveredMessages.isEmpty()) {
                        if (this->internal->optimizeAcknowledge) {

                            this->internal->ackCounter++;
                            if (this->internal->isTimeForOptimizedAck(this->consumerInfo->getPrefetchSize())) {
                                Pointer<MessageAck> ack =
                                    makeAckForAllDeliveredMessages(ActiveMQConstants::ACK_TYPE_CONSUMED);
                                if (ack != NULL) {
                                    this->internal->deliveredMessages.clear();
                                    this->internal->ackCounter = 0;
                                    this->session->sendAck(ack);
                                    this->internal->optimizeAckTimestamp = System::currentTimeMillis();
                                }

                                // As further optimization send ack for expired messages when there
                                // are any. This resets the deliveredCounter to 0 so that we won't
                                // send standard acks with every message just because the deliveredCounter
                                // just below 0.5 * prefetch as used in ackLater()
                                if (this->internal->pendingAck != NULL && this->internal->deliveredCounter > 0) {
                                    this->session->sendAck(this->internal->pendingAck);
                                    this->internal->pendingAck.reset(NULL);
                                    this->internal->deliveredCounter = 0;
                                }
                            }
                        } else {
                            Pointer<MessageAck> ack =
                                makeAckForAllDeliveredMessages(ActiveMQConstants::ACK_TYPE_CONSUMED);
                            if (ack != NULL) {
                                this->internal->deliveredMessages.clear();
                                session->sendAck(ack);
                            }
                        }
                    }
                }

                this->internal->deliveringAcks.set(false);
            }
        } else if (isAutoAcknowledgeBatch()) {
            ackLater(message, ActiveMQConstants::ACK_TYPE_CONSUMED);
        } else if (session->isClientAcknowledge() || session->isIndividualAcknowledge()) {
            bool messageUnackedByConsumer = false;
            synchronized(&this->internal->deliveredMessages) {
                messageUnackedByConsumer = this->internal->deliveredMessages.contains(message);
            }

            if (messageUnackedByConsumer) {
                this->ackLater(message, ActiveMQConstants::ACK_TYPE_DELIVERED);
            }
        } else {
            throw IllegalStateException(__FILE__, __LINE__, "Invalid Session State");
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::deliverAcks() {

    try {

        Pointer<MessageAck> ack;
        if (this->internal->deliveringAcks.compareAndSet(false, true)) {

            if (isAutoAcknowledgeEach()) {
                synchronized(&this->internal->deliveredMessages) {
                    ack = makeAckForAllDeliveredMessages(ActiveMQConstants::ACK_TYPE_CONSUMED);
                    if (ack != NULL) {
                        this->internal->deliveredMessages.clear();
                        this->internal->ackCounter = 0;
                    } else {
                        ack.swap(internal->pendingAck);
                    }
                }
            } else if (this->internal->pendingAck != NULL && this->internal->pendingAck->isStandardAck()) {
                ack.swap(this->internal->pendingAck);
            }

            if (ack != NULL) {
                if (this->internal->executor == NULL) {
                    this->internal->executor.reset(Executors::newSingleThreadExecutor());
                }

                Pointer< Future<bool> >( this->internal->executor->submit(
                    new AsyncMessageAckTask(ack, this->session, this->internal), true));
            } else {
                this->internal->deliveringAcks.set(false);
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::ackLater(Pointer<MessageDispatch> dispatch, int ackType) {

    // Don't acknowledge now, but we may need to let the broker know the
    // consumer got the message to expand the pre-fetch window
    if (session->isTransacted()) {
        registerSync();
    }

    // The delivered message list is only needed for the recover method
    // which is only used with client ack.
    this->internal->deliveredCounter++;

    Pointer<MessageAck> oldPendingAck = this->internal->pendingAck;
    this->internal->pendingAck.reset(new MessageAck(dispatch, ackType, internal->deliveredCounter));

    if (oldPendingAck == NULL) {
        this->internal->pendingAck->setFirstMessageId(this->internal->pendingAck->getLastMessageId());
    } else if (oldPendingAck->getAckType() == this->internal->pendingAck->getAckType()) {
        this->internal->pendingAck->setFirstMessageId(oldPendingAck->getFirstMessageId());
    } else {
        // old pending ack being superseded by ack of another type, if is is not a delivered
        // ack and hence important, send it now so it is not lost.
        if (!oldPendingAck->isDeliveredAck()) {
            session->sendAck(oldPendingAck);
        }
    }

    if (session->isTransacted()) {
        this->internal->pendingAck->setTransactionId(this->session->getTransactionContext()->getTransactionId());
    }

    // Need to evaluate both expired and normal messages as otherwise consumer may get stalled
    int pendingAcks = (internal->deliveredCounter + internal->ackCounter) - internal->additionalWindowSize;
    if ((0.5 * this->consumerInfo->getPrefetchSize()) <= pendingAcks) {
        session->sendAck(this->internal->pendingAck);
        this->internal->pendingAck.reset(NULL);
        this->internal->deliveredCounter = 0;
        this->internal->additionalWindowSize = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageAck> ActiveMQConsumerKernel::makeAckForAllDeliveredMessages(int type) {

    synchronized( &this->internal->deliveredMessages ) {

        if (!this->internal->deliveredMessages.isEmpty()) {

            Pointer<MessageDispatch> dispatched = this->internal->deliveredMessages.getFirst();
            Pointer<MessageAck> ack(new MessageAck(dispatched, type, this->internal->deliveredMessages.size()));
            ack->setFirstMessageId(this->internal->deliveredMessages.getLast()->getMessage()->getMessageId());

            return ack;
        }
    }

    return Pointer<MessageAck>();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::acknowledge(Pointer<commands::MessageDispatch> dispatch) {
    this->acknowledge(dispatch, ActiveMQConstants::ACK_TYPE_INDIVIDUAL);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::acknowledge(Pointer<commands::MessageDispatch> dispatch, int ackType) {

    try {
        Pointer<MessageAck> ack(new MessageAck(dispatch, ackType, 1));
        if (ack->isExpiredAck()) {
            ack->setFirstMessageId(ack->getLastMessageId());
        }
        session->sendAck(ack);
        synchronized(&this->internal->deliveredMessages) {
            this->internal->deliveredMessages.remove(dispatch);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::acknowledge() {

    try {

        this->internal->clearDeliveredList();
        this->internal->waitForRedeliveries();

        synchronized(&this->internal->deliveredMessages) {

            // Acknowledge all messages so far.
            Pointer<MessageAck> ack = makeAckForAllDeliveredMessages(ActiveMQConstants::ACK_TYPE_CONSUMED);

            if (ack == NULL) {
                return;
            }

            if (session->isTransacted()) {
                this->internal->rollbackOnFailedRecoveryRedelivery();
                session->doStartTransaction();
                ack->setTransactionId(session->getTransactionContext()->getTransactionId());
            }

            this->internal->pendingAck.reset(NULL);
            session->sendAck(ack);

            // Adjust the counters
            this->internal->deliveredCounter = Math::max(0, this->internal->deliveredCounter - (int) this->internal->deliveredMessages.size());
            this->internal->additionalWindowSize = Math::max(0, this->internal->additionalWindowSize - (int) this->internal->deliveredMessages.size());

            if (!session->isTransacted()) {
                this->internal->deliveredMessages.clear();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::commit() {

    synchronized(&(this->internal->deliveredMessages)) {
        this->internal->deliveredMessages.clear();
        this->internal->clearPreviouslyDelivered();
    }
    this->internal->redeliveryDelay = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::rollback() {

    clearDeliveredList();
    synchronized(this->internal->unconsumedMessages.get()) {
        if (this->internal->optimizeAcknowledge) {
            // remove messages read but not acknowledged at the broker yet through optimizeAcknowledge
            if (!this->consumerInfo->isBrowser()) {
                synchronized(&this->internal->deliveredMessages) {
                    for (int i = 0; (i < this->internal->deliveredMessages.size()) &&
                                    (i < this->internal->ackCounter); i++) {
                        // ensure we don't filter this as a duplicate
                        Pointer<MessageDispatch> md = this->internal->deliveredMessages.removeLast();
                        session->getConnection()->rollbackDuplicate(this, md->getMessage());
                    }
                }
            }
        }

        synchronized(&this->internal->deliveredMessages) {
            this->internal->rollbackPreviouslyDeliveredAndNotRedelivered();
            if (this->internal->deliveredMessages.isEmpty()) {
                return;
            }

            // Only increase the redelivery delay after the first redelivery..
            Pointer<MessageDispatch> lastMsg = this->internal->deliveredMessages.getFirst();
            const int currentRedeliveryCount = lastMsg->getMessage()->getRedeliveryCounter();
            if (currentRedeliveryCount > 0) {
                this->internal->redeliveryDelay = this->internal->redeliveryPolicy->getNextRedeliveryDelay(internal->redeliveryDelay);
            } else {
                this->internal->redeliveryDelay = this->internal->redeliveryPolicy->getInitialRedeliveryDelay();
            }

            Pointer<MessageId> firstMsgId = this->internal->deliveredMessages.getLast()->getMessage()->getMessageId();

            Pointer<Iterator<Pointer<MessageDispatch> > > iter(internal->deliveredMessages.iterator());
            while (iter->hasNext()) {
                Pointer<Message> message = iter->next()->getMessage();
                message->setRedeliveryCounter(message->getRedeliveryCounter() + 1);
                // ensure we don't filter this as a duplicate
                session->getConnection()->rollbackDuplicate(this, message);
            }

            if (this->internal->redeliveryPolicy->getMaximumRedeliveries() != RedeliveryPolicy::NO_MAXIMUM_REDELIVERIES &&
                lastMsg->getMessage()->getRedeliveryCounter() > this->internal->redeliveryPolicy->getMaximumRedeliveries()) {

                // We need to NACK the messages so that they get sent to the DLQ.
                // Acknowledge the last message.
                Pointer<MessageAck> ack(new MessageAck(lastMsg, ActiveMQConstants::ACK_TYPE_POISON,
                                        this->internal->deliveredMessages.size()));
                ack->setFirstMessageId(firstMsgId);

                std::string message = "Exceeded RedeliveryPolicy max redelivery limit: " +
                                       Integer::toString(internal->redeliveryPolicy->getMaximumRedeliveries());
                if (!lastMsg->getRollbackCause().getMessage().empty()) {
                    message.append(" cause: Exception -> ");
                    message.append(lastMsg->getRollbackCause().getMessage());
                }

                ack->setPoisonCause(internal->createBrokerError(message));
                session->sendAck(ack, true);
                // Adjust the window size.
                this->internal->additionalWindowSize = Math::max(0,
                    this->internal->additionalWindowSize - (int) this->internal->deliveredMessages.size());
                this->internal->redeliveryDelay = 0;

                this->internal->deliveredCounter -= (int) internal->deliveredMessages.size();
                this->internal->deliveredMessages.clear();

            } else {

                // only redelivery_ack after first delivery
                if (currentRedeliveryCount > 0) {
                    Pointer<MessageAck> ack(new MessageAck(lastMsg, ActiveMQConstants::ACK_TYPE_REDELIVERED,
                                            this->internal->deliveredMessages.size()));
                    ack->setFirstMessageId(firstMsgId);
                    session->sendAck(ack);
                }

                if (this->internal->nonBlockingRedelivery) {

                    if (!this->internal->unconsumedMessages->isClosed()) {
                        Pointer<ActiveMQConsumerKernel> self =
                            this->session->lookupConsumerKernel(this->consumerInfo->getConsumerId());

                        NonBlockingRedeliveryTask* redeliveryTask =
                            new NonBlockingRedeliveryTask(session, self, this->internal);

                        this->internal->deliveredCounter -= (int) internal->deliveredMessages.size();
                        this->internal->deliveredMessages.clear();

                        this->session->getScheduler()->executeAfterDelay(
                            redeliveryTask, this->internal->redeliveryDelay);
                    }
                } else {
                    // stop the delivery of messages.
                    this->internal->unconsumedMessages->stop();

                    std::auto_ptr<Iterator<Pointer<MessageDispatch> > > iter(
                        this->internal->deliveredMessages.iterator());
                    while (iter->hasNext()) {
                        this->internal->unconsumedMessages->enqueueFirst(iter->next());
                    }

                    this->internal->deliveredCounter -= (int) internal->deliveredMessages.size();
                    this->internal->deliveredMessages.clear();

                    if (internal->redeliveryDelay > 0 && !this->internal->unconsumedMessages->isClosed()) {
                        Pointer<ActiveMQConsumerKernel> self =
                            this->session->lookupConsumerKernel(this->consumerInfo->getConsumerId());
                        this->internal->scheduler->executeAfterDelay(
                            new StartConsumerTask(self, session), internal->redeliveryDelay);
                    } else {
                        start();
                    }
                }
            }
        }
    }

    if (this->internal->listener != NULL) {
        session->redispatch(*this->internal->unconsumedMessages);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::dispatch(const Pointer<MessageDispatch>& dispatch) {

    try {

        clearMessagesInProgress();
        clearDeliveredList();

        synchronized(this->internal->unconsumedMessages.get()) {

            if (!this->internal->unconsumedMessages->isClosed()) {

                if (this->consumerInfo->isBrowser() || !session->getConnection()->isDuplicate(this, dispatch->getMessage())) {

                    synchronized(&this->internal->listenerMutex) {

                        if (this->internal->listener != NULL && this->internal->unconsumedMessages->isRunning()) {
                            if (this->internal->redeliveryExceeded(dispatch)) {
                                internal->posionAck(dispatch,
                                                    "dispatch to " + getConsumerId()->toString() +
                                                    " exceeds redelivery policy limit:" +
                                                    Integer::toString(internal->redeliveryPolicy->getMaximumRedeliveries()));
                                return;
                            }
                            Pointer<cms::Message> message = createCMSMessage(dispatch);
                            beforeMessageIsConsumed(dispatch);
                            try {
                                bool expired = isConsumerExpiryCheckEnabled() && dispatch->getMessage()->isExpired();
                                if (!expired) {
                                    this->internal->listener->onMessage(message.get());
                                }
                                afterMessageIsConsumed(dispatch, expired);
                            } catch (RuntimeException& e) {
                                dispatch->setRollbackCause(e);
                                if (isAutoAcknowledgeBatch() || isAutoAcknowledgeEach() || session->isIndividualAcknowledge()) {
                                    // Schedule redelivery and possible DLQ processing
                                    rollback();
                                } else {
                                    // Transacted or Client ack: Deliver the next message.
                                    afterMessageIsConsumed(dispatch, false);
                                }
                            }
                        } else {
                            if (!this->internal->unconsumedMessages->isRunning()) {
                                // delayed redelivery, ensure it can be re delivered
                                session->getConnection()->rollbackDuplicate(this, dispatch->getMessage());
                            }
                            this->internal->unconsumedMessages->enqueue(dispatch);
                            if (this->internal->messageAvailableListener != NULL) {
                                this->internal->messageAvailableListener->onMessageAvailable(this);
                            }
                        }
                    }
                } else {
                    // deal with duplicate delivery
                    if (this->internal->redeliveryExpectedInCurrentTransaction(dispatch, true)) {
                        if (this->internal->transactedIndividualAck) {
                            immediateIndividualTransactedAck(dispatch);
                        } else {
                            Pointer<MessageAck> ack(new MessageAck(dispatch, ActiveMQConstants::ACK_TYPE_DELIVERED, 1));
                            internal->session->sendAck(ack);
                        }
                    } else if ((internal->redeliveryPendingInCompetingTransaction(dispatch))) {
                        internal->session->getConnection()->rollbackDuplicate(this, dispatch->getMessage());
                        this->dispatch(dispatch);
                    } else {
                        internal->posionAck(dispatch,
                            std::string("Suppressing duplicate delivery on connection, consumer ") + getConsumerId()->toString());
                    }
                }
            }

            if (++internal->dispatchedCount % 1000 == 0) {
                internal->dispatchedCount = 0;
                Thread::yield();
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<cms::Message> ActiveMQConsumerKernel::createCMSMessage(Pointer<MessageDispatch> dispatch) {

    try {

        Pointer<Message> message = dispatch->getMessage()->copy();
        if (this->internal->transformer != NULL) {
            cms::Message* source = dynamic_cast<cms::Message*>(message.get());
            cms::Message* transformed = NULL;

            if (this->internal->transformer->consumerTransform(
                (cms::Session*)this->session, (cms::MessageConsumer*)this, source, &transformed)) {

                if (transformed == NULL) {
                    throw NullPointerException(__FILE__, __LINE__, "Client MessageTransformer returned a NULL message");
                }

                Message* amqMessage = NULL;

                // If the transform create a new ActiveMQ Message command then we can discard the transformed
                // cms::Message here, otherwise the transformed message was already an ActiveMQ Message
                // command of some sort so we just place casted amqMessage in our Pointer and let it get
                // cleaned up after its been dispatched.
                if (ActiveMQMessageTransformation::transformMessage(transformed, this->session->getConnection(), &amqMessage)){
                    delete transformed;
                }

                message.reset(amqMessage);
            }
        }

        // If the Session is in ClientAcknowledge or IndividualAcknowledge mode, then
        // we set the handler in the message to this object and send it out.
        if (session->isClientAcknowledge()) {
            Pointer<ActiveMQAckHandler> ackHandler(new ClientAckHandler(this->session));
            message->setAckHandler(ackHandler);
        } else if (session->isIndividualAcknowledge()) {
            Pointer<ActiveMQAckHandler> ackHandler(new IndividualAckHandler(this, dispatch));
            message->setAckHandler(ackHandler);
        } else {
            Pointer<ActiveMQAckHandler> ackHandler(new NoOpAckHandler());
            message->setAckHandler(ackHandler);
        }

        return message.dynamicCast<cms::Message>();
    }
    AMQ_CATCH_RETHROW(cms::CMSException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::sendPullRequest(long long timeout) {

    try {
        this->internal->clearDeliveredList();

        // There are still local message, consume them first.
        if (!this->internal->unconsumedMessages->isEmpty()) {
            return;
        }

        if (this->consumerInfo->getPrefetchSize() == 0) {

            Pointer<MessagePull> messagePull(new MessagePull());
            messagePull->setConsumerId(this->consumerInfo->getConsumerId());
            messagePull->setDestination(this->consumerInfo->getDestination());
            messagePull->setTimeout(timeout);

            this->session->oneway(messagePull);
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::checkClosed() const {
    if (this->isClosed()) {
        throw cms::IllegalStateException("Consumer Already Closed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::checkMessageListener() const {
    if (this->internal->listener != NULL) {
        throw cms::IllegalStateException(
            "Cannot synchronously receive a message when a MessageListener is set");
    }

    this->session->checkMessageListener();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::iterate() {

    synchronized(&this->internal->listenerMutex) {
        if (this->internal->listener != NULL) {
            Pointer<MessageDispatch> dispatch = internal->unconsumedMessages->dequeueNoWait();
            if (dispatch != NULL) {
                this->dispatch(dispatch);
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::inProgressClearRequired() {
    this->internal->inProgressClearRequiredFlag.incrementAndGet();
    // Clears dispatched messages async to avoid lock contention with inprogress acks.
    this->internal->isClearDeliveredList = true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::clearDeliveredList() {
    this->internal->clearDeliveredList();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::clearMessagesInProgress() {
    if (this->internal->inProgressClearRequiredFlag.get() > 0) {
        synchronized(this->internal->unconsumedMessages.get()) {
            if (this->internal->inProgressClearRequiredFlag.get() > 0) {

                // ensure messages that were not yet consumed are rolled back up front as they
                // may get redelivered to another consumer by the Broker.
                std::vector< Pointer<MessageDispatch> > list = this->internal->unconsumedMessages->removeAll();
                if (!this->consumerInfo->isBrowser()) {
                    std::vector< Pointer<MessageDispatch> >::const_iterator iter = list.begin();

                    for (; iter != list.end(); ++iter) {
                        Pointer<MessageDispatch> md = *iter;
                        this->session->getConnection()->rollbackDuplicate(this, md->getMessage());
                    }
                }

                // allow dispatch on this connection to resume
                this->session->getConnection()->setTransportInterruptionProcessingComplete();
                this->internal->inProgressClearRequiredFlag.decrementAndGet();

                // Wake up any blockers and allow them to recheck state.
                this->internal->unconsumedMessages->notifyAll();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isAutoAcknowledgeEach() const {
    return this->session->isAutoAcknowledge() ||
           (this->session->isDupsOkAcknowledge() && this->consumerInfo->getDestination()->isQueue());
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isAutoAcknowledgeBatch() const {
    return this->session->isDupsOkAcknowledge() && !this->consumerInfo->getDestination()->isQueue();
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConsumerKernel::getMessageAvailableCount() const {
    return this->internal->unconsumedMessages->size();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::applyDestinationOptions(Pointer<ConsumerInfo> info) {

    decaf::lang::Pointer<commands::ActiveMQDestination> amqDestination = info->getDestination();

    // Get any options specified in the destination and apply them to the ConsumerInfo object.
    const ActiveMQProperties& options = amqDestination->getOptions();

    std::string noLocalStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_NOLOCAL);
    if (options.hasProperty(noLocalStr)) {
        info->setNoLocal(Boolean::parseBoolean(options.getProperty(noLocalStr)));
    }

    std::string selectorStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_SELECTOR);
    if (options.hasProperty(selectorStr)) {
        info->setSelector(options.getProperty(selectorStr));
    }

    std::string priorityStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_PRIORITY);
    if (options.hasProperty(priorityStr)) {
        info->setPriority((unsigned char) Integer::parseInt(options.getProperty(priorityStr)));
    }

    std::string dispatchAsyncStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_DISPATCHASYNC);
    if (options.hasProperty(dispatchAsyncStr)) {
        info->setDispatchAsync(Boolean::parseBoolean(options.getProperty(dispatchAsyncStr)));
    }

    std::string exclusiveStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_EXCLUSIVE);
    if (options.hasProperty(exclusiveStr)) {
        info->setExclusive(Boolean::parseBoolean(options.getProperty(exclusiveStr)));
    }

    std::string maxPendingMsgLimitStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CUNSUMER_MAXPENDINGMSGLIMIT);
    if (options.hasProperty(maxPendingMsgLimitStr)) {
        info->setMaximumPendingMessageLimit(Integer::parseInt(options.getProperty(maxPendingMsgLimitStr)));
    }

    std::string prefetchSizeStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_PREFECTCHSIZE);
    if (options.hasProperty(prefetchSizeStr)) {
        info->setPrefetchSize(Integer::parseInt(options.getProperty(prefetchSizeStr, "1000")));
    }

    std::string retroactiveStr = core::ActiveMQConstants::toString(core::ActiveMQConstants::CONSUMER_RETROACTIVE);
    if (options.hasProperty(retroactiveStr)) {
        info->setRetroactive(Boolean::parseBoolean(options.getProperty(retroactiveStr)));
    }

    this->internal->nonBlockingRedelivery = Boolean::parseBoolean(
        options.getProperty("consumer.nonBlockingRedelivery", "false"));
    this->internal->transactedIndividualAck = Boolean::parseBoolean(
        options.getProperty("consumer.transactedIndividualAck", "false"));
    this->internal->consumerExpiryCheckEnabled = Boolean::parseBoolean(
        options.getProperty("consumer.consumerExpiryCheckEnabled", "true"));
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setRedeliveryPolicy(RedeliveryPolicy* policy) {
    if (policy != NULL) {
        this->internal->redeliveryPolicy.reset(policy);
    }
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConsumerKernel::getRedeliveryPolicy() const {
    return this->internal->redeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageListener* ActiveMQConsumerKernel::getMessageListener() const {
    return this->internal->listener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setMessageTransformer(cms::MessageTransformer* transformer) {
    this->internal->transformer = transformer;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageTransformer* ActiveMQConsumerKernel::getMessageTransformer() const {
    return this->internal->transformer;
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerInfo>& ActiveMQConsumerKernel::getConsumerInfo() const {
    return this->consumerInfo;
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerId>& ActiveMQConsumerKernel::getConsumerId() const {
    return this->consumerInfo->getConsumerId();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isSynchronizationRegistered() const {
    return this->internal->synchronizationRegistered;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setSynchronizationRegistered(bool value) {
    this->internal->synchronizationRegistered = value;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConsumerKernel::getLastDeliveredSequenceId() const {
    return this->internal->lastDeliveredSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setLastDeliveredSequenceId(long long value) {
    this->internal->lastDeliveredSequenceId = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isTransactedIndividualAck() const {
    return this->internal->transactedIndividualAck;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setTransactedIndividualAck(bool value) {
    this->internal->transactedIndividualAck = value;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConsumerKernel::setFailoverRedeliveryWaitPeriod() const {
    return this->internal->failoverRedeliveryWaitPeriod;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setFailoverRedeliveryWaitPeriod(long long value) {
    this->internal->failoverRedeliveryWaitPeriod = value;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setFailureError(decaf::lang::Exception* error) {
    if (error != NULL) {
        this->internal->failureError.reset(error->clone());
    }
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception* ActiveMQConsumerKernel::getFailureError() const {
    if (this->internal->failureError == NULL) {
        return NULL;
    }

    return this->internal->failureError.get();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setPrefetchSize(int prefetchSize) {
    deliverAcks();
    this->consumerInfo->setCurrentPrefetchSize(prefetchSize);
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isInUse(Pointer<ActiveMQDestination> destination) const {
    return this->consumerInfo->getDestination()->equals(destination.get());
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setMessageAvailableListener(cms::MessageAvailableListener* listener) {
    this->internal->messageAvailableListener = listener;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageAvailableListener* ActiveMQConsumerKernel::getMessageAvailableListener() const {
    return this->internal->messageAvailableListener;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConsumerKernel::getHashCode() const {
    return this->internal->hashCode;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConsumerKernel::getOptimizedAckScheduledAckInterval() const {
    return this->internal->optimizedAckScheduledAckInterval;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setOptimizedAckScheduledAckInterval(long long value) {
    this->internal->optimizedAckScheduledAckInterval = value;

    if (this->internal->optimizedAckTask != NULL) {
        try {
            this->session->getScheduler()->cancel(this->internal->optimizedAckTask);
            this->internal->optimizedAckTask = NULL;
        } catch (Exception& e) {
            this->internal->optimizedAckTask = NULL;
            throw CMSExceptionSupport::create(e);
        }
    }

    // Should we periodically send out all outstanding acks.
    if (this->internal->optimizeAcknowledge && this->internal->optimizedAckScheduledAckInterval > 0) {
        Pointer<ActiveMQConsumerKernel> self =
            this->session->lookupConsumerKernel(this->consumerInfo->getConsumerId());
        this->internal->optimizedAckTask = new OptimizedAckTask(self, this->internal);

        try {
            this->session->getScheduler()->executePeriodically(
                this->internal->optimizedAckTask, this->internal->optimizedAckScheduledAckInterval);
        } catch (Exception& e) {
            throw CMSExceptionSupport::create(e);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isOptimizeAcknowledge() const {
    return this->internal->optimizeAcknowledge;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setOptimizeAcknowledge(bool value) {
    if (this->internal->optimizeAcknowledge && !value) {
        deliverAcks();
    }

    this->internal->optimizeAcknowledge = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isConsumerExpiryCheckEnabled() {
    return this->internal->consumerExpiryCheckEnabled;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumerKernel::setConsumerExpiryCheckEnabled(bool consumerExpiryCheckEnabled) {
    this->internal->consumerExpiryCheckEnabled = consumerExpiryCheckEnabled;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumerKernel::isRedeliveryExpectedInCurrentTransaction(Pointer<MessageDispatch> dispatch) const {
    return this->internal->redeliveryExpectedInCurrentTransaction(dispatch, false);
}
