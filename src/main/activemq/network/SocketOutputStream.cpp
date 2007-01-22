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

#ifdef HAVE_WINSOCK2_H
    #include <Winsock2.h>
#else
    #include <sys/socket.h>
    extern int errno;
#endif

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#if !defined(SOCKET_NOSIGNAL)

    #if defined(SO_NOSIGPIPE)
        #define SOCKET_NOSIGNAL SO_NOSIGPIPE
    #elif defined(MSG_NOSIGNAL)
        #define SOCKET_NOSIGNAL MSG_NOSIGNAL
    #else
        #define SOCKET_NOSIGNAL 0
    #endif

#endif

using namespace activemq::network;
using namespace activemq::io;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::SocketOutputStream( Socket::SocketHandle socket )
{
    this->socket = socket;
    this->debug = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketOutputStream::~SocketOutputStream()
{
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( unsigned char c ) throw (IOException)
{
    write( &c, 1 );
}

////////////////////////////////////////////////////////////////////////////////
void SocketOutputStream::write( const unsigned char* buffer, int len ) 
    throw (IOException)
{
    int remaining = len;
    int sendOpts = SOCKET_NOSIGNAL;

    if( debug ){
        printf("SocketOutputStream:write(), numbytes:%d -", len);
        for( int ix=0; ix<len; ++ix ){
            char c = buffer[ix];
            if( c > 20 ){
                printf("%c", c );
            }
            else printf("[%d]", c );
        }
        printf("\n" );
    }
        
    while( remaining > 0 )
    {
        int length = ::send( socket, (const char*)buffer, remaining, sendOpts );      	
        if( length < 0 ){
            throw IOException( __FILE__, __LINE__, 
                "activemq::io::SocketOutputStream::write - %s", ::strerror(errno) );
        }
         
        buffer+=length;
        remaining -= length;
    }
}

