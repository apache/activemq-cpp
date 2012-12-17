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
#ifndef _DECAF_NET_SOCKETFACTORY_H_
#define _DECAF_NET_SOCKETFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/io/IOException.h>
#include <decaf/net/UnknownHostException.h>

namespace decaf{
namespace net{

    class Socket;
    class InetAddress;

    /**
     * The SocketFactory is used to create Socket objects and can be sub-classed to
     * provide other types of Sockets or Sockets with varying configurations.
     *
     * @see decaf.net.Socket
     *
     * @since 1.0
     */
    class DECAF_API SocketFactory {
    private:

        static SocketFactory* defaultFactory;

    private:

        SocketFactory( const SocketFactory& );
        SocketFactory& operator= ( const SocketFactory& );

    protected:

        SocketFactory();

    public:

        virtual ~SocketFactory();

        /**
         * Creates an unconnected Socket object.
         *
         * @return a new Socket object, caller must free this object when done.
         *
         * @throws IOException if the Socket cannot be created.
         */
        virtual Socket* createSocket();

        /**
         * Creates a new Socket object and connects it to the specified remote host and
         * port using the configuration of this SocketFactory.
         *
         * @param host
         *      The host to connect the socket to.
         * @param port
         *      The port on the remote host to connect to.
         *
         * @return a new Socket object, caller must free this object when done.
         *
         * @throws IOException if an I/O error occurs while creating the Socket object.
         * @throws UnknownHostException if the host name is not known.
         */
        virtual Socket* createSocket( const InetAddress* host, int port ) = 0;

        /**
         * Creates a new Socket object and connects it to the specified remote host and
         * port using the configuration of this SocketFactory.  The Socket will be bound
         * to the specified local address and port.
         *
         * @param host
         *      The host to connect the socket to.
         * @param port
         *      The port on the remote host to connect to.
         * @param ifAddress
         *      The address on the local machine to bind the Socket to.
         * @param localPort
         *      The local port to bind the Socket to.
         *
         * @return a new Socket object, caller must free this object when done.
         *
         * @throws IOException if an I/O error occurs while creating the Socket object.
         * @throws UnknownHostException if the host name is not known.
         */
        virtual Socket* createSocket( const InetAddress* host, int port,
                                      const InetAddress* ifAddress, int localPort ) = 0;

        /**
         * Creates a new Socket object and connects it to the specified remote host and
         * port using the configuration of this SocketFactory.
         *
         * @param host
         *      The host name or IP address to connect the socket to.
         * @param port
         *      The port on the remote host to connect to.
         *
         * @return a new Socket object, caller must free this object when done.
         *
         * @throws IOException if an I/O error occurs while creating the Socket object.
         * @throws UnknownHostException if the host name is not known.
         */
        virtual Socket* createSocket( const std::string& name, int port ) = 0;

        /**
         * Creates a new Socket object and connects it to the specified remote host and
         * port using the configuration of this SocketFactory.
         *
         * @param host
         *      The host name or IP address to connect the socket to.
         * @param port
         *      The port on the remote host to connect to.
         * @param ifAddress
         *      The address on the local machine to bind the Socket to.
         * @param localPort
         *      The local port to bind the Socket to.
         *
         * @return a new Socket object, caller must free this object when done.
         *
         * @throws IOException if an I/O error occurs while creating the Socket object.
         * @throws UnknownHostException if the host name is not known.
         */
        virtual Socket* createSocket( const std::string& name, int port,
                                      const InetAddress* ifAddress, int localPort ) = 0;

        /**
         * Returns an pointer to the default SocketFactory for this Application, there is only one
         * default SocketFactory per application, the pointer returned by this method is owned by
         * the SocketFactory class and in not to be deleted by the caller.
         *
         * @returns pointer to the applications default SocketFactory.
         *
         * @throws SocketException if an error occurs while getting the default instance.
         */
        static SocketFactory* getDefault();

    };

}}

#endif /*_DECAF_NET_SOCKETFACTORY_H_*/
