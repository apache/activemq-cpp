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

#ifndef _DECAF_INTERNAL_NET_DEFAULTSSLSERVERSOCKETFACTORY_H_
#define _DECAF_INTERNAL_NET_DEFAULTSSLSERVERSOCKETFACTORY_H_

#include <decaf/util/Config.h>

#include <decaf/net/ssl/SSLServerSocketFactory.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {

    /**
     * Default implementation of the SSLServerSocketFactory, this factory throws an Exception
     * from all its create methods to indicate that SSL is not supported, this factory is
     * used when OpenSSL is not enabled in the builds.
     *
     * @since 1.0
     */
    class DECAF_API DefaultSSLServerSocketFactory : public decaf::net::ssl::SSLServerSocketFactory {
    private:

        std::string errorMessage;

    public:

        DefaultSSLServerSocketFactory( const std::string& errorMessage );

        virtual ~DefaultSSLServerSocketFactory();

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::ServerSocket* createServerSocket();

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::ServerSocket* createServerSocket( int port );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::ServerSocket* createServerSocket( int port, int backlog );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::ServerSocket* createServerSocket( int port, int backlog,
                                                              const decaf::net::InetAddress* address );

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getDefaultCipherSuites();

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getSupportedCipherSuites();

    };

}}}}

#endif /* _DECAF_INTERNAL_NET_DEFAULTSSLSERVERSOCKETFACTORY_H_ */
