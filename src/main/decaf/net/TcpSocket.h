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
#ifndef _DECAF_NET_TCPSOCKET_H_
#define _DECAF_NET_TCPSOCKET_H_

#include <decaf/net/SocketException.h>
#include <decaf/net/Socket.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/util/Config.h>
#include <decaf/internal/AprPool.h>

namespace decaf{
namespace net{

    // Forward declarations
    class SocketInputStream;
    class SocketOutputStream;

    /**
     * Platform-independent implementation of the socket interface.
     */
    class DECAF_API TcpSocket : public Socket {
    private:

        /**
         * APR Socket Pool to allocate from
         */
        decaf::internal::AprPool apr_pool;

        /**
         * The handle for this socket.
         */
        SocketHandle socketHandle;

        /**
         * The Address info for this Socket
         */
        SocketAddress socketAddress;

        /**
         * The input stream for reading this socket.
         */
        SocketInputStream* inputStream;

        /**
         * The output stream for writing to this socket.
         */
        SocketOutputStream* outputStream;

    public:

        /**
         * Construct a non-connected socket.
         * @throws SocketException thrown one windows if the static initialization
         * call to WSAStartup was not successful.
         */
        TcpSocket() throw ( SocketException );

        /**
         * Construct a connected or bound socket based on given
         * socket handle.
         * @param socketHandle a socket handle to wrap in the object
         */
        TcpSocket( SocketHandle socketHandle );

        /**
         * Destruct.
         * Releases the socket handle but not
         * gracefully shut down the connection.
         */
        virtual ~TcpSocket();

        /**
         * Gets the handle for the socket.
         * @return SocketHabler for this Socket, can be NULL
         */
        SocketHandle getSocketHandle () {
            return socketHandle;
        }

        /**
         * Connects to the specified destination. Closes this socket if
         * connected to another destination.
         * @param host The host of the server to connect to.
         * @param port The port of the server to connect to.
         * @param timeout of socket in microseconds
         * @throws IOException Thrown if a failure occurred in the connect.
         */
        void connect( const char* host, int port, int timeout) throw( SocketException );

        /**
         * Connects to the specified destination. Closes this socket if
         * connected to another destination.
         * @param host The host of the server to connect to.
         * @param port The port of the server to connect to.
         * @throws IOException Thrown if a failure occurred in the connect.
         */
        virtual void connect( const char* host, int port ) throw(SocketException)
        {
            connect(host,port,-1);
        }

        /**
         * Indicates whether or not this socket is connected to a destination.
         * @return true if connected
         */
        virtual bool isConnected() const{
            return socketHandle != INVALID_SOCKET_HANDLE;
        }

        /**
         * Gets the InputStream for this socket.
         * @return The InputStream for this socket. NULL if not connected.
         */
        virtual io::InputStream* getInputStream();

        /**
         * Gets the OutputStream for this socket.
         * @return the OutputStream for this socket.  NULL if not connected.
         */
        virtual io::OutputStream* getOutputStream();

        /**
         * Gets the linger time.
         * @return The linger time in seconds.
         * @throws SocketException if the operation fails.
         */
        virtual int getSoLinger() const throw( SocketException );

        /**
         * Sets the linger time.
         * @param linger The linger time in seconds.  If 0, linger is off.
         * @throws SocketException if the operation fails.
         */
        virtual void setSoLinger( int linger ) throw( SocketException );

        /**
         * Gets the keep alive flag.
         * @return True if keep alive is enabled.
         * @throws SocketException if the operation fails.
         */
        virtual bool getKeepAlive() const throw( SocketException );

        /**
         * Enables/disables the keep alive flag.
         * @param keepAlive If true, enables the flag.
         * @throws SocketException if the operation fails.
         */
        virtual void setKeepAlive( bool keepAlive ) throw( SocketException );

        /**
         * Gets the receive buffer size.
         * @return the receive buffer size in bytes.
         * @throws SocketException if the operation fails.
         */
        virtual int getReceiveBufferSize() const throw( SocketException );

        /**
         * Sets the recieve buffer size.
         * @param size Number of bytes to set the receive buffer to.
         * @throws SocketException if the operation fails.
         */
        virtual void setReceiveBufferSize( int size ) throw( SocketException );

        /**
         * Gets the reuse address flag.
         * @return True if the address can be reused.
         * @throws SocketException if the operation fails.
         */
        virtual bool getReuseAddress() const throw( SocketException );

        /**
         * Sets the reuse address flag.
         * @param reuse If true, sets the flag.
         * @throws SocketException if the operation fails.
         */
        virtual void setReuseAddress( bool reuse ) throw( SocketException );

        /**
         * Gets the send buffer size.
         * @return the size in bytes of the send buffer.
         * @throws SocketException if the operation fails.
         */
        virtual int getSendBufferSize() const throw( SocketException );

        /**
         * Sets the send buffer size.
         * @param size The number of bytes to set the send buffer to.
         * @throws SocketException if the operation fails.
         */
        virtual void setSendBufferSize( int size ) throw( SocketException );

        /**
         * Gets the timeout for socket operations.
         * @return The timeout in milliseconds for socket operations.
         * @throws SocketException Thrown if unable to retrieve the information.
         */
        virtual int getSoTimeout() const throw( SocketException );

        /**
         * Sets the timeout for socket operations.
         * @param timeout The timeout in milliseconds for socket operations.<p>
         * @throws SocketException Thrown if unable to set the information.
         */
        virtual void setSoTimeout( int timeout ) throw(SocketException);

        /**
         * Closes this object and deallocates the appropriate resources.
         * @throws CMSException
         */
        virtual void close() throw( lang::Exception );

    public:

        /**
         * Gets the Status of the TCP_NODELAY param for this socket as a Bool
         * @returns true if TCP_NODELAY is enabled
         * @throws CMSException
         */
        virtual bool getTcpNoDelay() const throw ( lang::Exception );

        /**
         * Sets the Status of the TCP_NODELAY param for this socket as a Bool
         * @param value - true if TCP_NODELAY is to be enabled
         * @throws CMSException
         */
        virtual void setTcpNoDelay( bool value ) throw ( lang::Exception );

    protected:

        void checkResult( apr_status_t value ) const throw (SocketException);

    };

}}

#endif /*_DECAF_NET_TCPSOCKET_H_*/
