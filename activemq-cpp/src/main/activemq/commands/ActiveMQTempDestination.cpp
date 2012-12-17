/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/ActiveMQTempDestination.h>

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/lang/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::ActiveMQTempDestination() :
    ActiveMQDestination(), Closeable(), connection(NULL), connectionId(), sequenceId(0) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::ActiveMQTempDestination(const std::string& name) :
    ActiveMQDestination(name), Closeable(), connection(NULL), connectionId(), sequenceId(0) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::~ActiveMQTempDestination() throw() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTempDestination::getDataStructureType() const {
    return ActiveMQTempDestination::ID_ACTIVEMQTEMPDESTINATION;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempDestination::setPhysicalName(const std::string& physicalName) {

    ActiveMQDestination::setPhysicalName(physicalName);
    this->connectionId = "";
    this->sequenceId = 0;

    size_t pos = physicalName.find_last_of(":");
    if (pos != std::string::npos) {
        std::string seqStr = physicalName.substr(pos + 1);
        if (!seqStr.empty()) {

            try {
                this->sequenceId = Integer::parseInt(seqStr);
            } catch (decaf::lang::exceptions::NumberFormatException& e) {
                // Not the expected format so ignore.
            }

            // The rest should be the connection id.
            this->connectionId = physicalName.substr(0, pos);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTempDestination::toString() const {
    return ActiveMQDestination::toString();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempDestination::close() {
    try {
        if (this->connection != NULL) {
            Pointer<ActiveMQTempDestination> thisPtr(this);
            try {
                this->connection->deleteTempDestination(thisPtr);
                thisPtr.release();
            } catch(ActiveMQException& ex) {
                thisPtr.release();
                throw;
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
