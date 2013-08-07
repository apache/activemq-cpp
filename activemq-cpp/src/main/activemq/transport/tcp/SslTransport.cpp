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

#include "SslTransport.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>
#include <decaf/net/ssl/SSLSocket.h>
#include <decaf/net/ssl/SSLSocketFactory.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace activemq;
using namespace activemq::io;
using namespace activemq::transport;
using namespace activemq::transport::tcp;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::util;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SslTransport::SslTransport(const Pointer<Transport> next, const decaf::net::URI& location) :
    TcpTransport(next, location) {
}

////////////////////////////////////////////////////////////////////////////////
SslTransport::~SslTransport() {
}

////////////////////////////////////////////////////////////////////////////////
Socket* SslTransport::createSocket() {

    try {
        // The pointer returned from getDefault is owned by the SSLSocketFactory
        SocketFactory* factory = SSLSocketFactory::getDefault();
        return factory->createSocket();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void SslTransport::configureSocket(Socket* socket) {

    try {

        if (socket == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Socket instance passed was NULL");
        }

        SSLSocket* sslSocket = dynamic_cast<SSLSocket*>(socket);
        if (sslSocket == NULL) {
            throw IllegalArgumentException(__FILE__, __LINE__,
                "Socket passed was not an SSLSocket instance.");
        }

        SSLParameters params = sslSocket->getSSLParameters();

        std::vector<std::string> serverNames;
        serverNames.push_back(this->getLocation().getHost());
        params.setServerNames(serverNames);

        sslSocket->setSSLParameters(params);

        TcpTransport::configureSocket(socket);
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IllegalArgumentException)
    DECAF_CATCH_RETHROW(SocketException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, SocketException)
    DECAF_CATCHALL_THROW(SocketException)
}
