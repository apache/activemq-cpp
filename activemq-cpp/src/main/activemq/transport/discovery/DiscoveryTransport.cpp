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

#include "DiscoveryTransport.h"

#include <activemq/util/URISupport.h>
#include <activemq/util/Suspendable.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/util/HashMap.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/Properties.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/net/URISyntaxException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::transport::discovery;

////////////////////////////////////////////////////////////////////////////////
const std::string DiscoveryTransport::DISCOVERED_OPTION_PREFIX = "discovered.";

////////////////////////////////////////////////////////////////////////////////
namespace activemq {
namespace transport {
namespace discovery {

    class DiscoveryTransportData {
    public:

        Pointer<CompositeTransport> next;
        Pointer<DiscoveryAgent> agent;
        StlMap<std::string, URI> serviceURIs;
        Properties parameters;
        Mutex lock;

    private:

        DiscoveryTransportData(const DiscoveryTransportData&);
        DiscoveryTransportData& operator= (const DiscoveryTransportData&);

    public:

        DiscoveryTransportData() : next(), agent(), serviceURIs(), parameters(), lock() {}
    };

}}}

////////////////////////////////////////////////////////////////////////////////
DiscoveryTransport::DiscoveryTransport(Pointer<CompositeTransport> next) :
    TransportFilter(next), impl(new DiscoveryTransportData) {
}

////////////////////////////////////////////////////////////////////////////////
DiscoveryTransport::~DiscoveryTransport() {
    try {
        this->close();
    }
    AMQ_CATCHALL_NOTHROW()
    try {
        delete this->impl;
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::start() {
    if (this->impl->agent == NULL) {
        throw IllegalStateException(__FILE__, __LINE__, "discoveryAgent not configured");
    }

    // lets pass into the agent the broker name and connection details
    this->impl->agent->setDiscoveryListener(this);
    this->impl->agent->start();

    TransportFilter::start();
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::stop() {

    try {
        IOException error;
        bool hasException = false;

        try {
            this->impl->agent->stop();
        } catch (IOException& ex) {
            error = ex;
            error.setMark(__FILE__, __LINE__);
            hasException = true;
        }

        try {
            TransportFilter::stop();
        } catch (IOException& ex) {
            if (!hasException) {
                error = ex;
                error.setMark(__FILE__, __LINE__);
                hasException = true;
            }
        }

        if (hasException) {
            throw error;
        }
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::doClose() {
    try {
        this->impl->next.reset(NULL);
    }
    AMQ_CATCH_RETHROW(IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    AMQ_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::setDiscoveryAgent(decaf::lang::Pointer<DiscoveryAgent> agent) {
    if (agent == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "DiscoveryAgent required to be non-null");
    }

    this->impl->agent = agent;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<DiscoveryAgent> DiscoveryTransport::getDiscoveryAgent() const {
    return this->impl->agent;
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::setParameters(const Properties& properties) {
    this->impl->parameters = properties;
}

////////////////////////////////////////////////////////////////////////////////
Properties DiscoveryTransport::getParameters() const {
    return this->impl->parameters;
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::onServiceAdd(const DiscoveryEvent* event) {
    std::string url = event->getServiceName();
    if (!url.empty()) {
        try {
            URI uri(url);
            uri = URISupport::applyParameters(uri, this->impl->parameters, DISCOVERED_OPTION_PREFIX);
            synchronized(&this->impl->lock) {
                this->impl->serviceURIs.put(event->getServiceName(), uri);
            }
            LinkedList<URI> uris;
            uris.add(uri);
            this->impl->next->addURI(false, uris);
        } catch (URISyntaxException& e) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::onServiceRemove(const DiscoveryEvent* event) {
    try {
        URI uri;
        synchronized(&this->impl->lock) {
            uri = this->impl->serviceURIs.get(event->getServiceName());
        }
        LinkedList<URI> uris;
        uris.add(uri);
        this->impl->next->removeURI(false, uris);
    } catch (NoSuchElementException& e) {}
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::transportInterrupted() {

    Pointer<Suspendable> suspendable;
    try {
        suspendable = this->impl->next.dynamicCast<Suspendable>();
        suspendable->resume();
    } catch (ClassCastException& e) {
        // Not a Suspendable instance.
    } catch (Exception& e) {
        // Failed to Resume
    }

    TransportFilter::transportInterrupted();
}

////////////////////////////////////////////////////////////////////////////////
void DiscoveryTransport::transportResumed() {
    Pointer<Suspendable> suspendable;
    try {
        suspendable = this->impl->next.dynamicCast<Suspendable>();
        suspendable->suspend();
    } catch (ClassCastException& e) {
        // Not a Suspendable instance.
    } catch (Exception& e) {
        // Failed to Suspend
    }

    TransportFilter::transportResumed();
}
