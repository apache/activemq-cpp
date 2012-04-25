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

#include "ActiveMQSessionKernel.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQQueueBrowser.h>
#include <activemq/core/ActiveMQSessionExecutor.h>
#include <activemq/core/PrefetchPolicy.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQBytesMessage.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ActiveMQMapMessage.h>
#include <activemq/commands/ActiveMQStreamMessage.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
#include <decaf/util/Queue.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::threads;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{
namespace kernels{

    class CloseSynhcronization;

    class SessionConfig {
    public:

        typedef decaf::util::StlMap< Pointer<commands::ConsumerId>,
                                     Pointer<ActiveMQConsumerKernel>,
                                     commands::ConsumerId::COMPARATOR> ConsumersMap;

    private:

        SessionConfig(const SessionConfig&);
        SessionConfig& operator=(const SessionConfig&);

    public:

        AtomicBoolean synchronizationRegistered;
        decaf::util::concurrent::CopyOnWriteArrayList< Pointer<ActiveMQProducerKernel> > producers;
        Pointer<Scheduler> scheduler;
        Pointer<CloseSynhcronization> closeSync;

    public:

        SessionConfig() : synchronizationRegistered(false), producers(), scheduler(), closeSync() {}
        ~SessionConfig() {}
    };

    /**
     * Class used to clear a Consumer's dispatch queue asynchronously from the
     * connection class's Scheduler instance.
     */
    class ClearConsumerTask : public Runnable {
    private:

        Pointer<ActiveMQConsumerKernel> consumer;

    private:

        ClearConsumerTask(const ClearConsumerTask&);
        ClearConsumerTask& operator=(const ClearConsumerTask&);

    public:

        ClearConsumerTask(Pointer<ActiveMQConsumerKernel> consumer) : Runnable(), consumer(consumer) {

            if (consumer == NULL) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~ClearConsumerTask() {}

        virtual void run() {
            this->consumer->clearMessagesInProgress();
        }
    };

    /**
     * Class used to Hook a session that has been closed into the Transaction
     * it is currently a part of.  Once the Transaction has been Committed or
     * Rolled back this Synchronization can finish the Close of the session.
     */
    class CloseSynhcronization : public Synchronization {
    private:

        ActiveMQSessionKernel* session;
        SessionConfig* config;

    private:

        CloseSynhcronization(const CloseSynhcronization&);
        CloseSynhcronization& operator=(const CloseSynhcronization&);

    public:

        CloseSynhcronization(ActiveMQSessionKernel* session, SessionConfig* config) :
            Synchronization(), session(session), config(config) {

            if (session == NULL || config == NULL) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Session.");
            }
        }

        virtual ~CloseSynhcronization() {}

        virtual void beforeEnd() {
        }

        virtual void afterCommit() {
            config->closeSync.release();
            session->doClose();
            config->synchronizationRegistered.set(false);
        }

