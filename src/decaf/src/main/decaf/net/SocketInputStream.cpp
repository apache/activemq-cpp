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

#include <decaf/util/Config.h>

#if !defined(HAVE_WINSOCK2_H)
    #include <sys/select.h>
    #include <sys/socket.h>
#else
    #include <Winsock2.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#define BSD_COMP /* Get FIONREAD on Solaris2. */
#include <sys/ioctl.h>
#endif

// Pick up FIONREAD on Solaris 2.5.
#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#include <decaf/net/SocketInputStream.h>
#include <decaf/net/SocketError.h>
#include <decaf/io/IOException.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::SocketInputStream( net::Socket::SocketHandle socket )
{
    this->socket = socket;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::~SocketInputStream(){}

////////////////////////////////////////////////////////////////////////////////
void SocketInputStream::close() throw( cms::CMSException ){
    this->closed = true;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SocketInputStream::available() const throw ( io::IOException ){

// The windows version
#if defined(HAVE_WINSOCK2_H)

    unsigned long numBytes = 0;

    if (::ioctlsocket (socket, FIONREAD, &numBytes) == SOCKET_ERROR){
        throw SocketException( __FILE__, __LINE__, "ioctlsocket failed" );
    }

    return (std::size_t)numBytes;

#else // !defined(HAVE_WINSOCK2_H)

    // If FIONREAD is defined - use ioctl to find out how many bytes
    // are available.
    #if defined(FIONREAD)

        std::size_t numBytes = 0;
        if( ::ioctl (socket, FIONREAD, &numBytes) != -1 ){
            return numBytes;
        }

    #endif

    // If we didn't get anything we can use select.  This is a little
    // less functional.  We will poll on the socket - if there is data
    // available, we'll return 1, otherwise we'll return zero.
    #if defined(HAVE_SELECT)

        fd_set rd;
        FD_ZERO(&rd);
        FD_SET( socket, &rd );
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int returnCode = ::select(socket+1, &rd, NULL, NULL, &tv);
        if(returnCode == -1){
            throw IOException(__FILE__, __LINE__, SocketError::getErrorString().c_str() );
        }
        return (returnCode == 0)? 0 : 1;

    #else

        return 0;

    #endif /* HAVE_SELECT */


#endif // !defined(HAVE_WINSOCK2_H)
}

////////////////////////////////////////////////////////////////////////////////
unsigned char SocketInputStream::read() throw ( IOException ){

    unsigned char c;
    std::size_t len = read( &c, 1 );
    if( len != sizeof(c)  && !closed ){
        throw IOException( __FILE__, __LINE__,
            "activemq::io::SocketInputStream::read - failed reading a byte");
    }

    return c;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SocketInputStream::read( unsigned char* buffer,
                                     std::size_t bufferSize ) throw ( IOException )
{
    int len = 0;

    // Loop to ignore any signal interruptions that occur during the read.
    do {

        // Read data from the socket.
        len = ::recv(socket, (char*)buffer, (int)bufferSize, 0);

        // Check for a closed socket.
        if( len == 0 && !closed ){
            throw IOException( __FILE__, __LINE__,
                "activemq::io::SocketInputStream::read - The connection is broken" );
        }

    } while( !closed && len == -1 &&
             SocketError::getErrorCode() == SocketError::INTERRUPTED );

    // Check for error.
    if( len == -1 && !closed ){

        // Otherwise, this was a bad error - throw an exception.
        throw IOException( __FILE__, __LINE__,
                "activemq::io::SocketInputStream::read - %s", SocketError::getErrorString().c_str() );
    }

    return len;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SocketInputStream::skip( std::size_t num DECAF_UNUSED )
throw ( io::IOException, lang::exceptions::UnsupportedOperationException ) {
    throw lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "skip() method is not supported");
}

