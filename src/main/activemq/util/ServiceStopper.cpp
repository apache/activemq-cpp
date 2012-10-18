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

#include "ServiceStopper.h"

#include <activemq/util/Service.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ServiceStopper::ServiceStopper() : firstException(), hasException(false) {
}

////////////////////////////////////////////////////////////////////////////////
ServiceStopper::~ServiceStopper() {
}

////////////////////////////////////////////////////////////////////////////////
void ServiceStopper::stop(Service* service) {

    if(service == NULL) {
        return;
    }

    try {
        service->stop();
    } catch(Exception& ex) {
        this->onException(service, ex);
    } catch(std::exception& stdex) {
        ActiveMQException wrapper(__FILE__, __LINE__, stdex.what());
        this->onException(service, wrapper);
    } catch(...) {
        ActiveMQException wrapper(__FILE__, __LINE__, "Caught Unknown Exception");
        this->onException(service, wrapper);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServiceStopper::throwFirstException() {
    if(this->hasException) {
        throw this->firstException;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServiceStopper::onException(Service* service AMQCPP_UNUSED, Exception& ex) {
    if(!this->hasException) {
        this->firstException = ex;
        this->hasException = true;
    }
}
