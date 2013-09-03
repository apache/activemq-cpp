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

#include "ActiveMQDestinationEvent.h"

#include <activemq/core/ActiveMQConstants.h>
#include <activemq/commands/ActiveMQDestination.h>

#include <decaf/lang/exceptions/NullPointerException.h>

using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestinationEvent::ActiveMQDestinationEvent(Pointer<DestinationInfo> destination) :
    DestinationEvent(), destination(destination) {

    if (destination == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Events cannot have null destinations");
    }
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQDestinationEvent::~ActiveMQDestinationEvent() {
}

////////////////////////////////////////////////////////////////////////////////
const cms::Destination* ActiveMQDestinationEvent::getDestination() const {
    Pointer<ActiveMQDestination> dest = this->destination->getDestination();
    if (dest != NULL) {
        return dest->getCMSDestination();
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestinationEvent::isAddOperation() const {
    return this->destination->getOperationType() == ActiveMQConstants::DESTINATION_ADD_OPERATION;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQDestinationEvent::isRemoveOperation() const {
    return this->destination->getOperationType() == ActiveMQConstants::DESTINATION_REMOVE_OPERATION;
}
