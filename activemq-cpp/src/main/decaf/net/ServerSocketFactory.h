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
#ifndef _DECAF_NET_SERVERSOCKETFACTORY_H_
#define _DECAF_NET_SERVERSOCKETFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/net/InetAddress.h>

namespace decaf {
namespace net {

    class ServerSocket;

    /**
     * Class used to create Server Sockets, subclasses can be created that create certain
     * types of server sockets according to specific policies.
     *
     * @since 1.0
     */
    class DECAF_API ServerSocketFactory {
    private:

        static ServerSocketFactory* defaultFactory;

    protected:

        ServerSocketFactory();

    public:

        virtual ~ServerSocketFactory();

        /**
         * Create a new ServerSocket that is unbound.  The ServerSocket will have been configured
         * with the defaults from the factory.
         *
         * @return new ServerSocket pointer that is owned by the caller.
         *
         * @throw IOException if the ServerSocket cannot be created for some reason.
         */
        virtual ServerSocket* createServerSocket();

        /**
         * Create a new ServerSocket that is bound to the given port.  The ServerSocket will have
         * been configured with the defaults from the factory.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         *
         * @return new ServerSocket pointer that is owned by the caller.
         *
         * @throw IOException if the ServerSocket cannot be created for some reason.
         */
        virtual ServerSocket* createServerSocket( int port ) = 0;

        /**
         * Create a new ServerSocket that is bound to the given port.  The ServerSocket will have
         * been configured with the defaults from the factory.  The ServerSocket will use the
         * specified connection backlog setting.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         * @param backlog
         *      The number of pending connect request the ServerSocket can queue.
         *
         * @return new ServerSocket pointer that is owned by the caller.
         *
         * @throw IOException if the ServerSocket cannot be created for some reason.
         */
        virtual ServerSocket* createServerSocket( int port, int backlog ) = 0;

        /**
         * Create a new ServerSocket that is bound to the given port.  The ServerSocket will have
         * been configured with the defaults from the factory.  The ServerSocket will bind to the
         * specified interface on the local host, and accept connections only on that interface.
         * If the address parameter is NULL than the ServerSocket will listen on all interfaces.
         *
         * @param port
         *      The port to bind the ServerSocket to.
         * @param backlog
         *      The number of pending connect request the ServerSocket can queue.
         * @param address
         *      The address of the interface on the local machine to bind to.
         *
         * @return new ServerSocket pointer that is owned by the caller.
         *
         * @throw IOException if the ServerSocket cannot be created for some reason.
         */
        virtual ServerSocket* createServerSocket( int port, int backlog, const InetAddress* address ) = 0;

        /**
         * Returns the Default ServerSocket factory, the pointer is owned by the Decaf runtime and
         * should not be deleted by the caller.  Only one default ServerSocketFactory exists for the
         * lifetime of the Application.
         *
         * @return the default ServerSocketFactory for this application.
         */
        static ServerSocketFactory* getDefault();

    };

}}

#endif /* _DECAF_NET_SERVERSOCKETFACTORY_H_ */
