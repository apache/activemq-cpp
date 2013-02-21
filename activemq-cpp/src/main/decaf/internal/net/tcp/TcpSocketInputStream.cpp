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

#include <decaf/internal/net/tcp/TcpSocketInputStream.h>

#include <decaf/net/Socket.h>
#include <decaf/io/IOException.h>
#include <decaf/internal/net/tcp/TcpSocket.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::tcp;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
TcpSocketInputStream::TcpSocketInputStream(TcpSocket* socket) :
    InputStream(), socket(socket), closed(false) {

    if (socket == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "TcpSocket instance passed was NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
TcpSocketInputStream::~TcpSocketInputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocketInputStream::close() {

    if (this->closed) {
        return;
    }

    try {
        this->closed = true;
        this->socket->close();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocketInputStream::available() const {

    if (this->closed) {
        throw IOException(__FILE__, __LINE__, "The stream is closed");
    }

    return this->socket->available();
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocketInputStream::doReadByte() {

    if (this->closed) {
        throw IOException(__FILE__, __LINE__, "The stream is closed");
    }

    try {

        unsigned char buffer[1];
        int result = this->socket->read(buffer, 1, 0, 1);
        return result == -1 ? result : buffer[0];
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocketInputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    if (closed) {
        throw IOException(__FILE__, __LINE__, "The stream is closed");
    }

    if (buffer == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Buffer passed was NULL.");
    }

    try {
        return this->socket->read(buffer, size, offset, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long TcpSocketInputStream::skip(long long num) {

    try {

        if (num == 0) {
            return 0;
        }

        return InputStream::skip(num);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(UnsupportedOperationException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}
