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

#include "SocketOutputStream.h"
#include <decaf/util/Config.h>
#include <decaf/lang/Character.h>
#include "SocketError.h"

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::util;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::SocketOutputStream( Socket::SocketHandle socket ) {
    this->socket = socket;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::~SocketOutputStream() {
    close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::close() throw( lang::Exception ) {
    this->closed = true;
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( unsigned char c ) throw ( IOException ) {
    write( &c, 1 );
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( const unsigned char* buffer, std::size_t len )
    throw ( IOException ) {

    apr_size_t remaining = (apr_size_t)len;
    apr_status_t result = APR_SUCCESS;

    while( remaining > 0 && !closed ) {
        // On input remaining is the bytes to send, after return remaining
        // is the amount actually sent.
        result = apr_socket_send( socket, (const char*)buffer, &remaining );

        if( result != APR_SUCCESS || closed ) {
            throw IOException(
                __FILE__, __LINE__,
                "decaf::net::SocketOutputStream::write - %s",
                SocketError::getErrorString().c_str() );
        }

        // move us to next position to write, or maybe end.
        buffer += remaining;
        remaining = len - remaining;
    }
}
