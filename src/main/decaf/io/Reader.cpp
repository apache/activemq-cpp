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

#include "Reader.h"

#include <decaf/lang/Math.h>
#include <decaf/nio/CharBuffer.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::nio;

////////////////////////////////////////////////////////////////////////////////
Reader::Reader() {
}

////////////////////////////////////////////////////////////////////////////////
Reader::~Reader() {
}

////////////////////////////////////////////////////////////////////////////////
void Reader::mark(int readAheadLimit DECAF_UNUSED) {
    throw IOException(__FILE__, __LINE__, "Default implementation of Reader, mark not supported.");
}

////////////////////////////////////////////////////////////////////////////////
bool Reader::ready() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Reader::reset() {
    throw IOException(__FILE__, __LINE__, "Default implementation of Reader, reset not supported.");
}

////////////////////////////////////////////////////////////////////////////////
long long Reader::skip(long long count) {

    try {

        long long skipped = 0;
        int toRead = count < 512 ? (int) count : 512;

        std::vector<char> charsSkipped(toRead);

        while (skipped < count) {

            int read = this->doReadArrayBounded(&charsSkipped[0], (int) charsSkipped.size(), 0, toRead);

            if (read == -1) {
                return skipped;
            }

            skipped += read;

            if (read < toRead) {
                return skipped;
            }

            if (count - skipped < (long long) toRead) {
                toRead = (int) (count - skipped);
            }
        }

        return skipped;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::read(std::vector<char>& buffer) {

    try {
        return this->doReadVector(buffer);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::read(char* buffer, int length) {

    try {
        return this->doReadArray(buffer, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::read(char* buffer, int size, int offset, int length) {

    try {
        return this->doReadArrayBounded(buffer, size, offset, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::read() {

    try {
        return this->doReadChar();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::read(decaf::nio::CharBuffer* charBuffer) {

    try {
        // Error checking is done in the Impl.
        return this->doReadCharBuffer(charBuffer);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(ReadOnlyBufferException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::doReadVector(std::vector<char>& buffer) {

    try {

        if (buffer.size() == 0) {
            return -1;
        }

        return this->doReadArrayBounded(&buffer[0], (int) buffer.size(), 0, (int) buffer.size());
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::doReadArray(char* buffer, int length) {

    try {

        if (length == 0) {
            return 0;
        }

        return this->doReadArrayBounded(buffer, length, 0, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::doReadChar() {

    try {

        char buffer;

        if (this->doReadArrayBounded(&buffer, 1, 0, 1) == -1) {
            return -1;
        }

        return buffer;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int Reader::doReadCharBuffer(decaf::nio::CharBuffer* charBuffer) {

    if (charBuffer == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Pointer to CharBuffer cannot be NULL.");
    }

    try {

        if (charBuffer->length() == 0) {
            return 0;
        }

        int length = (int) charBuffer->length();

        std::vector<char> buffer(length);

        length = Math::min(length, this->doReadVector(buffer));

        if (length > 0) {
            charBuffer->put(&buffer[0], (int) buffer.size(), 0, length);
        }

        return length;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(ReadOnlyBufferException)
    DECAF_CATCHALL_THROW(IOException)
}
