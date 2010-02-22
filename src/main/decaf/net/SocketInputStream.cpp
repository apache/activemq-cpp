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
#include <unistd.h>
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

#include <apr_portable.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::SocketInputStream( net::Socket::SocketHandle socket ) : InputStream() {
    this->socket = socket;
    this->closed = false;
}

////////////////////////////////////////////////////////////////////////////////
SocketInputStream::~SocketInputStream(){}

////////////////////////////////////////////////////////////////////////////////
void SocketInputStream::close() throw( decaf::io::IOException ){
    this->closed = true;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SocketInputStream::available() const throw ( io::IOException ){

    // Check for a closed call from socket class, if closed then this read fails.
    if( closed ){
        throw IOException(
            __FILE__, __LINE__,
            "decaf::io::SocketInputStream::available - The stream is closed" );
    }

    // Convert to an OS level socket.
    apr_os_sock_t oss;
    apr_os_sock_get( (apr_os_sock_t*)&oss, socket );

// The windows version
#if defined(HAVE_WINSOCK2_H)

    unsigned long numBytes = 0;

    if( ::ioctlsocket( oss, FIONREAD, &numBytes ) == SOCKET_ERROR ){
        throw SocketException( __FILE__, __LINE__, "ioctlsocket failed" );
    }

    return (std::size_t)numBytes;

#else // !defined(HAVE_WINSOCK2_H)

    // If FIONREAD is defined - use ioctl to find out how many bytes
    // are available.
    #if defined(FIONREAD)

        std::size_t numBytes = 0;
        if( ::ioctl( oss, FIONREAD, &numBytes ) != -1 ){
            return numBytes;
        }

    #endif

    // If we didn't get anything we can use select.  This is a little
    // less functional.  We will poll on the socket - if there is data
    // available, we'll return 1, otherwise we'll return zero.
    #if defined(HAVE_SELECT)

        fd_set rd;
        FD_ZERO(&rd);
        FD_SET( oss, &rd );
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int returnCode = ::select( oss+1, &rd, NULL, NULL, &tv );
        if( returnCode == -1 ){
            throw IOException(
                __FILE__, __LINE__,
                SocketError::getErrorString().c_str() );
        }
        return (returnCode == 0) ? 0 : 1;

    #else

        return 0;

    #endif /* HAVE_SELECT */

#endif // !defined(HAVE_WINSOCK2_H)
}

////////////////////////////////////////////////////////////////////////////////
int SocketInputStream::doReadByte() throw ( IOException ){

    // Check for a closed call from socket class, if closed then this read fails.
    if( closed ){
        throw IOException(
            __FILE__, __LINE__,
            "decaf::io::SocketInputStream::read - The Stream has been closed" );
    }

    apr_status_t result = APR_SUCCESS;
    char c;
    apr_size_t size = 1;

    result = apr_socket_recv( socket, &c, &size );

    if( ( size != sizeof(c) && !closed ) || result != APR_SUCCESS ){
        throw IOException( __FILE__, __LINE__,
            "activemq::io::SocketInputStream::read - failed reading a byte");
    }

    return c;
}

////////////////////////////////////////////////////////////////////////////////
int SocketInputStream::doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                           std::size_t offset, std::size_t length )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::IndexOutOfBoundsException,
            decaf::lang::exceptions::NullPointerException ) {

    // Check for a closed call from socket class, if closed then this read fails.
    if( closed ){
        throw IOException(
            __FILE__, __LINE__,
            "decaf::io::SocketInputStream::read - The Stream has been closed" );
    }

    if( length == 0 ) {
        return 0;
    }

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__,
            "SocketInputStream::read - Buffer passed is Null" );
    }

    if( length > size - offset ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__,
            "Given size{%d} - offset{%d} is less than length{%d}.", size, offset, length );
    }

    apr_size_t aprSize = (apr_size_t)length;
    apr_status_t result = APR_SUCCESS;

    // Read data from the socket, size on input is size of buffer, when done
    // size is the number of bytes actually read, can be <= bufferSize.
    result = apr_socket_recv( socket, (char*)buffer + offset, &aprSize );

    // Check for EOF, on windows we only get size==0 so check that to, if we
    // were closed though then we throw an IOException so the caller knows we
    // aren't usable anymore.
    if( ( APR_STATUS_IS_EOF( result ) || aprSize == 0 ) && !closed ) {
        return -1;
    }

    // Check for a closed call from socket class, if closed then this read fails.
    if( closed ){
        throw IOException(
            __FILE__, __LINE__,
            "decaf::io::SocketInputStream::read - The connection is broken" );
    }

    // Check for error.
    if( result != APR_SUCCESS ){
        throw IOException(
            __FILE__, __LINE__,
            "decaf::net::SocketInputStream::read - %s",
            SocketError::getErrorString().c_str() );
    }

    return (int)aprSize;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SocketInputStream::skip( std::size_t num DECAF_UNUSED )
    throw ( io::IOException, lang::exceptions::UnsupportedOperationException ) {

    throw lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__,
        "SocketInputStream::skip() method is not supported");
}
