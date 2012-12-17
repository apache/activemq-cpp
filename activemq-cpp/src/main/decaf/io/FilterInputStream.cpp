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

#include "FilterInputStream.h"

#include <decaf/io/InputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
FilterInputStream::FilterInputStream(InputStream* inputStream, bool own) :
    InputStream(), inputStream(inputStream), own(own), closed(inputStream == NULL ? true : false) {
}

////////////////////////////////////////////////////////////////////////////////
FilterInputStream::~FilterInputStream() {

    try {
        this->close();
    }
    DECAF_CATCHALL_NOTHROW()

    try {
        if (own) {
            delete inputStream;
        }
        inputStream = NULL;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
int FilterInputStream::available() const {

    try {

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::available - Stream is closed");
        }

        return inputStream->available();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStream::close() {

    try {
        if (!closed && inputStream != NULL) {
            inputStream->close();
        }
        this->closed = true;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStream::mark(int readLimit) {
    try {
        if (!isClosed()) {
            inputStream->mark(readLimit);
        }
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStream::reset() {
    try {
        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::reset - Stream is closed");
        }

        return inputStream->reset();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
bool FilterInputStream::markSupported() const {
    try {
        if (!isClosed()) {
            return inputStream->markSupported();
        }
    }
    DECAF_CATCHALL_NOTHROW()
    return false;
}

////////////////////////////////////////////////////////////////////////////////
long long FilterInputStream::skip(long long num) {

    try {

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::skip - Stream is closed");
        }

        return inputStream->skip(num);
    }
    DECAF_CATCH_RETHROW(UnsupportedOperationException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int FilterInputStream::doReadByte() {

    try {

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::doReadByte - Stream is closed");
        }

        return inputStream->read();
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int FilterInputStream::doReadArray(unsigned char* buffer, int size) {

    try {

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::doReadArray - Stream is closed");
        }

        return doReadArrayBounded(buffer, size, 0, size);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int FilterInputStream::doReadArrayBounded(unsigned char* buffer, int size, int offset, int length) {

    try {

        if (isClosed()) {
            throw IOException(__FILE__, __LINE__, "FilterInputStream::doReadArrayBounded - Stream is closed");
        }

        return inputStream->read(buffer, size, offset, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
bool FilterInputStream::isClosed() const {
    return this->closed || this->inputStream == NULL;
}
