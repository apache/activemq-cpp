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

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::ActiveMQTempDestination() :
    ActiveMQDestination(), connection( NULL ) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::ActiveMQTempDestination( const std::string& name ) :
    ActiveMQDestination( name ), connection( NULL ) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempDestination::~ActiveMQTempDestination() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTempDestination::getDataStructureType() const {
    return ActiveMQTempDestination::ID_ACTIVEMQTEMPDESTINATION;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTempDestination::toString() const {
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQTempDestination" << std::endl;
    stream << ActiveMQDestination::toString();
    stream << "End Class = ActiveMQTempDestination" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempDestination::close() throw( cms::CMSException ) {
    try {
        if( this->connection != NULL ) {
            this->connection->destroyDestination( this );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
