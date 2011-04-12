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

#ifndef _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETINPUTSTREAM_H_
#define _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETINPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/InputStream.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    class OpenSSLSocket;

    /**
     * An output stream for reading data from an OpenSSL Socket instance.
     *
     * @since 1.0
     */
    class DECAF_API OpenSSLSocketInputStream : public decaf::io::InputStream {
    private:

        OpenSSLSocket* socket;
        volatile bool closed;

    private:

        OpenSSLSocketInputStream( const OpenSSLSocketInputStream& );
        OpenSSLSocketInputStream& operator= ( const OpenSSLSocketInputStream& );

    public:

        OpenSSLSocketInputStream( OpenSSLSocket* socket );

        virtual ~OpenSSLSocketInputStream();

        /**
         * {@inheritDoc}
         */
        virtual int available() const;

        /**
         * Close - does nothing.  It is the responsibility of the owner
         * of the socket object to close it.
         *
         * {@inheritDoc}
         */
        virtual void close();

        /**
         * Not supported.
         *
         * {@inheritDoc}
         */
        virtual long long skip( long long num );

    protected:

        virtual int doReadByte();

        virtual int doReadArrayBounded( unsigned char* buffer, int size, int offset, int length );

    };

}}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETINPUTSTREAM_H_ */
