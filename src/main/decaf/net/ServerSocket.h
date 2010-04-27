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
#ifndef _DECAF_NET_SERVERSOCKET_H_
#define _DECAF_NET_SERVERSOCKET_H_

#include <decaf/util/Config.h>

#include <decaf/net/SocketImpl.h>
#include <decaf/net/SocketImplFactory.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/net/UnknownHostException.h>
#include <decaf/net/SocketTimeoutException.h>
#include <decaf/io/IOException.h>

#include <string>

namespace decaf{
namespace net{

    class Socket;
    class SocketImpl;

    /**
     * This class implements server sockets. A server socket waits for requests to come in over
     * the network.
     *
     * The actual work of the server socket is performed by an instance of the SocketImpl class.
     * An application can change the socket factory that creates the socket implementation to configure
     * itself to create sockets of a particular type.
     *
     * @since 1.0
     */
    class DECAF_API ServerSocket {
    private:

        // Factory for creating sockets, if not set a Plan TCP Socket is created
        static SocketImplFactory* factory;

        // The actual Socket that this Socket represents.
        mutable SocketImpl* impl;
        mutable volatile bool created;

        bool closed;
        bool bound;

        int backlog;
        int port;

    public:

        /**
         * Creates a non-bound server socket.
         */
        ServerSocket();

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
        ServerSocket( int port )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IllegalArgumentException );

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
        ServerSocket( int port, int backlog )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Creates a new ServerSocket bound to the specified port, if the value of port is 0, then
         * any free port is chosen.  If the value of the ipAddress is empty then the ANY address is
         * used.
         *
         * When this constructor is called the size of the backlog queue is set at backlog, connections
         * that arrive after the backlog has been reached are refused.  If backlog is zero or negative
         * then the default backlog value of 50 is used.
         *
         * If a SocketImplFactory is registered then the createSocketImpl method on the factory
         * will be called otherwise a default SocketImpl is created.
         *
         * @param ipAddress
         *      The IP Address to bind to on the local machine.
         * @param port
         *      The port to bind the ServerSocket to.
         * @param backlog
         *      The the number of incoming connection attempts to queue before connections are refused.
         *
         * @throws IOException if there is an I/O error while performing this operation.
         * @throws IllegalArgumentException if the port value is negative or greater than 65535.
         */
        ServerSocket( const std::string& ipAddress, int port, int backlog )
            throw( decaf::io::IOException,
                   decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Releases socket handle if close() hasn't been called.
         */
        virtual ~ServerSocket();

    protected:

        /**
         * Creates a ServerSocket wrapping the provided SocketImpl instance, this Socket is
         * considered unconnected.  The ServerSocket class takes ownership of this SocketImpl
         * pointer and will delete it when the Socket class is destroyed.
         *
         * @param impl
         *      The SocketImpl instance to wrap.
         *
         * @throws NullPointerException if the passed SocketImpl is Null.
         */
        ServerSocket( SocketImpl* impl );

    public:

        /**
         * Bind and listen to given local IPAddress and port, if the address is empty
         * than a valid local address will be chosen, and if the port of 0 than an
         * available open port will be chosen.
         *
         * @param host
         *      The IP address or host name.
         * @param port
         *      The TCP port between 1..655535.
         *
         * @throws IOException if an I/O error occurs while binding the socket.
         * @throws IllegalArgumentException if the parameters are not valid.
         */
        void bind( const std::string& host, int port )
            throw ( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Bind and listen to given local IPAddress and port, if the address is empty
         * than a valid local address will be chosen, and if the port of 0 than an
         * available open port will be chosen.
         *
         * If the backlog is greater than zero it will be used instead of the default
         * value, otherwise the default value is used and no error is generated.
         *
         * @param host
         *      The IP address or host name.
         * @param port
         *      The TCP port between 1..655535.
         * @param backlog
         *      The size of listen backlog.
         *
         * @throws IOException if an I/O error occurs while binding the socket.
         * @throws IllegalArgumentException if the parameters are not valid.
         */
        void bind( const std::string& host, int port, int backlog )
            throw ( decaf::io::IOException, decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Listens for a connection request on the bound IPAddress and Port for this ServerSocket, the
         * caller blocks until a connection is made.  If the SO_TIMEOUT option is set this method could
         * throw a SocketTimeoutException if the operation times out.
         *
         * @return a new Socket object pointer. Never returns NULL, the returned pointer is owned by
         *         the caller and must be explicitly freed by them.
         *
         * @throws IOException if an I/O error occurs while binding the socket.
         * @throws SocketException if an error occurs while blocking on the accept call.
         * @throws SocketTimeoutException if the SO_TIMEOUT option was used and the accept timed out.
         */
        Socket* accept() throw( decaf::io::IOException );

        /**
         * Closes the server socket, causing any Threads blocked on an accept call to
         * throw an Exception.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        void close() throw( decaf::io::IOException );

        /**
         * @returns true if the close method has been called on the ServerSocket.
         */
        bool isClosed() const;

        /**
         * @return true of the server socket is bound.
         */
        bool isBound() const;

        /**
         * Gets the receive buffer size for this socket, SO_RCVBUF.  This is the buffer used
         * by the underlying platform socket to buffer received data.
         *
         * @return the receive buffer size in bytes.
         *
         * @throws SocketException if the operation fails.
         */
        int getReceiveBufferSize() const throw( SocketException );

        /**
         * Sets the receive buffer size for this socket, SO_RCVBUF.
         *
         * @param size
         *      Number of bytes to set the receive buffer to.
         *
         * @throws SocketException if the operation fails.
         * @throws IllegalArgumentException if the value is zero or negative.
         */
        void setReceiveBufferSize( int size )
            throw( SocketException, decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Gets the reuse address flag, SO_REUSEADDR.
         *
         * @return True if the address can be reused.
         *
         * @throws SocketException if the operation fails.
         */
        bool getReuseAddress() const throw( SocketException );

        /**
         * Sets the reuse address flag, SO_REUSEADDR.
         *
         * @param reuse
         *      If true, sets the flag.
         *
         * @throws SocketException if the operation fails.
         */
        void setReuseAddress( bool reuse ) throw( SocketException );

        /**
         * Gets the timeout for socket operations, SO_TIMEOUT.
         *
         * @return The timeout in milliseconds for socket operations.
         *
         * @throws SocketException Thrown if unable to retrieve the information.
         */
        int getSoTimeout() const throw( SocketException );

        /**
         * Sets the timeout for socket operations, SO_TIMEOUT.  A value of zero indicates that timeout
         * is infinite for operations on this socket.
         *
         * @param timeout
         *      The timeout in milliseconds for socket operations.
         *
         * @throws SocketException Thrown if unable to set the information.
         * @throws IllegalArgumentException if the timeout value is negative.
         */
        void setSoTimeout( int timeout )
            throw( SocketException, decaf::lang::exceptions::IllegalArgumentException );

        /**
         * Gets the port number on the Local machine that this ServerSocket is bound to.
         *
         * @returns the port number of this machine that is bound, if not bound returns -1.
         */
        int getLocalPort() const;

        /**
         * @returns a string representing this ServerSocket.
         */
        std::string toString() const;

    public:

        /**
         * Sets the instance of a SocketImplFactory that the ServerSocket class should use when new
         * instances of this class are created.  This method is only allowed to be used once
         * during the lifetime of the application.
         *
         * @param factory
         *      The instance of a SocketImplFactory to use when new SocketImpl objects are created.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         * @throws SocketException if this method has already been called with a valid factory.
         */
        static void setSocketImplFactory( SocketImplFactory* factory )
            throw( decaf::io::IOException,
                   decaf::net::SocketException );

    protected:

        /**
         * Virtual method that allows a ServerSocket subclass to override the accept call and
         * provide its own SocketImpl for the socket.
         *
         * @param socket
         *      The socket object whose SocketImpl should be used for the accept call.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual void implAccept( Socket* socket )
            throw( decaf::io::IOException );

        /**
         * Allows a subclass to override what is considered the default backlog.
         *
         * @returns the default backlog for connections.
         */
        virtual int getDefaultBacklog();

    protected:

        // Check for already closed and throw an error if so.
        void checkClosed() const throw( decaf::io::IOException );

        // Create but don't connect the underlying OS Socket instance, if not already created.
        void ensureCreated() const throw( decaf::io::IOException );

        // Binds and sets up the Listen for this Server Socket
        void setupSocketImpl( const std::string ipAddress, int port, int backlog );

    };

}}

#endif // _DECAF_NET_SERVERSOCKETIMPL_H_

