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

#ifndef ACTIVEMQ_NETWORK_SSLSOCKETINPUTSTREAM_H_
#define ACTIVEMQ_NETWORK_SSLSOCKETINPUTSTREAM_H_

#include <activemq/network/SocketInputStream.h>
#include <activemq/util/Config.h>

#ifdef AMQ_HAVE_OPENSSL
#include <openssl/ssl.h>

namespace activemq{
namespace network{

    /**
     * Input stream for performing reads on a socket.  This
     * class will only work properly for blocking sockets.
     */
    class SSLSocketInputStream : public SocketInputStream
    {
    private:

	SSL* ssl;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SSLSocketInputStream( Socket::SocketHandle socket, SSL* ssl );

        /**
         * Reads an array of bytes from the buffer.  If no data
         * is available, blocks until there is.
         * @param buffer (out) the target buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer,
                                  std::size_t bufferSize )
            throw (io::IOException);
    };

}}

#endif /* AMQ_HAVE_OPENSSL */
#endif /*ACTIVEMQ_NETWORK_SSLSOCKETINPUTSTREAM_H_*/
