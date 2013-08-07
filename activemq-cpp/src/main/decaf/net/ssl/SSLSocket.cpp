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

#include "SSLSocket.h"

#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket() : Socket() {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket(const InetAddress* address, int port) : Socket(address, port) {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket(const InetAddress* address, int port, const InetAddress* localAddress, int localPort) : Socket(address, port, localAddress, localPort) {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket(const std::string& host, int port) : Socket(host, port) {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocket::SSLSocket(const std::string& host, int port, const InetAddress* localAddress, int localPort) : Socket(host, port, localAddress, localPort) {
}

////////////////////////////////////////////////////////////////////////////////
SSLSocket::~SSLSocket() {
}

////////////////////////////////////////////////////////////////////////////////
SSLParameters SSLSocket::getSSLParameters() const {

    SSLParameters params(this->getEnabledCipherSuites(), this->getEnabledProtocols());

    return params;
}

////////////////////////////////////////////////////////////////////////////////
void SSLSocket::setSSLParameters(const SSLParameters& value) {

    try {

        if (!value.getCipherSuites().empty()) {
            this->setEnabledCipherSuites(value.getCipherSuites());
        }

        if (!value.getProtocols().empty()) {
            this->setEnabledProtocols(value.getProtocols());
        }
    }
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IllegalArgumentException)
    DECAF_CATCHALL_THROW(IllegalArgumentException)
}
