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

#include <cms/IllegalStateException.h>

using namespace cms;
using namespace activemq::cmsutil;

/**
 * A catch-all that throws an CMSException.
 */
#define CMSTEMPLATE_CATCHALL() \
    catch(cms::CMSException& ex){ \
        throw; \
    } catch(std::exception& ex) { \
        throw CMSException(ex.what(), NULL); \
    } catch( ... ){ \
        throw CMSException("caught unknown exception", NULL); \
    }

////////////////////////////////////////////////////////////////////////////////
CmsAccessor::CmsAccessor() :
    resourceLifecycleManager(), connectionFactory(NULL), sessionAcknowledgeMode(cms::Session::AUTO_ACKNOWLEDGE) {
}

////////////////////////////////////////////////////////////////////////////////
CmsAccessor::~CmsAccessor() {
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessor::init() {
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CmsAccessor::createConnection() {

    try {

        checkConnectionFactory();

        // Create the connection.
        cms::Connection* c = getConnectionFactory()->createConnection();

        // Manage the lifecycle of this resource.
        getResourceLifecycleManager()->addConnection(c);

        return c;
    }
    CMSTEMPLATE_CATCHALL()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* CmsAccessor::createSession(cms::Connection* con) {

    try {

        if (con == NULL) {
            throw CMSException("connection object is invalid", NULL);
        }

        // Create the session.
        cms::Session* s = con->createSession(getSessionAcknowledgeMode());

        // Manage the lifecycle of this resource.
        getResourceLifecycleManager()->addSession(s);

        return s;
    }
    CMSTEMPLATE_CATCHALL()
}

////////////////////////////////////////////////////////////////////////////////
void CmsAccessor::checkConnectionFactory() {
    if (getConnectionFactory() == NULL) {
        throw IllegalStateException("Property 'connectionFactory' is required", NULL);
    }
}
