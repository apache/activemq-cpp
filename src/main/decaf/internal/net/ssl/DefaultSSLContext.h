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

#ifndef _DECAF_INTERNAL_NET_SSL_DEFAULTSSLCONTEXT_H_
#define _DECAF_INTERNAL_NET_SSL_DEFAULTSSLCONTEXT_H_

#include <decaf/util/Config.h>

#include <decaf/net/ssl/SSLContext.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {

    /**
     * Default SSLContext manager for the Decaf library.  If the user doesn't supply or
     * specify the SSLContext that they wish to use then we load the Decaf library's
     * default SSLContext using whatever SSL provider is enabled an preferred.
     *
     * @since 1.0
     */
    class DefaultSSLContext {
    private:

        static decaf::net::ssl::SSLContext* defaultSSLContext;

    private:

        DefaultSSLContext( const DefaultSSLContext& );
        DefaultSSLContext& operator= ( const DefaultSSLContext& );

    protected:

        DefaultSSLContext();

    public:

        virtual ~DefaultSSLContext();

    public:

        static decaf::net::ssl::SSLContext* getContext();

    };

}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_DEFAULTSSLCONTEXT_H_ */
