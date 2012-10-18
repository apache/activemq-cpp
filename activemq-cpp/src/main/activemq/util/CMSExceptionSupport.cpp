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

#include "CMSExceptionSupport.h"

#include <typeinfo>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
CMSExceptionSupport::CMSExceptionSupport() {
}

////////////////////////////////////////////////////////////////////////////////
CMSExceptionSupport::~CMSExceptionSupport() {
}

////////////////////////////////////////////////////////////////////////////////
CMSException CMSExceptionSupport::create(const std::string& msg, const Exception& cause) {

    CMSException exception(msg, cause.clone());
    return exception;
}

////////////////////////////////////////////////////////////////////////////////
CMSException CMSExceptionSupport::create(const Exception& cause) {

    std::string msg = cause.getMessage();

    if (msg.length() == 0) {
        msg = typeid( &cause ).name();
    }

    CMSException exception(msg, cause.clone());

    return exception;
}

////////////////////////////////////////////////////////////////////////////////
MessageEOFException CMSExceptionSupport::createMessageEOFException(const Exception& cause) {

    std::string msg = cause.getMessage();

    if (msg.length() == 0) {
        msg = typeid( &cause ).name();
    }

    MessageEOFException exception(msg, cause.clone());

    return exception;
}

////////////////////////////////////////////////////////////////////////////////
MessageFormatException CMSExceptionSupport::createMessageFormatException(const Exception& cause) {

    std::string msg = cause.getMessage();

    if (msg.length() == 0) {
        msg = typeid( &cause ).name();
    }

    MessageFormatException exception(msg, cause.clone());

    return exception;
}
