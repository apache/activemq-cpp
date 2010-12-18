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
#ifndef _DECAF_NET_SOCKET_H_
#define _DECAF_NET_SOCKET_H_

#include <decaf/net/InetAddress.h>
#include <decaf/net/SocketImplFactory.h>
#include <decaf/net/SocketException.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/Closeable.h>
#include <decaf/util/Config.h>

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/net/UnknownHostException.h>
#include <decaf/net/SocketTimeoutException.h>
#include <decaf/io/IOException.h>

namespace decaf{
namespace net{

    class SocketImpl;
    class ServerSocket;

    /**
     *
     * @since 1.0
     */
    class DECAF_API Socket : public decaf::io::Closeable {
    protected:

        // The actual Socket that this Socket represents.
        mutable SocketImpl* impl;

    private:

        // Factory for creating sockets, if not set a Plan TCP Socket is created
        static SocketImplFactory* factory;

        mutable volatile bool created;

        bool connected;
        bool closed;
        bool bound;
        bool inputShutdown;
        bool outputShutdown;

        friend class ServerSocket;

    private:

        Socket( const Socket& );
        Socket& operator= ( const Socket& );

    public:

        /**
         * Creates an unconnected Socket using the set SocketImplFactory or if non is set
         * than the default SockImpl type is created.
         */
        Socket();

        /**
         * Creates a Socket wrapping the provided SocketImpl instance, this Socket is
         * considered unconnected.  The Socket class takes ownership of this SocketImpl
         * pointer and will delete it when the Socket class is destroyed.
         *
         * @param impl
         *      The SocketImpl instance to wrap.
         *
         * @throws NullPointerException if the passed SocketImpl is Null.
         */
        Socket( SocketImpl* impl );

        /**
         * Creates a new Socket instance and connects it to the given address and port.  If
         * there is a SocketImplFactory set then the SokcetImpl is created using the factory
         * otherwise the default Socket implementation is used.
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
        Socket( const InetAddress* address, int port );

        /**
         * Creates a new Socket instance and connects it to the given address and port.  If
         * there is a SocketImplFactory set then the SokcetImpl is created using the factory
         * otherwise the default Socket implementation is used.  The Socket will also bind
         * to the local address and port specified.
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
        Socket( const InetAddress* address, int port, const InetAddress* localAddress, int localPort );

        /**
         * Creates a new Socket instance and connects it to the given host and port.  If
         * there is a SocketImplFactory set then the SokcetImpl is created using the factory
         * otherwise the default Socket implementation is used.
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
        Socket( const std::string& host, int port );

        /**
         * Creates a new Socket instance and connects it to the given host and port.  If
         * there is a SocketImplFactory set then the SokcetImpl is created using the factory
         * otherwise the default Socket implementation is used.
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
        Socket( const std::string& host, int port, const InetAddress* localAddress, int localPort );

        virtual ~Socket();

        /**
         * Binds this Socket to the given local address and port.  If the SocketAddress
         * value is NULL then the Socket will be bound to an available local address and
         * port.
         *
         * @param ipaddress
         *      The local address and port to bind the socket to.
         * @param port
         *      The port on the local machine to bind to.
         *
         * @throws IOException if an error occurs during the bind operation.
         * @throws IllegalArgumentException if the Socket can't process the subclass of SocketAddress
         *         that has been provided.
         */
        virtual void bind( const std::string& ipaddress, int port );

        /**
         * Closes the Socket.  Once closed a Socket cannot be connected or otherwise
         * operated upon, a new Socket instance must be created.
         *
         * @throws IOException if an I/O error occurs while closing the Socket.
         */
        virtual void close();

        /**
         * Connects to the specified destination.
         *
         * @param host
         *      The host name or IP address of the remote host to connect to.
         * @param port
         *      The port on the remote host to connect to.
         *
         * @throws IOException Thrown if a failure occurred in the connect.
         * @throws IllegalArguementException if the timeout value is negative or the endpoint is invalid.
         */
        virtual void connect( const std::string& host, int port );

        /**
         * Connects to the specified destination, with a specified timeout value.  If a connection
         * to the remote host is not established within the specified timeout interval than an
         * SocketTimeoutException is thrown.  A timeout value of zero is treated as an inifinite
         * timeout.
         *
         * @param host
         *      The host name or IP address of the remote host to connect to.
         * @param port
         *      The port on the remote host to connect to.
         * @param timeout
         *      The number of Milliseconds to wait before treating the connection as failed.
         *
         * @throws IOException Thrown if a failure occurred in the connect.
         * @throws SocketTimeoutException if the timeout for connection is exceeded.
         * @throws IllegalArguementException if the timeout value is negative or the endpoint is invalid.
         */
        virtual void connect( const std::string& host, int port, int timeout );

