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

#include "FutureResponse.h"

#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/Config.h>
#include <typeinfo>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
FutureResponse::FutureResponse() : responseLatch(1), response(), responseCallback() {}

////////////////////////////////////////////////////////////////////////////////
FutureResponse::FutureResponse(const Pointer<ResponseCallback> responseCallback) :
    responseLatch(1), response(), responseCallback(responseCallback) {}

////////////////////////////////////////////////////////////////////////////////
FutureResponse::~FutureResponse() {}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FutureResponse::getResponse() const {
    try {
        this->responseLatch.await();
        return response;
    } catch (decaf::lang::exceptions::InterruptedException& ex) {
        decaf::lang::Thread::currentThread()->interrupt();
        throw decaf::io::InterruptedIOException(__FILE__, __LINE__, "Interrupted while awaiting a response");
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FutureResponse::getResponse() {
    try {
        this->responseLatch.await();
        return response;
    } catch (decaf::lang::exceptions::InterruptedException& ex) {
        decaf::lang::Thread::currentThread()->interrupt();
        throw decaf::io::InterruptedIOException(__FILE__, __LINE__, "Interrupted while awaiting a response");
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FutureResponse::getResponse(unsigned int timeout) const {
    try {
        this->responseLatch.await(timeout);
        return response;
    } catch (decaf::lang::exceptions::InterruptedException& ex) {
        throw decaf::io::InterruptedIOException(__FILE__, __LINE__, "Interrupted while awaiting a response");
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Response> FutureResponse::getResponse(unsigned int timeout) {
    try {
        this->responseLatch.await(timeout);
        return response;
    } catch (decaf::lang::exceptions::InterruptedException& ex) {
        throw decaf::io::InterruptedIOException(__FILE__, __LINE__, "Interrupted while awaiting a response");
    }
}

////////////////////////////////////////////////////////////////////////////////
void FutureResponse::setResponse(Pointer<Response> response) {
    this->response = response;
    this->responseLatch.countDown();
    if (responseCallback != NULL) {
        responseCallback->onComplete(this->response);
    }
}
