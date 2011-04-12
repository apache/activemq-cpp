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

#include "DefaultSSLContext.h"

#include <decaf/io/IOException.h>
#include <decaf/lang/Runnable.h>

#include <decaf/security/SecureRandom.h>
#include <decaf/internal/net/Network.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLContextSpi.h>

#include <memory>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ShutdownTask : public decaf::lang::Runnable {
    private:

        SSLContext** defaultRef;

    private:

        ShutdownTask( const ShutdownTask& );
        ShutdownTask& operator= ( const ShutdownTask& );

    public:

        ShutdownTask( SSLContext** defaultRef ) : defaultRef( defaultRef ) {}
        virtual ~ShutdownTask() {}

        virtual void run() {
            *defaultRef = NULL;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
SSLContext* DefaultSSLContext::defaultSSLContext = NULL;

////////////////////////////////////////////////////////////////////////////////
DefaultSSLContext::DefaultSSLContext() {
}

////////////////////////////////////////////////////////////////////////////////
DefaultSSLContext::~DefaultSSLContext() {
}

////////////////////////////////////////////////////////////////////////////////
SSLContext* DefaultSSLContext::getContext() {

#ifdef HAVE_OPENSSL

    if( defaultSSLContext == NULL ) {

        std::auto_ptr<SecureRandom> random( new SecureRandom() );
        std::auto_ptr<SSLContextSpi> contextSpi( new OpenSSLContextSpi() );

        // TODO - This should eventually move to a call to SSLContext::init();
        contextSpi->providerInit( random.release() );

        // Update the default, this is the Application default from now on.
        defaultSSLContext = new SSLContext( contextSpi.release() );

        // Store the default in the Network Runtime, it will be destroyed when the
        // Application calls the Decaf shutdownLibrary method.
        Network::getNetworkRuntime()->addAsResource( defaultSSLContext );
        Network::getNetworkRuntime()->addShutdownTask( new ShutdownTask( &defaultSSLContext ) );
    }

#endif

    return defaultSSLContext;
}
