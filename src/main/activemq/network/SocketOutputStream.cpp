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
#include <activemq/util/Config.h>
#include <activemq/util/Character.h>
#include "SocketError.h"

#ifdef HAVE_WINSOCK2_H
    #include <Winsock2.h>
#else
    #include <sys/socket.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#if defined(SOCKET_NOSIGNAL)
    #define AMQ_SEND_OPTS SOCKET_NOSIGNAL
#elif defined(MSG_NOSIGNAL)
    #define AMQ_SEND_OPTS MSG_NOSIGNAL
#else
    #define AMQ_SEND_OPTS 0
#endif

using namespace activemq::network;
using namespace activemq::io;
using namespace activemq::util;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::SocketOutputStream( Socket::SocketHandle socket )
{
    this->socket = socket;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::~SocketOutputStream() {
    close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::close() throw( cms::CMSException ) {
    this->closed = true;
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( unsigned char c ) throw (IOException)
{
    write( &c, 1 );
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( const unsigned char* buffer, std::size_t len )
    throw (IOException)
{
    std::size_t remaining = len;
    int sendOpts = AMQ_SEND_OPTS;

    while( remaining > 0 && !closed )
    {
        int length = ::send( socket, (const char*)buffer, (int)remaining, sendOpts );
        if( length == -1 && !closed ){
            throw IOException( __FILE__, __LINE__,
                "activemq::io::SocketOutputStream::write - %s", SocketError::getErrorString().c_str() );
        }

        buffer+=length;
        remaining -= length;
    }
}

