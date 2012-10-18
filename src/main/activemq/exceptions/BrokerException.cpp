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

#include "BrokerException.h"

using namespace activemq;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
BrokerException::BrokerException() : ActiveMQException() {}

////////////////////////////////////////////////////////////////////////////////
BrokerException::BrokerException(const ActiveMQException& ex) : exceptions::ActiveMQException() {
    *(exceptions::ActiveMQException*)this = ex;
}

////////////////////////////////////////////////////////////////////////////////
BrokerException::BrokerException(const BrokerException& ex) : exceptions::ActiveMQException() {
    *(exceptions::ActiveMQException*)this = ex;
}

////////////////////////////////////////////////////////////////////////////////
BrokerException::BrokerException(const char* file, const int lineNumber, const char* msg, ...) : exceptions::ActiveMQException() {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
BrokerException::BrokerException(const char* file, const int lineNumber, const commands::BrokerError* error) : exceptions::ActiveMQException() {

    std::ostringstream ostream;
    ostream << "*** BEGIN SERVER-SIDE STACK TRACE ***" << std::endl;
    ostream << "Message: " << error->getMessage() << std::endl;
    if (error->getCause() != NULL) {
        ostream << "Cause: " << error->getCause()->toString() << std::endl;
    }
    ostream << "Exception Class " << error->getExceptionClass() << std::endl;

    for (std::size_t ix = 0; ix < error->getStackTraceElements().size(); ++ix) {
        ostream << "\t[FILE: " << error->getStackTraceElements()[ix]->FileName
                << ", LINE: " << error->getStackTraceElements()[ix]->LineNumber
                << "] occurred in: " << error->getStackTraceElements()[ix]->ClassName
                << "." << error->getStackTraceElements()[ix]->MethodName << std::endl;
    }

    ostream << "*** END SERVER-SIDE STACK TRACE ***";

    setMessage(ostream.str().c_str());
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
BrokerException::~BrokerException() throw() {}

////////////////////////////////////////////////////////////////////////////////
BrokerException* BrokerException::clone() const{
    return new BrokerException( *this );
}
