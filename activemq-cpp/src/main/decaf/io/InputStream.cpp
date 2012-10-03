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

#include "InputStream.h"

#include <typeinfo>

#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
InputStream::InputStream() : mutex() {
}

////////////////////////////////////////////////////////////////////////////////
InputStream::~InputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void InputStream::close() {

}

////////////////////////////////////////////////////////////////////////////////
void InputStream::mark(int readLimit DECAF_UNUSED) {

}

////////////////////////////////////////////////////////////////////////////////
std::string InputStream::toString() const {
    return typeid(this).name();
}

////////////////////////////////////////////////////////////////////////////////
void InputStream::reset() {
    throw IOException(__FILE__, __LINE__, "Base InputStream class does not support Reset.");
}

////////////////////////////////////////////////////////////////////////////////
int InputStream::read() {
    try {
        return this->doReadByte();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int InputStream::read(unsigned char* buffer, int size) {

    try {
        return this->doReadArray(buffer, size);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int InputStream::read(unsigned char* buffer, int size, int offset, int length) {

    try {
        return this->doReadArrayBounded(buffer, size, offset, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long InputStream::skip(long long num) {

    try {

        if (num <= 0) {
            return 0;
        }

        long long skipped = 0;

        // Lets not try and buffer every byte since it could be as large as
        // whatever size_t is on this platform, read the data in reasonable
        // chunks until finished.
        int toRead = num < 4096LL ? (int) num : 4096;
        std::vector<unsigned char> buffer(toRead);

        while (skipped < num) {

            int read = doReadArrayBounded(&buffer[0], (int) buffer.size(), 0, toRead);

            // Is it EOF?
            if (read == -1) {
                return skipped;
            }

            skipped += read;

            if (read < toRead) {
                return skipped;
            }
            if (num - skipped < toRead) {
                toRead = (int) (num - skipped);
            }
        }
        return skipped;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(UnsupportedOperationException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int InputStream::doReadArray(unsigned char* buffer, int size) {

    try {
        return this->doReadArrayBounded(buffer, size, 0, size);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int InputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    try {

        if (length == 0) {
            return 0;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Buffer passed was NULL");
        }

        if (length > (size - offset)) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "Offset + Length given exceeds Buffer size.");
        }

        for (int i = 0; i < length; i++) {

            int c;
            try {
                if ((c = doReadByte()) == -1) {
                    return i == 0 ? -1 : (int) i;
                }
            } catch (IOException& e) {
                if (i != 0) {
                    return (int) i;
                }
                throw e;
            }
            buffer[offset + i] = (unsigned char) c;
        }

        return (int) length;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}
