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

#include "FailoverTransport.h"

#include <activemq/commands/ConnectionControl.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/threads/DedicatedTaskRunner.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <decaf/util/Random.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::failover;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace failover {

    class FailoverTransportImpl {
    private:

        FailoverTransportImpl(const FailoverTransportImpl&);
        FailoverTransportImpl& operator= (const FailoverTransportImpl&);

    public:

        bool closed;
        bool connected;
        bool started;

        long long timeout;
        long long initialReconnectDelay;
        long long maxReconnectDelay;
        long long backOffMultiplier;
        bool useExponentialBackOff;
        bool initialized;
        int maxReconnectAttempts;
        int startupMaxReconnectAttempts;
        int connectFailures;
        long long reconnectDelay;
        bool trackMessages;
        bool trackTransactionProducers;
        int maxCacheSize;
        bool connectionInterruptProcessingComplete;
        bool firstConnection;
        bool updateURIsSupported;
        bool reconnectSupported;
        bool rebalanceUpdateURIs;
        bool priorityBackup;

        mutable decaf::util::concurrent::Mutex reconnectMutex;
        mutable decaf::util::concurrent::Mutex sleepMutex;
        mutable decaf::util::concurrent::Mutex listenerMutex;

        decaf::util::StlMap<int, Pointer<Command> > requestMap;

        Pointer<URIPool> uris;
        Pointer<URIPool> priorityUris;
        decaf::util::LinkedList<URI> updated;
        Pointer<URI> connectedTransportURI;
        Pointer<Transport> connectedTransport;
        Pointer<Exception> connectionFailure;
        Pointer<BackupTransportPool> backups;
        Pointer<CloseTransportsTask> closeTask;
        Pointer<CompositeTaskRunner> taskRunner;
        Pointer<TransportListener> disposedListener;
        Pointer<TransportListener> myTransportListener;

        FailoverTransportImpl(FailoverTransport* parent) :
            closed(false),
            connected(false),
            started(false),
            timeout(-1),
            initialReconnectDelay(10),
            maxReconnectDelay(1000*30),
            backOffMultiplier(2),
            useExponentialBackOff(true),
            initialized(false),
            maxReconnectAttempts(0),
            startupMaxReconnectAttempts(0),
            connectFailures(0),
            reconnectDelay(10),
            trackMessages(false),
            trackTransactionProducers(true),
            maxCacheSize(128*1024),
            connectionInterruptProcessingComplete(false),
            firstConnection(true),
            updateURIsSupported(true),
            reconnectSupported(true),
            rebalanceUpdateURIs(true),
            priorityBackup(false),
            reconnectMutex(),
            sleepMutex(),
            listenerMutex(),
            requestMap(),
            uris(new URIPool()),
            priorityUris(new URIPool()),
            updated(),
            connectedTransportURI(),
            connectedTransport(),
            connectionFailure(),
            backups(),
            closeTask(new CloseTransportsTask()),
            taskRunner(new CompositeTaskRunner()),
            disposedListener(),
            myTransportListener(new FailoverTransportListener(parent)) {

            this->backups.reset(new BackupTransportPool(taskRunner, closeTask, uris));

            this->taskRunner->addTask(parent);
            this->taskRunner->addTask(this->closeTask.get());
        }

        bool isPriority(const decaf::net::URI& uri) {
            return priorityUris->contains(uri) || uris->isPriority(uri);
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::FailoverTransport() : stateTracker(), impl(NULL), transportListener(NULL) {
    this->impl = new FailoverTransportImpl(this);
    this->stateTracker.setTrackTransactions(true);
}

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::~FailoverTransport() {
    try {
        close();
    }
    AMQ_CATCHALL_NOTHROW()

    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::add(const std::string& uri) {

    try {
        this->impl->uris->addURI(URI(uri));
        reconnect(false);
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::addURI(bool rebalance, const List<URI>& uris) {

    std::auto_ptr<Iterator<URI> > iter(uris.iterator());

    while (iter->hasNext()) {
        this->impl->uris->addURI(iter->next());
    }

    reconnect(rebalance);
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::removeURI(bool rebalance, const List<URI>& uris) {

    std::auto_ptr<Iterator<URI> > iter(uris.iterator());

    synchronized( &this->impl->reconnectMutex ) {

        // We need to lock so that the reconnect doesn't get kicked off until
        // we have a chance to remove the URIs in case one of them was the one
        // we had a connection to and it gets reinserted into the URI pool.

        reconnect(rebalance);

        while (iter->hasNext()) {
            this->impl->uris->removeURI(iter->next());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect(const decaf::net::URI& uri) {

    try {
        this->impl->uris->addURI(uri);
        reconnect(true);
    }
    AMQ_CATCH_RETHROW( IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException)
    AMQ_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTransportListener(TransportListener* listener) {
    synchronized( &this->impl->listenerMutex ) {
        this->transportListener = listener;
        this->impl->listenerMutex.notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
TransportListener* FailoverTransport::getTransportListener() const {
    synchronized( &this->impl->listenerMutex ) {
        return this->transportListener;
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::string FailoverTransport::getRemoteAddress() const {
    synchronized( &this->impl->reconnectMutex ) {
        if (this->impl->connectedTransport != NULL) {
            return this->impl->connectedTransport->getRemoteAddress();
        }
    }
    return "";
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::oneway(const Pointer<Command> command) {

    Pointer<Exception> error;

    try {

        synchronized(&this->impl->reconnectMutex) {

            if (command != NULL && this->impl->connectedTransport == NULL) {

                if (command->isShutdownInfo()) {
                    // Skipping send of ShutdownInfo command when not connected.
                    return;
                }

                if (command->isRemoveInfo() || command->isMessageAck()) {
                    // Simulate response to RemoveInfo command or Ack as they will be stale.
                    stateTracker.track(command);

                    if (command->isResponseRequired()) {
                        Pointer<Response> response(new Response());
                        response->setCorrelationId(command->getCommandId());
                        this->impl->myTransportListener->onCommand(response);
                    }

                    return;
                }
            }

            // Keep trying until the message is sent.
            for (int i = 0; !this->impl->closed; i++) {
                try {

                    // Wait for transport to be connected.
                    Pointer<Transport> transport = this->impl->connectedTransport;
                    long long start = System::currentTimeMillis();
                    bool timedout = false;

                    while (transport == NULL && !this->impl->closed && this->impl->connectionFailure == NULL) {
                        long long end = System::currentTimeMillis();
                        if (this->impl->timeout > 0 && (end - start > this->impl->timeout)) {
                            timedout = true;
                            break;
                        }

                        this->impl->reconnectMutex.wait(100);
                        transport = this->impl->connectedTransport;
                    }

                    if (transport == NULL) {
                        // Previous loop may have exited due to us being disposed.
                        if (this->impl->closed) {
                            error.reset(new IOException(__FILE__, __LINE__, "Transport disposed."));
                        } else if (this->impl->connectionFailure != NULL) {
                            error = this->impl->connectionFailure;
                        } else if (timedout == true) {
                            error.reset(new IOException(__FILE__, __LINE__,
                                "Failover timeout of %d ms reached.", this->impl->timeout));
                        } else {
                            error.reset(new IOException(__FILE__, __LINE__, "Unexpected failure."));
                        }

                        break;
                    }

                    // If it was a request and it was not being tracked by the state
                    // tracker, then hold it in the requestMap so that we can replay
                    // it later.
                    Pointer<Tracked> tracked;
                    try {
                        tracked = stateTracker.track(command);
                        synchronized( &this->impl->requestMap ) {
                            if (tracked != NULL && tracked->isWaitingForResponse()) {
                                this->impl->requestMap.put(command->getCommandId(), tracked);
                            } else if (tracked == NULL && command->isResponseRequired()) {
                                this->impl->requestMap.put(command->getCommandId(), command);
                            }
                        }
                    } catch (Exception& ex) {
                        ex.setMark(__FILE__, __LINE__);
                        error.reset(ex.clone());
                        break;
                    }

                    // Send the message.
                    try {
                        transport->oneway(command);
                        stateTracker.trackBack(command);
                    } catch (IOException& e) {

                        e.setMark(__FILE__, __LINE__);

                        // If the command was not tracked.. we will retry in
                        // this method
                        if (tracked == NULL) {

                            // since we will retry in this method.. take it out of the
                            // request map so that it is not sent 2 times on recovery
                            if (command->isResponseRequired()) {
                                this->impl->requestMap.remove(command->getCommandId());
                            }

                            // Rethrow the exception so it will handled by
                            // the outer catch
                            throw e;
                        } else {
                            // Trigger the reconnect since we can't count on inactivity or
                            // other socket events to trip the failover condition.
                            handleTransportFailure(e);
                        }
                    }

                    return;
                } catch (IOException& e) {
                    e.setMark(__FILE__, __LINE__);
                    handleTransportFailure(e);
                }
            }
        }
    }
    AMQ_CATCH_NOTHROW( Exception)
    AMQ_CATCHALL_NOTHROW()

    if (!this->impl->closed) {
        if (error != NULL) {
            throw IOException(*error);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<FutureResponse> FailoverTransport::asyncRequest(const Pointer<Command> command AMQCPP_UNUSED,
                                                        const Pointer<ResponseCallback> responseCallback AMQCPP_UNUSED) {
    throw decaf::lang::exceptions::UnsupportedOperationException(__FILE__, __LINE__, "FailoverTransport::asyncRequest - Not Supported");
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request(const Pointer<Command> command AMQCPP_UNUSED) {
    throw decaf::lang::exceptions::UnsupportedOperationException(__FILE__, __LINE__, "FailoverTransport::request - Not Supported");
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FailoverTransport::request(const Pointer<Command> command AMQCPP_UNUSED, unsigned int timeout AMQCPP_UNUSED) {
    throw decaf::lang::exceptions::UnsupportedOperationException(__FILE__, __LINE__, "FailoverTransport::request - Not Supported");
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::start() {

    try {

        synchronized(&this->impl->reconnectMutex) {

            if (this->impl->started) {
                return;
            }

            this->impl->started = true;

            this->impl->taskRunner->start();

            stateTracker.setMaxCacheSize(this->getMaxCacheSize());
            stateTracker.setTrackMessages(this->isTrackMessages());
            stateTracker.setTrackTransactionProducers(this->isTrackTransactionProducers());

            if (this->impl->connectedTransport != NULL) {
                stateTracker.restore(this->impl->connectedTransport);
            } else {
                reconnect(false);
            }
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::stop() {

    try {
    }
    AMQ_CATCH_RETHROW( IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException)
    AMQ_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::close() {

    try {

        Pointer<Transport> transportToStop;

        synchronized( &this->impl->reconnectMutex ) {
            if (!this->impl->started) {
                return;
            }

            this->impl->started = false;
            this->impl->closed = true;
            this->impl->connected = false;

            this->impl->backups->setEnabled(false);
            this->impl->requestMap.clear();

            if (this->impl->connectedTransport != NULL) {
                transportToStop.swap(this->impl->connectedTransport);
            }

            this->impl->reconnectMutex.notifyAll();
        }

        synchronized( &this->impl->sleepMutex ) {
            this->impl->sleepMutex.notifyAll();
        }

        this->impl->taskRunner->shutdown(2000);

        if (transportToStop != NULL) {
            transportToStop->close();
        }
    }
    AMQ_CATCH_RETHROW( IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException)
    AMQ_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect(bool rebalance) {

    Pointer<Transport> transport;

    synchronized( &this->impl->reconnectMutex ) {
        if (this->impl->started) {

            if (rebalance) {

                transport.swap(this->impl->connectedTransport);

                if (transport != NULL) {

                    if (this->impl->disposedListener != NULL) {
                        transport->setTransportListener(this->impl->disposedListener.get());
                    }

                    // Hand off to the close task so it gets done in a different thread.
                    this->impl->closeTask->add(transport);

                    if (this->impl->connectedTransportURI != NULL) {
                        this->impl->uris->addURI(*this->impl->connectedTransportURI);
                        this->impl->connectedTransportURI.reset(NULL);
                    }
                }
            }

            this->impl->taskRunner->wakeup();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::restoreTransport(const Pointer<Transport> transport) {

    try {

        transport->start();

        //send information to the broker - informing it we are an ft client
        Pointer<ConnectionControl> cc(new ConnectionControl());
        cc->setFaultTolerant(true);
        transport->oneway(cc);

        stateTracker.restore(transport);

        decaf::util::StlMap<int, Pointer<Command> > commands;
        synchronized(&this->impl->requestMap) {
            commands.copy(this->impl->requestMap);
        }

        Pointer<Iterator<Pointer<Command> > > iter(commands.values().iterator());
        while (iter->hasNext()) {
            transport->oneway(iter->next());
        }
    }
    AMQ_CATCH_RETHROW( IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException)
    AMQ_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::handleTransportFailure(const decaf::lang::Exception& error AMQCPP_UNUSED) {

    Pointer<Transport> transport;
    synchronized(&this->impl->reconnectMutex) {
        this->impl->connectedTransport.swap(transport);
    }

    if (transport != NULL) {

        if (this->impl->disposedListener != NULL) {
            transport->setTransportListener(this->impl->disposedListener.get());
        }

        // Hand off to the close task so it gets done in a different thread.
        this->impl->closeTask->add(transport);

        synchronized(&this->impl->reconnectMutex) {

            this->impl->initialized = false;
            this->impl->uris->addURI(*this->impl->connectedTransportURI);
            this->impl->connectedTransportURI.reset(NULL);
            this->impl->connected = false;

            // Place the State Tracker into a reconnection state.
            this->stateTracker.transportInterrupted();

            // Notify before we attempt to reconnect so that the consumers have a chance
            // to cleanup their state.
            if (transportListener != NULL) {
                transportListener->transportInterrupted();
            }

            if (this->impl->started) {
                this->impl->taskRunner->wakeup();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::handleConnectionControl(const Pointer<Command> control) {

    try {

        Pointer<ConnectionControl> ctrlCommand = control.dynamicCast<ConnectionControl>();

        std::string reconnectStr = ctrlCommand->getReconnectTo();
        if (!reconnectStr.empty()) {

            std::remove(reconnectStr.begin(), reconnectStr.end(), ' ');

            if (reconnectStr.length() > 0) {
                try {
                    if (isReconnectSupported()) {
                        reconnect(URI(reconnectStr));
                    }
                } catch (Exception e) {
                }
            }
        }

        processNewTransports(ctrlCommand->isRebalanceConnection(), ctrlCommand->getConnectedBrokers());
    }
    AMQ_CATCH_RETHROW( Exception)
    AMQ_CATCHALL_THROW( Exception)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::processNewTransports(bool rebalance, std::string newTransports) {

    if (!newTransports.empty()) {

        std::remove(newTransports.begin(), newTransports.end(), ' ');

        if (newTransports.length() > 0 && isUpdateURIsSupported()) {

            LinkedList<URI> list;
            StringTokenizer tokenizer(newTransports, ",");

            while (tokenizer.hasMoreTokens()) {
                std::string str = tokenizer.nextToken();
                try {
                    URI uri(str);
                    list.add(uri);
                } catch (Exception& e) {
                }
            }

            if (!list.isEmpty()) {
                try {
                    updateURIs(rebalance, list);
                } catch (IOException& e) {
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::updateURIs(bool rebalance, const decaf::util::List<decaf::net::URI>& updatedURIs) {

    if (isUpdateURIsSupported()) {

        LinkedList<URI> copy(this->impl->updated);
        LinkedList<URI> add;

        if (!updatedURIs.isEmpty()) {

            StlSet<URI> set;

            for (int i = 0; i < updatedURIs.size(); i++) {
                set.add(updatedURIs.get(i));
            }

            Pointer<Iterator<URI> > setIter(set.iterator());
            while (setIter->hasNext()) {
                URI value = setIter->next();
                if (copy.remove(value) == false) {
                    add.add(value);
                }
            }

            synchronized( &this->impl->reconnectMutex ) {

                this->impl->updated.clear();
                Pointer<Iterator<URI> > listIter1(add.iterator());
                while (listIter1->hasNext()) {
                    this->impl->updated.add(listIter1->next());
                }

                Pointer<Iterator<URI> > listIter2(copy.iterator());
                while (listIter2->hasNext()) {
                    this->impl->uris->removeURI(listIter2->next());
                }

                this->addURI(rebalance, add);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isPending() const {
    bool result = false;

    synchronized(&this->impl->reconnectMutex) {
        if (this->impl->connectedTransport == NULL && !this->impl->closed && this->impl->started) {

            int reconnectAttempts = 0;
            if (this->impl->firstConnection) {
                if (this->impl->startupMaxReconnectAttempts != 0) {
                    reconnectAttempts = this->impl->startupMaxReconnectAttempts;
                }
            }
            if (reconnectAttempts == 0) {
                reconnectAttempts = this->impl->maxReconnectAttempts;
            }

            if (reconnectAttempts > 0 && this->impl->connectFailures >= reconnectAttempts) {
                result = false;
            } else {
                result = true;
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::iterate() {

    Pointer<Exception> failure;

    synchronized( &this->impl->reconnectMutex ) {

        if (this->impl->closed || this->impl->connectionFailure != NULL) {
            this->impl->reconnectMutex.notifyAll();
        }

        if (this->impl->connectedTransport != NULL || this->impl->closed || this->impl->connectionFailure != NULL) {
            return false;
        } else {

            LinkedList<URI> failures;
            Pointer<Transport> transport;
            URI uri;

            if (!this->impl->useExponentialBackOff) {
                this->impl->reconnectDelay = this->impl->initialReconnectDelay;
            }

            if (this->impl->backups->isEnabled()) {

                Pointer<BackupTransport> backupTransport = this->impl->backups->getBackup();

                if (backupTransport != NULL) {

                    transport = backupTransport->getTransport();
                    uri = backupTransport->getUri();
                    transport->setTransportListener(this->impl->myTransportListener.get());

                    try {

                        if (this->impl->started) {
                            restoreTransport(transport);
                        }

                    } catch (Exception& e) {

                        if (transport != NULL) {
                            if (this->impl->disposedListener != NULL) {
                                transport->setTransportListener(this->impl->disposedListener.get());
                            }

                            // Hand off to the close task so it gets done in a different thread
                            // this prevents a deadlock from occurring if the Transport happens
                            // to call back through our onException method or locks in some other
                            // way.
                            this->impl->closeTask->add(transport);
                            this->impl->taskRunner->wakeup();
                            transport.reset(NULL);
                        }

                        this->impl->uris->addURI(uri);
                    }
                }
            }

            while (transport == NULL && !this->impl->closed) {

                try {
                    uri = this->impl->uris->getURI();
                } catch (NoSuchElementException& ex) {
                    break;
                }

                try {

                    transport = createTransport(uri);
                    transport->setTransportListener(this->impl->myTransportListener.get());
                    transport->start();

                    if (this->impl->started) {
                        restoreTransport(transport);
                    }

                } catch (Exception& e) {
                    e.setMark(__FILE__, __LINE__);

                    if (transport != NULL) {
                        if (this->impl->disposedListener != NULL) {
                            transport->setTransportListener(this->impl->disposedListener.get());
                        }

                        try {
                            transport->stop();
                        } catch (...) {
                        }

                        // Hand off to the close task so it gets done in a different thread
                        // this prevents a deadlock from occurring if the Transport happens
                        // to call back through our onException method or locks in some other
                        // way.
                        this->impl->closeTask->add(transport);
                        this->impl->taskRunner->wakeup();
                        transport.reset(NULL);
                    }

                    failures.add(uri);
                    failure.reset(e.clone());
                }
            }

            // Return the failures to the pool, we will try again on the next iteration.
            this->impl->uris->addURIs(failures);

            if (transport != NULL) {
                this->impl->reconnectDelay = this->impl->initialReconnectDelay;
                this->impl->connectedTransportURI.reset(new URI(uri));
                this->impl->connectedTransport = transport;
                this->impl->reconnectMutex.notifyAll();
                this->impl->connectFailures = 0;
                this->impl->connected = true;

                // Make sure on initial startup, that the transportListener
                // has been initialized for this instance.
                synchronized(&this->impl->listenerMutex) {
                    if (transportListener == NULL) {
                        // if it isn't set after 2secs - it probably never will be
                        this->impl->listenerMutex.wait(2000);
                    }
                }

                if (transportListener != NULL) {
                    transportListener->transportResumed();
                }

                if (this->impl->firstConnection) {
                    this->impl->firstConnection = false;
                }

                return false;
            }
        }

        int reconnectAttempts = 0;
        if (this->impl->firstConnection) {
            if (this->impl->startupMaxReconnectAttempts != 0) {
                reconnectAttempts = this->impl->startupMaxReconnectAttempts;
            }
        }
        if (reconnectAttempts == 0) {
            reconnectAttempts = this->impl->maxReconnectAttempts;
        }

        if (reconnectAttempts > 0 && ++this->impl->connectFailures >= reconnectAttempts) {
            this->impl->connectionFailure = failure;

            // Make sure on initial startup, that the transportListener has been initialized
            // for this instance.
            synchronized(&this->impl->listenerMutex) {
                if (transportListener == NULL) {
                    this->impl->listenerMutex.wait(2000);
                }
            }

            if (transportListener != NULL) {

                Pointer<IOException> ioException;
                try {
                    ioException = this->impl->connectionFailure.dynamicCast<IOException>();
                }
                AMQ_CATCH_NOTHROW( ClassCastException)

                if (ioException != NULL) {
                    transportListener->onException(*this->impl->connectionFailure);
                } else {
                    transportListener->onException(IOException(*this->impl->connectionFailure));
                }
            }

            this->impl->reconnectMutex.notifyAll();
            return false;
        }
    }

    if (!this->impl->closed) {

        synchronized(&this->impl->sleepMutex) {
            this->impl->sleepMutex.wait((unsigned int) this->impl->reconnectDelay);
        }

        if (this->impl->useExponentialBackOff) {
            // Exponential increment of reconnect delay.
            this->impl->reconnectDelay *= this->impl->backOffMultiplier;
            if (this->impl->reconnectDelay > this->impl->maxReconnectDelay) {
                this->impl->reconnectDelay = this->impl->maxReconnectDelay;
            }
        }
    }

    return !this->impl->closed;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> FailoverTransport::createTransport(const URI& location) const {

    try {

        TransportFactory* factory = TransportRegistry::getInstance().findFactory(location.getScheme());

        if (factory == NULL) {
            throw new IOException(__FILE__, __LINE__, "Invalid URI specified, no valid Factory Found.");
        }

        Pointer<Transport> transport(factory->createComposite(location));

        return transport;
    }
    AMQ_CATCH_RETHROW( IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException)
    AMQ_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setConnectionInterruptProcessingComplete(const Pointer<commands::ConnectionId> connectionId) {

    synchronized(&this->impl->reconnectMutex) {
        stateTracker.connectionInterruptProcessingComplete(this, connectionId);
    }
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isConnected() const {
    return this->impl->connected;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isClosed() const {
    return this->impl->closed;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isInitialized() const {
    return this->impl->initialized;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setInitialized(bool value) {
    this->impl->initialized = value;
}

////////////////////////////////////////////////////////////////////////////////
Transport* FailoverTransport::narrow(const std::type_info& typeId) {

    if (typeid( *this ) == typeId) {
        return this;
    }

    if (this->impl->connectedTransport != NULL) {
        return this->impl->connectedTransport->narrow(typeId);
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::processResponse(const Pointer<Response> response) {

    Pointer<Command> object;

    synchronized(&(this->impl->requestMap)) {
        try {
            object = this->impl->requestMap.remove(response->getCorrelationId());
        } catch (NoSuchElementException& ex) {
            // Not tracking this request in our map, not an error.
        }
    }

    if (object != NULL) {
        try {
            Pointer<Tracked> tracked = object.dynamicCast<Tracked>();
            tracked->onResponse();
        }
        AMQ_CATCH_NOTHROW( ClassCastException)
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<wireformat::WireFormat> FailoverTransport::getWireFormat() const {

    Pointer<wireformat::WireFormat> result;
    Pointer<Transport> transport = this->impl->connectedTransport;

    if (transport != NULL) {
        result = transport->getWireFormat();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long FailoverTransport::getTimeout() const {
    return this->impl->timeout;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTimeout(long long value) {
    this->impl->timeout = value;
}

////////////////////////////////////////////////////////////////////////////////
long long FailoverTransport::getInitialReconnectDelay() const {
    return this->impl->initialReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setInitialReconnectDelay(long long value) {
    this->impl->initialReconnectDelay = value;
}

////////////////////////////////////////////////////////////////////////////////
long long FailoverTransport::getMaxReconnectDelay() const {
    return this->impl->maxReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setMaxReconnectDelay(long long value) {
    this->impl->maxReconnectDelay = value;
}

////////////////////////////////////////////////////////////////////////////////
long long FailoverTransport::getBackOffMultiplier() const {
    return this->impl->backOffMultiplier;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setBackOffMultiplier(long long value) {
    this->impl->backOffMultiplier = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isUseExponentialBackOff() const {
    return this->impl->useExponentialBackOff;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setUseExponentialBackOff(bool value) {
    this->impl->useExponentialBackOff = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isRandomize() const {
    return this->impl->uris->isRandomize();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setRandomize(bool value) {
    this->impl->uris->setRandomize(value);
}

////////////////////////////////////////////////////////////////////////////////
int FailoverTransport::getMaxReconnectAttempts() const {
    return this->impl->maxReconnectAttempts;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setMaxReconnectAttempts(int value) {
    this->impl->maxReconnectAttempts = value;
}

////////////////////////////////////////////////////////////////////////////////
int FailoverTransport::getStartupMaxReconnectAttempts() const {
    return this->impl->startupMaxReconnectAttempts;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setStartupMaxReconnectAttempts(int value) {
    this->impl->startupMaxReconnectAttempts = value;
}

////////////////////////////////////////////////////////////////////////////////
long long FailoverTransport::getReconnectDelay() const {
    return this->impl->reconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setReconnectDelay(long long value) {
    this->impl->reconnectDelay = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isBackup() const {
    return this->impl->backups->isEnabled();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setBackup(bool value) {
    this->impl->backups->setEnabled(value);
}

////////////////////////////////////////////////////////////////////////////////
int FailoverTransport::getBackupPoolSize() const {
    return this->impl->backups->getBackupPoolSize();
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setBackupPoolSize(int value) {
    this->impl->backups->setBackupPoolSize(value);
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isTrackMessages() const {
    return this->impl->trackMessages;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTrackMessages(bool value) {
    this->impl->trackMessages = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isTrackTransactionProducers() const {
    return this->impl->trackTransactionProducers;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setTrackTransactionProducers(bool value) {
    this->impl->trackTransactionProducers = value;
}

////////////////////////////////////////////////////////////////////////////////
int FailoverTransport::getMaxCacheSize() const {
    return this->impl->maxCacheSize;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setMaxCacheSize(int value) {
    this->impl->maxCacheSize = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isReconnectSupported() const {
    return this->impl->reconnectSupported;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setReconnectSupported(bool value) {
    this->impl->reconnectSupported = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isUpdateURIsSupported() const {
    return this->impl->updateURIsSupported;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setUpdateURIsSupported(bool value) {
    this->impl->updateURIsSupported = value;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isRebalanceUpdateURIs() const {
    return this->impl->rebalanceUpdateURIs;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setRebalanceUpdateURIs(bool rebalanceUpdateURIs) {
    this->impl->rebalanceUpdateURIs = rebalanceUpdateURIs;
}

////////////////////////////////////////////////////////////////////////////////
bool FailoverTransport::isPriorityBackup() const {
    return this->impl->priorityBackup;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setPriorityBackup(bool priorityBackup) {
    this->impl->priorityBackup = priorityBackup;
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::setPriorityURIs(const std::string& priorityURIs AMQCPP_UNUSED) {
//    StringTokenizer tokenizer = new StringTokenizer(priorityURIs, ",");
//    while (tokenizer.hasMoreTokens()) {
//        String str = tokenizer.nextToken();
//        try {
//            URI uri = new URI(str);
//            priorityList.add(uri);
//        } catch (Exception e) {
//            LOG.error("Failed to parse broker address: " + str, e);
//        }
//    }
}
