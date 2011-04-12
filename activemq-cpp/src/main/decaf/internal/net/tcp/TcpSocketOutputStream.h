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

#ifndef _DECAF_INTERNAL_NET_TCP_TCPSOCKETOUTPUTSTREAM_H_
#define _DECAF_INTERNAL_NET_TCP_TCPSOCKETOUTPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/OutputStream.h>

namespace decaf {
namespace internal {
namespace net {
namespace tcp {

    class TcpSocket;

    /**
     * Output stream for performing write operations on a socket.
     *
     * @since 1.0
     */
    class DECAF_API TcpSocketOutputStream : public decaf::io::OutputStream {
    private:

        TcpSocket* socket;
        volatile bool closed;

    private:

        TcpSocketOutputStream( const TcpSocketOutputStream& );
        TcpSocketOutputStream& operator= ( const TcpSocketOutputStream& );

    public:

        /**
         * Create a new instance of a Socket OutputStream class.
         *
         * @param socket
         *      The socket to use to write out the data.
         */
        TcpSocketOutputStream( TcpSocket* socket );

        virtual ~TcpSocketOutputStream();

        virtual void close();

    protected:

        virtual void doWriteByte( unsigned char c );

        virtual void doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length );

    };

}}}}

#endif /*_DECAF_INTERNAL_NET_TCP_TCPSOCKETOUTPUTSTREAM_H_*/
