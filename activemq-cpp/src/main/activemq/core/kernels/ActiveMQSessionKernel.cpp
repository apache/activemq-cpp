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
#include <activemq/util/ActiveMQMessageTransformation.h>
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
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Runnable.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
#include <decaf/util/Queue.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/locks/ReentrantReadWriteLock.h>
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
    private:

        SessionConfig(const SessionConfig&);
        SessionConfig& operator=(const SessionConfig&);

    public:

        AtomicBoolean synchronizationRegistered;
        decaf::util::concurrent::locks::ReentrantReadWriteLock producerLock;
        decaf::util::LinkedList< Pointer<ActiveMQProducerKernel> > producers;
        decaf::util::concurrent::locks::ReentrantReadWriteLock consumerLock;
        decaf::util::LinkedList< Pointer<ActiveMQConsumerKernel> > consumers;
        Pointer<Scheduler> scheduler;
        Pointer<CloseSynhcronization> closeSync;
        Mutex sendMutex;
        cms::MessageTransformer* transformer;
        int hashCode;

    public:

        SessionConfig() : synchronizationRegistered(false),
                          producerLock(), producers(), consumerLock(), consumers(),
                          scheduler(), closeSync(), sendMutex(), transformer(NULL),
                          hashCode() {}
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

    this->config->hashCode = id->getHashCode();

    try {
        this->connection->oneway(this->sessionInfo);
    } catch (...) {
        this->sessionInfo.reset(NULL);
        delete this->config;
        throw;
    }

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
        try {
            this->start();
        } catch (...) {
            this->transaction.reset(NULL);
            this->executor.reset(NULL);
            delete this->config;
            throw;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSessionKernel::~ActiveMQSessionKernel() {
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        // Free the executor here so that its threads are gone before any of the
        // other member data of this class is destroyed as it might be accessing
        // from its run thread.
        this->executor.reset(NULL);
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->config;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::fire(const ActiveMQException& ex) {
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
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
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

    try {

        Finalizer final(this, this->connection);

        // Stop the dispatch executor.
        stop();

        // Roll Back the transaction since we were closed without an explicit call
        // to commit it.
        if (this->transaction->isInTransaction()) {
            this->transaction->rollback();
        }

        // Dispose of all Consumers, the dispose method skips the RemoveInfo command.
        this->config->consumerLock.writeLock().lock();
        try {
            // We have to copy all the consumers to another list since we aren't using a
            // CopyOnWriteArrayList right now.
            ArrayList<Pointer<ActiveMQConsumerKernel> > consumers(this->config->consumers);
            Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > consumerIter(consumers.iterator());
            while (consumerIter->hasNext()) {
                try{
                    Pointer<ActiveMQConsumerKernel> consumer = consumerIter->next();
                    consumer->setFailureError(this->connection->getFirstFailureError());
                    consumer->dispose();
                    this->lastDeliveredSequenceId =
                        Math::max(this->lastDeliveredSequenceId, consumer->getLastDeliveredSequenceId());
                } catch (cms::CMSException& ex) {
                    /* Absorb */
                }
            }
            this->config->consumers.clear();
            this->config->consumerLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->consumerLock.writeLock().unlock();
            throw;
        }

        // Dispose of all Producers, the dispose method skips the RemoveInfo command.
        this->config->producerLock.writeLock().lock();
        try {
            // We have to copy all the producers to another list since we aren't using a
            // CopyOnWriteArrayList right now.
            ArrayList<Pointer<ActiveMQProducerKernel> > producers(this->config->producers);
            std::auto_ptr<Iterator<Pointer<ActiveMQProducerKernel> > > producerIter(producers.iterator());

            while (producerIter->hasNext()) {
                try{
                    producerIter->next()->dispose();
                } catch (cms::CMSException& ex) {
                    /* Absorb */
                }
            }
            this->config->producers.clear();
            this->config->producerLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->producerLock.writeLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::commit() {

    try {

        this->checkClosed();

        if (!this->isTransacted()) {
            throw ActiveMQException(
                __FILE__, __LINE__, "ActiveMQSessionKernel::commit - This Session is not Transacted");
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
            throw ActiveMQException(
                __FILE__, __LINE__, "ActiveMQSessionKernel::rollback - This Session is not Transacted");
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

        this->config->consumerLock.readLock().lock();
        try {
            Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());
            while (iter->hasNext()) {
                Pointer<ActiveMQConsumerKernel> consumer = iter->next();
                consumer->rollback();
            }
            this->config->consumerLock.readLock().unlock();
        } catch (Exception& ex) {
            this->config->consumerLock.readLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::clearMessagesInProgress() {

    if (this->executor.get() != NULL) {
        this->executor->clearMessagesInProgress();
    }

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());
        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            consumer->inProgressClearRequired();
            this->connection->getScheduler()->executeAfterDelay(
                new ClearConsumerTask(consumer), 0LL);
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::acknowledge() {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());
        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            consumer->acknowledge();
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::deliverAcks() {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());
        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            consumer->deliverAcks();
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
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
            this->removeConsumer(consumer);
            throw;
        }

        consumer->setMessageTransformer(this->config->transformer);

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
            this->removeConsumer(consumer);
            throw;
        }

        consumer->setMessageTransformer(this->config->transformer);

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
        Pointer<ActiveMQProducerKernel> producer(new ActiveMQProducerKernel(
            this, this->getNextProducerId(), dest, this->connection->getSendTimeout()));

        try {
            this->addProducer(producer);
            this->connection->oneway(producer->getProducerInfo());
        } catch (Exception& ex) {
            this->removeProducer(producer);
            throw;
        }

        producer->setMessageTransformer(this->config->transformer);

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

        if (queueName.find(commands::ActiveMQDestination::TEMP_DESTINATION_NAME_PREFIX) == 0) {
            return new ActiveMQTempQueue(queueName);
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

        if (topicName.find(commands::ActiveMQDestination::TEMP_DESTINATION_NAME_PREFIX) == 0) {
            return new ActiveMQTempTopic(topicName);
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
void ActiveMQSessionKernel::send(kernels::ActiveMQProducerKernel* producer, Pointer<commands::ActiveMQDestination> destination,
                                 cms::Message* message, int deliveryMode, int priority, long long timeToLive,
                                 util::MemoryUsage* producerWindow, long long sendTimeout, cms::AsyncCallback* onComplete) {

    try {

        this->checkClosed();

        if (destination->isTemporary()) {
            Pointer<ActiveMQTempDestination> tempDest = destination.dynamicCast<ActiveMQTempDestination>();
            if (this->connection->isDeleted(tempDest)) {
                throw cms::InvalidDestinationException(
                    std::string("Cannot publish to a deleted Destination: ") + destination->toString());
            }
        }

        synchronized(&this->config->sendMutex) {

            // Ensure that a new transaction is started if this is the first message
            // sent since the last commit, Broker is notified of a new TX.
            doStartTransaction();

            Pointer<TransactionId> txId = this->transaction->getTransactionId();
            Pointer<ProducerInfo> producerInfo = producer->getProducerInfo();
            Pointer<ProducerId> producerId = producerInfo->getProducerId();
            long long sequenceId = producer->getNextMessageSequence();

            // Set the "CMS" header fields on the original message, see JMS 1.1 spec section 3.4.11
            message->setCMSDeliveryMode(deliveryMode);
            long long expiration = 0LL;
            if (!producer->getDisableMessageTimeStamp()) {
                long long timeStamp = System::currentTimeMillis();
                message->setCMSTimestamp(timeStamp);
                if (timeToLive > 0) {
                    expiration = timeToLive + timeStamp;
                }
            }
            message->setCMSExpiration(expiration);
            message->setCMSPriority(priority);
            message->setCMSRedelivered(false);

            // transform to our own message format here
            commands::Message* transformed = NULL;
            Pointer<commands::Message> amqMessage;

            // Always assign the message ID, regardless of the disable flag.
            // Not adding a message ID will cause an NPE at the broker.
            decaf::lang::Pointer<commands::MessageId> id(new commands::MessageId());
            id->setProducerId(producerId);
            id->setProducerSequenceId(sequenceId);

            // NOTE:
            // Now we copy the message before sending, this allows the user to reuse the
            // message object without interfering with the copy that's being sent.  We
            // could make this step optional to increase performance but for now we won't.
            // To not do this implies that the user must never reuse the message object, or
            // know that the configuration of Transports doesn't involve the message hanging
            // around beyond the point that send returns.  When the transform step results in
            // a new Message object being created we can just use that new instance, but when
            // the original cms::Message pointer was already a commands::Message then we need
            // to clone it.
            if (ActiveMQMessageTransformation::transformMessage(message, connection, &transformed)) {
                amqMessage.reset(transformed);
            } else {
                amqMessage.reset(transformed->cloneDataStructure());
            }

            // Sets the Message ID on the original message per spec.
            message->setCMSMessageID(id->toString());
            message->setCMSDestination(destination.dynamicCast<cms::Destination>().get());

            amqMessage->setMessageId(id);
            amqMessage->getBrokerPath().clear();
            amqMessage->setTransactionId(txId);
            amqMessage->setConnection(this->connection);

            // destination format is provider specific so only set on transformed message
            amqMessage->setDestination(destination);

            amqMessage->onSend();
            amqMessage->setProducerId(producerId);

            if (onComplete == NULL && sendTimeout <= 0 && !amqMessage->isResponseRequired() && !this->connection->isAlwaysSyncSend() &&
                (!amqMessage->isPersistent() || this->connection->isUseAsyncSend() || amqMessage->getTransactionId() != NULL)) {

                // No Response Required, send is asynchronous.
                this->connection->oneway(amqMessage);

                if (producerWindow != NULL) {
                    producerWindow->enqueueUsage(amqMessage->getSize());
                }

            } else {
                if (sendTimeout > 0 && onComplete == NULL) {
                    this->connection->syncRequest(amqMessage, (unsigned int)sendTimeout);
                } else {
                    this->connection->asyncRequest(amqMessage, onComplete);
                }
            }
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
void ActiveMQSessionKernel::setMessageTransformer(cms::MessageTransformer* transformer) {
    this->config->transformer = transformer;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageTransformer* ActiveMQSessionKernel::getMessageTransformer() const {
    return this->config->transformer;
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

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            consumer->start();
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
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
        this->connection->addTempDestination(Pointer<ActiveMQTempDestination>(tempDestination->cloneDataStructure()));
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSessionKernel::isInUse(Pointer<ActiveMQDestination> destination) {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->isInUse(destination)) {
                this->config->consumerLock.readLock().unlock();
                return true;
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }

    return false;
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
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQSessionKernel::createTemporaryDestinationName() {

    try {
        return this->connection->getConnectionId().getValue() + ":" +
               Long::toString(this->connection->getNextTempDestinationId());
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::oneway(Pointer<Command> command) {

    try {
        this->connection->oneway(command);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ActiveMQSessionKernel::syncRequest(Pointer<Command> command, unsigned int timeout) {

    try {
        this->checkClosed();
        return this->connection->syncRequest(command, timeout);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
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

        this->config->consumerLock.writeLock().lock();
        try {
            this->config->consumers.add(consumer);
            this->config->consumerLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->consumerLock.writeLock().unlock();
            throw;
        }

        // Register this as a message dispatcher for the consumer.
        this->connection->addDispatcher(consumer->getConsumerInfo()->getConsumerId(), this);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::removeConsumer(Pointer<ActiveMQConsumerKernel> consumer) {

    try {
        this->connection->removeDispatcher(consumer->getConsumerId());
        this->config->consumerLock.writeLock().lock();
        try {
            this->config->consumers.remove(consumer);
            this->config->consumerLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->consumerLock.writeLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::addProducer(Pointer<ActiveMQProducerKernel> producer) {

    try {
        this->checkClosed();

        this->config->producerLock.writeLock().lock();
        try {
            this->config->producers.add(producer);
            this->config->producerLock.writeLock().unlock();
        } catch(Exception& ex) {
            this->config->producerLock.writeLock().unlock();
            throw;
        }

        this->connection->addProducer(producer);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::removeProducer(Pointer<ActiveMQProducerKernel> producer) {

    try {
        this->connection->removeProducer(producer->getProducerId());
        this->config->producerLock.writeLock().lock();
        try {
            this->config->producers.remove(producer);
            this->config->producerLock.writeLock().unlock();
        } catch(Exception& ex) {
            this->config->producerLock.writeLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQProducerKernel> ActiveMQSessionKernel::lookupProducerKernel(Pointer<ProducerId> id) {

    this->config->producerLock.readLock().lock();
    try {

        std::auto_ptr<Iterator<Pointer<ActiveMQProducerKernel> > > producerIter(this->config->producers.iterator());

        while (producerIter->hasNext()) {
            Pointer<ActiveMQProducerKernel> producer = producerIter->next();
            if (producer->getProducerId()->equals(*id)) {
                this->config->producerLock.readLock().unlock();
                return producer;
            }
        }

        this->config->producerLock.readLock().unlock();
    } catch(Exception& ex) {
        this->config->producerLock.readLock().unlock();
        throw;
    }

    return Pointer<ActiveMQProducerKernel>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQConsumerKernel> ActiveMQSessionKernel::lookupConsumerKernel(Pointer<ConsumerId> id) {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->getConsumerId()->equals(*id)) {
                this->config->consumerLock.readLock().unlock();
                return consumer;
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }

    return Pointer<ActiveMQConsumerKernel>();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSessionKernel::iterateConsumers() {

    if (this->closed.get()) {
        return false;
    }

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->iterate()) {
                this->config->consumerLock.readLock().unlock();
                return true;
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::setPrefetchSize(Pointer<ConsumerId> id, int prefetch) {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->getConsumerId()->equals(*id)) {
                consumer->setPrefetchSize(prefetch);
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::close(Pointer<ConsumerId> id) {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->getConsumerId()->equals(*id)) {
                try {
                    consumer->close();
                } catch (cms::CMSException& e) {
                }
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
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

////////////////////////////////////////////////////////////////////////////////
int ActiveMQSessionKernel::getHashCode() const {
    return this->config->hashCode;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::checkMessageListener() const {

    this->config->consumerLock.readLock().lock();
    try {
        Pointer<Iterator< Pointer<ActiveMQConsumerKernel> > > iter(this->config->consumers.iterator());
        while (iter->hasNext()) {
            Pointer<ActiveMQConsumerKernel> consumer = iter->next();
            if (consumer->getMessageListener() != NULL) {
                throw cms::IllegalStateException(
                    "Cannot synchronously receive a message when a MessageListener is set");
            }
        }
        this->config->consumerLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->consumerLock.readLock().unlock();
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionKernel::sendAck(Pointer<MessageAck> ack, bool async) {
    if (async || this->connection->isSendAcksAsync() || this->isTransacted()) {
        this->connection->oneway(ack);
    } else {
        this->connection->syncRequest(ack);
    }
}
