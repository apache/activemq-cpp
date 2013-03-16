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

#include "CloneNotSupportedException.h"

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException() : Exception() {
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::~CloneNotSupportedException() throw () {
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException(const Exception& ex) : Exception() {
    *(Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException(const CloneNotSupportedException& ex) : Exception() {
    *(Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException(const std::exception* cause) : Exception(cause) {
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException(const char* file, const int lineNumber, const char* msg, ...) : Exception() {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
CloneNotSupportedException::CloneNotSupportedException(const char* file, const int lineNumber, const std::exception* cause, const char* msg, ...) : Exception(cause) {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}
