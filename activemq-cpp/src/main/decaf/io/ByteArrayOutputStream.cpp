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

#include "ByteArrayOutputStream.h"

#include <decaf/lang/System.h>

#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream() :
    OutputStream(), buffer(new unsigned char[32]), bufferSize(32), count(0) {
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::ByteArrayOutputStream(int bufferSize) :
    OutputStream(), buffer(NULL), bufferSize(bufferSize), count(0) {

    if (bufferSize <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Buffer size given was invalid: %d", bufferSize);
    }

    this->buffer = new unsigned char[bufferSize];
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayOutputStream::~ByteArrayOutputStream() {
    try {
        delete[] buffer;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::pair<unsigned char*, int> ByteArrayOutputStream::toByteArray() const {

    unsigned char* temp = NULL;

    if (this->count == 0) {
        return std::pair<unsigned char*, int>(temp, 0);
    }

    temp = new unsigned char[this->count];
    System::arraycopy(this->buffer, 0, temp, 0, this->count);

    return std::make_pair(temp, this->count);
}

////////////////////////////////////////////////////////////////////////////////
long long ByteArrayOutputStream::size() const {
    return this->count;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::reset() {
    this->count = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::doWriteByte(unsigned char c) {

    try {

        if (this->count == this->bufferSize) {
            checkExpandSize(1);
        }

        this->buffer[this->count++] = c;
    }
    DECAF_CATCH_RETHROW( IOException)
    DECAF_CATCHALL_THROW( IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::doWriteArrayBounded(const unsigned char* buffer, int size, int offset, int length) {

    if (length == 0) {
        return;
    }

    if (buffer == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "passed buffer is null");
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

        checkExpandSize(length);

        System::arraycopy(buffer, offset, this->buffer, this->count, length);
        this->count += length;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string ByteArrayOutputStream::toString() const {

    if (this->count == 0) {
        return "";
    }

    return string((const char*) this->buffer, this->count);
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::writeTo(OutputStream* out) const {

    try {

        if (this->count == 0) {
            return;
        }

        if (out == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Passed stream pointer is null");
        }

        out->write(this->buffer, this->count);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void ByteArrayOutputStream::checkExpandSize(int needed) {

    try {

        if (this->count + needed <= this->bufferSize) {
            return;
        }

        int newSize = (this->count + needed) * 2;
        unsigned char* temp = new unsigned char[newSize];
        System::arraycopy(this->buffer, 0, temp, 0, this->count);

        std::swap(temp, this->buffer);

        this->bufferSize = newSize;

        delete[] temp;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_EXCEPTION_CONVERT(Exception, IOException)
    DECAF_CATCHALL_THROW(IOException)
}
