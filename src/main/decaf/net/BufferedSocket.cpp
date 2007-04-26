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

#include "BufferedSocket.h"

#include <decaf/lang/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
BufferedSocket::BufferedSocket( Socket* socket,
                                int inputBufferSize,
                                int outputBufferSize,
                                bool own ) :
    socket(NULL),
    own(false),
    inputStream(NULL),
    outputStream(NULL),
    inputBufferSize(0),
    outputBufferSize(0)
{
    if(inputBufferSize < 0 || outputBufferSize < 0 )
    {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "BufferedSocket::BufferedSocket - buffer sizes must be >=0! "
            "Given input buffer size: %d, Given output buffer size: %d",
            inputBufferSize,
            outputBufferSize );
    }

    if(socket == NULL)
    {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "BufferedSocket::BufferedSocket - Constructed with NULL Socket");
    }

    this->socket = socket;
    this->inputBufferSize = inputBufferSize;
    this->outputBufferSize = outputBufferSize;
    this->own = own;
}

////////////////////////////////////////////////////////////////////////////////
BufferedSocket::~BufferedSocket()
{
    try
    {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void BufferedSocket::connect( const char* host, int port )
    throw( SocketException )
{
    try
    {
        if( socket->isConnected() )
        {
            throw SocketException( __FILE__, __LINE__,
                 "BufferedSocket::connect() - socket already connected" );
        }

        // Connect the socket.
        socket->connect( host, port );

        // Now create the buffered streams that wrap around the socket.
        inputStream = new BufferedInputStream(
            socket->getInputStream(), (std::size_t)inputBufferSize );
        outputStream = new BufferedOutputStream(
            socket->getOutputStream(), (std::size_t)outputBufferSize );
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void BufferedSocket::close() throw( cms::CMSException )
{
    try
    {
        if( outputStream != NULL )
        {
            // Ensure all data is written
            outputStream->flush();

            delete outputStream;
            outputStream = NULL;
        }

        if( inputStream != NULL ){
            delete inputStream;
            inputStream = NULL;
        }

        if( socket != NULL ){
            // Close the socket
            try{
                socket->close();
            } catch( cms::CMSException& ex ){ /* Absorb */ }

            // if we own it, delete it.
            if( own ) {
                delete socket;
            }
            socket = NULL;
        }
    }
    AMQ_CATCH_RETHROW( SocketException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, SocketException )
    AMQ_CATCHALL_THROW( SocketException )
}
