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

#include <activemq/transport/discovery/AbstractDiscoveryAgent.h>

#include <activemq/transport/discovery/DiscoveredBrokerData.h>
#include <activemq/transport/discovery/DiscoveryListener.h>

#include <decaf/net/URI.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/concurrent/LinkedBlockingQueue.h>

using namespace activemq;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

////////////////////////////////////////////////////////////////////////////////
const int AbstractDiscoveryAgent::DEFAULT_INITIAL_RECONNECT_DELAY = 5000;
const int AbstractDiscoveryAgent::DEFAULT_BACKOFF_MULTIPLIER = 2;
const int AbstractDiscoveryAgent::DEFAULT_MAX_RECONNECT_DELAY = 30000;
const int AbstractDiscoveryAgent::WORKER_KILL_TIME_SECONDS = 1000;
const int AbstractDiscoveryAgent::HEARTBEAT_MISS_BEFORE_DEATH = 10;
const int AbstractDiscoveryAgent::DEFAULT_KEEPALIVE_INTERVAL = 500;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace discovery {

    class AbstractDiscoveryAgentImpl {
    private:

        AbstractDiscoveryAgentImpl(const AbstractDiscoveryAgentImpl&);
        AbstractDiscoveryAgentImpl& operator=(const AbstractDiscoveryAgentImpl&);

    public:

        long long initialReconnectDelay;
        long long maxReconnectDelay;
        long long backOffMultiplier;
        bool useExponentialBackOff;
        int maxReconnectAttempts;
        long long keepAliveInterval;

        AtomicBoolean started;
        Pointer<Thread> worker;
        Pointer<ThreadPoolExecutor> executor;

        HashMap<std::string, Pointer<DiscoveredBrokerData> > discoveredServices;
        Mutex discoveredServicesLock;

        URI discoveryUri;
        std::string selfService;
        std::string group;
        DiscoveryListener* listener;
        long long lastAdvertizeTime;
        bool reportAdvertizeFailed;

    public:

        AbstractDiscoveryAgentImpl() : initialReconnectDelay(AbstractDiscoveryAgent::DEFAULT_INITIAL_RECONNECT_DELAY),
                                       maxReconnectDelay(AbstractDiscoveryAgent::DEFAULT_MAX_RECONNECT_DELAY),
                                       backOffMultiplier(AbstractDiscoveryAgent::DEFAULT_BACKOFF_MULTIPLIER),
                                       useExponentialBackOff(false),
                                       maxReconnectAttempts(0),
                                       keepAliveInterval(AbstractDiscoveryAgent::DEFAULT_KEEPALIVE_INTERVAL),
                                       started(),
                                       worker(),
                                       executor(),
                                       discoveredServices(),
                                       discoveredServicesLock(),
                                       discoveryUri(),
                                       selfService(),
                                       group("default"),
                                       listener(),
                                       lastAdvertizeTime(0),
                                       reportAdvertizeFailed(true)
        {}

        ~AbstractDiscoveryAgentImpl() {
            if (started.compareAndSet(true, false)) {
                if (worker == NULL) {
                    worker->join(5000);

                    if (!worker->isAlive()) {
                        worker->interrupt();
                        worker->join(1000);
                    }

                    worker.reset(NULL);
                }

                executor->shutdown();
                executor->awaitTermination(1, TimeUnit::MINUTES);
            }

        }

        Executor& getExecutor() {
            if (executor == NULL) {
                synchronized(&discoveredServicesLock) {
                    if (executor == NULL) {
                        executor.reset(
                            new ThreadPoolExecutor(1, 1, 45, TimeUnit::SECONDS,
                                new LinkedBlockingQueue<Runnable*>()));
                    }
                }
            }
            return *executor;
        }

        /**
         * Returns true if this Broker has been marked as failed and it is now time to
         * start a recovery attempt.
         */
        bool isTimeForRecovery(Pointer<DiscoveredBrokerData> service) {
            synchronized(&discoveredServicesLock) {

                if (!service->isFailed()) {
                    return false;
                }

                int maxReconnectAttempts = this->maxReconnectAttempts;

                // Are we done trying to recover this guy?
                if (maxReconnectAttempts > 0 && service->getFailureCount() > maxReconnectAttempts) {
                    return false;
                }

                // Is it not yet time?
                if (System::currentTimeMillis() < service->getNextRecoveryTime()) {
                    return false;
                }

                service->setFailed(false);
                return true;
            }

            return false;
        }

        void updateHeartBeat(Pointer<DiscoveredBrokerData> service) {
            synchronized(&discoveredServicesLock) {

                service->setLastHeartBeatTime(System::currentTimeMillis());

                // Consider that the broker recovery has succeeded if it has not failed in 60 seconds.
                if (!service->isFailed() && service->getFailureCount() > 0 &&
                    (service->getLastHeartBeatTime() - service->getNextRecoveryTime()) > TimeUnit::MINUTES.toSeconds(60)) {

                    service->setFailureCount(0);
                    service->setNextRecoveryTime(System::currentTimeMillis());
                }
            }
        }

        bool markFailed(Pointer<DiscoveredBrokerData> service) {
            synchronized(&discoveredServicesLock) {

                if (!service->isFailed()) {
                    service->setFailed(true);
                    service->setFailureCount(service->getFailureCount() + 1);

                    long long reconnectDelay = 0;
                    if (!useExponentialBackOff) {
                        reconnectDelay = initialReconnectDelay;
                    } else {
                        reconnectDelay = (long) Math::pow((double)backOffMultiplier, (double)service->getFailureCount());
                        reconnectDelay = Math::min(reconnectDelay, maxReconnectDelay);
                    }

                    service->setNextRecoveryTime(System::currentTimeMillis() + reconnectDelay);
                    return true;
                }
            }
            return false;
        }
    };

    class ServiceAddedRunnable : public Runnable {
    private:

        AbstractDiscoveryAgent* agent;
        Pointer<DiscoveredBrokerData> event;

    public:

        ServiceAddedRunnable(AbstractDiscoveryAgent* agent, Pointer<DiscoveredBrokerData> event) :
            Runnable(), agent(agent), event(event) {
        }
        virtual ~ServiceAddedRunnable() {}

        virtual void run() {
            DiscoveryListener* listener = agent->getDiscoveryListener();
            if (listener != NULL) {
                listener->onServiceAdd(event.get());
            }
        }
    };

    class ServiceRemovedRunnable : public Runnable {
    private:

        AbstractDiscoveryAgent* agent;
        Pointer<DiscoveredBrokerData> event;

    public:

        ServiceRemovedRunnable(AbstractDiscoveryAgent* agent, Pointer<DiscoveredBrokerData> event) :
            Runnable(), agent(agent), event(event) {}
        virtual ~ServiceRemovedRunnable() {}

        virtual void run() {
            DiscoveryListener* listener = agent->getDiscoveryListener();
            if (listener != NULL) {
                listener->onServiceRemove(event.get());
            }
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
AbstractDiscoveryAgent::AbstractDiscoveryAgent() : DiscoveryAgent(), impl(new AbstractDiscoveryAgentImpl) {
}

////////////////////////////////////////////////////////////////////////////////
AbstractDiscoveryAgent::~AbstractDiscoveryAgent() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractDiscoveryAgent::isStarted() const {
    return impl->started.get();
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::start() {
    if (impl->started.compareAndSet(false, true)) {
        doStart();

        if (impl->worker == NULL) {
            impl->worker.reset(new Thread(this));
            impl->worker->start();
        }

        doAdvertizeSelf();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::stop() {
    // Changing the isStarted flag will signal the thread that it needs to shut down.
    if (impl->started.compareAndSet(true, false)) {
        doStop();

        if (impl->worker == NULL) {
            impl->worker->join(WORKER_KILL_TIME_SECONDS);

            if (!impl->worker->isAlive()) {
                impl->worker->interrupt();
                impl->worker->join(WORKER_KILL_TIME_SECONDS);
            }

            impl->worker.reset(NULL);
        }

        impl->executor->shutdown();
        impl->executor->awaitTermination(1, TimeUnit::MINUTES);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::run() {

    Thread::currentThread()->setName("Discovery Agent Thread.");

    while (impl->started.get()) {
        doTimeKeepingServices();
        try {
            doDiscovery();
        } catch (InterruptedException& ex) {
            return;
        } catch (Exception& ignore) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::registerService(const std::string& name) {
    impl->selfService = name;
    if (impl->started.get()) {
        try {
            doAdvertizeSelf();
        } catch (Exception& e) {
            // If a the advertise fails, chances are all subsequent sends will fail
            // too.. No need to keep reporting the same error over and over.
            if (impl->reportAdvertizeFailed) {
                impl->reportAdvertizeFailed = false;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::serviceFailed(const activemq::commands::DiscoveryEvent& event) {

    Pointer<DiscoveredBrokerData> service;
    synchronized(&impl->discoveredServicesLock) {
        try {
            service = impl->discoveredServices.get(event.getServiceName());
        } catch (NoSuchElementException& ex) {}
    }

    if (service != NULL && impl->markFailed(service)) {
        fireServiceRemovedEvent(service);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setDiscoveryListener(DiscoveryListener* listener) {
    this->impl->listener = listener;
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryListener* AbstractDiscoveryAgent::getDiscoveryListener() const {
    return this->impl->listener;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setDiscoveryURI(const URI& discoveryURI) {
    impl->discoveryUri = discoveryURI;
}

////////////////////////////////////////////////////////////////////////////////
URI AbstractDiscoveryAgent::getDiscoveryURI() const {
    return impl->discoveryUri;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setServiceName(const std::string& name) {
    impl->selfService = name;
}

////////////////////////////////////////////////////////////////////////////////
std::string AbstractDiscoveryAgent::getServiceName() const {
    return impl->selfService;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setKeepAliveInterval(long long interval) {
    impl->keepAliveInterval = interval;
}

////////////////////////////////////////////////////////////////////////////////
long long AbstractDiscoveryAgent::getKeepAliveInterval() const {
    return impl->keepAliveInterval;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setInitialReconnectDelay(long long initialReconnectDelay) {
    impl->initialReconnectDelay = initialReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
long long AbstractDiscoveryAgent::getInitialReconnectDelay() const {
    return impl->initialReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setMaxReconnectAttempts(int maxReconnectAttempts) {
    impl->maxReconnectAttempts = maxReconnectAttempts;
}

////////////////////////////////////////////////////////////////////////////////
int AbstractDiscoveryAgent::getMaxReconnectAttempts() const {
    return impl->maxReconnectAttempts;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setMaxReconnectDelay(long long maxReconnectDelay) {
    impl->maxReconnectDelay = maxReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
long long AbstractDiscoveryAgent::getMaxReconnectDelay() const {
    return impl->maxReconnectDelay;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setUseExponentialBackOff(bool useExponentialBackOff) {
    impl->useExponentialBackOff = useExponentialBackOff;
}

////////////////////////////////////////////////////////////////////////////////
bool AbstractDiscoveryAgent::isUseExponentialBackOff() const {
    return impl->useExponentialBackOff;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setBackOffMultiplier(long long multiplier) {
    impl->backOffMultiplier = multiplier;
}

////////////////////////////////////////////////////////////////////////////////
long long AbstractDiscoveryAgent::getBackOffMultiplier() const {
    return impl->backOffMultiplier;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::setGroup(const std::string& group) {
    impl->group = group;
}

////////////////////////////////////////////////////////////////////////////////
std::string AbstractDiscoveryAgent::getGroup() const {
    return impl->group;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::fireServiceRemovedEvent(Pointer<DiscoveredBrokerData> event) {
    if (impl->listener != NULL && impl->started.get()) {
        // Have the listener process the event async so that
        // he does not block this thread since we are doing time sensitive
        // processing of events.
        impl->getExecutor().execute(new ServiceRemovedRunnable(this, event));
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::fireServiceAddedEvent(Pointer<DiscoveredBrokerData> event) {
    if (impl->listener != NULL && impl->started.get()) {
        // Have the listener process the event async so that
        // he does not block this thread since we are doing time sensitive
        // processing of events.
        impl->getExecutor().execute(new ServiceAddedRunnable(this, event));
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::processLiveService(const std::string& brokerName, const std::string& service) {

    if (getServiceName().empty() || service != getServiceName()) {
        Pointer<DiscoveredBrokerData> remoteBroker;
        synchronized(&impl->discoveredServicesLock) {
            try {
                remoteBroker = impl->discoveredServices.get(service);
            } catch (NoSuchElementException& ignored) {
            }
        }

        if (remoteBroker == NULL) {
            remoteBroker.reset(new DiscoveredBrokerData(brokerName, service));
            impl->discoveredServices.put(service, remoteBroker);
            fireServiceAddedEvent(remoteBroker);
            doAdvertizeSelf();
        } else {
            impl->updateHeartBeat(remoteBroker);
            if (impl->isTimeForRecovery(remoteBroker)) {
                fireServiceAddedEvent(remoteBroker);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::processDeadService(const std::string& service) {

    if (service != getServiceName()) {

        Pointer<DiscoveredBrokerData> remoteBroker;
        synchronized(&impl->discoveredServicesLock) {
            try {
                remoteBroker = impl->discoveredServices.get(service);
            } catch (NoSuchElementException& ignored) {
            }
        }

        if (remoteBroker != NULL && !remoteBroker->isFailed()) {
            fireServiceRemovedEvent(remoteBroker);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::doTimeKeepingServices() {

    if (impl->started.get()) {
        long long currentTime = System::currentTimeMillis();
        if (currentTime < impl->lastAdvertizeTime ||
            ((currentTime - impl->keepAliveInterval) > impl->lastAdvertizeTime)) {

            doAdvertizeSelf();
            impl->lastAdvertizeTime = currentTime;
        }
        doExpireOldServices();
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgent::doExpireOldServices() {
    long long expireTime = System::currentTimeMillis() -
        (impl->keepAliveInterval * HEARTBEAT_MISS_BEFORE_DEATH);

    std::vector< Pointer<DiscoveredBrokerData> > services;
    synchronized(&impl->discoveredServicesLock) {
        services = impl->discoveredServices.values().toArray();
    }

    std::vector< Pointer<DiscoveredBrokerData> >::iterator iter = services.begin();
    for (; iter != services.end(); ++iter) {
        Pointer<DiscoveredBrokerData> service = *iter;
        if (service->getLastHeartBeatTime() < expireTime) {
            processDeadService(service->getServiceName());
        }
    }
}
