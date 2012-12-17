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

#ifndef _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSERVERSOCKET_H_
#define _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSERVERSOCKET_H_

#include <decaf/util/Config.h>

#include <decaf/net/ssl/SSLServerSocket.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    class OpenSSLParameters;
    class ServerSocketData;

    /**
     * SSLServerSocket based on OpenSSL library code.
     *
     * @since 1.0
     */
    class DECAF_API OpenSSLServerSocket : public decaf::net::ssl::SSLServerSocket {
    private:

        ServerSocketData* data;

        OpenSSLParameters* parameters;

    private:

        OpenSSLServerSocket( const OpenSSLServerSocket& );
        OpenSSLServerSocket& operator= ( const OpenSSLServerSocket& );

    public:

        OpenSSLServerSocket( OpenSSLParameters* parameters );

        virtual ~OpenSSLServerSocket();

    public: // SSLServerSocket overrides

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getSupportedCipherSuites() const;

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getSupportedProtocols() const;

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getEnabledCipherSuites() const;

        /**
         * {@inheritDoc}
         */
        virtual void setEnabledCipherSuites( const std::vector<std::string>& suites );

        /**
         * {@inheritDoc}
         */
        virtual std::vector<std::string> getEnabledProtocols() const;

        /**
         * {@inheritDoc}
         */
        virtual void setEnabledProtocols( const std::vector<std::string>& protocols );

        /**
         * {@inheritDoc}
         */
        virtual bool getWantClientAuth() const;

        /**
         * {@inheritDoc}
         */
        virtual void setWantClientAuth( bool value );

        /**
         * {@inheritDoc}
         */
        virtual bool getNeedClientAuth() const;

        /**
         * {@inheritDoc}
         */
        virtual void setNeedClientAuth( bool value );

    public:  // ServerSocket overrides

        /**
         * {@inheritDoc}
         */
        virtual decaf::net::Socket* accept();

    };

}}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSERVERSOCKET_H_ */
