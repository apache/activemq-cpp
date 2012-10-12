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
#include <stdio.h>
#include "Exception.h"
#include <decaf/util/logging/LoggerDefines.h>
#include <decaf/internal/AprPool.h>
#include <decaf/lang/Pointer.h>

#include <sstream>
#include <apr_strings.h>

using namespace std;
using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;
using namespace decaf::util::logging;

namespace decaf {
namespace lang {

    class ExceptionData {
    public:

        /**
         * The cause of this exception.
         */
        std::string message;

        /**
         * The Exception that caused this one to be thrown.
         */
        decaf::lang::Pointer<const std::exception> cause;

        /**
         * The stack trace.
         */
        std::vector< std::pair< std::string, int> > stackTrace;

    public:

        ExceptionData() : message(), cause(NULL), stackTrace() {}

    };

}}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception() : Throwable(), data(new ExceptionData) {
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception(const Exception& ex) : Throwable(), data(new ExceptionData) {
    *this = ex;
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception(const std::exception* cause) : Throwable(), data(new ExceptionData) {
    this->initCause(cause);
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception(const char* file, const int lineNumber, const char* msg, ...) :
    Throwable(), data(new ExceptionData) {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);
    va_end(vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception(const char* file, const int lineNumber, const std::exception* cause, const char* msg, ...) :
    Throwable(), data(new ExceptionData) {

    va_list vargs;
    va_start(vargs, msg);
    this->buildMessage(msg, vargs);
    va_end(vargs);

    // Store the cause
    this->initCause(cause);

    // Set the first mark for this exception.
    this->setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
Exception::~Exception() throw () {
    delete this->data;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMessage(const char* msg, ...) {
    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);
    va_end(vargs);
}

////////////////////////////////////////////////////////////////////////////////
void Exception::buildMessage(const char* format, va_list& vargs) {

    // Allocate buffer with a guess of it's size
    AprPool pool;

    // Allocate a buffer of the specified size.
    char* buffer = apr_pvsprintf(pool.getAprPool(), format, vargs);

    // Guessed size was enough. Assign the string.
    this->data->message.assign(buffer, strlen(buffer));
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMark(const char* file, const int lineNumber) {
    // Add this mark to the end of the stack trace.
    this->data->stackTrace.push_back(std::make_pair(std::string(file), (int) lineNumber));
}

////////////////////////////////////////////////////////////////////////////////
Exception* Exception::clone() const {
    return new Exception(*this);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::pair<std::string, int> > Exception::getStackTrace() const {
    return this->data->stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setStackTrace(const std::vector<std::pair<std::string, int> >& trace) {
    this->data->stackTrace = trace;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::printStackTrace() const {
    printStackTrace(std::cerr);
}

////////////////////////////////////////////////////////////////////////////////
void Exception::printStackTrace(std::ostream& stream) const {
    stream << getStackTraceString();
}

////////////////////////////////////////////////////////////////////////////////
std::string Exception::getStackTraceString() const {

    // Create the output stream.
    std::ostringstream stream;

    // Write the message and each stack entry.
    stream << this->data->message << std::endl;
    for (unsigned int ix = 0; ix < this->data->stackTrace.size(); ++ix) {
        stream << "\tFILE: " << this->data->stackTrace[ix].first;
        stream << ", LINE: " << this->data->stackTrace[ix].second;
        stream << std::endl;
    }

    // Return the string from the output stream.
    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
Exception& Exception::operator =(const Exception& ex) {
    this->data->message = ex.data->message;
    this->data->stackTrace = ex.data->stackTrace;
    this->data->cause = ex.data->cause;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::initCause(const std::exception* cause) {

    if (cause == NULL || cause == this) {
        return;
    }

    this->data->cause.reset(cause);

    if (this->data->message == "") {
        this->data->message = cause->what();
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string Exception::getMessage() const {
    return this->data->message;
}

////////////////////////////////////////////////////////////////////////////////
const std::exception*Exception:: getCause() const {
    return this->data->cause.get();
}

////////////////////////////////////////////////////////////////////////////////
const char* Exception::what() const throw () {
    return this->data->message.c_str();
}
