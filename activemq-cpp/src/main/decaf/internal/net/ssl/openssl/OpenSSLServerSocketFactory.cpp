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

#include "OpenSSLServerSocketFactory.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <decaf/internal/net/ssl/openssl/OpenSSLParameters.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocket.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLContextSpi.h>

#include <memory>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocketFactory::OpenSSLServerSocketFactory( OpenSSLContextSpi* parent ) :
    SSLServerSocketFactory(), parent( parent ) {

    if( parent == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Parent Pointer was NULL." );
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocketFactory::~OpenSSLServerSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* OpenSSLServerSocketFactory::createServerSocket() {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* OpenSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED ) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* OpenSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED, int backlog DECAF_UNUSED ) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* OpenSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED, int backlog DECAF_UNUSED, const InetAddress* address DECAF_UNUSED ) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocketFactory::getDefaultCipherSuites() {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLServerSocketFactory::getSupportedCipherSuites() {
    return std::vector<std::string>();
}
