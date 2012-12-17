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

#include "URISyntaxException.h"

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException() : decaf::lang::Exception(), reason(), input(), index(-1) {
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::~URISyntaxException() throw () {
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const Exception& ex) : Exception(), reason(), input(), index(-1) {
    *(Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const URISyntaxException& ex) :
    Exception(), reason(ex.getReason()), input(ex.getInput()), index(ex.getIndex()) {

    *(Exception*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const char* file, const int lineNumber, const std::exception* cause, const char* msg, ...) :
    lang::Exception(cause), reason(), input(), index(-1) {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const std::exception* cause) :
    lang::Exception(cause), reason(), input(), index(-1) {
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const char* file, const int lineNumber, const char* msg DECAF_UNUSED) :
    Exception(), reason("<Unknown Reason>"), input("<No Address Given>"), index(-1) {

    const char * message = "Input: %s, Reason it failed: %s";
    this->setMessage(message, input.c_str(), reason.c_str());

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const char* file, const int lineNumber, const std::string& input, const std::string& reason) :
    Exception(), reason(reason), input(input), index(-1) {

    const char * message = "Input: %s, Reason it failed: %s";
    this->setMessage(message, input.c_str(), reason.c_str());

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
URISyntaxException::URISyntaxException(const char* file, const int lineNumber, const std::string& input, const std::string& reason, int index) :
    Exception(), reason(reason), input(input), index(index) {

    const char * message = "Input: %s, Index %d resulted in this error: %s";
    this->setMessage(message, input.c_str(), index, reason.c_str());

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}
