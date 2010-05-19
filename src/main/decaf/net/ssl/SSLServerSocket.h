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

#ifndef _DECAF_NET_SSL_SSLSERVERSOCKET_H_
#define _DECAF_NET_SSL_SSLSERVERSOCKET_H_

#include <decaf/util/Config.h>

#include <decaf/net/ServerSocket.h>

namespace decaf {
namespace net {
namespace ssl {

    /**
     * Represents a server socket that is used to accept connections from clients using the
     * Secure Sockets protocol or the Top Level Security protocol.  The main function of this
     * class is to create SSLSocket objects by accepting connections from client sockets over SSL.
     *
     * @since 1.0
     */
    class DECAF_API SSLServerSocket : public ServerSocket {
    protected:

        /**
         * Creates a non-bound server socket.
         */
        SSLServerSocket();

        /**
         * Creates a new ServerSocket bound to the specified port, if the value of port is 0, then
         * any free port is chosen.
         *
         * When this constructor is called the size of the backlog queue is set at 50, connections
         * that arrive after the backlog has been reached are refused.
         *
         * If a SocketImplFactory is registered then the createSocketImpl method on the factory
         * will be called otherwise a default SocketImpl is created.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         *
         * @throws IOException if there is an I/O error while performing this operation.
         * @throws IllegalArgumentException if the port value is negative or greater than 65535.
         */
        SSLServerSocket( int port );

        /**
         * Creates a new ServerSocket bound to the specified port, if the value of port is 0, then
         * any free port is chosen.
         *
         * When this constructor is called the size of the backlog queue is set at backlog, connections
         * that arrive after the backlog has been reached are refused.  If backlog is zero or negative
         * then the default backlog value of 50 is used.
         *
         * If a SocketImplFactory is registered then the createSocketImpl method on the factory
         * will be called otherwise a default SocketImpl is created.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         * @param backlog
         *      The the number of incoming connection attempts to queue before connections are refused.
         *
         * @throws IOException if there is an I/O error while performing this operation.
         * @throws IllegalArgumentException if the port value is negative or greater than 65535.
         */
        SSLServerSocket( int port, int backlog );

        /**
         * Creates a new ServerSocket bound to the specified port, if the value of port is 0, then
         * any free port is chosen.  If the value of the ifAddress is empty or NULL then the ANY address
         * is used.
         *
         * When this constructor is called the size of the backlog queue is set at backlog, connections
         * that arrive after the backlog has been reached are refused.  If backlog is zero or negative
         * then the default backlog value of 50 is used.
         *
         * If a SocketImplFactory is registered then the createSocketImpl method on the factory
         * will be called otherwise a default SocketImpl is created.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         * @param backlog
         *      The the number of incoming connection attempts to queue before connections are refused.
         * @param ifAddress
         *      The IP Address to bind to on the local machine.
         *
         * @throws IOException if there is an I/O error while performing this operation.
         * @throws IllegalArgumentException if the port value is negative or greater than 65535.
         */
        SSLServerSocket( int port, int backlog, const decaf::net::InetAddress* address );

    public:

        virtual ~SSLServerSocket();

        /**
         * Gets a vector containing the names of all the cipher suites that are supported by this
         * SSLServerSocket.  Normally not all of these cipher suites will be enabled on the Socket.
         *
         * @returns a vector containing the names of all the supported cipher suites.
         */
        virtual std::vector<std::string> getSupportedCipherSuites() const = 0;

        /**
         * Gets a vector containing the names of all the protocols that could be enabled for
         * this SSLServerSocket instance.
         *
         * @returns a vector containing the names of all the supported protocols.
         */
        virtual std::vector<std::string> getSupportedProtocols() const = 0;

        /**
         * Returns a vector containing the names of all the currently enabled Cipher Suites for
         * this SSLServerSocket.
         *
         * @return vector of the names of all enabled Cipher Suites.
         */
        virtual std::vector<std::string> getEnabledCipherSuites() const = 0;

        /**
         * Sets the Cipher Suites that are to be enabled on the SSLServerSocket connection.  Each of the
         * named Cipher Suites must appear in the list of supported cipher suites for this connection
         * or an exception will be thrown.
         *
         * @param suites
         *      An Vector of names for all the Cipher Suites that are to be enabled.
         *
         * @throws IllegalArgumentException if the vector is empty or one of the names is invalid.
         */
        virtual void setEnabledCipherSuites( const std::vector<std::string>& suites ) = 0;

        /**
         * Returns a vector containing the names of all the currently enabled Protocols for
         * this SSLServerSocket.
         *
         * @return vector of the names of all enabled Protocols.
         */
        virtual std::vector<std::string> getEnabledProtocols() const = 0;

        /**
         * Sets the Protocols that are to be enabled on the SSLServerSocket connection.  Each of the
         * named Protocols must appear in the list of supported protocols suites for this connection
         * or an exception will be thrown.
         *
         * @param protocols
         *      An Vector of names for all the Protocols that are to be enabled.
         *
         * @throws IllegalArgumentException if the vector is empty or one of the names is invalid.
         */
        virtual void setEnabledProtocols( const std::vector<std::string>& protocols ) = 0;

        /**
         * @returns true if the Socket request client Authentication.
         */
        virtual bool getWantClientAuth() const = 0;

        /**
         * Sets whether or not this Socket will request Client Authentication.  If set to true the
         * Socket (when used in server mode) will request that the client authenticate itself, if the
         * client doesn't send authentication the socket will still allow negotiation to continue.
         *
         * @param value
         *      Whether the server socket should request client authentication.
         */
        virtual void setWantClientAuth( bool value ) = 0;

        /**
         * @returns true if the Socket requires client Authentication.
         */
        virtual bool getNeedClientAuth() const = 0;

        /**
         * Sets whether or not this Socket will require Client Authentication.  If set to true the
         * Socket (when used in server mode) will require that the client authenticate itself, if the
         * client doesn't send authentication the socket will not allow negotiation to continue.
         *
         * @param value
         *      Whether the server socket should require client authentication.
         */
        virtual void setNeedClientAuth( bool value ) = 0;

    };

}}}

#endif /* _DECAF_NET_SSL_SSLSERVERSOCKET_H_ */
