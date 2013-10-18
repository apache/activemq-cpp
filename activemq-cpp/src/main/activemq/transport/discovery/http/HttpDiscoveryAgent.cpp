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

#include <activemq/transport/discovery/http/HttpDiscoveryAgent.h>

#include <decaf/lang/Long.h>
#include <decaf/net/URI.h>
#include <decaf/util/HashSet.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/net/SocketFactory.h>
#include <decaf/net/Socket.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/io/BufferedOutputStream.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace activemq::transport::discovery::http;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace discovery {
namespace http {

    enum UpdateState {
        SUSPENDED,
        RESUMING,
        RESUMED
    };

    class HttpDiscoveryAgentImpl {
    private:

        HttpDiscoveryAgentImpl(const HttpDiscoveryAgentImpl&);
        HttpDiscoveryAgentImpl& operator= (const HttpDiscoveryAgentImpl&);

    public:

        UpdateState updateState;
        Mutex updateLock;
        long long updateInterval;
        URI registryUrl;

    public:

        HttpDiscoveryAgentImpl() : updateState(RESUMED),
                                   updateLock(),
                                   updateInterval(10 * 1000),
                                   registryUrl() {
        }

        HashSet<std::string> doLookup() {

            HashSet<std::string> result;
            try {
                return result;
            } catch (Exception& e) {
                std::cout << "Caught exception: " << e.getMessage() << std::endl;
            }

            return result;
        }
    };

}}}}

////////////////////////////////////////////////////////////////////////////////
HttpDiscoveryAgent::HttpDiscoveryAgent() : AbstractDiscoveryAgent(), impl(new HttpDiscoveryAgentImpl) {
}

////////////////////////////////////////////////////////////////////////////////
HttpDiscoveryAgent::~HttpDiscoveryAgent() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::string HttpDiscoveryAgent::toString() const {
    return "HttpDiscoveryAgent";
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::suspend() {
    synchronized(&impl->updateLock) {
        impl->updateState = SUSPENDED;
    }
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::resume() {
    synchronized(&impl->updateLock) {
        impl->updateState = RESUMING;
        impl->updateLock.notify();
    }
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::doStart() {

    if (impl->registryUrl.toString().empty()) {
        impl->registryUrl = getDiscoveryURI();
    }

    resume();
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::doStop() {
    suspend();
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::doAdvertizeSelf() {

}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::setUpdateInterval(long long updateInterval) {
    impl->updateInterval = updateInterval;
}

////////////////////////////////////////////////////////////////////////////////
long long HttpDiscoveryAgent::getUpdateInterval() const {
    return impl->updateInterval;
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::setRegistryURL(const std::string& registryUrl) {
    impl->registryUrl.create(registryUrl);
}

////////////////////////////////////////////////////////////////////////////////
std::string HttpDiscoveryAgent::getRegistryURL() const{
    return impl->registryUrl.toString();
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::doDiscovery() {
    try {
        updateServices();
        synchronized(&impl->updateLock) {
            do {
                if (impl->updateState == RESUMING) {
                    impl->updateState = RESUMED;
                } else {
                    impl->updateLock.wait(impl->updateInterval);
                }
            } while (impl->updateState == SUSPENDED && isStarted());
        }
    } catch (InterruptedException& e) {
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgent::updateServices() {
    DiscoveryListener* discoveryListener = getDiscoveryListener();
    if (discoveryListener != NULL) {
        HashSet<std::string> activeServices = impl->doLookup();
        if (activeServices.isEmpty()) {
            Pointer< Iterator<std::string> > discovered(activeServices.iterator());
            while (discovered->hasNext()) {
                std::string service = discovered->next();
                processLiveService("", service);
            }
        }
    }
}
