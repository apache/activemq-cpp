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

#ifndef _DECAF_NET_SOCKETIMPL_H_
#define _DECAF_NET_SOCKETIMPL_H_

#include <decaf/util/Config.h>

#include <decaf/io/IOException.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/io/FileDescriptor.h>

#include <decaf/net/SocketException.h>
#include <decaf/net/SocketTimeoutException.h>
#include <decaf/net/SocketOptions.h>

#include <string>

namespace decaf {
namespace net {

    /**
     * Acts as a base class for all physical Socket implementations.
     *
     * @since 1.0
     */
    class DECAF_API SocketImpl : public SocketOptions {
    protected:

        /**
         * The remote port that this Socket is connected to.
         */
        int port;

        /**
         * The port on the Local Machine that this Socket is Bound to.
         */
        int localPort;

        /**
         * The Remote Address that the Socket is connected to.
         */
        std::string address;

        /**
         * The File Descriptor for this Socket.
         */
        io::FileDescriptor* fd;

    private:

        SocketImpl( const SocketImpl& );
        SocketImpl& operator= ( const SocketImpl& );

    public:

        SocketImpl();

        virtual ~SocketImpl();

    public:

        /**
         * Creates the underlying platform Socket data structures which allows for
         * Socket options to be applied.  The created socket is in an unconnected state.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void create() = 0;

        /**
         * Accepts a new connection on the given Socket.
         *
         * @param socket
         *      The accepted connection.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         * @throws SocketException if an error occurs while performing an Accept on the socket.
         * @throws SocketTimeoutException if the accept call times out due to SO_TIMEOUT being set.
         */
        virtual void accept( SocketImpl* socket ) = 0;

        /**
         * Connects this socket to the given host and port.
         *
         * @param hostname
         *      The name of the host to connect to, or IP address.
         * @param port
         *      The port number on the host to connect to.
         * @param timeout
         *      Time in milliseconds to wait for a connection, 0 indicates forever.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         * @throws SocketTimeoutException if the connect call times out due to timeout being set.
         * @throws IllegalArguementException if a parameter has an illegal value.
         */
        virtual void connect( const std::string& hostname, int port, int timeout ) = 0;

        /**
         * Binds this Socket instance to the local ip address and port number given.
         *
         * @param ipaddress
         *      The address of local ip to bind to.
         * @param port
         *      The port number on the host to bind to.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void bind( const std::string& ipaddress, int port ) = 0;

        /**
         * Sets the maximum queue length for incoming connection indications (a request to
         * connect) to the count argument. If a connection indication arrives when the queue is
         * full, the connection is refused.
         *
         * @param backlog
         *      The maximum length of the connection queue.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void listen( int backlog ) = 0;

        /**
         * Gets the InputStream linked to this Socket.
         *
         * @returns an InputStream pointer owned by the Socket object.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual decaf::io::InputStream* getInputStream() = 0;

        /**
         * Gets the OutputStream linked to this Socket.
         *
         * @returns an OutputStream pointer owned by the Socket object.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual decaf::io::OutputStream* getOutputStream() = 0;

        /**
         * Gets the number of bytes that can be read from the Socket without blocking.
         *
         * @returns the number of bytes that can be read from the Socket without blocking.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual int available() = 0;

        /**
         * Closes the socket, terminating any blocked reads or writes.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void close() = 0;

        /**
         * Places the input stream for this socket at "end of stream". Any data sent to this
         * socket is acknowledged and then silently discarded. If you read from a socket input
         * stream after invoking shutdownInput() on the socket, the stream will return EOF.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void shutdownInput() = 0;

        /**
         * Disables the output stream for this socket. For a TCP socket, any previously written
         * data will be sent followed by TCP's normal connection termination sequence. If you
         * write to a socket output stream after invoking shutdownOutput() on the socket, the
         * stream will throw an IOException.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void shutdownOutput() = 0;

        /**
         * Gets the specified Socket option.
         *
         * @param option
         *      The Socket options whose value is to be retrieved.
         *
         * @returns the value of the given socket option.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual int getOption( int option ) const = 0;

        /**
         * Sets the specified option on the Socket if supported.
         *
         * @param option
         *      The Socket option to set.
         * @param value
         *      The value of the socket option to apply to the socket.
         *
         * @throws IOException if an I/O error occurs while performing this operation.
         */
        virtual void setOption( int option, int value ) = 0;

        /**
         * Gets the port that this socket has been assigned.
         *
         * @return the Socket's port number.
         */
         int getPort() const {
             return this->port;
         }

         /**
          * Gets the value of this SocketImpl's local port field.
          *
          * @returns the value of localPort.
          */
         int getLocalPort() const {
             return this->localPort;
         }

         /**
          * Gets the value of this SocketImpl's address field.
          *
          * @returns the value of the address field.
          */
         std::string getInetAddress() const {
             return this->address;
         }

         /**
          * Gets the FileDescriptor for this Socket, the Object is owned by this Socket and
          * should not be deleted by the caller.
          *
          * @returns a pointer to this Socket's FileDescriptor object.
          */
         const decaf::io::FileDescriptor* getFileDescriptor() const {
             return this->fd;
         }

         /**
          * Gets the value of the local Inet address the Socket is bound to if bound, otherwise
          * return the InetAddress ANY value "0.0.0.0".
          *
          * @returns the local address bound to, or ANY.
          */
         virtual std::string getLocalAddress() const = 0;

         /**
          * Returns a string containing the address and port of this Socket instance.
          *
          * @returns a string containing the address and port of this socket.
          */
         std::string toString() const;

         /**
          * @returns true if this SocketImpl supports sending Urgent Data.  The default
          *          implementation always returns false.
          */
         virtual bool supportsUrgentData() const {
             return false;
         }

         /**
          * Sends on byte of urgent data to the Socket.
          *
          * @param data
          *      The value to write as urgent data, only the lower eight bits are sent.
          *
          * @throws IOException if an I/O error occurs while performing this operation.
          */
         virtual void sendUrgentData( int data );

    };

}}

#endif /* _DECAF_NET_SOCKETIMPL_H_ */
