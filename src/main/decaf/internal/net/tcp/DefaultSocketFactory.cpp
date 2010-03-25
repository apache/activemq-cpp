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

#include "DefaultSocketFactory.h"

#include <decaf/net/Socket.h>
#include <decaf/net/SocketException.h>

#include <decaf/internal/net/tcp/TcpSocket.h>

#include <memory>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::tcp;

////////////////////////////////////////////////////////////////////////////////
DefaultSocketFactory::DefaultSocketFactory() : SocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
DefaultSocketFactory::~DefaultSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket()
    throw( decaf::io::IOException ) {

    return new TcpSocket();
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket( const std::string& hostname, int port )
    throw( decaf::io::IOException, decaf::net::UnknownHostException ) {

    try {

        // Ensure something is actually passed in for the URI
        if( hostname == "" ) {
            throw SocketException( __FILE__, __LINE__, "uri not provided" );
        }

        if( port <= 0 ) {
            throw SocketException( __FILE__, __LINE__, "valid port not provided" );
        }

        std::auto_ptr<TcpSocket> tcpSocket( new TcpSocket() );

        // Connect the socket.
        tcpSocket->connect( hostname.c_str(), port );

        return tcpSocket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}
