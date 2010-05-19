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

#ifndef _DECAF_NET_SSL_SSLCONTEXTSPI_H_
#define _DECAF_NET_SSL_SSLCONTEXTSPI_H_

#include <decaf/util/Config.h>

#include <decaf/security/SecureRandom.h>

namespace decaf {
namespace net {

    class SocketFactory;
    class ServerSocketFactory;

namespace ssl {

    class SSLParameters;

    /**
     * Defines the interface that should be provided by an SSLContext provider.
     *
     * @since 1.0
     */
    class DECAF_API SSLContextSpi {
    public:

        virtual ~SSLContextSpi();

        /**
         * Perform the initialization of this Context.
         *
         * @param random
         *      Pointer to an instance of a secure random number generator.
         *
         * @throw NullPointerException if the SecureRandom instance is NULL.
         * @throw KeyManagementException if an error occurs while initializing the context.
         */
        virtual void providerInit( security::SecureRandom* random ) = 0;

        /**
         * Creates a returns a new SSLParameters instance that contains the default settings for
         * this Providers SSLContext.
         *
         * The returned SSLParameters instance is requires to have non-empty values in its ciphersuites
         * and protocols.
         *
         * @returns new SSLParameters instance with the SSLContext defaults.
         *
         * @throws UnsupportedOperationException if the defaults cannot be obtained.
         */
        virtual SSLParameters* providerGetDefaultSSLParameters();

        /**
         * Creates and returns a new SSLParameters instance that contains the full set of supported
         * parameters for this SSL Context.
         *
         * The returned SSLParameters instance is requires to have non-empty values in its ciphersuites
         * and protocols.
         *
         * @returns a new SSLParameters instance with the full set of settings that are supported.
         *
         * @throws UnsupportedOperationException if the supported parameters cannot be obtained.
         */
        virtual SSLParameters* providerGetSupportedSSLParameters();

        /**
         * Returns a SocketFactory instance that can be used to create new SSLSocket objects.
         *
         * The SocketFactory is owned by the Service Provider and should not be destroyed by the caller.
         *
         * @returns SocketFactory instance that can be used to create new SSLSockets.
         *
         * @throws IllegalStateException if the SSLContextSpi object requires initialization but
         *         has not been initialized yet.
         */
        virtual SocketFactory* providerGetSocketFactory() = 0;

        /**
         * Returns a ServerSocketFactory instance that can be used to create new SSLServerSocket objects.
         *
         * The ServerSocketFactory is owned by the Service Provider and should not be destroyed by the caller.
         *
         * @returns SocketFactory instance that can be used to create new SSLServerSockets.
         *
         * @throws IllegalStateException if the SSLContextSpi object requires initialization but
         *         has not been initialized yet.
         */
        virtual ServerSocketFactory* providerGetServerSocketFactory() = 0;

    };

}}}

#endif /* _DECAF_NET_SSL_SSLCONTEXTSPI_H_ */
