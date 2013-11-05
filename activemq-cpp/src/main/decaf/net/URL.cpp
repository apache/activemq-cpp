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

#include "URL.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/String.h>
#include <decaf/lang/Pointer.h>
#include <decaf/net/URI.h>
#include <decaf/net/URLConnection.h>
#include <decaf/net/URLStreamHandler.h>
#include <decaf/net/URLStreamHandlerFactory.h>
#include <decaf/net/MalformedURLException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/internal/net/URLType.h>
#include <decaf/internal/net/URLUtils.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/concurrent/Mutex.h>

#include <decaf/internal/net/URLStreamHandlerManager.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace net {

    class URLImpl {
    public:

        URLType url;
        Pointer<URLStreamHandler> streamHandler;

    public:

        URLImpl() : url(), streamHandler() {}

        void fixURL(bool fixHost) {
            int index;
            if (!url.getHost().isEmpty()) {
                url.setAuthority(url.getHost());
                if (url.getPort() != -1) {
                    url.setAuthority(url.getAuthority() + ":" + Integer::toString(url.getPort()));
                }
            }

            if (fixHost) {
                String host = url.getHost();
                if (!host.isEmpty() && (index = host.lastIndexOf('@')) > -1) {
                    url.setUserInfo(host.substring(0, index));
                    url.setHost(host.substring(index + 1));
                } else {
                    url.setUserInfo("");
                }
            }

            String file = url.getFile();
            if (!file.isEmpty() && (index = file.indexOf('?')) > -1) {
                url.setQuery(file.substring(index + 1));
                url.setPath(file.substring(0, index));
            } else {
                url.setQuery("");
                url.setPath(url.getFile());
            }
        }

        /**
         * Sets the receiver's stream handler to one which is appropriate for its
         * protocol. Throws a MalformedURLException if no reasonable handler is
         * available.
         * <p>
         * Note that this will overwrite any existing stream handler with the new
         * one. Senders must check if the streamHandler is NULL before calling the
         * method if they do not want this behavior (a speed optimization).
         */
        void setupStreamHandler() {
            URLStreamHandlerManager* manager = URLStreamHandlerManager::getInstance();
            streamHandler.reset(manager->getURLStreamHandler(url.getProtocol()));
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const String& url) : impl(new URLImpl) {
    initialize(NULL, url, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const URL& context, const String& spec) : impl(new URLImpl) {
    initialize(&context, spec, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const URL& context, const String& spec, URLStreamHandler* streamHandler) : impl(new URLImpl) {
    initialize(&context, spec, streamHandler);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const String& protocol, const String& host, int port,
         const String& file, URLStreamHandler* handler) : impl(new URLImpl) {
    initialize(protocol, host, port, file, handler);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const String& protocol, const String& host, const String& file) : impl(new URLImpl) {
    initialize(protocol, host, -1, file, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const String& protocol, const String& host, int port, const String& file) : impl(new URLImpl) {
    initialize(protocol, host, port, file, NULL);
}

////////////////////////////////////////////////////////////////////////////////
void URL::initialize(const URL* context, const String& theSpec, URLStreamHandler* handler) {

    if (handler != NULL) {
        impl->streamHandler.reset(handler);
    }

    if (theSpec.isEmpty()) {
        throw MalformedURLException(__FILE__, __LINE__, "spec was NULL");
    }
    String spec = theSpec.trim();  // trim

    // The spec includes a protocol if it includes a colon character
    // before the first occurrence of a slash character. Note that,
    // "protocol" is the field which holds this URLs protocol.
    int index = spec.indexOf(':');
    String protocol;
    int startIPv6Addr = spec.indexOf('[');
    if (index >= 0) {
        if ((startIPv6Addr == -1) || (index < startIPv6Addr)) {
            protocol = spec.substring(0, index);
            impl->url.setProtocol(protocol);
            // According to RFC 2396 scheme part should match the following expression:
            // alpha *( alpha | digit | "+" | "-" | "." )
            char c = protocol.charAt(0);
            bool valid = true;
//            bool valid = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
//            for (int i = 1; valid && (i < protocol.length()); i++) {
            for (int i = 0; valid && (i < protocol.length()); i++) {
                c = protocol.charAt(i);
                valid = URLUtils::isValidSchemeChar(i, c);
//                valid = ('a' <= c && c <= 'z') ||
//                        ('A' <= c && c <= 'Z') ||
//                        ('0' <= c && c <= '9') ||
//                        (c == '+') ||
//                        (c == '-') ||
//                        (c == '.');
            }
            if (!valid) {
                impl->url.setProtocol(String());
                index = -1;
            } else {
                // Ignore case in protocol names.
                // Scheme is defined by ASCII characters.
                impl->url.setProtocol(protocol.toLowerCase());
            }
        }
    }

    if (!impl->url.getProtocol().isEmpty()) {
        // If the context was specified, and it had the same protocol
        // as the spec, then fill in the receiver's slots from the values
        // in the context but still allow them to be over-ridden later
        // by the values in the spec.
        if (context != NULL && protocol.equals(context->getProtocol())) {
            String cPath = context->getPath();
            if (cPath.startsWith("/")) {
                set(protocol, context->getHost(), context->getPort(),
                    context->getAuthority(), context->getUserInfo(), cPath,
                    context->getQuery(), String());
            }
            if (impl->streamHandler == NULL) {
                impl->streamHandler = context->impl->streamHandler;
            }
        }
    } else {
        // If the spec did not include a protocol, then the context
        // *must* be specified. Fill in the receiver's slots from the
        // values in the context, but still allow them to be over-ridden
        // by the values in the ("relative") spec.
        if (context == NULL) {
            throw MalformedURLException(
                __FILE__, __LINE__,
                (std::string("Protocol not found: ") + spec.toString()).c_str());
        }
        set(context->getProtocol(), context->getHost(), context->getPort(),
            context->getAuthority(), context->getUserInfo(),
            context->getPath(), context->getQuery(), String());
        if (impl->streamHandler == NULL) {
            impl->streamHandler = context->impl->streamHandler;
        }
    }

    // If the stream handler has not been determined, set it
    // to the default for the specified protocol.
    if (impl->streamHandler == NULL) {
        impl->setupStreamHandler();
        if (impl->streamHandler == NULL) {
            throw MalformedURLException(
                __FILE__, __LINE__,
                (std::string("unknown protocol: ") + impl->url.getProtocol().toString()).c_str());
        }
    }

    // Let the handler parse the URL. If the handler throws
    // any exception, throw MalformedURLException instead.
    //
    // Note: We want "index" to be the index of the start of the scheme
    // specific part of the URL. At this point, it will be either
    // -1 or the index of the colon after the protocol, so we
    // increment it to point at either character 0 or the character
    // after the colon.
    try {
        impl->streamHandler->parseURL(*this, spec, ++index, (int) spec.length());
    } catch (Exception& e) {
        throw MalformedURLException(__FILE__, __LINE__, e.getMessage().c_str());
    }

    if (impl->url.getPort() < -1) {
        throw MalformedURLException(
            __FILE__, __LINE__, "port out of range: %d", impl->url.getPort());
    }
}

////////////////////////////////////////////////////////////////////////////////
void URL::initialize(const String& protocol, const String& host, int port,
                     const String& file, URLStreamHandler* handler) {

    if (port < -1) {
        throw MalformedURLException(__FILE__, __LINE__, "Port out of range: %d", port);
    }

    String theHost;

    if (host.indexOf(":") != -1 && host.charAt(0) != '[') {
        theHost = String("[").concat(host).concat("]");
    } else {
        theHost = host;
    }

    if (protocol.isEmpty()) {
        throw NullPointerException(
        __FILE__, __LINE__, "Unknown protocol: %s", "NULL");
    }

    impl->url.setProtocol(protocol);
    impl->url.setHost(theHost);
    impl->url.setPort(port);

    // Set the fields from the arguments. Handle the case where the
    // passed in "file" includes both a file and a reference part.
    int index = -1;
    index = file.indexOf("#", file.lastIndexOf("/"));
    if (index >= 0) {
        impl->url.setFile(file.substring(0, index));
        impl->url.setRef(file.substring(index + 1));
    } else {
        impl->url.setFile(file);
    }
    impl->fixURL(false);

    // Set the stream handler for the URL either to the handler argument if it was
    // specified, or to the default for the receiver's protocol if the handler was NULL.
    if (handler == NULL) {
        impl->setupStreamHandler();
        if (impl->streamHandler == NULL) {
            throw MalformedURLException(
                __FILE__, __LINE__, (std::string("Unknown protocol: ") + protocol.toString()).c_str());
        }
    } else {
        impl->streamHandler.reset(handler);
    }
}

////////////////////////////////////////////////////////////////////////////////
URL::~URL() {
    try {
        delete impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
bool URL::equals(const URL& other) const {
    return impl->streamHandler->equals(*this, other);
}

////////////////////////////////////////////////////////////////////////////////
String URL::getAuthority() const {
    return impl->url.getAuthority();
}

////////////////////////////////////////////////////////////////////////////////
int URL::getDefaultPort() const {
    return impl->streamHandler->getDefaultPort();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getFile() const {
    return impl->url.getFile();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getHost() const {
    return impl->url.getHost();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getPath() const {
    return impl->url.getPath();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getUserInfo() const {
    return impl->url.getUserInfo();
}

////////////////////////////////////////////////////////////////////////////////
int URL::getPort() const {
    return impl->url.getPort();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getProtocol() const {
    return impl->url.getProtocol();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getQuery() const {
    return impl->url.getQuery();
}

////////////////////////////////////////////////////////////////////////////////
String URL::getRef() const {
    return impl->url.getRef();
}

////////////////////////////////////////////////////////////////////////////////
int URL::hashCode() const {
    int hashCode = impl->url.getHashCode();
    if (hashCode == -1) {
        hashCode = impl->streamHandler->hashCode(*this);
        impl->url.setHashCode(hashCode);
    }

    return hashCode;
}

////////////////////////////////////////////////////////////////////////////////
URLConnection* URL::openConnection() {
    return impl->streamHandler->openConnection(*this);
}

////////////////////////////////////////////////////////////////////////////////
URLConnection* URL::openConnection(const Proxy* proxy) {
    if (proxy == NULL) {
        throw IllegalArgumentException(__FILE__, __LINE__, "proxy should not be NULL");
    }

    return impl->streamHandler->openConnection(*this, proxy);
}

////////////////////////////////////////////////////////////////////////////////
InputStream* URL::openStream() {
    return NULL;  // TODO figure out Connection lifetime
}

////////////////////////////////////////////////////////////////////////////////
bool URL::sameFile(const URL& other) const {
    if (impl->streamHandler == NULL) {
        throw MalformedURLException(
            __FILE__, __LINE__, (std::string("Unknown protocol: ") + getProtocol().toString()).c_str());
    }
    return impl->streamHandler->sameFile(*this, other);
}

////////////////////////////////////////////////////////////////////////////////
String URL::toExternalForm() const {
    if (impl->streamHandler == NULL) {
        return String("unknown protocol(").concat(getProtocol()).concat(")://").concat(getHost()).concat(getFile());
    }
    return impl->streamHandler->toExternalForm(*this);
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::toString() const {
    return toExternalForm().toString();
}

////////////////////////////////////////////////////////////////////////////////
URI URL::toURI() const {
    return URI(this->toString());
}

////////////////////////////////////////////////////////////////////////////////
void URL::set(const String& protocol, const String& host, int port, const String& file, const String& ref) {

    if (impl->url.getProtocol().isEmpty()) {
        impl->url.setProtocol(protocol);
    }

    impl->url.setHost(host);
    impl->url.setFile(file);
    impl->url.setPort(port);
    impl->url.setRef(ref);
    impl->url.setHashCode(-1);

    impl->fixURL(true);
}

////////////////////////////////////////////////////////////////////////////////
void URL::set(const String& protocol, const String& host, int port,
              const String& authority, const String& userInfo,
              const String& path, const String& query, const String& ref) {

    String filePart = path;
    if (!query.isEmpty()) {
        if (!filePart.isEmpty()) {
            filePart = filePart.concat("?").concat(query);
        } else {
            filePart = String("?").concat(query);
        }
    }
    set(protocol, host, port, filePart, ref);

    impl->url.setAuthority(authority);
    impl->url.setUserInfo(userInfo);
    impl->url.setPath(path);
    impl->url.setQuery(query);
}

////////////////////////////////////////////////////////////////////////////////
URLStreamHandler* URL::getURLStreamHandler() const {
    return this->impl->streamHandler.get();
}

////////////////////////////////////////////////////////////////////////////////
void URL::setURLStreamHandlerFactory(URLStreamHandlerFactory* factory) {
    URLStreamHandlerManager* manager = URLStreamHandlerManager::getInstance();
    manager->setURLStreamHandlerFactory(factory);
}
