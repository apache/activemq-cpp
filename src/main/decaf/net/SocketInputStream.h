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

#ifndef _DECAF_NET_SOCKETINPUTSTREAM_H_
#define _DECAF_NET_SOCKETINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/net/Socket.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace net{

    /**
     * Input stream for performing reads on a socket.  This class will only
     * work properly for blocking sockets.
     *
     * @since 1.0
     */
    class DECAF_API SocketInputStream : public io::InputStream {
    private:

        Socket::SocketHandle socket;
        bool closed;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SocketInputStream( Socket::SocketHandle socket );

        virtual ~SocketInputStream();

        /**
         * Returns the number of bytes available on the socket to
         * be read right now.
         * @return The number of bytes currently available to
         * be read on the socket.
         */
        virtual std::size_t available() const throw ( io::IOException );

        /**
         * Close - does nothing.  It is the responsibility of the owner
         * of the socket object to close it.
         * @throws IOException
         */
        virtual void close() throw( decaf::io::IOException );

        /**
         * Not supported.
         * @throws an UnsupportedOperationException.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( io::IOException,
                    lang::exceptions::UnsupportedOperationException );

    protected:

        virtual int doReadByte() throw ( io::IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                        std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    };

}}

#endif /*_DECAF_NET_SOCKETINPUTSTREAM_H_*/
