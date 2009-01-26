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

#include "FailoverTransport.h"

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::failover;

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::FailoverTransport() {

    this->timeout = -1;
    this->initialReconnectDelay = 10;
    this->maxReconnectDelay = 1000 * 30;
    this->backOffMultiplier = 2;
    this->useExponentialBackOff = true;
    this->randomize = true;
    this->initialized = false;
    this->maxReconnectAttempts = 0;
    this->connectFailures = 0;
    this->reconnectDelay = this->initialReconnectDelay;
    this->firstConnection = true;
    this->backup = false;
    this->backupPoolSize = 1;
    this->trackMessages = false;
    this->maxCacheSize = 128 * 1024;
}

////////////////////////////////////////////////////////////////////////////////
FailoverTransport::~FailoverTransport() {
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::addURI( const decaf::net::URI& uri ) {

    synchronized( &this->uris ) {
        this->uris.add( uri );
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::removeURI( const decaf::net::URI& uri ) {

    synchronized( &this->uris ) {
        this->uris.remove( uri );
    }
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::oneway( Command* command )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

}

////////////////////////////////////////////////////////////////////////////////
Response* FailoverTransport::request( Command* command AMQCPP_UNUSED )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
Response* FailoverTransport::request( Command* command AMQCPP_UNUSED, unsigned int timeout AMQCPP_UNUSED )
    throw( CommandIOException,
           decaf::lang::exceptions::UnsupportedOperationException ) {

    throw decaf::lang::exceptions::UnsupportedOperationException(
        __FILE__, __LINE__, "FailoverTransport::request - Not Supported" );
}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::start() throw( cms::CMSException ) {

}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::close() throw( cms::CMSException ) {

}

////////////////////////////////////////////////////////////////////////////////
void FailoverTransport::reconnect() {

}
