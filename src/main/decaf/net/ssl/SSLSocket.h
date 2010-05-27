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

#ifndef _DECAF_NET_SSL_SSLSOCKET_H_
#define _DECAF_NET_SSL_SSLSOCKET_H_

#include <decaf/util/Config.h>

#include <decaf/net/Socket.h>
#include <decaf/net/ssl/SSLParameters.h>

namespace decaf {
namespace net {
namespace ssl {

    /**
     *
     * @since 1.0
     */
    class DECAF_API SSLSocket : public Socket {
    public:

        SSLSocket();

        /**
         * Creates a new SSLSocket instance and connects it to the given address and port.
         *
         * If the host parameter is empty then the loop back address is used.
         *
         * @param address
         *      The address to connect to.
         * @param port
         *      The port number to connect to [0...65535]
         *
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws IOException if an I/O error occurs while connecting the Socket.
         * @throws NullPointerException if the InetAddress instance in NULL.
         * @throws IllegalArgumentException if the port if not in range [0...65535]
         */
        SSLSocket( const InetAddress* address, int port );

        /**
         * Creates a new SSLSocket instance and connects it to the given address and port.
         * The Socket will also bind to the local address and port specified.
         *
         * @param address
         *      The address to connect to.
         * @param port
         *      The port number to connect to [0...65535]
         * @param localAddress
         *      The IP address on the local machine to bind to.
         * @param localPort
         *      The port on the local machine to bind to.
         *
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws IOException if an I/O error occurs while connecting the Socket.
         * @throws NullPointerException if the InetAddress instance in NULL.
         * @throws IllegalArgumentException if the port if not in range [0...65535]
         */
        SSLSocket( const InetAddress* address, int port, const InetAddress* localAddress, int localPort );

        /**
         * Creates a new SSLSocket instance and connects it to the given host and port.
         *
         * If the host parameter is empty then the loop back address is used.
         *
         * @param host
         *      The host name or IP address to connect to, empty string means loopback.
         * @param port
         *      The port number to connect to [0...65535]
         *
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws IOException if an I/O error occurs while connecting the Socket.
         * @throws IllegalArgumentException if the port if not in range [0...65535]
         */
        SSLSocket( const std::string& host, int port );

        /**
         * Creates a new SSLSocket instance and connects it to the given host and port.
         *
         * If the host parameter is empty then the loop back address is used.
         *
         * @param host
         *      The host name or IP address to connect to, empty string means loopback.
         * @param port
         *      The port number to connect to [0...65535]
         * @param localAddress
         *      The IP address on the local machine to bind to.
         * @param localPort
         *      The port on the local machine to bind to.
         *
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws IOException if an I/O error occurs while connecting the Socket.
         * @throws IllegalArgumentException if the port if not in range [0...65535]
         */
        SSLSocket( const std::string& host, int port, const InetAddress* localAddress, int localPort );

        virtual ~SSLSocket();

    public:

        /**
         * Gets a vector containing the names of all the cipher suites that are supported by this
         * SSLSocket.  Normally not all of these cipher suites will be enabled on the Socket.
         *
         * @returns a vector containing the names of all the supported cipher suites.
         */
        virtual std::vector<std::string> getSupportedCipherSuites() const = 0;

        /**
         * Gets a vector containing the names of all the protocols that could be enabled for
         * this SSLSocket instance.
         *
         * @returns a vector containing the names of all the supported protocols.
         */
        virtual std::vector<std::string> getSupportedProtocols() const = 0;

        /**
         * Returns a vector containing the names of all the currently enabled Cipher Suites for
         * this SSL Socket.
         *
         * @return vector of the names of all enabled Cipher Suites.
         */
        virtual std::vector<std::string> getEnabledCipherSuites() const = 0;

        /**
         * Sets the Cipher Suites that are to be enabled on the SSL Socket connection.  Each of the
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
         * this SSL Socket.
         *
         * @return vector of the names of all enabled Protocols.
         */
        virtual std::vector<std::string> getEnabledProtocols() const = 0;

