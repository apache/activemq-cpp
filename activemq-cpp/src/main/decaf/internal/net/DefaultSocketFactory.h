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

#ifndef _DECAF_INTERNAL_NET_DEFAULTSOCKETFACTORY_H_
#define _DECAF_INTERNAL_NET_DEFAULTSOCKETFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/net/SocketFactory.h>

namespace decaf {
namespace internal {
namespace net {

    /**
     * SocketFactory implementation that is used to create Sockets.
     *
     * @since 1.0
     */
    class DECAF_API DefaultSocketFactory : public decaf::net::SocketFactory {
    private:

        DefaultSocketFactory( const DefaultSocketFactory& );
        DefaultSocketFactory& operator= ( const DefaultSocketFactory& );

    public:

        DefaultSocketFactory();

        virtual ~DefaultSocketFactory();

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket();

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket( const decaf::net::InetAddress* host, int port );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket( const decaf::net::InetAddress* host, int port,
                                                  const decaf::net::InetAddress* ifAddress, int localPort );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket( const std::string& name, int port );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* createSocket( const std::string& name, int port,
                                                  const decaf::net::InetAddress* ifAddress, int localPort );

    };

}}}

#endif /* _DECAF_INTERNAL_NET_DEFAULTSOCKETFACTORY_H_ */
