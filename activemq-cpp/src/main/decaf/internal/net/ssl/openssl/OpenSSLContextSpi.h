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

#ifndef _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLCONTEXTSPI_H_
#define _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLCONTEXTSPI_H_

#include <decaf/util/Config.h>

#include <decaf/net/ssl/SSLContextSpi.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    class ContextData;

    /**
     * Provides an SSLContext that wraps the OpenSSL API.
     *
     * @since 1.0
     */
    class DECAF_API OpenSSLContextSpi : public decaf::net::ssl::SSLContextSpi {
    private:

        ContextData* data;

    private:

        OpenSSLContextSpi( const OpenSSLContextSpi& );
        OpenSSLContextSpi& operator= ( const OpenSSLContextSpi& );

    public:

        OpenSSLContextSpi();

        virtual ~OpenSSLContextSpi();

        /**
         * {@inheritDoc}
         */
        virtual void providerInit( security::SecureRandom* random );

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::SocketFactory* providerGetSocketFactory();

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::ServerSocketFactory* providerGetServerSocketFactory();

    private:

        friend class OpenSSLSocket;
        friend class OpenSSLSocketFactory;

        std::vector<std::string> getDefaultCipherSuites();
        std::vector<std::string> getSupportedCipherSuites();
        void* getOpenSSLCtx();

    };

}}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLCONTEXTSPI_H_ */
