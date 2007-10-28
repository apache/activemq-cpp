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

#include <activemq/util/Config.h>

#ifdef AMQ_HAVE_OPENSSL

#include "SSLSocketInputStream.h"
#include "SSLError.h"
#include "SocketError.h"

using namespace activemq;
using namespace activemq::network;
using namespace activemq::io;
using namespace activemq::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SSLSocketInputStream::SSLSocketInputStream(
        network::Socket::SocketHandle socket, SSL* ssl )
: SocketInputStream( socket ),
ssl( ssl )
{
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SSLSocketInputStream::read( unsigned char* buffer,
        std::size_t bufferSize )
throw (IOException)
{
    int len = 0;

    // Loop to ignore any signal interruptions and repeatble errors
    // that occur during the read.
    while( !closed ) {

        // Read data from the socket.
        len = SSL_read( ssl, buffer, bufferSize );

        switch (SSL_get_error( ssl, len ))
        {
            case SSL_ERROR_NONE:
            return len;

            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
            // Repeat the operation.
            break;

            case SSL_ERROR_ZERO_RETURN:
            throw IOException( __FILE__, __LINE__,
                    "activemq::io::SSLSocketInputStream::read"
                    " - The connection is broken" );

            case SSL_ERROR_SSL:
            throw IOException( __FILE__, __LINE__,
                    "activemq::io::SSLSocketInputStream::read"
                    " - %s",
                    SSLError::getErrorString().c_str());

            case SSL_ERROR_SYSCALL:
            if( SocketError::getErrorCode() != SocketError::INTERRUPTED )
            throw IOException( __FILE__, __LINE__,
                    "activemq::io::SSLSocketInputStream::read"
                    " - %s",
                    SocketError::getErrorString().c_str() );
            break;
        }
    }

    // Not reached, but avoid warnings.
    return 0;
}

#endif /* AMQ_HAVE_OPENSSL */
