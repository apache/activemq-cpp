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

#include "DefaultSSLSocketFactory.h"

#include <decaf/io/IOException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;

////////////////////////////////////////////////////////////////////////////////
DefaultSSLSocketFactory::DefaultSSLSocketFactory( const std::string& errorMessage ) :
     SSLSocketFactory(), errorMessage( errorMessage ) {
}

////////////////////////////////////////////////////////////////////////////////
DefaultSSLSocketFactory::~DefaultSSLSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
decaf::net::Socket* DefaultSSLSocketFactory::createSocket() {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSSLSocketFactory::createSocket( const decaf::net::InetAddress* host DECAF_UNUSED, int port DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSSLSocketFactory::createSocket( const decaf::net::InetAddress* host DECAF_UNUSED, int port DECAF_UNUSED,
                                               const decaf::net::InetAddress* ifAddress DECAF_UNUSED, int localPort DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
decaf::net::Socket* DefaultSSLSocketFactory::createSocket( const std::string& name DECAF_UNUSED, int port DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
decaf::net::Socket* DefaultSSLSocketFactory::createSocket( const std::string& name DECAF_UNUSED, int port DECAF_UNUSED,
                                                           const InetAddress* ifAddress DECAF_UNUSED, int localPort DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> DefaultSSLSocketFactory::getDefaultCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> DefaultSSLSocketFactory::getSupportedCipherSuites() {

    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSSLSocketFactory::createSocket( Socket* socket DECAF_UNUSED, std::string host DECAF_UNUSED,
                                               int port DECAF_UNUSED, bool autoClose DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}
