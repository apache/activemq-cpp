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

#include "ReadOnlyBufferException.h"

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException() : lang::exceptions::UnsupportedOperationException() {
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::~ReadOnlyBufferException() throw () {
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException(const lang::Exception& ex) : lang::exceptions::UnsupportedOperationException() {
    *(lang::Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException(const ReadOnlyBufferException& ex) : lang::exceptions::UnsupportedOperationException() {
    *(lang::Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException(
    const char* file, const int lineNumber, const std::exception* cause, const char* msg, ...) : lang::exceptions::UnsupportedOperationException(cause) {

    va_list vargs;
    va_start( vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException(const std::exception* cause) : lang::exceptions::UnsupportedOperationException(cause) {
}

////////////////////////////////////////////////////////////////////////////////
ReadOnlyBufferException::ReadOnlyBufferException(
    const char* file, const int lineNumber, const char* msg, ...) : lang::exceptions::UnsupportedOperationException() {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}
