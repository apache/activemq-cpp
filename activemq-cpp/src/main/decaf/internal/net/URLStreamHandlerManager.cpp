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

#include <decaf/internal/net/URLStreamHandlerManager.h>

#include <decaf/lang/Runnable.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/Exception.h>
#include <decaf/net/URLStreamHandler.h>
#include <decaf/net/URLStreamHandlerFactory.h>
#include <decaf/internal/net/Network.h>

#include <decaf/internal/net/http/HttpHandler.h>
#include <decaf/internal/net/https/HttpsHandler.h>
#include <decaf/internal/net/file/FileHandler.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::http;
using namespace decaf::internal::net::https;
using namespace decaf::internal::net::file;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
URLStreamHandlerManager* URLStreamHandlerManager::instance;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ShutdownTask : public decaf::lang::Runnable {
    private:

        URLStreamHandlerManager** defaultRef;

    private:

        ShutdownTask( const ShutdownTask& );
        ShutdownTask& operator= ( const ShutdownTask& );

    public:

        ShutdownTask( URLStreamHandlerManager** defaultRef ) : defaultRef( defaultRef ) {}
        virtual ~ShutdownTask() {}

        virtual void run() {
            *defaultRef = NULL;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace net {

    class URLStreamHandlerManagerImpl {
    public:

        URLStreamHandlerFactory* factory;

    public:

        URLStreamHandlerManagerImpl() : factory(NULL) {
        }

        virtual ~URLStreamHandlerManagerImpl() {
            try {
                delete factory;
            } catch(...) {}
        }

    };

}}}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandlerManager::URLStreamHandlerManager() : impl(new URLStreamHandlerManagerImpl) {
}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandlerManager::~URLStreamHandlerManager() {
    try {
        delete impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandlerManager* URLStreamHandlerManager::getInstance() {

    if (instance == NULL) {

        synchronized(Network::getNetworkRuntime()->getRuntimeLock()) {

            if (instance != NULL) {
                return instance;
            }

            instance = new URLStreamHandlerManager;

            // Store the default in the Network Runtime, it will be destroyed when the
            // Application calls the Decaf shutdownLibrary method.
            Network::getNetworkRuntime()->addAsResource(instance);
            Network::getNetworkRuntime()->addShutdownTask(new ShutdownTask(&instance));
        }
    }

    return instance;
}

////////////////////////////////////////////////////////////////////////////////
void URLStreamHandlerManager::setURLStreamHandlerFactory(URLStreamHandlerFactory* factory) {

    synchronized(Network::getNetworkRuntime()->getRuntimeLock()) {

        if (impl->factory != NULL) {
            throw RuntimeException(
                __FILE__, __LINE__, "Application already set a URLStreamHandlerFactory");
        }

        impl->factory = factory;
    }
}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandler* URLStreamHandlerManager::getURLStreamHandler(const decaf::lang::String& protocol) {

    URLStreamHandler* streamHandler = NULL;

    synchronized(Network::getNetworkRuntime()->getRuntimeLock()) {

        // If there is a stream handler factory, then attempt to
        // use it to create the handler.
        if (impl->factory != NULL) {
            streamHandler = impl->factory->createURLStreamHandler(protocol.toString());
            if (streamHandler != NULL) {
                return streamHandler;
            }
        }

        // No one else has provided a handler, so try our internal one.
        if (protocol.equalsIgnoreCase("http")) {
            return new HttpHandler;
        } else if (protocol.equalsIgnoreCase("https")) {
            return new HttpsHandler;
        } else if (protocol.equalsIgnoreCase("file")) {
            return new FileHandler;
        }

        // TODO we should cache the stream handlers and return the cached version
        //      we just need to ensure we manage the lifetime from within this object.
    }

    return streamHandler;
}

