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

#include "SSLContext.h"

#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <decaf/net/SocketFactory.h>
#include <decaf/net/ssl/SSLParameters.h>

#include <decaf/internal/net/ssl/DefaultSSLContext.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;

////////////////////////////////////////////////////////////////////////////////
SSLContext* SSLContext::defaultSSLContext = NULL;

////////////////////////////////////////////////////////////////////////////////
SSLContext::SSLContext( SSLContextSpi* contextImpl ) : contextImpl( contextImpl ) {

    if( contextImpl == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "SSLContextSpi cannot be NULL" );
    }
}

////////////////////////////////////////////////////////////////////////////////
SSLContext::~SSLContext() {
    try{
        delete contextImpl;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
SSLContext* SSLContext::getDefault() {
    try{
        if( SSLContext::defaultSSLContext != NULL ) {
            return SSLContext::defaultSSLContext;
        }

        return DefaultSSLContext::getContext();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalStateException)
    DECAF_CATCHALL_THROW( IllegalStateException)
}

////////////////////////////////////////////////////////////////////////////////
void SSLContext::setDefault( SSLContext* context ) {

    if( context == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "SSLContextSpi cannot be NULL" );
    }

    SSLContext::defaultSSLContext = context;
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory* SSLContext::getSocketFactory() {
    try{
        return this->contextImpl->providerGetSocketFactory();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalStateException)
    DECAF_CATCHALL_THROW( IllegalStateException)
}

////////////////////////////////////////////////////////////////////////////////
ServerSocketFactory* SSLContext::getServerSocketFactory() {
    try{
        return this->contextImpl->providerGetServerSocketFactory();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalStateException)
    DECAF_CATCHALL_THROW( IllegalStateException)
}

////////////////////////////////////////////////////////////////////////////////
SSLParameters* SSLContext::getDefaultSSLParameters() {
    try{
        return this->contextImpl->providerGetDefaultSSLParameters();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalStateException)
    DECAF_CATCHALL_THROW( IllegalStateException)
}

////////////////////////////////////////////////////////////////////////////////
SSLParameters* SSLContext::getSupportedSSLParameters() {
    try{
        return this->contextImpl->providerGetSupportedSSLParameters();
    }
    DECAF_CATCH_RETHROW( IllegalStateException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IllegalStateException)
    DECAF_CATCHALL_THROW( IllegalStateException)
}
