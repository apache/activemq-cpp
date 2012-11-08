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

#include "BufferOverflowException.h"

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException() : lang::Exception() {
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::~BufferOverflowException() throw () {
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException(const lang::Exception& ex) : lang::Exception() {
    *(lang::Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException(const BufferOverflowException& ex) : lang::Exception() {
    *(lang::Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException(
    const char* file, const int lineNumber, const std::exception* cause, const char* msg, ...) : lang::Exception(cause) {

    va_list vargs;
    va_start( vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException(const std::exception* cause) : lang::Exception(cause) {
}

////////////////////////////////////////////////////////////////////////////////
BufferOverflowException::BufferOverflowException(
    const char* file, const int lineNumber, const char* msg, ...) : lang::Exception() {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}
