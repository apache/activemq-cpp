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

#ifndef _DECAF_NET_SSL_SSLCONTEXT_H_
#define _DECAF_NET_SSL_SSLCONTEXT_H_

#include <decaf/util/Config.h>

#include <decaf/net/ssl/SSLContextSpi.h>

namespace decaf {
namespace net {
namespace ssl {

    /**
     * Represents on implementation of the Secure Socket Layer for streaming based sockets.
     *
     * This class servers a a source of factories to be used to create new SSL Socket instances.
     *
     * @since 1.0
     */
    class DECAF_API SSLContext {
    private:

        static SSLContext* defaultSSLContext;

        SSLContextSpi* contextImpl;

    private:

        SSLContext( const SSLContext& );
        SSLContext& operator= ( const SSLContext& );

    public:

        /**
         * Gets the Default SSLContext.
         *
         * The default instance of the SSLContext should be immediately usable without any need
         * for the client to initialize this context.
         *
         * @returns a pointer to the Default SSLContext instance.
         */
        static SSLContext* getDefault();

        /**
         * Sets the default SSLContext to be returned from future calls to getDefault.
         *
         * The set SSLContext must be fully initialized and usable.  The caller is responsible for
         * deleting this object before the Library shutdown methods are called.
         *
         * @throws NullPointerException if the context passed is NULL.
         */
        static void setDefault( SSLContext* context );

    public:

        SSLContext( SSLContextSpi* contextImpl );

        virtual ~SSLContext();

        /**
         * Returns an SocketFactory instance for use with this Context, the SocketFactory is owned
         * by the Context and should not be deleted by the caller.
         *
         * @returns a pointer to this SSLContext's SocketFactory for creating SSLSocket objects.
         *
         * @throws IllegalStateException if the SSLContextSpi requires initialization but it
         *         has not yet been initialized.
         */
        SocketFactory* getSocketFactory();

        /**
         * Returns an ServerSocketFactory instance for use with this Context, the ServerSocketFactory
         * is owned by the Context and should not be deleted by the caller.
         *
         * @returns a pointer to this SSLContext's ServerSocketFactory for creating SSLServerSocket objects.
         *
         * @throws IllegalStateException if the SSLContextSpi requires initialization but it
         *         has not yet been initialized.
         */
        ServerSocketFactory* getServerSocketFactory();

        /**
         * @return a new instance of an SSLParameters object containing the default set
         *         of settings for this SSLContext.
         *
         * @throws UnsupportedOperationException if the parameters cannot be retrieved.
         */
        SSLParameters* getDefaultSSLParameters();

        /**
         * @return a new instance of an SSLParameters object containing the complete set
         *         of settings for this SSLContext.
         *
         * @throws UnsupportedOperationException if the parameters cannot be retrieved.
         */
        SSLParameters* getSupportedSSLParameters();

    };

}}}

#endif /* _DECAF_NET_SSL_SSLCONTEXT_H_ */
