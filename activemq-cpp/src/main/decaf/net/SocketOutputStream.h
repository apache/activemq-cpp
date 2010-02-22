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

#ifndef _DECAF_NET_SOCKETOUTPUTSTREAM_H_
#define _DECAF_NET_SOCKETOUTPUTSTREAM_H_

#include <decaf/io/OutputStream.h>
#include <decaf/net/Socket.h>

namespace decaf{
namespace net{

    /**
     * Output stream for performing write operations on a socket.
     *
     * @since 1.0
     */
    class DECAF_API SocketOutputStream : public io::OutputStream {
    private:

        // The socket.
        Socket::SocketHandle socket;
        bool closed;

    public:

        /**
         * Create a new instance of a Socket OutputStream class.
         *
         * @param socket the socket handle.
         */
        SocketOutputStream( Socket::SocketHandle socket );

        virtual ~SocketOutputStream();

        virtual void close() throw( decaf::io::IOException );

    protected:

        virtual void doWriteByte( unsigned char c ) throw ( decaf::io::IOException );

        virtual void doWriteArrayBounded( const unsigned char* buffer, std::size_t size,
                                          std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::NullPointerException,
                    decaf::lang::exceptions::IndexOutOfBoundsException );

    };

}}

#endif /*_DECAF_NET_SOCKETOUTPUTSTREAM_H_*/
