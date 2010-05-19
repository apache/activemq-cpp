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

#include "DefaultSSLServerSocketFactory.h"

#include <decaf/io/IOException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;

////////////////////////////////////////////////////////////////////////////////
DefaultSSLServerSocketFactory::DefaultSSLServerSocketFactory( const std::string& errorMessage ) :
    SSLServerSocketFactory(), errorMessage( errorMessage ) {

}

////////////////////////////////////////////////////////////////////////////////
DefaultSSLServerSocketFactory::~DefaultSSLServerSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultSSLServerSocketFactory::createServerSocket() {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED, int backlog DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultSSLServerSocketFactory::createServerSocket( int port DECAF_UNUSED,
                                                                 int backlog DECAF_UNUSED,
                                                                 const InetAddress* address DECAF_UNUSED ) {

    throw IOException( __FILE__, __LINE__, errorMessage.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> DefaultSSLServerSocketFactory::getDefaultCipherSuites() {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> DefaultSSLServerSocketFactory::getSupportedCipherSuites() {
    return std::vector<std::string>();
}