        /**
         * Indicates whether or not this socket is connected to am end point.
         *
         * @returns true if connected, false otherwise.
         */
        bool isConnected() const {
            return connected;
        }

        /**
         * @returns true if the Socket has been closed.
         */
        bool isClosed() const {
            return closed;
        }

        /**
         * @returns true if this Socket has been bound to a Local address.
         */
        bool isBound() const {
            return bound;
        }

        /**
         * @returns true if input on this Socket has been shutdown.
         */
        bool isInputShutdown() const {
            return inputShutdown;
        }

        /**
         * @returns true if output on this Socket has been shutdown.
         */
        bool isOutputShutdown() const {
            return outputShutdown;
        }

        /**
         * Gets the InputStream for this socket if its connected.  The pointer returned is the
         * property of the associated Socket and should not be deleted by the caller.
         *
         * When the returned InputStream is performing a blocking operation and the underlying
         * connection is closed or otherwise broker the read calls will normally throw an exception
         * to indicate the failure.
         *
         * Closing the InputStream will also close the underlying Socket.
         *
         * @return The InputStream for this socket.
         *
         * @throws IOException if an error occurs during creation of the InputStream, also if the
         *         Socket is not connected or the input has been shutdown previously.
         */
        virtual decaf::io::InputStream* getInputStream();

        /**
         * Gets the OutputStream for this socket if it is connected.  The pointer returned is the
         * property of the Socket instance and should not be deleted by the caller.
         *
         * Closing the returned Socket will also close the underlying Socket.
         *
         * @return the OutputStream for this socket.
         *
         * @throws IOException if an error occurs during the creation of this OutputStream, or
         *         if the Socket is closed or the output has been shutdown previously.
         */
        virtual decaf::io::OutputStream* getOutputStream();

        /**
         * Gets the on the remote host this Socket is connected to.
         *
         * @return the port on the remote host the socket is connected to, or 0 if not connected.
         */
        int getPort() const;

        /**
         * Gets the local port the socket is bound to.
         *
         * @return the local port the socket was bound to, or -1 if the socket is not bound.
         */
        int getLocalPort() const;

        /**
         * Returns the address to which the socket is connected.
         *
         * @returns the remote IP address to which this socket is connected, or null if the socket is not connected.
         */
        std::string getInetAddress() const;

        /**
         * Gets the local address to which the socket is bound.
         *
         * @returns the local address to which the socket is bound or InetAddress.anyLocalAddress() if the socket is not bound yet.
         */
        std::string getLocalAddress() const;

        /**
         * Shuts down the InputStream for this socket essentially marking it as EOF.  The stream
         * returns EOF for any calls to read after this method has been called.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual void shutdownInput();

        /**
         * Shuts down the OutputStream for this socket, any data already written to the socket will
         * be sent, any further calls to OuputStream::write will throw an IOException.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual void shutdownOutput();

        /**
         * Gets the linger time for the socket, SO_LINGER.  A return value of -1 indicates that
         * the option is disabled.
         *
         * @return The linger time in seconds.
         *
         * @throws SocketException if the operation fails.
         */
        virtual int getSoLinger() const;

        /**
         * Sets the linger time (SO_LINGER) using a specified time value, this limits of this
         * value are platform specific.
         *
         * @param state
         *      The state of SO_LINGER, true is on.
         * @param timeout
         *      The linger time in seconds, must be non-negative.
         *
         * @throws SocketException if the operation fails.
         * @throws IllegalArgumentException if state is true and timeout is negative.
         */
        virtual void setSoLinger( bool state, int timeout );

        /**
         * Gets the keep alive flag for this socket, SO_KEEPALIVE.
         *
         * @return true if keep alive is enabled for this socket.
         *
         * @throws SocketException if the operation fails.
         */
        virtual bool getKeepAlive() const;

        /**
         * Enables/disables the keep alive flag for this socket, SO_KEEPALIVE.
         *
         * @param keepAlive
         *      If true, enables the flag.
         *
         * @throws SocketException if the operation fails.
         */
        virtual void setKeepAlive( bool keepAlive );

        /**
         * Gets the receive buffer size for this socket, SO_RCVBUF.  This is the buffer used
         * by the underlying platform socket to buffer received data.
         *
         * @return the receive buffer size in bytes.
         *
         * @throws SocketException if the operation fails.
         */
        virtual int getReceiveBufferSize() const;

        /**
         * Sets the receive buffer size for this socket, SO_RCVBUF.
         *
         * @param size
         *      Number of bytes to set the receive buffer to.
         *
         * @throws SocketException if the operation fails.
         * @throws IllegalArgumentException if the value is zero or negative.
         */
        virtual void setReceiveBufferSize( int size );

        /**
         * Gets the reuse address flag, SO_REUSEADDR.
         *
         * @return True if the address can be reused.
         *
         * @throws SocketException if the operation fails.
         */
        virtual bool getReuseAddress() const;

