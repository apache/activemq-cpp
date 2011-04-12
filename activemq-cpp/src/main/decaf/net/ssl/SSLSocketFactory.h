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

#ifndef _DECAF_NET_SSL_SSLSOCKETFACTORY_H_
#define _DECAF_NET_SSL_SSLSOCKETFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/net/SocketFactory.h>

#include <vector>
#include <string>

namespace decaf {
namespace net {
namespace ssl {

    /**
     * Factory class interface for a SocketFactory that can create SSLSocket objects.
     *
     * @since 1.0
     */
    class DECAF_API SSLSocketFactory : public SocketFactory {
    private:

        static SocketFactory* defaultSocketFactory;

    protected:

        SSLSocketFactory();

    public:

        virtual ~SSLSocketFactory();

        /**
         * Returns the current default SSL SocketFactory, the factory is returned as a pointer
         * however the caller does not own this pointer and should not delete it.
         *
         * This method returns SSLContext::getDefault()->getSocketFactory(). If that call fails,
         * a non-functional factory is returned.
         *
         * @returns the default SSL SocketFactory pointer.
         *
         * @see decaf::net::ssl::SSLContext::getDefault()
         */
        static SocketFactory* getDefault();

        /**
         * Returns the list of cipher suites which are enabled by default. Unless a different list
         * is enabled, handshaking on an SSL connection will use one of these cipher suites. The
         * minimum quality of service for these defaults requires confidentiality protection and
         * server authentication (that is, no anonymous cipher suites).
         *
         * @returns an STL vector containing the list of cipher suites enabled by default.
         *
         * @see getSupportedCipherSuites()
         */
        virtual std::vector<std::string> getDefaultCipherSuites() = 0;

        /**
         * Returns the names of the cipher suites which could be enabled for use on an SSL connection.
         * Normally, only a subset of these will actually be enabled by default, since this list may
         * include cipher suites which do not meet quality of service requirements for those defaults.
         * Such cipher suites are useful in specialized applications.
         *
         * @returns an STL vector containing the list of supported cipher suites.
         *
         * @see getDefaultCipherSuites()
         */
        virtual std::vector<std::string> getSupportedCipherSuites() = 0;

        /**
         * Returns a socket layered over an existing socket connected to the named host, at the given
         * port. This constructor can be used when tunneling SSL through a proxy or when negotiating
         * the use of SSL over an existing socket. The host and port refer to the logical peer
         * destination. This socket is configured using the socket options established for this factory.
         *
         * @param socket
         *      The existing socket to layer over.
         * @param host
         *      The server host the original Socket is connected to.
         * @param port
         *      The server port the original Socket is connected to.
         * @param autoClose
         *      Should the layered over Socket be closed when the topmost socket is closed.
         *
         * @returns a new Socket instance that wraps the given Socket.
         *
         * @throws IOException if an I/O exception occurs while performing this operation.
         * @throws UnknownHostException if the host is unknown.
         */
        virtual Socket* createSocket( Socket* socket, std::string host, int port, bool autoClose ) = 0;

    };

}}}

#endif /* _DECAF_NET_SSL_SSLSOCKETFACTORY_H_ */
