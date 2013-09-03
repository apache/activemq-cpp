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

#include "ActiveMQConnection.h"

#include <cms/Session.h>

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQConnectionMetaData.h>
#include <activemq/core/ActiveMQMessageAudit.h>
#include <activemq/core/ActiveMQDestinationSource.h>
#include <activemq/core/AdvisoryConsumer.h>
#include <activemq/core/ConnectionAudit.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <activemq/core/kernels/ActiveMQProducerKernel.h>
#include <activemq/core/policies/DefaultPrefetchPolicy.h>
#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/BrokerException.h>
#include <activemq/exceptions/ConnectionFailedException.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/IdGenerator.h>
#include <activemq/transport/failover/FailoverTransport.h>
#include <activemq/transport/ResponseCallback.h>

#include <decaf/lang/Math.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/Set.h>
#include <decaf/util/Collection.h>
#include <decaf/util/LinkedList.h>
#include <decaf/util/UUID.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>
#include <decaf/util/concurrent/locks/ReentrantReadWriteLock.h>

#include <activemq/commands/Command.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/BrokerInfo.h>
#include <activemq/commands/BrokerError.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/commands/ProducerAck.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/WireFormatInfo.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::kernels;
using namespace activemq::core::policies;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::threads;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class ConnectionThreadFactory : public ThreadFactory {
    private:

        std::string connectionId;

    public:

        ConnectionThreadFactory(std::string connectionId) : connectionId(connectionId) {
            if (connectionId.empty()) {
                throw NullPointerException(__FILE__, __LINE__, "Connection Id must be set.");
            }
        }

        virtual ~ConnectionThreadFactory() {}

        virtual Thread* newThread(decaf::lang::Runnable* runnable) {
            static std::string prefix = "ActiveMQ Connection Executor: ";

            std::string name = prefix + connectionId;
            Thread* thread = new Thread(runnable, name);
            return thread;
        }

    };

    class ConnectionConfig {
    private:

        ConnectionConfig(const ConnectionConfig&);
        ConnectionConfig& operator=(const ConnectionConfig&);

    public:

        typedef decaf::util::StlMap< Pointer<commands::ConsumerId>,
                                     Dispatcher*,
                                     commands::ConsumerId::COMPARATOR > DispatcherMap;

        typedef decaf::util::StlMap< Pointer<commands::ProducerId>,
                                     Pointer<ActiveMQProducerKernel>,
                                     commands::ProducerId::COMPARATOR > ProducerMap;

        typedef decaf::util::concurrent::ConcurrentStlMap< Pointer<commands::ActiveMQTempDestination>,
                                                           Pointer<commands::ActiveMQTempDestination>,
                                                           commands::ActiveMQTempDestination::COMPARATOR > TempDestinationMap;

    public:

        static util::IdGenerator CONNECTION_ID_GENERATOR;

        Pointer<decaf::util::Properties> properties;
        Pointer<transport::Transport> transport;
        Pointer<util::IdGenerator> clientIdGenerator;
        Pointer<Scheduler> scheduler;
        Pointer<ExecutorService> executor;

        util::LongSequenceGenerator sessionIds;
        util::LongSequenceGenerator consumerIdGenerator;
        util::LongSequenceGenerator tempDestinationIds;
        util::LongSequenceGenerator localTransactionIds;

        std::string brokerURL;

        bool clientIDSet;
        bool isConnectionInfoSentToBroker;
        bool userSpecifiedClientID;

        decaf::util::concurrent::Mutex ensureConnectionInfoSentMutex;
        decaf::util::concurrent::Mutex onExceptionLock;
        decaf::util::concurrent::Mutex mutex;

        bool dispatchAsync;
        bool alwaysSyncSend;
        bool useAsyncSend;
        bool sendAcksAsync;
        bool messagePrioritySupported;
        bool watchTopicAdvisories;
        bool useCompression;
        bool useRetroactiveConsumer;
        bool checkForDuplicates;
        bool optimizeAcknowledge;
        bool exclusiveConsumer;
        bool transactedIndividualAck;
        bool nonBlockingRedelivery;
        int compressionLevel;
        unsigned int sendTimeout;
        unsigned int closeTimeout;
        unsigned int producerWindowSize;
        int auditDepth;
        int auditMaximumProducerNumber;
        long long optimizeAcknowledgeTimeOut;
        long long optimizedAckScheduledAckInterval;
        long long consumerFailoverRedeliveryWaitPeriod;

        std::auto_ptr<PrefetchPolicy> defaultPrefetchPolicy;
        std::auto_ptr<RedeliveryPolicy> defaultRedeliveryPolicy;

        cms::ExceptionListener* exceptionListener;
        cms::MessageTransformer* transformer;

        Pointer<commands::ConnectionInfo> connectionInfo;
        Pointer<commands::BrokerInfo> brokerInfo;
        Pointer<commands::WireFormatInfo> brokerWireFormatInfo;
        Pointer<CountDownLatch> transportInterruptionProcessingComplete;
        Pointer<CountDownLatch> brokerInfoReceived;
        Pointer<AdvisoryConsumer> advisoryConsumer;

        Pointer<Exception> firstFailureError;

        DispatcherMap dispatchers;
        ProducerMap activeProducers;

        decaf::util::concurrent::locks::ReentrantReadWriteLock sessionsLock;
        decaf::util::LinkedList< Pointer<ActiveMQSessionKernel> > activeSessions;
        decaf::util::LinkedList<transport::TransportListener*> transportListeners;

        TempDestinationMap activeTempDestinations;

        ConnectionAudit connectionAudit;

        ConnectionConfig(const Pointer<transport::Transport> transport,
                         const Pointer<decaf::util::Properties> properties) :
                             properties(properties),
                             transport(transport),
                             clientIdGenerator(),
                             scheduler(),
                             executor(),
                             sessionIds(),
                             consumerIdGenerator(),
                             tempDestinationIds(),
                             localTransactionIds(),
                             brokerURL(""),
                             clientIDSet(false),
                             isConnectionInfoSentToBroker(false),
                             userSpecifiedClientID(false),
                             ensureConnectionInfoSentMutex(),
                             onExceptionLock(),
                             mutex(),
                             dispatchAsync(true),
                             alwaysSyncSend(false),
                             useAsyncSend(false),
                             sendAcksAsync(true),
                             messagePrioritySupported(true),
                             watchTopicAdvisories(true),
                             useCompression(false),
                             useRetroactiveConsumer(false),
                             checkForDuplicates(true),
                             optimizeAcknowledge(false),
                             exclusiveConsumer(false),
                             transactedIndividualAck(false),
                             nonBlockingRedelivery(false),
                             compressionLevel(-1),
                             sendTimeout(0),
                             closeTimeout(15000),
                             producerWindowSize(0),
                             auditDepth(ActiveMQMessageAudit::DEFAULT_WINDOW_SIZE),
                             auditMaximumProducerNumber(ActiveMQMessageAudit::MAXIMUM_PRODUCER_COUNT),
                             optimizeAcknowledgeTimeOut(300),
                             optimizedAckScheduledAckInterval(0),
                             consumerFailoverRedeliveryWaitPeriod(0),
                             defaultPrefetchPolicy(NULL),
                             defaultRedeliveryPolicy(NULL),
                             exceptionListener(NULL),
                             transformer(NULL),
                             connectionInfo(),
                             brokerInfo(),
                             brokerWireFormatInfo(),
                             transportInterruptionProcessingComplete(),
                             brokerInfoReceived(),
                             advisoryConsumer(),
                             firstFailureError(),
                             dispatchers(),
                             activeProducers(),
                             sessionsLock(),
                             activeSessions(),
                             transportListeners(),
                             activeTempDestinations() {

            this->defaultPrefetchPolicy.reset(new DefaultPrefetchPolicy());
            this->defaultRedeliveryPolicy.reset(new DefaultRedeliveryPolicy());
            this->clientIdGenerator.reset(new util::IdGenerator);
            this->connectionInfo.reset(new ConnectionInfo());
            this->brokerInfoReceived.reset(new CountDownLatch(1));

            // Generate a connectionId
            std::string uniqueId = CONNECTION_ID_GENERATOR.generateId();
            decaf::lang::Pointer<ConnectionId> connectionId(new ConnectionId());
            connectionId->setValue(uniqueId);

            this->executor.reset(
                new ThreadPoolExecutor(1, 1, 5, TimeUnit::SECONDS,
                    new LinkedBlockingQueue<Runnable*>(),
                    new ConnectionThreadFactory(connectionId->toString())));

            this->connectionInfo->setConnectionId(connectionId);
            this->scheduler.reset(new Scheduler(std::string("ActiveMQConnection[")+uniqueId+"] Scheduler"));
            this->scheduler->start();
        }

        ~ConnectionConfig() {
            try {
                synchronized(&onExceptionLock) {
                    this->scheduler->shutdown();
                    this->executor->shutdown();
                    this->executor->awaitTermination(10, TimeUnit::MINUTES);
                }
            }
            AMQ_CATCHALL_NOTHROW()
        }

        void waitForBrokerInfo() {
            this->brokerInfoReceived->await();
        }
    };

    // Static init.
    util::IdGenerator ConnectionConfig::CONNECTION_ID_GENERATOR;

    class ConnectionErrorRunnable : public Runnable {
    private:

        ActiveMQConnection* connection;
        Pointer<ConnectionError> error;

    private:

        ConnectionErrorRunnable(const ConnectionErrorRunnable&);
        ConnectionErrorRunnable& operator= (const ConnectionErrorRunnable&);

    public:

        ConnectionErrorRunnable(ActiveMQConnection* connection, Pointer<ConnectionError> error) :
            Runnable(), connection(connection), error(error) {}
        virtual ~ConnectionErrorRunnable() {}

        virtual void run() {
            try {
                if (error != NULL && error->getException() != NULL) {
                    this->connection->onAsyncException(error->getException()->createExceptionObject());
                }
            } catch(Exception& ex) {}
        }
    };

    class OnAsyncExceptionRunnable : public Runnable {
    private:

        ActiveMQConnection* connection;
        Exception ex;

    private:

        OnAsyncExceptionRunnable(const OnAsyncExceptionRunnable&);
        OnAsyncExceptionRunnable& operator= (const OnAsyncExceptionRunnable&);

    public:

        OnAsyncExceptionRunnable(ActiveMQConnection* connection, const Exception& ex) :
            Runnable(), connection(connection), ex(ex) {}
        virtual ~OnAsyncExceptionRunnable() {}

        virtual void run() {
            try {
                cms::ExceptionListener* listener = this->connection->getExceptionListener();
                if (listener != NULL) {

                    const cms::CMSException* cause = dynamic_cast<const cms::CMSException*>(ex.getCause());
                    if (cause != NULL) {
                        listener->onException(*cause);
                    } else {
                        ActiveMQException amqEx(ex);
                        listener->onException(amqEx.convertToCMSException());
                    }
                }
            } catch(Exception& ex) {}
        }
    };

    class OnExceptionRunnable : public Runnable {
    private:

        ActiveMQConnection* connection;
        ConnectionConfig* config;
        Pointer<Exception> ex;

    private:

        OnExceptionRunnable(const OnExceptionRunnable&);
        OnExceptionRunnable& operator= (const OnExceptionRunnable&);

    public:

        OnExceptionRunnable(ActiveMQConnection* connection, ConnectionConfig* config, Exception* ex) :
            Runnable(), connection(connection), config(config), ex(ex) {}
        virtual ~OnExceptionRunnable() {}

        virtual void run() {
            try {

                // Take control of this pointer, it will be given to the Connection who
                // will destroy it when it closes.
                Exception* error = ex.release();

                // Mark this Connection as having a Failed transport.
                this->connection->setFirstFailureError(error);

                Pointer<Transport> transport = this->config->transport;
                if (transport != NULL) {
                    try {
                        transport->stop();
                    } catch(...) {
                    }
                }

                this->config->brokerInfoReceived->countDown();

                // Clean up the Connection resources.
                this->connection->cleanup();

                synchronized(&this->config->transportListeners) {
                    Pointer< Iterator<TransportListener*> > iter( this->config->transportListeners.iterator() );

                    while (iter->hasNext()) {
                        try {
                            iter->next()->onException(*error);
                        } catch(...) {}
                    }
                }
            } catch(Exception& ex) {}
        }
    };

    class AsyncResponseCallback : public ResponseCallback {
    private:

        ConnectionConfig* config;
        cms::AsyncCallback* callback;

    private:

        AsyncResponseCallback(const AsyncResponseCallback&);
        AsyncResponseCallback& operator= (const AsyncResponseCallback&);

    public:

        AsyncResponseCallback(ConnectionConfig* config, cms::AsyncCallback* callback) :
            ResponseCallback(), config(config), callback(callback) {
        }

        virtual ~AsyncResponseCallback() {
        }

        virtual void onComplete(Pointer<commands::Response> response) {

            commands::ExceptionResponse* exceptionResponse =
                dynamic_cast<ExceptionResponse*> (response.get());

            if (exceptionResponse != NULL) {

                Exception ex = exceptionResponse->getException()->createExceptionObject();
                const cms::CMSException* cmsError = dynamic_cast<const cms::CMSException*>(ex.getCause());
                if (cmsError != NULL) {
                    this->callback->onException(*cmsError);
                } else {
                    BrokerException error = BrokerException(__FILE__, __LINE__, exceptionResponse->getException()->getMessage().c_str());
                    this->callback->onException(error.convertToCMSException());
                }
            } else {
                this->callback->onSuccess();
            }
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection(const Pointer<transport::Transport> transport,
                                       const Pointer<decaf::util::Properties> properties) :
    config(NULL), connectionMetaData(new ActiveMQConnectionMetaData()), started(false),
    closed(false), closing(false), transportFailed(false) {

    Pointer<ConnectionConfig> configuration(
            new ConnectionConfig(transport, properties));

    // Register for messages and exceptions from the connector.
    transport->setTransportListener(this);

    // Set the initial state of the ConnectionInfo
    configuration->connectionInfo->setManageable(true);
    configuration->connectionInfo->setFaultTolerant(transport->isFaultTolerant());

    configuration->connectionAudit.setCheckForDuplicates(transport->isFaultTolerant());

    this->config = configuration.release();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::~ActiveMQConnection() {

    try {
        this->close();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        // This must happen even if exceptions occur in the Close attempt.
        delete this->config;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addDispatcher(const decaf::lang::Pointer<ConsumerId>& consumer, Dispatcher* dispatcher) {

    try {
        synchronized(&this->config->dispatchers) {
            this->config->dispatchers.put(consumer, dispatcher);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher(const decaf::lang::Pointer<ConsumerId>& consumer) {

    try {
        synchronized(&this->config->dispatchers) {
            this->config->dispatchers.remove(consumer);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession() {
    try {
        return createSession(Session::AUTO_ACKNOWLEDGE);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession(cms::Session::AcknowledgeMode ackMode) {

    try {

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        // Create the session instance as a Session Kernel we then create and return a
        // ActiveMQSession instance that acts as a proxy to the kernel caller can delete
        // that at any time since we only refer to the Pointer to the session kernel.
        Pointer<ActiveMQSessionKernel> session(new ActiveMQSessionKernel(this, getNextSessionId(), ackMode, *this->config->properties));

        session->setMessageTransformer(this->config->transformer);

        this->addSession(session);

        return new ActiveMQSession(session);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
Pointer<SessionId> ActiveMQConnection::getNextSessionId() {

    decaf::lang::Pointer<SessionId> sessionId(new SessionId());
    sessionId->setConnectionId(this->config->connectionInfo->getConnectionId()->getValue());
    sessionId->setValue(this->config->sessionIds.getNextSequenceId());

    return sessionId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addSession(Pointer<ActiveMQSessionKernel> session) {
    try {
        this->config->sessionsLock.writeLock().lock();
        try {
            this->config->activeSessions.add(session);
            this->config->sessionsLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->sessionsLock.writeLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeSession(Pointer<ActiveMQSessionKernel> session) {
    try {
        this->config->sessionsLock.writeLock().lock();
        try {
            this->config->activeSessions.remove(session);
            this->config->connectionAudit.removeDispatcher(session.get());
            this->config->sessionsLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->sessionsLock.writeLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addProducer(Pointer<ActiveMQProducerKernel> producer) {

    try {
        synchronized(&this->config->activeProducers) {
            this->config->activeProducers.put(producer->getProducerInfo()->getProducerId(), producer);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeProducer(const decaf::lang::Pointer<ProducerId>& producerId) {

    try {
        synchronized(&this->config->activeProducers) {
            this->config->activeProducers.remove(producerId);
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getClientID() const {

    if (this->isClosed()) {
        return "";
    }

    return this->config->connectionInfo->getClientId();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setClientID(const std::string& clientID) {

    if (this->closed.get()) {
        throw cms::IllegalStateException("Connection is already closed", NULL);
    }

    if (this->config->clientIDSet) {
        throw cms::IllegalStateException("Client ID is already set", NULL);
    }

    if (this->config->isConnectionInfoSentToBroker) {
        throw cms::IllegalStateException("Cannot set client Id on a Connection already in use.", NULL);
    }

    if (clientID.empty()) {
        throw cms::InvalidClientIdException("Client ID cannot be an empty string", NULL);
    }

    this->config->connectionInfo->setClientId(clientID);
    this->config->userSpecifiedClientID = true;

    try {
        ensureConnectionInfoSent();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setDefaultClientId(const std::string& clientId) {
    this->setClientID(clientId);
    this->config->userSpecifiedClientID = true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::close() {

    try {

        if (this->isClosed()) {
            return;
        }

        Exception ex;
        bool hasException = false;

        // If we are running lets stop first.
        if (!this->transportFailed.get()) {
            try {
                this->stop();
            } catch (cms::CMSException& error) {
                if (!hasException) {
                    ex = ActiveMQException(error.clone());
                    hasException = true;
                }
            }
        }

        // Indicates we are on the way out to suppress any exceptions getting
        // passed on from the transport as it goes down.
        this->closing.set(true);

        if (this->config->scheduler != NULL) {
            try {
                this->config->scheduler->stop();
            } catch (Exception& error) {
                if (!hasException) {
                    ex = error;
                    ex.setMark(__FILE__, __LINE__);
                    hasException = true;
                }
            }
        }

        long long lastDeliveredSequenceId = 0;

        // Get the complete list of active sessions.
        try {
            this->config->sessionsLock.writeLock().lock();

            // We need to use a copy since we aren't able to use CopyOnWriteArrayList
            ArrayList<Pointer<ActiveMQSessionKernel> > sessions(this->config->activeSessions);
            std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > iter(sessions.iterator());

            // Dispose of all the Session resources we know are still open.
            while (iter->hasNext()) {
                Pointer<ActiveMQSessionKernel> session = iter->next();
                try {
                    session->dispose();
                    lastDeliveredSequenceId = Math::max(lastDeliveredSequenceId, session->getLastDeliveredSequenceId());
                } catch (cms::CMSException& ex) {
                }
            }

            this->config->activeSessions.clear();
            this->config->sessionsLock.writeLock().unlock();
        } catch (Exception& error) {
            this->config->sessionsLock.writeLock().unlock();
            if (!hasException) {
                ex = error;
                ex.setMark(__FILE__, __LINE__);
                hasException = true;
            }
        }

        // As TemporaryQueue and TemporaryTopic instances are bound to a connection
        // we should just delete them after the connection is closed to free up memory
        ArrayList<Pointer<ActiveMQTempDestination> > tempDests(this->config->activeTempDestinations.values());
        Pointer<Iterator<Pointer<ActiveMQTempDestination> > > iterator(tempDests.iterator());

        try {
            while (iterator->hasNext()) {
                Pointer<ActiveMQTempDestination> dest = iterator->next();
                dest->close();
            }
        } catch (cms::CMSException& error) {
            if (!hasException) {
                ex = ActiveMQException(error.clone());
                hasException = true;
            }
        }

        try {
            if (this->config->executor != NULL) {
                this->config->executor->shutdown();
            }
        } catch (Exception& error) {
            if (!hasException) {
                ex = error;
                ex.setMark(__FILE__, __LINE__);
                hasException = true;
            }
        }

        // Ensure that interruption processing completes in case any consumers were
        // still in the process when we closed them.
        try {
            Pointer<CountDownLatch> latch = this->config->transportInterruptionProcessingComplete;
            if (latch != NULL) {
                int count = latch->getCount();
                for (; count > 0; count--) {
                    latch->countDown();
                }
            }
        } catch (Exception& error) {
            if (!hasException) {
                ex = error;
                ex.setMark(__FILE__, __LINE__);
                hasException = true;
            }
        }

        // Now inform the Broker we are shutting down.
        try {
            this->disconnect(lastDeliveredSequenceId);
        } catch (Exception& error) {
            if (!hasException) {
                ex = error;
                ex.setMark(__FILE__, __LINE__);
                hasException = true;
            }
        }

        // Once current deliveries are done this stops the delivery
        // of any new messages.
        this->started.set(false);
        this->closed.set(true);

        if (hasException) {
            throw ex;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::cleanup() {

    try {

        this->config->sessionsLock.writeLock().lock();
        try {
            // We need to use a copy since we aren't able to use CopyOnWriteArrayList
            ArrayList<Pointer<ActiveMQSessionKernel> > sessions(this->config->activeSessions);
            std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > iter(sessions.iterator());

            // Dispose of all the Session resources we know are still open.
            while (iter->hasNext()) {
                Pointer<ActiveMQSessionKernel> session = iter->next();
                try {
                    session->dispose();
                } catch (cms::CMSException& ex) {
                    /* Absorb */
                }
            }
            this->config->activeSessions.clear();
            this->config->sessionsLock.writeLock().unlock();
        } catch (Exception& ex) {
            this->config->sessionsLock.writeLock().unlock();
            throw;
        }

        if (this->config->isConnectionInfoSentToBroker) {
            if (!transportFailed.get() && !closing.get()) {
                this->syncRequest(this->config->connectionInfo->createRemoveCommand());
            }
            this->config->isConnectionInfoSentToBroker = false;
        }

        if (this->config->userSpecifiedClientID) {
            this->config->connectionInfo->setClientId("");
            this->config->userSpecifiedClientID = false;
        }

        this->config->clientIDSet = false;
        this->started.set(false);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::start() {

    try {

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        try {
            // This starts or restarts the delivery of all incoming messages
            // messages delivered while this connection is stopped are dropped
            // and not acknowledged.
            if (this->started.compareAndSet(false, true)) {
                this->config->sessionsLock.readLock().lock();

                // Start all the sessions.
                std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > iter(this->config->activeSessions.iterator());
                while (iter->hasNext()) {
                    iter->next()->start();
                }

                this->config->sessionsLock.readLock().unlock();
            }
        } catch (Exception& ex) {
            this->config->sessionsLock.readLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::stop() {

    try {

        checkClosedOrFailed();

        try {
            // Once current deliveries are done this stops the delivery of any
            // new messages.
            if (this->started.compareAndSet(true, false)) {
                this->config->sessionsLock.readLock().lock();
                std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > iter(this->config->activeSessions.iterator());

                while (iter->hasNext()) {
                    iter->next()->stop();
                }
                this->config->sessionsLock.readLock().unlock();
            }
        } catch (Exception& ex) {
            this->config->sessionsLock.readLock().unlock();
            throw;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disconnect(long long lastDeliveredSequenceId) {

    try {

        // Clear the listener, we don't care about async errors at this point.
        this->config->transport->setTransportListener(NULL);

        // Allow the Support class to shutdown its resources, including the Transport.
        bool hasException = false;
        exceptions::ActiveMQException e;

        if (this->config->isConnectionInfoSentToBroker) {

            try {
                // Remove our ConnectionId from the Broker
                Pointer<RemoveInfo> command(this->config->connectionInfo->createRemoveCommand());
                command->setLastDeliveredSequenceId(lastDeliveredSequenceId);
                this->syncRequest(command, this->config->closeTimeout);
            } catch (exceptions::ActiveMQException& ex) {
                if (!hasException) {
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__);
                    e = ex;
                }
            }

            try {
                // Send the disconnect command to the broker.
                Pointer<ShutdownInfo> shutdown(new ShutdownInfo());
                oneway(shutdown);
            } catch (exceptions::ActiveMQException& ex) {
                if (!hasException) {
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__);
                    e = ex;
                }
            }
        }

        if (this->config->transport != NULL) {

            try {
                this->config->transport->close();
            } catch (exceptions::ActiveMQException& ex) {
                if (!hasException) {
                    hasException = true;
                    ex.setMark(__FILE__, __LINE__);
                    e = ex;
                }
            }
        }

        // If we encountered an exception - throw the first one we encountered.
        // This will preserve the stack trace for logging purposes.
        if (hasException) {
            throw e;
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::sendPullRequest(const ConsumerInfo* consumer, long long timeout) {

    try {

        if (consumer->getPrefetchSize() == 0) {

            Pointer<MessagePull> messagePull(new MessagePull());
            messagePull->setConsumerId(consumer->getConsumerId());
            messagePull->setDestination(consumer->getDestination());
            messagePull->setTimeout(timeout);

            this->oneway(messagePull);
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination(const ActiveMQDestination* destination) {

    try {

        if (destination == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Destination passed was NULL");
        }

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        Pointer<DestinationInfo> command(new DestinationInfo());

        command->setConnectionId(this->config->connectionInfo->getConnectionId());
        command->setOperationType(ActiveMQConstants::DESTINATION_REMOVE_OPERATION);
        command->setDestination(Pointer<ActiveMQDestination>(destination->cloneDataStructure()));

        // Send the message to the broker.
        syncRequest(command);
    }
    AMQ_CATCH_RETHROW(NullPointerException)
    AMQ_CATCH_RETHROW(decaf::lang::exceptions::IllegalStateException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination(const cms::Destination* destination) {

    try {

        if (destination == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Destination passed was NULL");
        }

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        const ActiveMQDestination* amqDestination = dynamic_cast<const ActiveMQDestination*>(destination);

        this->destroyDestination(amqDestination);
    }
    AMQ_CATCH_RETHROW(NullPointerException)
    AMQ_CATCH_RETHROW(decaf::lang::exceptions::IllegalStateException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onCommand(const Pointer<Command> command) {

    try {

        if (command->isMessageDispatch()) {

            Pointer<MessageDispatch> dispatch = command.dynamicCast<MessageDispatch>();

            // Check first to see if we are recovering.
            waitForTransportInterruptionProcessingToComplete();

            // Look up the dispatcher.
            Dispatcher* dispatcher = NULL;
            synchronized(&this->config->dispatchers) {

                dispatcher = this->config->dispatchers.get(dispatch->getConsumerId());

                // If we have no registered dispatcher, the consumer was probably
                // just closed.
                if (dispatcher != NULL) {

                    Pointer<commands::Message> message = dispatch->getMessage();

                    // Message == NULL to signal the end of a Queue Browse.
                    if (message != NULL) {
                        message->setReadOnlyBody(true);
                        message->setReadOnlyProperties(true);
                        message->setRedeliveryCounter(dispatch->getRedeliveryCounter());
                        message->setConnection(this);
                    }

                    dispatcher->dispatch(dispatch);
                }
            }

        } else if (command->isProducerAck()) {

            ProducerAck* producerAck = dynamic_cast<ProducerAck*>(command.get());

            // Get the consumer info object for this consumer.
            Pointer<ActiveMQProducerKernel> producer;
            synchronized(&this->config->activeProducers) {
                producer = this->config->activeProducers.get(producerAck->getProducerId());
                if (producer != NULL) {
                    producer->onProducerAck(*producerAck);
                }
            }

        } else if (command->isWireFormatInfo()) {
            this->config->brokerWireFormatInfo = command.dynamicCast<WireFormatInfo>();
        } else if (command->isBrokerInfo()) {
            this->config->brokerInfo = command.dynamicCast<BrokerInfo>();
            this->config->brokerInfoReceived->countDown();
        } else if (command->isConnectionControl()) {
            this->onConnectionControl(command);
        } else if (command->isControlCommand()) {
            this->onControlCommand(command);
        } else if (command->isConnectionError()) {

            Pointer<ConnectionError> connectionError = command.dynamicCast<ConnectionError>();
            this->config->executor->execute(new ConnectionErrorRunnable(this, connectionError));

        } else if (command->isConsumerControl()) {
            this->onConsumerControl(command);
        }

        synchronized(&this->config->transportListeners) {
            Pointer<Iterator<TransportListener*> > iter(this->config->transportListeners.iterator());
            while (iter->hasNext()) {
                try {
                    iter->next()->onCommand(command);
                } catch (...) {
                }
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onControlCommand(Pointer<commands::Command> command AMQCPP_UNUSED) {
    // Don't need to do anything yet as close and shutdown are applicable yet.
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onConnectionControl(Pointer<commands::Command> command AMQCPP_UNUSED) {
    // Don't need to do anything yet as we don't support optimizeAcknowledge.
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onConsumerControl(Pointer<commands::Command> command) {

    Pointer<ConsumerControl> consumerControl = command.dynamicCast<ConsumerControl>();

    this->config->sessionsLock.readLock().lock();
    try {
        // Get the complete list of active sessions.
        std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > iter(this->config->activeSessions.iterator());

        while (iter->hasNext()) {
            Pointer<ActiveMQSessionKernel> session = iter->next();
            if (consumerControl->isClose()) {
                session->close(consumerControl->getConsumerId());
            } else {
                session->setPrefetchSize(consumerControl->getConsumerId(), consumerControl->getPrefetch());
            }
        }
        this->config->sessionsLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->sessionsLock.readLock().unlock();
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onException(const decaf::lang::Exception& ex) {

    try {

        // Sync with the config destructor in case a client attempt to
        synchronized(&this->config->onExceptionLock) {
            onAsyncException(ex);

            // We're disconnected - the asynchronous error is expected.
            if (!this->isClosed() || !this->closing.get()) {
                this->config->executor->execute(new OnExceptionRunnable(this, config, ex.clone()));
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onAsyncException(const decaf::lang::Exception& ex) {

    if (!this->isClosed() || !this->closing.get()) {

        if (this->config->exceptionListener != NULL) {
            this->config->executor->execute(new OnAsyncExceptionRunnable(this, ex));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onClientInternalException(const decaf::lang::Exception& ex) {

    if (!closed.get() && !closing.get()) {

        if (this->config->exceptionListener != NULL) {
            this->config->executor->execute(new OnAsyncExceptionRunnable(this, ex));
        }

        // TODO Turn this into an invocation on a special ClientInternalExceptionListener
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::transportInterrupted() {

    int consumers = this->config->watchTopicAdvisories ? (int) this->config->dispatchers.size() - 1 : (int) this->config->dispatchers.size();

    this->config->transportInterruptionProcessingComplete.reset(new CountDownLatch(consumers));

    this->config->sessionsLock.readLock().lock();
    try {
        std::auto_ptr<Iterator<Pointer<ActiveMQSessionKernel> > > sessions(this->config->activeSessions.iterator());
        while (sessions->hasNext()) {
            sessions->next()->clearMessagesInProgress();
        }
        this->config->sessionsLock.readLock().unlock();
    } catch (Exception& ex) {
        this->config->sessionsLock.readLock().unlock();
        throw;
    }

    synchronized(&this->config->transportListeners) {
        Pointer<Iterator<TransportListener*> > listeners(this->config->transportListeners.iterator());
        while (listeners->hasNext()) {
            try {
                listeners->next()->transportInterrupted();
            } catch (...) {
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::transportResumed() {

    synchronized(&this->config->transportListeners) {
        Pointer<Iterator<TransportListener*> > iter(this->config->transportListeners.iterator());
        while (iter->hasNext()) {
            try {
                iter->next()->transportResumed();
            } catch (...) {
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::oneway(Pointer<Command> command) {

    try {
        checkClosedOrFailed();
        this->config->transport->oneway(command);
    }
    AMQ_CATCH_EXCEPTION_CONVERT(IOException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::exceptions::UnsupportedOperationException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> ActiveMQConnection::syncRequest(Pointer<Command> command, unsigned int timeout) {

    try {

        checkClosedOrFailed();

        Pointer<Response> response;

        if (timeout == 0) {
            response = this->config->transport->request(command);
        } else {
            response = this->config->transport->request(command, timeout);
        }

        commands::ExceptionResponse* exceptionResponse = dynamic_cast<ExceptionResponse*>(response.get());

        if (exceptionResponse != NULL) {
            throw exceptionResponse->getException()->createExceptionObject();
        }

        return response;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(IOException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::exceptions::UnsupportedOperationException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::asyncRequest(Pointer<Command> command, cms::AsyncCallback* onComplete) {

    try {

        if (onComplete == NULL) {
            this->syncRequest(command);
            return;
        }

        checkClosedOrFailed();

        Pointer<ResponseCallback> callback(new AsyncResponseCallback(this->config, onComplete));
        this->config->transport->asyncRequest(command, callback);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(IOException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(decaf::lang::exceptions::UnsupportedOperationException, ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::checkClosed() const {
    if (this->isClosed()) {
        throw ActiveMQException(__FILE__, __LINE__, "ActiveMQConnection::enforceConnected - Connection has already been closed!");
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::checkClosedOrFailed() const {

    checkClosed();
    if (this->transportFailed.get() == true) {
        throw ConnectionFailedException(*this->config->firstFailureError);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::ensureConnectionInfoSent() {

    try {

        // Can we skip sending the ConnectionInfo packet, cheap test
        if (this->config->isConnectionInfoSentToBroker || closed.get()) {
            return;
        }

        synchronized(&( this->config->ensureConnectionInfoSentMutex)) {

            // Can we skip sending the ConnectionInfo packet??
            if (this->config->isConnectionInfoSentToBroker || closed.get()) {
                return;
            }

            // check for a user specified Id
            if (!this->config->userSpecifiedClientID) {
                this->config->connectionInfo->setClientId(this->config->clientIdGenerator->generateId());
            }

            // Now we ping the broker and see if we get an ack / nack
            syncRequest(this->config->connectionInfo);

            this->config->isConnectionInfoSentToBroker = true;

            Pointer<SessionId> sessionId(new SessionId(this->config->connectionInfo->getConnectionId().get(), -1));
            Pointer<ConsumerId> consumerId(new ConsumerId(*sessionId, this->config->consumerIdGenerator.getNextSequenceId()));
            if (this->config->watchTopicAdvisories) {
                this->config->advisoryConsumer.reset(new AdvisoryConsumer(this, consumerId));
            }
        }
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::fire(const ActiveMQException& ex) {
    if (this->config->exceptionListener != NULL) {
        try {
            this->config->exceptionListener->onException(ex.convertToCMSException());
        } catch (...) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionInfo& ActiveMQConnection::getConnectionInfo() const {
    checkClosed();
    return *this->config->connectionInfo;
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionId& ActiveMQConnection::getConnectionId() const {
    checkClosed();
    return *(this->config->connectionInfo->getConnectionId());
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addTransportListener(TransportListener* transportListener) {

    if (transportListener == NULL) {
        return;
    }

    // Add this listener from the set of active TransportListeners
    synchronized(&this->config->transportListeners) {
        this->config->transportListeners.add(transportListener);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeTransportListener(TransportListener* transportListener) {

    if (transportListener == NULL) {
        return;
    }

    // Remove this listener from the set of active TransportListeners
    synchronized(&this->config->transportListeners) {
        this->config->transportListeners.remove(transportListener);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::waitForTransportInterruptionProcessingToComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;
    if (cdl != NULL) {

        while (!closed.get() && !transportFailed.get() && cdl->getCount() > 0) {
            cdl->await(10, TimeUnit::SECONDS);
        }

        signalInterruptionProcessingComplete();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setTransportInterruptionProcessingComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;
    if (cdl != NULL) {
        cdl->countDown();

        try {
            signalInterruptionProcessingComplete();
        } catch (InterruptedException& ignored) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::signalInterruptionProcessingComplete() {

    Pointer<CountDownLatch> cdl = this->config->transportInterruptionProcessingComplete;

    if (cdl->getCount() == 0) {

        this->config->transportInterruptionProcessingComplete.reset(NULL);
        FailoverTransport* failoverTransport = dynamic_cast<FailoverTransport*>(this->config->transport->narrow(typeid(FailoverTransport)));

        if (failoverTransport != NULL) {
            failoverTransport->setConnectionInterruptProcessingComplete(this->config->connectionInfo->getConnectionId());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUsername(const std::string& username) {
    this->config->connectionInfo->setUserName(username);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getUsername() const {
    return this->config->connectionInfo->getUserName();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setPassword(const std::string& password) {
    this->config->connectionInfo->setPassword(password);
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getPassword() const {
    return this->config->connectionInfo->getPassword();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setBrokerURL(const std::string& brokerURL) {
    this->config->brokerURL = brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnection::getBrokerURL() const {
    return this->config->brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setExceptionListener(cms::ExceptionListener* listener) {
    this->config->exceptionListener = listener;
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQConnection::getExceptionListener() const {
    return this->config->exceptionListener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setMessageTransformer(cms::MessageTransformer* transformer) {
    this->config->transformer = transformer;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageTransformer* ActiveMQConnection::getMessageTransformer() const {
    return this->config->transformer;
}

////////////////////////////////////////////////////////////////////////////////
cms::DestinationSource* ActiveMQConnection::getDestinationSource() {
    return new ActiveMQDestinationSource(this);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setPrefetchPolicy(PrefetchPolicy* policy) {
    this->config->defaultPrefetchPolicy.reset(policy);
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy* ActiveMQConnection::getPrefetchPolicy() const {
    return this->config->defaultPrefetchPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setRedeliveryPolicy(RedeliveryPolicy* policy) {
    this->config->defaultRedeliveryPolicy.reset(policy);
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConnection::getRedeliveryPolicy() const {
    return this->config->defaultRedeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isDispatchAsync() const {
    return this->config->dispatchAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setDispatchAsync(bool value) {
    this->config->dispatchAsync = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isAlwaysSyncSend() const {
    return this->config->alwaysSyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setAlwaysSyncSend(bool value) {
    this->config->alwaysSyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isUseAsyncSend() const {
    return this->config->useAsyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUseAsyncSend(bool value) {
    this->config->useAsyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isUseCompression() const {
    return this->config->useCompression;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUseCompression(bool value) {
    this->config->useCompression = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnection::getCompressionLevel() const {
    return this->config->compressionLevel;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setCompressionLevel(int value) {

    if (value < 0) {
        this->config->compressionLevel = -1;
    }

    this->config->compressionLevel = Math::min(value, 9);
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getSendTimeout() const {
    return this->config->sendTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setSendTimeout(unsigned int timeout) {
    this->config->sendTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getCloseTimeout() const {
    return this->config->closeTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setCloseTimeout(unsigned int timeout) {
    this->config->closeTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnection::getProducerWindowSize() const {
    return this->config->producerWindowSize;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setProducerWindowSize(unsigned int windowSize) {
    this->config->producerWindowSize = windowSize;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getNextTempDestinationId() {
    return this->config->tempDestinationIds.getNextSequenceId();
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getNextLocalTransactionId() {
    return this->config->localTransactionIds.getNextSequenceId();
}

////////////////////////////////////////////////////////////////////////////////
transport::Transport& ActiveMQConnection::getTransport() const {
    return *(this->config->transport);
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Scheduler> ActiveMQConnection::getScheduler() const {
    return this->config->scheduler;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isMessagePrioritySupported() const {
    return this->config->messagePrioritySupported;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setMessagePrioritySupported(bool value) {
    this->config->messagePrioritySupported = value;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setFirstFailureError(decaf::lang::Exception* error) {

    this->transportFailed.set(true);

    if (this->config->firstFailureError == NULL) {
        this->config->firstFailureError.reset(error);
    } else {
        delete error;
    }
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception* ActiveMQConnection::getFirstFailureError() const {
    return this->config->firstFailureError.get();
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getResourceManagerId() const {
    try {
        this->config->waitForBrokerInfo();

        if (this->config->brokerInfo == NULL) {
            throw CMSException("Connection failed before Broker info was received.");
        }

        return this->config->brokerInfo->getBrokerId()->getValue();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
const decaf::util::Properties& ActiveMQConnection::getProperties() const {
    return *(this->config->properties);
}

////////////////////////////////////////////////////////////////////////////////
ExecutorService* ActiveMQConnection::getExecutor() const {
    return this->config->executor.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isWatchTopicAdvisories() const {
    return this->config->watchTopicAdvisories;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setWatchTopicAdvisories(bool value) {
    this->config->watchTopicAdvisories = value;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnection::getAuditDepth() const {
    return this->config->auditDepth;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setAuditDepth(int auditDepth) {
    this->config->auditDepth = auditDepth;
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConnection::getAuditMaximumProducerNumber() const {
    return this->config->auditMaximumProducerNumber;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setAuditMaximumProducerNumber(int auditMaximumProducerNumber) {
    this->config->auditMaximumProducerNumber = auditMaximumProducerNumber;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isCheckForDuplicates() const {
    return this->config->checkForDuplicates;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setCheckForDuplicates(bool checkForDuplicates) {
    this->config->checkForDuplicates = checkForDuplicates;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isSendAcksAsync() const {
    return this->config->sendAcksAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setSendAcksAsync(bool sendAcksAsync) {
    this->config->sendAcksAsync = sendAcksAsync;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isTransactedIndividualAck() const {
    return this->config->transactedIndividualAck;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setTransactedIndividualAck(bool transactedIndividualAck) {
    this->config->transactedIndividualAck = transactedIndividualAck;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isNonBlockingRedelivery() const {
    return this->config->nonBlockingRedelivery;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setNonBlockingRedelivery(bool nonBlockingRedelivery) {
    this->config->nonBlockingRedelivery = nonBlockingRedelivery;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isOptimizeAcknowledge() const {
    return this->config->optimizeAcknowledge;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setOptimizeAcknowledge(bool optimizeAcknowledge) {
    this->config->optimizeAcknowledge = optimizeAcknowledge;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getOptimizeAcknowledgeTimeOut() const {
    return this->config->optimizeAcknowledgeTimeOut;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setOptimizeAcknowledgeTimeOut(long long optimizeAcknowledgeTimeOut) {
    this->config->optimizeAcknowledgeTimeOut = optimizeAcknowledgeTimeOut;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getOptimizedAckScheduledAckInterval() const {
    return this->config->optimizedAckScheduledAckInterval;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setOptimizedAckScheduledAckInterval(long long optimizedAckScheduledAckInterval) {
    this->config->optimizedAckScheduledAckInterval = optimizedAckScheduledAckInterval;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConnection::getConsumerFailoverRedeliveryWaitPeriod() const {
    return this->config->consumerFailoverRedeliveryWaitPeriod;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setConsumerFailoverRedeliveryWaitPeriod(long long value) {
    this->config->consumerFailoverRedeliveryWaitPeriod = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isUseRetroactiveConsumer() const {
    return this->config->useRetroactiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setUseRetroactiveConsumer(bool useRetroactiveConsumer) {
    this->config->useRetroactiveConsumer = useRetroactiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isExclusiveConsumer() const {
    return this->config->exclusiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::setExclusiveConsumer(bool exclusiveConsumer) {
    this->config->exclusiveConsumer = exclusiveConsumer;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addTempDestination(Pointer<ActiveMQTempDestination> destination) {
    this->config->activeTempDestinations.put(destination, destination);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeTempDestination(Pointer<ActiveMQTempDestination> destination) {
    this->config->activeTempDestinations.remove(destination);
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::deleteTempDestination(Pointer<ActiveMQTempDestination> destination) {

    try {

        if (destination == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Destination passed was NULL");
        }

        checkClosedOrFailed();
        ensureConnectionInfoSent();

        this->config->sessionsLock.readLock().lock();
        try {
            Pointer<Iterator<Pointer<ActiveMQSessionKernel> > > iterator(this->config->activeSessions.iterator());
            while (iterator->hasNext()) {
                Pointer<ActiveMQSessionKernel> session = iterator->next();
                if (session->isInUse(destination)) {
                    this->config->sessionsLock.readLock().unlock();
                    throw ActiveMQException(__FILE__, __LINE__, "A consumer is consuming from the temporary destination");
                }
            }
            this->config->sessionsLock.readLock().unlock();
        } catch (Exception& ex) {
            this->config->sessionsLock.readLock().unlock();
            throw;
        }

        this->config->activeTempDestinations.remove(destination);

        Pointer<DestinationInfo> command(new DestinationInfo());

        command->setConnectionId(this->config->connectionInfo->getConnectionId());
        command->setOperationType(ActiveMQConstants::DESTINATION_REMOVE_OPERATION);
        command->setDestination(Pointer<ActiveMQDestination>(destination->cloneDataStructure()));

        // Send the message to the broker.
        syncRequest(command);
    }
    AMQ_CATCH_RETHROW(NullPointerException)
    AMQ_CATCH_RETHROW(decaf::lang::exceptions::IllegalStateException)
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::cleanUpTempDestinations() {

    if (this->config->activeTempDestinations.isEmpty()) {
        return;
    }

    ArrayList< Pointer<ActiveMQTempDestination> > tempDests(this->config->activeTempDestinations.values());
    Pointer<Iterator<Pointer<ActiveMQTempDestination> > > iterator(tempDests.iterator());
    while (iterator->hasNext()) {
        Pointer<ActiveMQTempDestination> dest = iterator->next();

        try {
            // Only delete this temporary destination if it was created from this connection, since the
            // advisory consumer tracks all temporary destinations there can be others in our mapping that
            // this connection did not create.
            std::string thisConnectionId =
                    this->config->connectionInfo->getConnectionId() != NULL ? this->config->connectionInfo->getConnectionId()->toString() : "";
            if (dest->getConnectionId() == thisConnectionId) {
                this->deleteTempDestination(dest);
            }
        } catch (Exception& ex) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isDeleted(Pointer<ActiveMQTempDestination> destination) const {

    if (this->config->advisoryConsumer == NULL) {
        return false;
    }

    return !this->config->activeTempDestinations.containsKey(destination);
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnection::isDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message) {

    if (this->config->checkForDuplicates) {
        return this->config->connectionAudit.isDuplicate(dispatcher, message);
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::rollbackDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message) {
    this->config->connectionAudit.rollbackDuplicate(dispatcher, message);
}
