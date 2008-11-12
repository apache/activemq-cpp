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

#include "CmsAccessor.h"
#include <activemq/exceptions/ExceptionDefines.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq::cmsutil;
using namespace activemq::exceptions;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CmsAccessor::CmsAccessor() {
    sessionAcknowledgeMode = cms::Session::AUTO_ACKNOWLEDGE;
}

////////////////////////////////////////////////////////////////////////////////
CmsAccessor::~CmsAccessor() {
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CmsAccessor::createConnection()
    throw ( cms::CMSException,IllegalStateException ) {

    try {

        checkConnectionFactory();

        // Create the connection.
        cms::Connection* c = getConnectionFactory()->createConnection();

        // Manage the lifecycle of this resource.
        getResourceLifecycleManager()->addConnection( c );

        return c;
    }
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* CmsAccessor::createSession( cms::Connection* con )
    throw ( cms::CMSException,IllegalStateException ) {

    try {

        if( con == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "connection object is invalid" );
        }

        // Create the session.
        cms::Session* s = con->createSession( getSessionAcknowledgeMode() );

        // Manage the lifecycle of this resource.
        getResourceLifecycleManager()->addSession( s );

        return s;
    }
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessor::checkConnectionFactory() throw ( IllegalStateException ) {
    if( getConnectionFactory() == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__,
            "Property 'connectionFactory' is required" );
    }
}
