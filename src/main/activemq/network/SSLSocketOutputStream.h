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

#ifndef ACTIVEMQ_NETWORK_SSLSOCKETOUTPUTSTREAM_H_
#define ACTIVEMQ_NETWORK_SSLSOCKETOUTPUTSTREAM_H_

#include <activemq/network/SocketOutputStream.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/Config.h>

#ifdef AMQ_HAVE_OPENSSL
#include <openssl/ssl.h>

namespace activemq{
namespace network{

    /**
     * Output stream for performing write operations
     * on a socket.
     */
    class SSLSocketOutputStream : public SocketOutputStream
    {
    private:

	SSL* ssl;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SSLSocketOutputStream( Socket::SocketHandle socket, SSL* ssl );

        /**
         * Writes an array of bytes to the output stream.
         * @param buffer The array of bytes to write.
         * @param len The number of bytes from the buffer to be written.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t len ) throw ( io::IOException );

    };

}}

#endif /* AMQ_HAVE_OPENSSL */
#endif /*ACTIVEMQ_NETWORK_SSLSOCKETOUTPUTSTREAM_H_*/
