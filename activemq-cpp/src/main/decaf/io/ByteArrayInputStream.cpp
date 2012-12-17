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

#include "ByteArrayInputStream.h"

#include <decaf/lang/System.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream() :
    InputStream(), buffer(NULL), size(0), own(false), count(0), pos(0), markpos(0) {
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream(const vector<unsigned char>& buffer) :
    InputStream(), buffer(NULL), size(0), own(false), count(0), pos(0), markpos(0) {

    if (buffer.size() > 0) {
        setByteArray(&buffer[0], (int) buffer.size(), 0, (int) buffer.size());
    }
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream(const unsigned char* buffer, int bufferSize, bool own) :
    InputStream(), buffer(NULL), size(0), own(own), count(0), pos(0), markpos(0) {

    setByteArray(buffer, bufferSize, 0, bufferSize);
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::ByteArrayInputStream(const unsigned char* buffer, int bufferSize, int offset, int length, bool own) :
    InputStream(), buffer(NULL), size(0), own(own), count(0), pos(0), markpos(0) {

    setByteArray(buffer, bufferSize, offset, length);
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayInputStream::~ByteArrayInputStream() {
    try {
        if (this->own) {
            delete[] this->buffer;
        }
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setByteArray(const vector<unsigned char>& buffer) {
    if (buffer.size() > 0) {
        setByteArray(&buffer[0], (int) buffer.size(), 0, (int) buffer.size());
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setByteArray(const unsigned char* buffer, int bufferSize) {
    setByteArray(buffer, bufferSize, 0, bufferSize);
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::setByteArray(const unsigned char* buffer, int bufferSize, int offset, int length) {

    if (buffer == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Input Buffer cannot be NULL.");
    }

    if (bufferSize < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Size given for input buffer was negative.");
    }

    if (offset < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Offset given was negative: %d.", offset);
    }

    if (length < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Length given was negative: %d.", offset);
    }

    // We're using the default buffer.
    this->buffer = buffer;
    this->size = bufferSize;
    this->pos = offset;
    this->markpos = offset;
    this->count = offset + length > bufferSize ? bufferSize : offset + length;
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::available() const {
    return this->count - this->pos;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::mark(int readLimit DECAF_UNUSED) {

    // the reset point is now the marked position until a new byte buffer
    // is set on this stream.
    this->markpos = this->pos;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayInputStream::reset() {

    try {

        // Begin at the Beginning if mark hasn't been called otherwise it
        // starts at the marked pos.
        this->pos = this->markpos;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::doReadByte() {

    try {
        return pos < count ? buffer[pos++] : -1;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int ByteArrayInputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    try {

        if (length == 0) {
            return 0;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "ByteArrayInputStream::read - Buffer passed is Null");
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

        if (this->pos >= this->count) {
            return -1;
        }

        int copylen = this->count - this->pos < length ? this->count - this->pos : length;
        System::arraycopy(this->buffer, this->pos, buffer, offset, copylen);
        this->pos += copylen;
        return copylen;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayInputStream::skip(long long num) {

    try {

        if (num <= 0) {
            return 0;
        }

        int temp = this->pos;
        this->pos = this->count - this->pos < num ? this->count : (int) (this->pos + num);
        return this->pos - temp;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}
