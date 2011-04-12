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

#include <decaf/net/InetAddress.h>
#include <decaf/net/Socket.h>
#include <decaf/net/SocketException.h>

#include <memory>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
DefaultSocketFactory::DefaultSocketFactory() : SocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
DefaultSocketFactory::~DefaultSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket() {

    try{

        std::auto_ptr<Socket> socket( new Socket() );

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket( const decaf::net::InetAddress* host, int port ) {

    try {

        // Ensure something is actually passed in for the URI
        if( host == NULL ) {
            throw SocketException( __FILE__, __LINE__, "host address not provided" );
        }

        if( port <= 0 ) {
            throw SocketException( __FILE__, __LINE__, "valid port not provided" );
        }

        std::auto_ptr<Socket> socket( new Socket( host, port ) );

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket( const decaf::net::InetAddress* host, int port,
                                            const decaf::net::InetAddress* ifAddress, int localPort ) {

    try {

        // Ensure something is actually passed in for the URI
        if( host == NULL ) {
            throw SocketException( __FILE__, __LINE__, "host addres not provided" );
        }

        if( port <= 0 ) {
            throw SocketException( __FILE__, __LINE__, "valid port not provided" );
        }

        std::auto_ptr<Socket> socket( new Socket( host, port, ifAddress, localPort ) );

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket( const std::string& hostname, int port ) {

    try {

        // Ensure something is actually passed in for the URI
        if( hostname == "" ) {
            throw SocketException( __FILE__, __LINE__, "uri not provided" );
        }

        if( port <= 0 ) {
            throw SocketException( __FILE__, __LINE__, "valid port not provided" );
        }

        std::auto_ptr<Socket> socket( new Socket( hostname, port ) );

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
Socket* DefaultSocketFactory::createSocket( const std::string& hostname, int port,
                                            const InetAddress* ifAddress, int localPort ) {

    try {

        // Ensure something is actually passed in for the URI
        if( hostname == "" ) {
            throw SocketException( __FILE__, __LINE__, "uri not provided" );
        }

        if( port <= 0 ) {
            throw SocketException( __FILE__, __LINE__, "valid port not provided" );
        }

        std::auto_ptr<Socket> socket( new Socket( hostname, port, ifAddress, localPort ) );

        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}
