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

#include <decaf/net/SocketException.h>
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
    private:

        int port;
        std::string hostname;

    public:

        SocketImpl();

        virtual ~SocketImpl();

    protected:

        /**
         * Accepts a new connection on the given Socket.
         *
         * @param socket
         *      The accepted connection.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void accept( SocketImpl* socket ) throw( decaf::io::IOException ) = 0;

        /**
         * Connects this socket to the given host and port.
         *
         * @param hostname
         *      The name of the host to connect to, or IP address.
         * @param port
         *      The port number on the host to connect to.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void connect( const std::string& hostname, int port )
            throw( decaf::io::IOException ) = 0;

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
        virtual void bind( const std::string& ipaddress, int port )
            throw( decaf::io::IOException ) = 0;

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
        virtual void listen( int backlog )
            throw( decaf::io::IOException ) = 0;

        /**
         * Gets the InputStream linked to this Socket.
         *
         * @returns an InputStream pointer owned by the Socket object.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual decaf::io::InputStream* getInputStream()
            throw( decaf::io::IOException ) = 0;

        /**
         * Gets the OutputStream linked to this Socket.
         *
         * @returns an OutputStream pointer owned by the Socket object.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual decaf::io::OutputStream* getOutputStream()
            throw( decaf::io::IOException ) = 0;

        /**
         * Gets the number of bytes that can be read from the Socket without blocking.
         *
         * @returns the number of bytes that can be read from the Socket without blocking.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual int available()
            throw( decaf::io::IOException ) = 0;

        /**
         * Closes the socket, terminating any blocked reads or writes.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void close() throw( decaf::io::IOException ) = 0;

        /**
         * Places the input stream for this socket at "end of stream". Any data sent to this
         * socket is acknowledged and then silently discarded. If you read from a socket input
         * stream after invoking shutdownInput() on the socket, the stream will return EOF.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void shutdownInput() throw( decaf::io::IOException ) = 0;

        /**
         * Disables the output stream for this socket. For a TCP socket, any previously written
         * data will be sent followed by TCP's normal connection termination sequence. If you
         * write to a socket output stream after invoking shutdownOutput() on the socket, the
         * stream will throw an IOException.
         *
         * @throws IOException if an I/O error occurs while attempting this operation.
         */
        virtual void shutdownOutput() throw( decaf::io::IOException ) = 0;

        /**
         * Gets the port that this socket has been assigned.
         *
         * @return the Socket's port number.
         */
         int getPort() {
             return this->port;
         }

         /**
          * Returns a string containing the address and port of this Socket instance.
          *
          * @returns a string containing the address and port of this socket.
          */
         std::string toString() const;

    };

}}

#endif /* _DECAF_NET_SOCKETIMPL_H_ */