        /**
         * Sets the Protocols that are to be enabled on the SSL Socket connection.  Each of the
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
         * Returns an SSLParameters object for this SSLSocket instance.
         *
         * The cipherSuites and protocols vectors in the returned SSLParameters reference will
         * never be empty.
         *
         * @returns an SSLParameters object with the settings in use for the SSLSocket.
         */
        virtual SSLParameters getSSLParameters() const;

        /**
         * Sets the SSLParameters for this SSLSocket using the supplied SSLParameters instance.
         *
         * If the cipherSutes vector in the SSLParameters instance is not empty them the
         * setEnabledCipherSuites method is called with that vector, if the protocols vector in
         * the SSLParameters instance is not empty then the setEnabledProtocols method is called
         * with that vector.  If the needClientAuth value or the wantClientAuth value is true then
         * the setNeedClientAuth and setWantClientAuth methods are called respectively with a
         * value of true, otherwise the setWantClientAuth method is called with a value of false.
         *
         * @param value
         *      The SSLParameters instance that is used to update this SSLSocket's settings.
         *
         * @throws IllegalArgumentException if an error occurs while calling setEnabledCipherSuites
         *         or setEnabledProtocols.
         */
        virtual void setSSLParameters( const SSLParameters& value );

        /**
         * Initiates a handshake for this SSL Connection, this can be necessary for several reasons such
         * as using new encryption keys, or starting a new session.
         *
         * When called for the first time after the socket connects this method blocks until the handshake
         * is completed.  The provider is not require to support multiple handshakes and can throw an
         * IOException to indicate an error.
         *
         * @throw IOException if an I/O error occurs while performing the Handshake
         */
        virtual void startHandshake() = 0;

        /**
         * Determines the mode that the socket uses when a handshake is initiated, client or server.
         *
         * This method must be called prior to any handshake attempts on this Socket, once a handshake
         * has be initiated this socket remains the the set mode; client or server, for the life of
         * this object.
         *
         * @param value
         *      The mode setting, true for client or false for server.
         *
         * @throw IllegalArguementException if the handshake process has begun and mode is lcoked.
         */
        virtual void setUseClientMode( bool value ) = 0;

        /**
         * Gets whether this Socket is in Client or Server mode, true indicates that the mode is
         * set to Client.
         *
         * @return true if the Socket is in Client mode, false otherwise.
         */
        virtual bool getUseClientMode() const = 0;

        /**
         * Sets the Socket to require that a client authenticate itself by sending a valid Certificate that
         * is trusted by this Server mode socket.  This option only applies to sockets in the Server mode.
         *
         * If the option is enabled an the client does not provide a certificate then the handshake is
         * considered failed and the connection is refused.  Calling this method resets any previous
         * value for this option as well as clears any value set in the setWantClientAuth method.
         *
         * @param value
         *      The value indicating if a client is required to authenticate itself or not.
         */
        virtual void setNeedClientAuth( bool value ) = 0;

        /**
         * Returns if this socket is configured to require client authentication, true means that is has
         * and that clients that failed to authenticate will be rejected.  This option is only useful when
         * the socket is operating in server mode.
         *
         * @return true if client authentication is required.
         */
        virtual bool getNeedClientAuth() const = 0;

        /**
         * Sets the Socket to request that a client authenticate itself by sending a valid Certificate that
         * is trusted by this Server mode socket.  This option only applies to sockets in the Server mode.
         *
         * If the option is enabled an the client does not provide a certificate then the handshake is
         * considered to have succeeded, if it does send a certificate and that certificate is invalid the
         * the handshake will fail.  Calling this method resets any previous value for this option as well
         * as clears any value set in the setNeedClientAuth method.
         *
         * @param value
         *      The value indicating if a client is requested to authenticate itself or not.
         */
        virtual void setWantClientAuth( bool value ) = 0;

        /**
         * Returns if this socket is configured to request client authentication, true means that is has
         * and that clients that failed to authenticate will be rejected but that cleints that do not send
         * a certificate are not considered to have failed authentication.  This option is only useful when
         * the socket is operating in server mode.
         *
         * @return true if client authentication is required.
         */
        virtual bool getWantClientAuth() const = 0;

    };

}}}

#endif /* _DECAF_NET_SSL_SSLSOCKET_H_ */
