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

#include "InputStreamReader.h"

#include <decaf/io/InputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
InputStreamReader::InputStreamReader(InputStream* stream, bool own) :
    Reader(), stream(stream), own(own), closed(false) {

    if (stream == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "The passed InputStream cannot be NULL.");
    }
}

////////////////////////////////////////////////////////////////////////////////
InputStreamReader::~InputStreamReader() {

    try {
        this->close();
    }
    DECAF_CATCHALL_NOTHROW()

    try {
        if (this->own) {
            delete this->stream;
        }

        this->stream = NULL;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReader::close() {

    try {
        if (!closed) {
            this->stream->close();
            this->closed = true;
        }
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
bool InputStreamReader::ready() const {

    try {
        checkClosed();
        try {
            return this->stream->available() != 0;
        } catch (IOException& ex) {
            return false;
        }
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int InputStreamReader::doReadArrayBounded(char* buffer, int size, int offset, int length) {

    try {
        checkClosed();

        if (length == 0) {
            return 0;
        }

        return this->stream->read((unsigned char*) buffer, size, offset, length);
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void InputStreamReader::checkClosed() const {
    if (closed) {
        throw IOException(__FILE__, __LINE__, "This Reader is Closed");
    }
}
