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

#include <activemq/transport/discovery/AbstractDiscoveryAgentFactory.h>

#include <activemq/util/URISupport.h>
#include <activemq/exceptions/ActiveMQException.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::transport::discovery;

////////////////////////////////////////////////////////////////////////////////
AbstractDiscoveryAgentFactory::~AbstractDiscoveryAgentFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Pointer<DiscoveryAgent> AbstractDiscoveryAgentFactory::createAgent(const URI& agentURI) {

    try {

        Pointer<AbstractDiscoveryAgent> agent = this->doCreateAgent();

        agent->setDiscoveryURI(agentURI);
        Properties options = URISupport::parseParameters(agentURI);
        doConfigureAgent(agent, options);

        return agent;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
void AbstractDiscoveryAgentFactory::doConfigureAgent(Pointer<AbstractDiscoveryAgent> agent, const Properties& options) {

    try {

        agent->setKeepAliveInterval(
            Long::parseLong(options.getProperty("keepAliveInterval", "500")));
        agent->setMaxReconnectDelay(
            Long::parseLong(options.getProperty("maxReconnectDelay", "30000")));
        agent->setUseExponentialBackOff(
            Boolean::parseBoolean(options.getProperty("useExponentialBackOff", "true")));
        agent->setBackOffMultiplier(
            Long::parseLong(options.getProperty("backOffMultiplier", "2")));
        agent->setMaxReconnectAttempts(
            Integer::parseInt(options.getProperty("maxReconnectAttempts", "-1")));
        agent->setInitialReconnectDelay(
            Long::parseLong(options.getProperty("initialReconnectDelay", "10")));
        agent->setGroup(options.getProperty("group", "default"));
        agent->setGroup(options.getProperty("service", ""));
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
