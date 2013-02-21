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
#ifndef _DECAF_INTERNAL_NET_TCP_TCPSOCKET_H_
#define _DECAF_INTERNAL_NET_TCP_TCPSOCKET_H_

#include <decaf/net/SocketException.h>
#include <decaf/net/SocketImpl.h>
#include <decaf/io/InputStream.h>
#include <decaf/io/OutputStream.h>
#include <decaf/util/Config.h>
#include <decaf/internal/AprPool.h>
#include <decaf/io/IOException.h>
#include <decaf/net/SocketTimeoutException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

namespace decaf {
namespace internal {
namespace net {
namespace tcp {

    class TcpSocketInputStream;
    class TcpSocketOutputStream;
    class TcpSocketImpl;

    /**
     * Platform-independent implementation of the socket interface.
     */
    class DECAF_API TcpSocket: public decaf::net::SocketImpl {
    private:

        TcpSocketImpl* impl;

    private:

        TcpSocket(const TcpSocket&);
        TcpSocket& operator=(const TcpSocket&);

    public:

        /**
         * Construct a non-connected socket.
         *
         * @throws SocketException thrown if an error occurs while creating the Socket.
         */
        TcpSocket();

        /**
         * Releases the socket handle but not gracefully shut down the connection.
         */
        virtual ~TcpSocket();

        /**
         * @returns true if the socketHandle is not in a disconnected state.
         */
        bool isConnected() const;

        /**
         * @returns true if the close method has been called on this Socket.
         */
        bool isClosed() const;

        /**
         * {@inheritDoc}
         */
        virtual std::string getLocalAddress() const;

        /**
         * {@inheritDoc}
         */
        virtual void create();

        /**
         * {@inheritDoc}
         */
        virtual void accept(SocketImpl* socket);

        /**
         * {@inheritDoc}
         */
        virtual void bind(const std::string& ipaddress, int port);

        /**
         * {@inheritDoc}
         */
        virtual void connect(const std::string& hostname, int port, int timeout);

        /**
         * {@inheritDoc}
         */
        virtual void listen(int backlog);

        /**
         * {@inheritDoc}
         */
        virtual decaf::io::InputStream* getInputStream();

        /**
         * {@inheritDoc}
         */
        virtual decaf::io::OutputStream* getOutputStream();

        /**
         * {@inheritDoc}
         */
        virtual int available();

        /**
         * {@inheritDoc}
         */
        virtual void close();

        /**
         * {@inheritDoc}
         */
        virtual void shutdownInput();

        /**
         * {@inheritDoc}
         */
        virtual void shutdownOutput();

        /**
         * {@inheritDoc}
         */
        virtual int getOption(int option) const;

        /**
         * {@inheritDoc}
         */
        virtual void setOption(int option, int value);

    public:

        /**
         * Reads the requested data from the Socket and write it into the passed in buffer.
         *
         * @param buffer
         *      The buffer to read into
         * @param size
         *      The size of the specified buffer
         * @param offset
         *      The offset into the buffer where reading should start filling.
         * @param length
         *      The number of bytes past offset to fill with data.
         *
         * @return the actual number of bytes read or -1 if at EOF.
         *
         * @throw IOException if an I/O error occurs during the read.
         * @throw NullPointerException if buffer is Null.
         * @throw IndexOutOfBoundsException if offset + length is greater than buffer size.
         */
        int read(unsigned char* buffer, int size, int offset, int length);

        /**
         * Writes the specified data in the passed in buffer to the Socket.
         *
         * @param buffer
         *      The buffer to write to the socket.
         * @param size
         *      The size of the specified buffer.
         * @param offset
         *      The offset into the buffer where the data to write starts at.
         * @param length
         *      The number of bytes past offset to write.
         *
         * @throw IOException if an I/O error occurs during the write.
         * @throw NullPointerException if buffer is Null.
         * @throw IndexOutOfBoundsException if offset + length is greater than buffer size.
         */
        void write(const unsigned char* buffer, int size, int offset, int length);

    protected:

        void checkResult(apr_status_t value) const;

    };

}}}}

#endif /*_DECAF_INTERNAL_NET_TCP_TCPSOCKET_H_*/
