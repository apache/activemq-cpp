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
#ifndef ACTIVEMQ_NETWORK_SSLSOCKET_H
#define ACTIVEMQ_NETWORK_SSLSOCKET_H

#include <activemq/network/TcpSocket.h>
#include <activemq/util/Config.h>

#ifdef AMQ_HAVE_OPENSSL
#include <openssl/ssl.h>

namespace activemq {
namespace network {

    /**
     * Platform-independent implementation of the SSL socket interface.
     */
    class SSLSocket : public TcpSocket
    {
    private:
        SSL_CTX *ctx;
        SSL *ssl;

        /**
         * Password for certificate and key files.
         */
        std::string password;

        /**
         * If peer certificate needs to be verified.
         */
        bool verify_peer;

        /**
         * Additional peer name that is accepted.
         */
        std::string verify_name;

    protected:
        /**
         * Internal initialize for TcpSocket.
         */
        virtual void initialize () throw (SocketException);

        /**
         * Verify peer certificate if needed.
         */
        virtual void verifyPeerCertificate( const char* host)
        throw (SocketException);

        /**
         * Initialize SSL connection structure.
         */
        virtual void initializeSSL( SocketHandle handle )
        throw (SocketException);

        /**
         * OpenSSL password callback.
         */
        static int password_cb( char* buffer, int size, int rw, void* data);

    public:

        /**
         * Construct a non-connected socket.
         * @throws SocketException thrown one windows if the static initialization
         * call to WSAStartup was not successful.
         */
        SSLSocket() throw (SocketException);

        /**
         * Construct a connected or bound socket based on given
         * socket handle.
         * @param socketHandle a socket handle to wrap in the object
         */
        SSLSocket( SocketHandle socketHandle ) throw (SocketException);

        /**
         * Destruct.
         * Releases the socket handle but not
         * gracefully shut down the connection.
         */
        virtual ~SSLSocket();

        /**
         * Connects to the specified destination. Closes this socket if
         * connected to another destination.
         * @param host The host of the server to connect to.
         * @param port The port of the server to connect to.
         * @throws IOException Thrown if a failure occurred in the connect.
         */
        void connect( const char* host, int port ) throw( SocketException );

        /**
         * Indicates whether or not this socket is connected to a destination.
         * @return true if connected
         */
        bool isConnected() const {
            return TcpSocket::isConnected() && ssl != 0;
        }

        /**
         * Closes this object and deallocates the appropriate resources.
         * @throws CMSException
         */
        void close() throw( cms::CMSException );

        /**
         * Set CA file and path.
         */
        virtual void setCAFilePath( std::string const& file,
                std::string const& path )
        throw( SocketException );

        /**
         * Set client certificate file and private key file.
         */
        virtual void setCertFile( std::string const& cert_file,
                std::string const& key_file )
        throw( SocketException );

        /**
         * Set password for key and possibly certificate.
         */
        virtual void setPassword( std::string const& password )
        throw( SocketException );

        /**
         * If the peer should be verified.
         */
        virtual void setVerifyPeer( bool value, std::string const& name )
        throw( SocketException );

        /**
         * Set cipher list for SSL.
         */
        virtual void setCiphers( std::string const& ciphers )
        throw( SocketException );
    };

}}

#endif /* AMQ_HAVE_OPENSSL */

#endif /*ACTIVEMQ_NETWORK_SOCKET_H*/
