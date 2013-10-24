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

#include <decaf/lang/Pointer.h>
#include <decaf/net/URI.h>
#include <decaf/net/URLConnection.h>
#include <decaf/net/URLStreamHandler.h>
#include <decaf/net/URLStreamHandlerFactory.h>
#include <decaf/net/MalformedURLException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/internal/net/URLType.h>
#include <decaf/util/HashMap.h>
#include <decaf/util/concurrent/Mutex.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
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

        static URLStreamHandlerFactory* streamHandlerFactory;
        static HashMap<std::string, URLStreamHandler*> streamHandlers;

    public:

        URLImpl() : url(), streamHandler() {}

        void initialize(const URL* context, const std::string& spec, URLStreamHandler* handler) {
            if (handler != NULL) {
                streamHandler.reset(handler);
            }

            if (spec.empty()) {
                throw MalformedURLException(__FILE__, __LINE__, "spec was NULL");
            }
//            // spec = spec.trim();  // trim
//
//            // The spec includes a protocol if it includes a colon character
//            // before the first occurrence of a slash character. Note that,
//            // "protocol" is the field which holds this URLs protocol.
//            int index;
//            try {
//                index = spec.indexOf(':');
//            } catch (NullPointerException e) {
//                throw new MalformedURLException(e.toString());
//            }
//            int startIPv6Addr = spec.indexOf('[');
//            if (index >= 0) {
//                if ((startIPv6Addr == -1) || (index < startIPv6Addr)) {
//                    protocol = spec.substring(0, index);
//                    // According to RFC 2396 scheme part should match
//                    // the following expression:
//                    // alpha *( alpha | digit | "+" | "-" | "." )
//                    char c = protocol.charAt(0);
//                    boolean valid = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
//                    for (int i = 1; valid && (i < protocol.length()); i++) {
//                        c = protocol.charAt(i);
//                        valid = ('a' <= c && c <= 'z') ||
//                                ('A' <= c && c <= 'Z') ||
//                                ('0' <= c && c <= '9') ||
//                                (c == '+') ||
//                                (c == '-') ||
//                                (c == '.');
//                    }
//                    if (!valid) {
//                        protocol = NULL;
//                        index = -1;
//                    } else {
//                        // Ignore case in protocol names.
//                        // Scheme is defined by ASCII characters.
//                        protocol = Util.toASCIILowerCase(protocol);
//                    }
//                }
//            }
//
//            if (url.getProtocol().empty()) {
//                // If the context was specified, and it had the same protocol
//                // as the spec, then fill in the receiver's slots from the values
//                // in the context but still allow them to be over-ridden later
//                // by the values in the spec.
//                if (context != NULL && protocol.equals(context.getProtocol())) {
//                    String cPath = context.getPath();
//                    if (cPath != NULL && cPath.startsWith("/")) {
//                        set(protocol, context.getHost(), context.getPort(), context
//                                .getAuthority(), context.getUserInfo(), cPath,
//                                context.getQuery(), NULL);
//                    }
//                    if (streamHandler == NULL) {
//                        streamHandler = context.streamHandler;
//                    }
//                }
//            } else {
//                // If the spec did not include a protocol, then the context
//                // *must* be specified. Fill in the receiver's slots from the
//                // values in the context, but still allow them to be over-ridden
//                // by the values in the ("relative") spec.
//                if (context == NULL) {
//                    throw MalformedURLException(__FILE__, __LINE__, "Protocol not found: %s", spec);
//                }
//                set(context.getProtocol(), context.getHost(), context.getPort(),
//                        context.getAuthority(), context.getUserInfo(), context
//                                .getPath(), context.getQuery(), NULL);
//                if (streamHandler == NULL) {
//                    streamHandler = context.streamHandler;
//                }
//            }
//
//            // If the stream handler has not been determined, set it
//            // to the default for the specified protocol.
//            if (streamHandler == NULL) {
//                setupStreamHandler();
//                if (streamHandler == NULL) {
//                    throw MalformedURLException(__FILE__, __LINE__, "unknown protocol: %s", url.getProtocol().c_str());
//                }
//            }
//
//            // Let the handler parse the URL. If the handler throws
//            // any exception, throw MalformedURLException instead.
//            //
//            // Note: We want "index" to be the index of the start of the scheme
//            // specific part of the URL. At this point, it will be either
//            // -1 or the index of the colon after the protocol, so we
//            // increment it to point at either character 0 or the character
//            // after the colon.
//            try {
//                streamHandler->parseURL(*this, spec, ++index, (int) spec.length());
//            } catch (Exception& e) {
//                throw MalformedURLException(__FILE__, __LINE__, e.getMessage());
//            }
//
//            if (url.getPort() < -1) {
//                throw MalformedURLException(__FILE__, __LINE__, "port out of range: %d", url.getPort());
//            }
        }

        void initialize(const std::string& protocol, const std::string& host, int port,
                        const std::string& file, const URLStreamHandler* handler) {

            if (port < -1) {
                throw MalformedURLException(__FILE__, __LINE__, "Port out of range: %d", port);
            }

//            if (host != NULL && host.indexOf(":") != -1 && host.charAt(0) != '[') {
//                host = "[" + host + "]";
//            }
//
//            if (protocol == NULL) {
//                throw NullPointerException(
//                    __FILE__, __LINE__, "Unknown protocol: %s", "NULL");
//            }
//
//            url.setProtocol(protocol);
//            url.setHost(host);
//            url.setPort(port);
//
//            // Set the fields from the arguments. Handle the case where the
//            // passed in "file" includes both a file and a reference part.
//            std::size_t index = -1;
//            index = file.find_first_of("#", file.find_last_of("/"));
//            if (index != string::npos) {
//                url.setFile(file.substr(0, index));
//                url.setRef(file.substr(index + 1));
//            } else {
//                url.setFile(file);
//            }
//            fixURL(false);
//
//            // Set the stream handler for the URL either to the handler argument if it was
//            // specified, or to the default for the receiver's protocol if the handler was NULL.
//            if (handler == NULL) {
//                setupStreamHandler();
//                if (streamHandler == NULL) {
//                    throw MalformedURLException(
//                        __FILE__, __LINE__, "Unknown protocol: %s", protocol.c_str());
//                }
//            } else {
//                streamHandler = handler;
//            }
        }

        void fixURL(bool fixHost) {
//            int index;
//            if (!url.getHost().empty()) {
//                url.setAuthority(url.getHost());
//                if (url.getPort() != -1) {
//                    url.setAuthority(url.getAuthority() + ":" + Integer::toString(url.getPort()));
//                }
//            }
//
//            if (fixHost) {
//                if (host != NULL && (index = host.lastIndexOf('@')) > -1) {
//                    userInfo = host.substring(0, index);
//                    host = host.substring(index + 1);
//                } else {
//                    url.setUserInfo("");
//                }
//            }
//
//            if (file != NULL && (index = file.indexOf('?')) > -1) {
//                query = file.substring(index + 1);
//                path = file.substring(0, index);
//            } else {
//                url.setQuery("");
//                url.setPath(url.getFile());
//            }
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
//            // Check for a cached (previously looked up) handler for the requested protocol.
//            streamHandler = streamHandlers.get(protocol);
//            if (streamHandler != NULL) {
//                return;
//            }
//
//            // If there is a stream handler factory, then attempt to
//            // use it to create the handler.
//            if (streamHandlerFactory != NULL) {
//                streamHandler = streamHandlerFactory.createURLStreamHandler(protocol);
//                if (streamHandler != NULL) {
//                    streamHandlers.put(protocol, streamHandler);
//                    return;
//                }
//            }
//
//            // Check if there is a list of packages which can provide handlers.
//            // If so, then walk this list looking for an applicable one.
//            String packageList = AccessController
//                    .doPrivileged(new PriviAction<String>(
//                            "java.protocol.handler.pkgs")); //$NON-NLS-1$
//            if (packageList != NULL) {
//                StringTokenizer st = new StringTokenizer(packageList, "|"); //$NON-NLS-1$
//                while (st.hasMoreTokens()) {
//                    String className = st.nextToken() + "." + protocol + ".Handler"; //$NON-NLS-1$ //$NON-NLS-2$
//
//                    try {
//                        streamHandler = (URLStreamHandler) Class.forName(className,
//                                true, ClassLoader.getSystemClassLoader())
//                                .newInstance();
//                        if (streamHandler != NULL) {
//                            streamHandlers.put(protocol, streamHandler);
//                        }
//                        return;
//                    } catch (IllegalAccessException e) {
//                    } catch (InstantiationException e) {
//                    } catch (ClassNotFoundException e) {
//                    }
//                }
//            }
//
//            // No one else has provided a handler, so try our internal one.
//
//            String className = "org.apache.harmony.luni.internal.net.www.protocol." + protocol //$NON-NLS-1$
//                    + ".Handler"; //$NON-NLS-1$
//            try {
//                streamHandler = (URLStreamHandler) Class.forName(className)
//                        .newInstance();
//            } catch (IllegalAccessException e) {
//            } catch (InstantiationException e) {
//            } catch (ClassNotFoundException e) {
//            }
//            if (streamHandler != NULL) {
//                streamHandlers.put(protocol, streamHandler);
//            }
        }
    };

    URLStreamHandlerFactory* URLImpl::streamHandlerFactory;
    HashMap<std::string, URLStreamHandler*> URLImpl::streamHandlers;
}}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const std::string& url) : impl(new URLImpl) {
    impl->initialize(NULL, url, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const URL& context, const std::string& spec) : impl(new URLImpl) {
    impl->initialize(&context, spec, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const URL& context, const std::string& spec, URLStreamHandler* streamHandler) : impl(new URLImpl) {
    impl->initialize(&context, spec, streamHandler);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const std::string& protocol, const std::string& host, int port,
         const std::string& file, URLStreamHandler* handler) : impl(new URLImpl) {
    impl->initialize(protocol, host, port, file, handler);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const std::string& protocol, const std::string& host, const std::string& file) : impl(new URLImpl) {
    impl->initialize(protocol, host, -1, file, NULL);
}

////////////////////////////////////////////////////////////////////////////////
URL::URL(const std::string& protocol, const std::string& host, int port, const std::string& file) : impl() {
    impl->initialize(protocol, host, port, file, NULL);
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
std::string URL::getAuthority() const {
    return impl->url.getAuthority();
}

////////////////////////////////////////////////////////////////////////////////
int URL::getDefaultPort() const {
    return impl->streamHandler->getDefaultPort();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getFile() const {
    return impl->url.getFile();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getHost() const {
    return impl->url.getHost();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getPath() const {
    return impl->url.getPath();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getUserInfo() const {
    return impl->url.getUserInfo();
}

////////////////////////////////////////////////////////////////////////////////
int URL::getPort() const {
    return impl->url.getPort();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getProtocol() const {
    return impl->url.getProtocol();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getQuery() const {
    return impl->url.getQuery();
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::getRef() const {
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
std::string URL::toExternalForm() const {
    if (impl->streamHandler == NULL) {
        return std::string("unknown protocol(") + getProtocol() + ")://" + getHost() + getFile();
    }
    return impl->streamHandler->toExternalForm(*this);
}

////////////////////////////////////////////////////////////////////////////////
std::string URL::toString() const {
    return toExternalForm();
}

////////////////////////////////////////////////////////////////////////////////
URI URL::toURI() const {
    return URI(this->toString());
}

////////////////////////////////////////////////////////////////////////////////
void URL::set(const std::string& protocol, const std::string& host, int port,
              const std::string& file, const std::string& ref) {

    if (impl->url.getProtocol().empty()) {
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
void URL::set(const std::string& protocol, const std::string& host, int port,
              const std::string& authority, const std::string& userInfo,
              const std::string& path, const std::string& query, const std::string& ref) {

    std::string filePart = path;
    if (!query.empty()) {
        if (!filePart.empty()) {
            filePart = filePart + "?" + query;
        } else {
            filePart = "?" + query;
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

    if (URLImpl::streamHandlerFactory != NULL) {
        throw Exception(__FILE__, __LINE__, "Attempt to set factory more than once");
    }

    // TODO thread safe
    URLImpl::streamHandlers.clear();
    URLImpl::streamHandlerFactory = factory;
}
