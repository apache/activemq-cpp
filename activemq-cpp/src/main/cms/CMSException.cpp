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

#include <cms/CMSException.h>

#include <vector>
#include <sstream>
#include <memory>
#include <utility>
#include <algorithm>

using namespace std;
using namespace cms;

namespace cms {

    class CMSExceptionData {
    public:

        std::string message;
        std::auto_ptr<const std::exception> cause;
        std::vector<std::pair<std::string, int> > stackTrace;

        CMSExceptionData() : message(), cause(), stackTrace() {
        }
    };

}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException() :
    std::exception(), data(new CMSExceptionData()) {
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException(const CMSException& ex) : std::exception(), data(new CMSExceptionData()) {
    this->data->cause.reset(ex.data->cause.release());
    this->data->message = ex.data->message;
    this->data->stackTrace = ex.data->stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException(const std::string& message) : std::exception(), data(new CMSExceptionData()) {
    this->data->message = message;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException(const std::string& message, const std::exception* cause) : std::exception(), data(new CMSExceptionData()) {

    this->data->cause.reset(cause);
    this->data->message = message;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::CMSException(const std::string& message, const std::exception* cause,
                           const std::vector<std::pair<std::string, int> >& stackTrace) :
    std::exception(), data(new CMSExceptionData()) {

    this->data->cause.reset(cause);
    this->data->message = message;
    this->data->stackTrace = stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
CMSException::~CMSException() throw() {
    delete this->data;
}

////////////////////////////////////////////////////////////////////////////////
CMSException* CMSException::clone() {
    return new CMSException(*this);
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::setMark(const char* file, const int lineNumber) {

    // Add this mark to the end of the stack trace.
    this->data->stackTrace.push_back(std::make_pair((std::string) file, (int) lineNumber));

    std::ostringstream stream;
    stream << "\tFILE: " << this->data->stackTrace[this->data->stackTrace.size() - 1].first;
    stream << ", LINE: " << this->data->stackTrace[this->data->stackTrace.size() - 1].second;
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::printStackTrace() const {
    printStackTrace(std::cerr);
}

////////////////////////////////////////////////////////////////////////////////
void CMSException::printStackTrace(std::ostream& stream) const {
    stream << getStackTraceString();
}

////////////////////////////////////////////////////////////////////////////////
std::string CMSException::getStackTraceString() const {

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
std::string CMSException::getMessage() const {
    return this->data->message;
}

////////////////////////////////////////////////////////////////////////////////
const std::exception* CMSException::getCause() const {
    return this->data->cause.get();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::pair<std::string, int> > CMSException::getStackTrace() const {
    return this->data->stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
const char* CMSException::what() const throw() {
    return this->data->message.c_str();
}