        virtual void afterRollback() {
            config->closeSync.release();
            session->doClose();
            config->synchronizationRegistered.set(false);
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionKernel::ActiveMQSessionKernel(ActiveMQConnection* connection,
                                             const Pointer<SessionId>& id,
                                             cms::Session::AcknowledgeMode ackMode,
                                             const Properties& properties) : config(new SessionConfig),
                                                                             sessionInfo(),
                                                                             transaction(),
                                                                             connection(connection),
                                                                             consumers(),
                                                                             closed(false),
                                                                             executor(),
                                                                             ackMode(ackMode),
                                                                             producerIds(),
                                                                             producerSequenceIds(),
                                                                             consumerIds(),
                                                                             lastDeliveredSequenceId(0) {

    if (id == NULL || connection == NULL) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQSessionKernel::ActiveMQSessionKernel - Constructor called with NULL data");
    }

    this->sessionInfo.reset(new SessionInfo());
    this->sessionInfo->setAckMode(ackMode);
    this->sessionInfo->setSessionId(id);

    this->connection->oneway(this->sessionInfo);

    this->closed.set(false);
    this->lastDeliveredSequenceId = -1;

    // Create a Transaction objet
    this->transaction.reset(new ActiveMQTransactionContext(this, properties));

    // Create the session executor object.
    this->executor.reset(new ActiveMQSessionExecutor(this));

    // Use the Connection's Scheduler.
    this->config->scheduler = this->connection->getScheduler();

    // If the connection is already started, start the session.
    if (this->connection->isStarted()) {
        this->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionKernel::~ActiveMQSessionKernel() {
    try {
        // Destroy this session's resources
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )

    delete this->config;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::fire(const activemq::exceptions::ActiveMQException& ex) {
    if (connection != NULL) {
        connection->fire(ex);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::close() {

    // If we're already closed, just return.
    if (this->closed.get()) {
        return;
    }

    try {

        if (this->transaction->isInXATransaction()) {
            if (!this->config->synchronizationRegistered.compareAndSet(false, true)) {
                this->config->closeSync.reset(new CloseSynhcronization(this, this->config));
                this->transaction->addSynchronization(this->config->closeSync);
            }
        } else {
            doClose();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::doClose() {

    try {
        dispose();

        // Remove this session from the Broker.
        Pointer<RemoveInfo> info(new RemoveInfo());
        info->setObjectId(this->sessionInfo->getSessionId());
        info->setLastDeliveredSequenceId(this->lastDeliveredSequenceId);
        this->connection->oneway(info);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::dispose() {

    // Prevent Dispose loop if transaction has a close synchronization registered.
    if (!closed.compareAndSet(false, true)) {
        return;
    }

    class Finalizer {
    private:

        ActiveMQSessionKernel* session;
        ActiveMQConnection* connection;

    private:

        Finalizer(const Finalizer&);
        Finalizer& operator=(const Finalizer&);

    public:

        Finalizer(ActiveMQSessionKernel* session, ActiveMQConnection* connection) :
            session(session), connection(connection) {
        }

        ~Finalizer() {
            Pointer<ActiveMQSessionKernel> session(this->session);
            try {
                this->connection->removeSession(session);
            } catch(...) {
                session.release();
            }
            session.release();
        }
    };

    try{

        Finalizer final(this, this->connection);

        // Stop the dispatch executor.
        stop();

        // Roll Back the transaction since we were closed without an explicit call
        // to commit it.
        if (this->transaction->isInTransaction()) {
            this->transaction->rollback();
        }

        // Dispose of all Consumers, the dispose method skips the RemoveInfo command.
        synchronized(&this->consumers) {

            std::vector< Pointer<ActiveMQConsumerKernel> > closables = this->consumers.values();

            for (std::size_t i = 0; i < closables.size(); ++i) {
                try{
                    closables[i]->setFailureError(this->connection->getFirstFailureError());
                    closables[i]->dispose();
                    this->lastDeliveredSequenceId =
                        Math::max(this->lastDeliveredSequenceId, closables[i]->getLastDeliveredSequenceId());
                } catch( cms::CMSException& ex ){
                    /* Absorb */
                }
            }
        }

        // Dispose of all Producers, the dispose method skips the RemoveInfo command.
        std::auto_ptr<Iterator<Pointer<ActiveMQProducerKernel> > > producerIter(this->config->producers.iterator());

        while (producerIter->hasNext()) {
            try{
                producerIter->next()->dispose();
            } catch (cms::CMSException& ex) {
                /* Absorb */
            }
        }
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::commit() {

    try {

        this->checkClosed();

        if (!this->isTransacted()) {
            throw ActiveMQException(__FILE__, __LINE__, "ActiveMQSessionKernel::commit - This Session is not Transacted");
        }

        // Commit the Transaction
        this->transaction->commit();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::rollback() {

    try {

        this->checkClosed();

        if (!this->isTransacted()) {
            throw ActiveMQException(__FILE__, __LINE__, "ActiveMQSessionKernel::rollback - This Session is not Transacted");
        }

        // Roll back the Transaction
        this->transaction->rollback();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::recover() {

    try {

        checkClosed();

        if (isTransacted()) {
            throw cms::IllegalStateException("This session is transacted");
        }

        synchronized( &this->consumers ) {
            std::vector< Pointer<ActiveMQConsumerKernel> > consumers = this->consumers.values();

            std::vector< Pointer<ActiveMQConsumerKernel> >::iterator iter = consumers.begin();
            for( ; iter != consumers.end(); ++iter ) {
                (*iter)->rollback();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::clearMessagesInProgress() {

    if (this->executor.get() != NULL) {
        this->executor->clearMessagesInProgress();
    }

    synchronized(&this->consumers) {
        std::vector< Pointer<ActiveMQConsumerKernel> > consumers = this->consumers.values();

        std::vector< Pointer<ActiveMQConsumerKernel> >::iterator iter = consumers.begin();
        for (; iter != consumers.end(); ++iter) {
            (*iter)->inProgressClearRequired();

            this->connection->getScheduler()->executeAfterDelay(
                new ClearConsumerTask(*iter), 0LL);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::acknowledge() {

    synchronized(&this->consumers) {
        std::vector< Pointer<ActiveMQConsumerKernel> > consumers = this->consumers.values();

        std::vector< Pointer<ActiveMQConsumerKernel> >::iterator iter = consumers.begin();
        for (; iter != consumers.end(); ++iter) {
            (*iter)->acknowledge();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::deliverAcks() {

    synchronized(&this->consumers) {
        std::vector< Pointer<ActiveMQConsumerKernel> > consumers = this->consumers.values();

        std::vector< Pointer<ActiveMQConsumerKernel> >::iterator iter = consumers.begin();
        for (; iter != consumers.end(); ++iter) {
            (*iter)->deliverAcks();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSessionKernel::createConsumer(const cms::Destination* destination) {

    try {
        this->checkClosed();
        return this->createConsumer(destination, "", false);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSessionKernel::createConsumer(const cms::Destination* destination, const std::string& selector) {

    try {
        this->checkClosed();
        return this->createConsumer(destination, selector, false);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSessionKernel::createConsumer(const cms::Destination* destination,
                                                            const std::string& selector, bool noLocal) {

    try {

        this->checkClosed();

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        if (amqDestination == NULL) {
            throw ActiveMQException(__FILE__, __LINE__, "Destination was either NULL or not created by this CMS Client");
        }

        Pointer<ActiveMQDestination> dest( amqDestination->cloneDataStructure() );

        int prefetch = 0;
        if (dest->isTopic()) {
            prefetch = this->connection->getPrefetchPolicy()->getTopicPrefetch();
        } else {
            prefetch = this->connection->getPrefetchPolicy()->getQueuePrefetch();
        }

        // Create the consumer instance.
        Pointer<ActiveMQConsumerKernel> consumer(
            new ActiveMQConsumerKernel(this, this->getNextConsumerId(),
                                       dest, "", selector, prefetch, 0, noLocal,
                                       false, this->connection->isDispatchAsync(), NULL));

        try{
            this->addConsumer(consumer);
            this->connection->syncRequest(consumer->getConsumerInfo());
        } catch (Exception& ex) {
            this->removeConsumer(consumer->getConsumerId());
            throw ex;
        }

        if (this->connection->isStarted()) {
            consumer->start();
        }

        return new ActiveMQConsumer(consumer);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSessionKernel::createDurableConsumer(const cms::Topic* destination, const std::string& name,
                                                                   const std::string& selector, bool noLocal) {

    try {

        this->checkClosed();

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination = dynamic_cast<const ActiveMQDestination*> (destination);

        if (amqDestination == NULL) {
            throw ActiveMQException(__FILE__, __LINE__, "Destination was either NULL or not created by this CMS Client");
        }

        Pointer<ActiveMQDestination> dest(amqDestination->cloneDataStructure());

        // Create the consumer instance.
        Pointer<ActiveMQConsumerKernel> consumer(
            new ActiveMQConsumerKernel(this, this->getNextConsumerId(),
                                       dest, name, selector,
                                       this->connection->getPrefetchPolicy()->getDurableTopicPrefetch(),
                                       0, noLocal, false, this->connection->isDispatchAsync(), NULL));

        try {
            this->addConsumer(consumer);
            this->connection->syncRequest(consumer->getConsumerInfo());
        } catch (Exception& ex) {
            this->removeConsumer(consumer->getConsumerId());
            throw ex;
        }

        if (this->connection->isStarted()) {
            consumer->start();
        }

        return new ActiveMQConsumer(consumer);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSessionKernel::createProducer( const cms::Destination* destination ) {

    try {

        this->checkClosed();

        Pointer<commands::ActiveMQDestination> dest;

        // Producers are allowed to have NULL destinations.  In this case, the
        // destination is specified by the messages as they are sent.
        if (destination != NULL) {

            const ActiveMQDestination* amqDestination =
                dynamic_cast<const ActiveMQDestination*> (destination);

            if (amqDestination == NULL) {
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "Destination was either NULL or not created by this CMS Client" );
            }

            // Cast the destination to an OpenWire destination, so we can
            // get all the goodies.
            dest.reset(amqDestination->cloneDataStructure());
        }

        // Create the producer instance.
        Pointer<ActiveMQProducerKernel> producer( new ActiveMQProducerKernel(
            this, this->getNextProducerId(), dest, this->connection->getSendTimeout() ) );

        try {
            this->addProducer(producer);
            this->connection->oneway(producer->getProducerInfo());
        } catch (Exception& ex) {
            this->removeProducer(producer);
            throw ex;
        }

        return new ActiveMQProducer(producer);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSessionKernel::createBrowser( const cms::Queue* queue ) {

    try {
        return ActiveMQSessionKernel::createBrowser(queue, "");
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSessionKernel::createBrowser(const cms::Queue* queue, const std::string& selector) {

    try {

        this->checkClosed();

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>(queue);

        if (amqDestination == NULL) {
            throw ActiveMQException(__FILE__, __LINE__, "Destination was either NULL or not created by this CMS Client");
        }

        Pointer<ActiveMQDestination> dest(amqDestination->cloneDataStructure());

        // Create the QueueBrowser instance
        std::auto_ptr<ActiveMQQueueBrowser> browser(
            new ActiveMQQueueBrowser(this, this->getNextConsumerId(), dest,
                                     selector, this->connection->isDispatchAsync()));

        return browser.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* ActiveMQSessionKernel::createQueue(const std::string& queueName) {

    try {

        this->checkClosed();

        if (queueName == "") {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Destination Name cannot be the Empty String." );
        }

        return new commands::ActiveMQQueue(queueName);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* ActiveMQSessionKernel::createTopic(const std::string& topicName) {

    try {

        this->checkClosed();

        if (topicName == "") {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Destination Name cannot be the Empty String." );
        }

        return new commands::ActiveMQTopic(topicName);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* ActiveMQSessionKernel::createTemporaryQueue() {

    try {

        this->checkClosed();

        std::auto_ptr<commands::ActiveMQTempQueue> queue(new
            commands::ActiveMQTempQueue(this->createTemporaryDestinationName()));

        // Register it with the Broker
        this->createTemporaryDestination(queue.get());

        return queue.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* ActiveMQSessionKernel::createTemporaryTopic() {

    try {

        this->checkClosed();

        std::auto_ptr<commands::ActiveMQTempTopic> topic(new
            commands::ActiveMQTempTopic(createTemporaryDestinationName()));

        // Register it with the Broker
        this->createTemporaryDestination(topic.get());

        return topic.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSessionKernel::createMessage() {

    try {
        this->checkClosed();
        commands::ActiveMQMessage* message = new commands::ActiveMQMessage();
        message->setConnection(this->connection);
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSessionKernel::createBytesMessage() {

    try {
        this->checkClosed();
        commands::ActiveMQBytesMessage* message = new commands::ActiveMQBytesMessage();
        message->setConnection(this->connection);
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSessionKernel::createBytesMessage(const unsigned char* bytes, int bytesSize) {

    try {
        this->checkClosed();
        cms::BytesMessage* msg = createBytesMessage();
        msg->setBodyBytes(bytes, bytesSize);
        return msg;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::StreamMessage* ActiveMQSessionKernel::createStreamMessage() {

    try {
        this->checkClosed();
        commands::ActiveMQStreamMessage* message = new commands::ActiveMQStreamMessage();
        message->setConnection(this->connection);
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSessionKernel::createTextMessage() {

    try {
        this->checkClosed();
        commands::ActiveMQTextMessage* message = new commands::ActiveMQTextMessage();
        message->setConnection(this->connection);
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSessionKernel::createTextMessage(const std::string& text) {

    try {
        this->checkClosed();
        cms::TextMessage* msg = createTextMessage();
        msg->setText(text.c_str());
        return msg;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* ActiveMQSessionKernel::createMapMessage() {

    try {
        this->checkClosed();
        commands::ActiveMQMapMessage* message = new commands::ActiveMQMapMessage();
        message->setConnection(this->connection);
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session::AcknowledgeMode ActiveMQSessionKernel::getAcknowledgeMode() const {
    return this->ackMode;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSessionKernel::isTransacted() const {
    return (this->ackMode == Session::SESSION_TRANSACTED) || this->transaction->isInXATransaction();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::send(cms::Message* message, ActiveMQProducerKernel* producer, util::Usage* usage) {

    try {

        this->checkClosed();

        commands::Message* amqMessage = dynamic_cast< commands::Message* >(message);

        if (amqMessage == NULL) {
            throw ActiveMQException(__FILE__, __LINE__,
                "ActiveMQSessionKernel::send - Message is not a valid Open Wire type.");
        }

        // Clear any old data that might be in the message object
        amqMessage->getMessageId().reset(NULL);
        amqMessage->getProducerId().reset(NULL);
        amqMessage->getTransactionId().reset(NULL);

        // Always assign the message ID, regardless of the disable
        // flag.  Not adding a message ID will cause an NPE at the broker.
        decaf::lang::Pointer<commands::MessageId> id(new commands::MessageId());
        id->setProducerId(producer->getProducerInfo()->getProducerId());
        id->setProducerSequenceId(this->getNextProducerSequenceId());

        amqMessage->setMessageId(id);

        // Ensure that a new transaction is started if this is the first message
        // sent since the last commit.
        doStartTransaction();
        amqMessage->setTransactionId(this->transaction->getTransactionId());

        // NOTE:
        // Now we copy the message before sending, this allows the user to reuse the
        // message object without interfering with the copy that's being sent.  We
        // could make this step optional to increase performance but for now we won't.
        // To not do this implies that the user must never reuse the message object, or
        // know that the configuration of Transports doesn't involve the message hanging
        // around beyond the point that send returns.
        Pointer<commands::Message> msgCopy(amqMessage->cloneDataStructure());

        msgCopy->onSend();
        msgCopy->setProducerId( producer->getProducerInfo()->getProducerId() );

        if (this->connection->getSendTimeout() <= 0 &&
            !msgCopy->isResponseRequired() &&
            !this->connection->isAlwaysSyncSend() &&
            (!msgCopy->isPersistent() || this->connection->isUseAsyncSend() ||
               msgCopy->getTransactionId() != NULL)) {

            if (usage != NULL) {
                usage->enqueueUsage(msgCopy->getSize());
            }

            // No Response Required.
            this->connection->oneway(msgCopy);

        } else {

            // Send the message to the broker.
            this->connection->syncRequest(msgCopy, this->connection->getSendTimeout());
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQSessionKernel::getExceptionListener() {

    if (connection != NULL) {
        return connection->getExceptionListener();
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Scheduler> ActiveMQSessionKernel::getScheduler() const {
    return this->config->scheduler;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::unsubscribe(const std::string& name) {

    try {

        this->checkClosed();

        Pointer<RemoveSubscriptionInfo> rsi(new RemoveSubscriptionInfo());

        rsi->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        rsi->setSubcriptionName(name);
        rsi->setClientId(this->connection->getConnectionInfo().getClientId());

        // Send the message to the broker.
        this->connection->syncRequest(rsi);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::dispatch(const Pointer<MessageDispatch>& dispatch) {

    if (this->executor.get() != NULL) {
        this->executor->execute(dispatch);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::redispatch(MessageDispatchChannel& unconsumedMessages) {

    std::vector< Pointer<MessageDispatch> > messages = unconsumedMessages.removeAll();
    std::vector< Pointer<MessageDispatch> >::reverse_iterator iter = messages.rbegin();

    for (; iter != messages.rend(); ++iter) {
        executor->executeFirst(*iter);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::start() {

    synchronized(&this->consumers) {
        std::vector< Pointer<ActiveMQConsumerKernel> > consumers = this->consumers.values();

        std::vector< Pointer<ActiveMQConsumerKernel> >::iterator iter = consumers.begin();
        for (; iter != consumers.end(); ++iter) {
            (*iter)->start();
        }
    }

    if (this->executor.get() != NULL) {
        this->executor->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::stop() {

    if (this->executor.get() != NULL) {
        this->executor->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSessionKernel::isStarted() const {

    if (this->executor.get() == NULL) {
        return false;
    }

    return this->executor->isRunning();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::createTemporaryDestination(commands::ActiveMQTempDestination* tempDestination) {

    try {

        Pointer<DestinationInfo> command(new DestinationInfo());
        command->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        command->setOperationType(ActiveMQConstants::DESTINATION_ADD_OPERATION);
        command->setDestination(Pointer<ActiveMQTempDestination> (tempDestination->cloneDataStructure()));

        // Send the message to the broker.
        this->syncRequest(command);

        // Now that its setup, link it to this Connection so it can be closed.
        tempDestination->setConnection(this->connection);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::destroyTemporaryDestination(
    commands::ActiveMQTempDestination* tempDestination) {

    try {

        Pointer<DestinationInfo> command(new DestinationInfo());

        command->setConnectionId(this->connection->getConnectionInfo().getConnectionId());
        command->setOperationType(ActiveMQConstants::DESTINATION_REMOVE_OPERATION);
        command->setDestination(Pointer<ActiveMQTempDestination> (tempDestination->cloneDataStructure()));

        // Send the message to the broker.
        this->connection->syncRequest(command);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQSessionKernel::createTemporaryDestinationName() {

    try {
        return this->connection->getConnectionId().getValue() + ":" +
               Long::toString(this->connection->getNextTempDestinationId());
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::oneway(Pointer<Command> command) {

    try {
        this->checkClosed();
        this->connection->oneway(command);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ActiveMQSessionKernel::syncRequest(Pointer<Command> command, unsigned int timeout) {

    try {
        this->checkClosed();
        return this->connection->syncRequest(command, timeout);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::checkClosed() const {
    if (this->closed.get()) {
        throw ActiveMQException(__FILE__, __LINE__, "ActiveMQSessionKernel - Session Already Closed");
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::addConsumer(Pointer<ActiveMQConsumerKernel> consumer) {

    try {

        this->checkClosed();

        // Add the consumer to the map.
        synchronized(&this->consumers) {
            this->consumers.put(consumer->getConsumerInfo()->getConsumerId(), consumer);
        }

        // Register this as a message dispatcher for the consumer.
        this->connection->addDispatcher(consumer->getConsumerInfo()->getConsumerId(), this);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::removeConsumer(const Pointer<ConsumerId>& consumerId) {

    try {

        this->checkClosed();

        synchronized(&this->consumers) {
            if (this->consumers.containsKey(consumerId)) {
                // Remove this Id both from the Sessions Map of Consumers and from the Connection.
                // If the kernels parent is destroyed then it will get cleaned up now.
                this->connection->removeDispatcher(consumerId);
                this->consumers.remove(consumerId);
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::addProducer(Pointer<ActiveMQProducerKernel> producer) {

    try {
        this->checkClosed();
        this->config->producers.add(producer);
        this->connection->addProducer(producer);
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::removeProducer(Pointer<ActiveMQProducerKernel> producer) {

    try {
        this->checkClosed();
        this->connection->removeProducer(producer->getProducerId());
        this->config->producers.remove(producer);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::setPrefetchSize(Pointer<ConsumerId> id, int prefetch) {

    synchronized(&this->consumers) {
        if (this->consumers.containsKey(id)) {
            Pointer<ActiveMQConsumerKernel> consumer = this->consumers.get(id);
            consumer->setPrefetchSize(prefetch);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::close(Pointer<ConsumerId> id) {

    synchronized(&this->consumers) {
        if (this->consumers.containsKey(id)) {
            Pointer<ActiveMQConsumerKernel> consumer = this->consumers.get(id);

            try {
                consumer->close();
            } catch (cms::CMSException& e) {
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::doStartTransaction() {

    if (this->isTransacted() && !this->transaction->isInXATransaction()) {
        this->transaction->begin();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::wakeup() {

    if (this->executor.get() != NULL) {
        this->executor->wakeup();
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::ConsumerId> ActiveMQSessionKernel::getNextConsumerId() {
    Pointer<ConsumerId> consumerId(new commands::ConsumerId());

    consumerId->setConnectionId(this->connection->getConnectionId().getValue());
    consumerId->setSessionId(this->sessionInfo->getSessionId()->getValue());
    consumerId->setValue(this->consumerIds.getNextSequenceId());

    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::ProducerId> ActiveMQSessionKernel::getNextProducerId() {
    Pointer<ProducerId> producerId(new ProducerId());

    producerId->setConnectionId(this->connection->getConnectionId().getValue());
    producerId->setSessionId(this->sessionInfo->getSessionId()->getValue());
    producerId->setValue(this->producerIds.getNextSequenceId());

    return producerId;
}