        /**
         * Sets the reuse address flag, SO_REUSEADDR.
         *
         * @param reuse
         *      If true, sets the flag.
         *
         * @throws SocketException if the operation fails.
         */
        virtual void setReuseAddress( bool reuse );

        /**
         * Gets the send buffer size for this socket, SO_SNDBUF, this value is used by the platform
         * socket to buffer data written to the socket.
         *
         * @return the size in bytes of the send buffer.
         *
         * @throws SocketException if the operation fails.
         */
        virtual int getSendBufferSize() const;

        /**
         * Gets the send buffer size for this socket, SO_SNDBUF, this value is used by the platform
         * socket to buffer data written to the socket.
         *
         * @param size
         *      The number of bytes to set the send buffer to, must be larger than zero.
         *
         * @throws SocketException if the operation fails.
         * @throws IllegalArgumentException if the value is zero or negative.
         */
        virtual void setSendBufferSize( int size );

        /**
         * Gets the timeout for socket operations, SO_TIMEOUT.
         *
         * @return The timeout in milliseconds for socket operations.
         *
         * @throws SocketException Thrown if unable to retrieve the information.
         */
        virtual int getSoTimeout() const;

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
        virtual void setSoTimeout( int timeout );

        /**
         * Gets the Status of the TCP_NODELAY setting for this socket.
         *
         * @returns true if TCP_NODELAY is enabled for the socket.
         *
         * @throws SocketException Thrown if unable to set the information.
         */
        virtual bool getTcpNoDelay() const;

        /**
         * Sets the Status of the TCP_NODELAY param for this socket., this setting is used
         * to disable or enable Nagle's algorithm on the Socket.
         *
         * @param value
         *      The setting for the socket's TCP_NODELAY option, true to enable.
         *
         * @throws SocketException Thrown if unable to set the information.
         */
        virtual void setTcpNoDelay( bool value );

        /**
         * Gets the Traffic Class setting for this Socket, sometimes referred to as Type of
         * Service setting.  This setting is dependent on the underlying network implementation
         * for the platform this Socket runs on and is not guaranteed to have any effect.
         *
         * Refer to your platforms network documentation regarding support for this setting.
         *
         * @returns the bitset result of querying the traffic class setting.
         *
         * @throws SocketException if an error is encountered while performing this operation.
         */
        virtual int getTrafficClass() const;

        /**
         * Gets the Traffic Class setting for this Socket, sometimes referred to as Type of
         * Service setting.  This setting is dependent on the underlying network implementation
         * for the platform this Socket runs on and is not guaranteed to have any effect.
         *
         * Refer to your platforms network documentation regarding support for this setting.
         *
         * @param value
         *      The integer value representing the traffic class setting bitset.
         *
         * @throws SocketException if an error is encountered while performing this operation.
         * @throws IllegalArgumentException if the value is not in the range [0..255].
         */
        virtual void setTrafficClass( int value );

        /**
         * Gets the value of the OOBINLINE for this socket.
         *
         * @return true if OOBINLINE is enabled, false otherwise.
         *
         * @throws SocketException if an error is encountered while performing this operation.
         */
        virtual bool getOOBInline() const;

        /**
         * Sets the value of the OOBINLINE for this socket, by default this option is disabled.  If
         * enabled the urgent data is read inline on the Socket's InputStream, no notification is give.
         *
         * @return true if OOBINLINE is enabled, false otherwise.
         *
         * @throws SocketException if an error is encountered while performing this operation.
         */
        virtual void setOOBInline( bool value );

        /**
         * Sends on byte of urgent data to the Socket.
         *
         * @param data
         *      The value to write as urgent data, only the lower eight bits are sent.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual void sendUrgentData( int data );

        /**
         * @returns a string representing this Socket.
         */
        virtual std::string toString() const;

    public:

        /**
         * Sets the instance of a SocketImplFactory that the Socket class should use when new
         * instances of this class are created.  This method is only allowed to be used once
         * during the lifetime of the application.
         *
         * @param factory
         *      The instance of a SocketImplFactory to use when new Socket objects are created.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         * @throws SocketException if this method has already been called with a valid factory.
         */
        static void setSocketImplFactory( SocketImplFactory* factory );

    protected:

        // Flags the Socket instance as created, bound and connected when its connected
        // via a ServerSocket accept call.
        void accepted();

        // Sets up a connected socket for the constructors that take connection arguments.
        void initSocketImpl( const std::string& address, int port, const InetAddress* localAddress, int localPort );

        // Check for already closed and throw an error if so.
        void checkClosed() const;

        // Create but don't connect the underlying OS Socket instance, if not already created.
        void ensureCreated() const;

    };

}}

#endif /*_DECAF_NET_SOCKET_H_*/
