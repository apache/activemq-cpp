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

#include "DefaultServerSocketFactory.h"

#include <decaf/net/ServerSocket.h>
#include <decaf/net/SocketException.h>

#include <memory>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
DefaultServerSocketFactory::DefaultServerSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
DefaultServerSocketFactory::~DefaultServerSocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultServerSocketFactory::createServerSocket() {

    try{
        std::auto_ptr<ServerSocket> socket( new ServerSocket() );
        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultServerSocketFactory::createServerSocket( int port ) {

    try{
        std::auto_ptr<ServerSocket> socket( new ServerSocket( port ) );
        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultServerSocketFactory::createServerSocket( int port, int backlog ) {

    try{
        std::auto_ptr<ServerSocket> socket( new ServerSocket( port, backlog ) );
        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket* DefaultServerSocketFactory::createServerSocket( int port, int backlog, const InetAddress* address ) {

    try{
        std::auto_ptr<ServerSocket> socket( new ServerSocket( port, backlog, address ) );
        return socket.release();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    DECAF_CATCHALL_THROW( IOException )
}
