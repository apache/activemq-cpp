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

#include "ConnectionFailedException.h"

using namespace activemq;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException::ConnectionFailedException() : ActiveMQException() {
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException::ConnectionFailedException(const exceptions::ActiveMQException& ex) : ActiveMQException() {
    *(exceptions::ActiveMQException*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException::ConnectionFailedException(const ConnectionFailedException& ex) : ActiveMQException() {
    *(exceptions::ActiveMQException*) this = ex;
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException::ConnectionFailedException(const char* file, const int lineNumber, const char* msg, ...) : ActiveMQException() {

    va_list vargs;
    va_start(vargs, msg);
    buildMessage(msg, vargs);

    // Set the first mark for this exception.
    setMark(file, lineNumber);
}

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException::~ConnectionFailedException() throw() {

}

////////////////////////////////////////////////////////////////////////////////
ConnectionFailedException* ConnectionFailedException::clone() const {
    return new ConnectionFailedException(*this);
}

