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

#include "BufferedInputStream.h"

#include <decaf/lang/System.h>

#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::BufferedInputStream(InputStream* stream, bool own) :
    FilterInputStream(stream, own), pos(0), count(0), markLimit(-1), markPos(-1), bufferSize(8192), buff(new unsigned char[bufferSize]), proxyBuffer(buff) {
}

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::BufferedInputStream(InputStream* stream, int bufferSize, bool own) :
    FilterInputStream(stream, own), pos(0), count(0), markLimit(-1), markPos(-1), bufferSize(bufferSize), buff(NULL), proxyBuffer(NULL) {

    if (bufferSize < 0) {
        throw new IllegalArgumentException(__FILE__, __LINE__, "Size must be greater than zero");
    }

    this->buff = new unsigned char[bufferSize];
    this->proxyBuffer = this->buff;
}

////////////////////////////////////////////////////////////////////////////////
BufferedInputStream::~BufferedInputStream() {
    try {
        this->close();
    }
    DECAF_CATCHALL_NOTHROW()

    delete[] this->buff;
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStream::close() {

    try {
        // let parent close the inputStream
        FilterInputStream::close();

        // Clear the proxy to the buffer, but don't actually delete the buffer yet since
        // other methods holding onto the buffer while blocked.
        this->proxyBuffer = NULL;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::available() const {

    if (this->proxyBuffer == NULL || this->isClosed()) {
        throw IOException(__FILE__, __LINE__, "BufferedInputStream::available - Buffer was closed");
    }

    return (this->count - this->pos) + inputStream->available();
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStream::mark(int readLimit) {
    if (this->proxyBuffer != NULL) {
        this->markLimit = readLimit;
        this->markPos = this->pos;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStream::reset() {

    if (this->proxyBuffer == NULL) {
        throw IOException(__FILE__, __LINE__, "BufferedInputStream::reset - This stream has been closed.");
    }

    if (this->markPos == -1) {
        throw IOException(__FILE__, __LINE__, "BufferedInputStream::reset - The mark position was invalidated.");
    }

    this->pos = this->markPos;
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::doReadByte() {

    try {

        // Use a local reference in case of unsynchronized close.
        InputStream* inputStream = this->inputStream;
        unsigned char* buffer = this->proxyBuffer;

        if (isClosed() || buffer == NULL) {
            throw IOException(__FILE__, __LINE__, "BufferedInputStream::bufferData - Stream is closed");
        }

        // Are there buffered bytes available?  Or can we read more?
        if (this->pos >= this->count && bufferData(inputStream, buffer) == -1) {
            return -1;
        }

        // Stream might have closed while we were buffering.
        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "BufferedInputStream::bufferData - Stream is closed");
        }

        if (this->pos != this->count) {
            return buffer[this->pos++];;
        }

        return -1;
    }
    DECAF_CATCH_RETHROW( IOException)
    DECAF_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    try {

        // Use a local reference in case of unsynchronized close.
        unsigned char* lbuffer = this->proxyBuffer;

        if (lbuffer == NULL) {
            throw IOException(__FILE__, __LINE__, "Stream is closed");
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Buffer passed was NULL.");
        }

        if (offset > size || offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size - offset) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
        }

        // For zero, do nothing
        if (length == 0) {
            return 0;
        }

        // Use a local reference in case of unsynchronized close.
        InputStream* inputStream = this->inputStream;

        if (inputStream == NULL) {
            throw IOException(__FILE__, __LINE__, "Stream is closed");
        }

        int required = 0;

        // There are bytes available in the buffer so use them up first and
        // then we check to see if any are available on the stream, if not
        // then just return what we had.
        if (this->pos < this->count) {

            int available = this->count - this->pos;
            int copylength = available >= length ? length : available;

            System::arraycopy(lbuffer, this->pos, buffer, offset, copylength);
            this->pos += copylength;

            if (copylength == length || inputStream->available() == 0) {
                return (int) copylength;
            }

            offset += copylength;
            required = length - copylength;
        } else {
            required = length;
        }

        while (true) {

            int read = 0;

            // If we're not marked and the required size is greater than the
            // buffer, simply read the bytes directly bypassing the buffer.
            if (this->markPos == -1 && required >= this->bufferSize) {

                read = inputStream->read(buffer, size, offset, required);
                if (read == -1) {
                    return required == length ? -1 : (int) (length - required);
                }

            } else {

                if (bufferData(inputStream, lbuffer) == -1) {
                    return required == length ? -1 : (int) (length - required);
                }

                // Stream might have closed while we were buffering.
                if (isClosed() || this->proxyBuffer == NULL) {
                    throw IOException(__FILE__, __LINE__, "BufferedInputStream::bufferData - Stream is closed");
                }

                int available = this->count - this->pos;
                read = available >= required ? required : available;
                System::arraycopy(lbuffer, this->pos, buffer, offset, read);
                this->pos += read;
            }

            required -= read;

            if (required == 0) {
                return length;
            }

            if (inputStream->available() == 0) {
                return length - required;
            }

            offset += read;
        }
    }
    DECAF_CATCH_RETHROW( IOException)
    DECAF_CATCH_RETHROW( NullPointerException)
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long BufferedInputStream::skip(long long amount) {

    try {

        if (amount == 0) {
            return 0;
        }

        // Use a local reference in case of unsynchronized close.
        InputStream* inputStream = this->inputStream;
        unsigned char* lbuffer = this->proxyBuffer;

        if (isClosed() || lbuffer == NULL) {
            throw IOException(__FILE__, __LINE__, "BufferedInputStream::skip - Stream is closed");
        }

        if ((this->count - this->pos) >= amount) {
            this->pos += (int) amount;
            return amount;
        }

        long long read = this->count - this->pos;

        this->pos = this->count;

        if (this->markPos != -1) {

            if (amount <= this->markLimit) {

                if (bufferData(inputStream, lbuffer) == -1) {
                    return read;
                }

                if ((this->count - this->pos) >= (amount - read)) {
                    this->pos += (int) (amount - read);
                    return amount;
                }

                // Couldn't get all the bytes, skip what we read
                read += this->count - this->pos;
                this->pos = this->count;

                return read;
            }
        }

        return read + inputStream->skip(amount - read);
    }
    DECAF_CATCH_RETHROW( IOException)
    DECAF_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
int BufferedInputStream::bufferData(InputStream* inputStream, unsigned char*& buffer) {

    try {

        if (this->markPos == -1 || pos - markPos >= markLimit) {
            // Mark position not set or exceeded readlimit
            int result = inputStream->read(buffer, this->bufferSize);
            if (result > 0) {
                this->markLimit = 0;
                this->markPos = -1;
                this->pos = this->count = 0;
                this->count = result == -1 ? 0 : result;
            }

            return result;
        }

        if (this->markPos == 0 && this->markLimit > this->bufferSize) {

            // Increase buffer size to accommodate the readlimit.
            int newLength = this->bufferSize * 2;
            if (newLength > markLimit) {
                newLength = markLimit;
            }

            unsigned char* temp = new unsigned char[newLength];
            System::arraycopy(temp, 0, buffer, 0, count);
            std::swap(temp, buffer);
            delete[] temp;
            this->bufferSize = newLength;

            if (this->proxyBuffer != NULL) {
                this->buff = buffer;
                this->proxyBuffer = this->buff;
            }

        } else if (this->markPos > 0) {
            System::arraycopy(buffer, markPos, buffer, 0, this->bufferSize - markPos);
        }

        // Set the new position and mark position
        this->pos -= this->markPos;
        this->count = this->markPos = 0;

        int bytesread = inputStream->read(buffer, this->bufferSize, this->pos, this->bufferSize - this->pos);

        this->count = bytesread <= 0 ? this->pos : this->pos + bytesread;

        return bytesread;
    }
    DECAF_CATCH_RETHROW( IOException)
    DECAF_CATCHALL_THROW( IOException)
}
