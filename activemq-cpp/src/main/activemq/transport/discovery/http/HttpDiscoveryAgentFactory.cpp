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

#include <activemq/transport/discovery/http/HttpDiscoveryAgentFactory.h>

#include <activemq/transport/discovery/http/HttpDiscoveryAgent.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::transport::discovery;
using namespace activemq::transport::discovery::http;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
HttpDiscoveryAgentFactory::~HttpDiscoveryAgentFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<AbstractDiscoveryAgent> HttpDiscoveryAgentFactory::doCreateAgent() {
    return Pointer<AbstractDiscoveryAgent>(new HttpDiscoveryAgent);
}

////////////////////////////////////////////////////////////////////////////////
void HttpDiscoveryAgentFactory::doConfigureAgent(Pointer<AbstractDiscoveryAgent> agent AMQCPP_UNUSED, const Properties& options AMQCPP_UNUSED) {

    try {

    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
