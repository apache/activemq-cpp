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

#include "BlockingByteArrayInputStream.h"
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::BlockingByteArrayInputStream() :
    InputStream(), buffer(), pos(buffer.end()), closing(false) {
}

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::BlockingByteArrayInputStream(const unsigned char* buffer, int bufferSize) :
    InputStream(), buffer(), pos(), closing(false) {

    setByteArray(buffer, bufferSize);
}

////////////////////////////////////////////////////////////////////////////////
BlockingByteArrayInputStream::~BlockingByteArrayInputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void BlockingByteArrayInputStream::setByteArray(const unsigned char* lbuffer, int lbufferSize) {

    synchronized(this) {

        // Remove old data
        this->buffer.clear();
        this->buffer.reserve(lbufferSize);

        // Copy data to internal buffer.
        this->buffer.insert(this->buffer.begin(), lbuffer, lbuffer + lbufferSize);

        // Begin at the Beginning.
        this->pos = this->buffer.begin();

        // Notify any listening threads that there is now data available.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
int BlockingByteArrayInputStream::available() const {
    return (int) std::distance(pos, buffer.end());
}

////////////////////////////////////////////////////////////////////////////////
void BlockingByteArrayInputStream::close() {

    synchronized(this) {

        // Indicate that we're shutting down.
        closing = true;

        // Clear out the buffer.
        buffer.clear();

        // Notify that this stream is shutting down.
        notifyAll();
    }
}

////////////////////////////////////////////////////////////////////////////////
int BlockingByteArrayInputStream::doReadByte() {

    try {

        synchronized(this) {

            while (!closing) {

                if (pos != buffer.end()) {
                    return *(pos++);
                }

                // Wait for more data
                wait();
            }

            throw IOException(__FILE__, __LINE__, "close occurred during read");
        }

        return 0;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int BlockingByteArrayInputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    if (length == 0) {
        return 0;
    }

    if (buffer == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "BlockingByteArrayInputStream::read - Passed buffer is Null");
    }

    if (size < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
    }

    if (offset > size || offset < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
    }

    if (length < 0 || length > size - offset) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
    }

    try {

        synchronized(this) {

            int ix = 0;

            for (; ix < length && !closing; ++ix) {

                if (pos == this->buffer.end()) {
                    // Wait for more data to come in.
                    wait();
                }

                if (!closing && pos != this->buffer.end()) {
                    buffer[ix + offset] = *(pos);
                    ++pos;
                }
            }

            if (closing) {
                throw IOException(__FILE__, __LINE__, "BlockingByteArrayInputStream::read - close occurred during read");
            }

            return (int) ix;
        }

        return 0;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long BlockingByteArrayInputStream::skip(long long num) {

    long long ix = 0;

    synchronized(this) {
        // Increment the pos until we'v skipped the desired num
        // or we've hit the end of the buffer.
        for (; ix < num && !closing && pos != buffer.end(); ++ix, ++pos) {}
    }

    return ix;
}
