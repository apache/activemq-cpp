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
#ifndef _DECAF_NET_SERVERSOCKETIMPL_H_
#define _DECAF_NET_SERVERSOCKETIMPL_H_

#include <decaf/net/TcpSocket.h>
#include <decaf/net/SocketException.h>
#include <decaf/util/Config.h>
#include <decaf/internal/AprPool.h>

#include <apr_network_io.h>

namespace decaf{
namespace net{

    /**
     * A server socket class (for testing purposes).
     */
    class DECAF_API ServerSocket
    {
    public:

        typedef apr_socket_t* SocketHandle;
        typedef apr_sockaddr_t* SocketAddress;

    private:

        SocketHandle socketHandle;
        SocketAddress socketAddress;
        decaf::internal::AprPool apr_pool;

    public:

        /**
         * Constructor.
         * Creates a non-bound server socket.
         */
        ServerSocket();

        /**
         * Destructor.
         * Releases socket handle if close() hasn't been called.
         */
        virtual ~ServerSocket();

    public:

        /**
         * Bind and listen to given IP/dns and port.
         * @param host IP address or host name.
         * @param port TCP port between 1..655535
         */
        virtual void bind( const char* host, int port ) throw ( SocketException );

        /**
         * Bind and listen to given IP/dns and port.
         * @param host IP address or host name.
         * @param port TCP port between 1..655535
         * @param backlog Size of listen backlog.
         */
        virtual void bind( const char* host,
                           int port,
                           int backlog ) throw ( SocketException );

        /**
         * Blocks until a client connects to the bound socket.
         * @return new socket. Never returns NULL.
         */
        virtual Socket* accept () throw ( SocketException );

        /**
         * Closes the server socket.
         */
        virtual void close() throw( lang::Exception );

        /**
         * @return true of the server socket is bound.
         */
        virtual bool isBound() const;

   };

}}

#endif // _DECAF_NET_SERVERSOCKETIMPL_H_

