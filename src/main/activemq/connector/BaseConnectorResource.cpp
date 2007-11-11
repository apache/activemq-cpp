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

#include "BaseConnectorResource.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/connector/Connector.h>

using namespace activemq;
using namespace activemq::connector;
using namespace activemq::exceptions;

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

///////////////////////////////////////////////////////////////////////////////
BaseConnectorResource::BaseConnectorResource() {
    this->connector = NULL;
    this->closed = false;
}

///////////////////////////////////////////////////////////////////////////////
BaseConnectorResource::BaseConnectorResource( Connector* connector ) {
    this->connector = connector;
    this->closed = false;
}

///////////////////////////////////////////////////////////////////////////////
void BaseConnectorResource::close() throw ( cms::CMSException ) {

    try {

        if( closed == true ) {
            return;
        }

        // We are now closed
        this->closed = true;

        if( connector != NULL ) {
            // Close the connector resource
            connector->closeResource( this );
        }

        // Notify the listeners
        std::set< ConnectorResourceListener* >::const_iterator iter =
            listeners.begin();

        for(; iter != listeners.end(); ++iter ) {
            try{
                (*iter)->onConnectorResourceClosed( this );
            } catch( ... ) {
                // Absorb
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

///////////////////////////////////////////////////////////////////////////////
void BaseConnectorResource::addListener( ConnectorResourceListener* listener ) {
    listeners.insert( listener );
}

///////////////////////////////////////////////////////////////////////////////
void BaseConnectorResource::removeListener( ConnectorResourceListener* listener ) {
    listeners.erase( listener );
}
