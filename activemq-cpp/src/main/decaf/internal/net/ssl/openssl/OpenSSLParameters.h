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

#ifndef _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLPARAMETERS_H_
#define _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLPARAMETERS_H_

#include <decaf/util/Config.h>

#include <string>
#include <vector>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    /**
     * Container class for parameters that are Common to OpenSSL socket classes.
     *
     * @since 1.0
     */
    class OpenSSLParameters {
    private:

        bool needClientAuth;
        bool wantClientAuth;
        bool useClientMode;

#ifdef HAVE_OPENSSL
        SSL_CTX* context;
        SSL* ssl;
#endif

        std::vector<std::string> enabledCipherSuites;
        std::vector<std::string> enabledProtocols;

    private:

        OpenSSLParameters( const OpenSSLParameters& );
        OpenSSLParameters& operator= ( const OpenSSLParameters& );

    public:

#ifdef HAVE_OPENSSL
        OpenSSLParameters( SSL_CTX* context );
#endif

        virtual ~OpenSSLParameters();

        bool getNeedClientAuth() const {
            return this->needClientAuth;
        }

        void setNeedClientAuth( bool value ) {
            this->needClientAuth = value;
            this->wantClientAuth = false;
        }

        bool getWantClientAuth() const {
            return this->wantClientAuth;
        }

        void setWantClientAuth( bool value ) {
            this->wantClientAuth = value;
            this->needClientAuth = false;
        }

        bool getUseClientMode() const {
            return this->useClientMode;
        }

        void setUseClientMode( bool value ) {
            this->useClientMode = value;
        }

        std::vector<std::string> getSupportedCipherSuites() const;

        std::vector<std::string> getSupportedProtocols() const;

        std::vector<std::string> getEnabledCipherSuites() const;

        void setEnabledCipherSuites( const std::vector<std::string>& suites );

        std::vector<std::string> getEnabledProtocols() const;

        void setEnabledProtocols( const std::vector<std::string>& protocols );

#ifdef HAVE_OPENSSL

        SSL_CTX* getSSLContext() const {
            return this->context;
        }

        SSL* getSSL() const {
            return this->ssl;
        }

#endif

        /**
         * Creates a clone of this object such that all settings are transferred to a new
         * instance of an SSL object whose parent is the same SSL_CTX as this object's.
         */
        OpenSSLParameters* clone() const;

    };

}}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLPARAMETERS_H_ */
