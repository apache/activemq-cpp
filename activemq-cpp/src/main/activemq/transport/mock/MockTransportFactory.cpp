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

#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/correlator/ResponseCorrelator.h>
#include <activemq/transport/logging/LoggingTransport.h>
#include <activemq/wireformat/openwire/OpenWireResponseBuilder.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/mock/MockTransport.h>
#include <activemq/transport/mock/ResponseBuilder.h>
#include <activemq/util/URISupport.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/io/IOException.h>

using namespace activemq;
using namespace activemq::util;
using namespace activemq::wireformat;
using namespace activemq::transport;
using namespace activemq::transport::mock;
using namespace activemq::transport::correlator;
using namespace activemq::transport::logging;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> MockTransportFactory::create(const decaf::net::URI& location) {

    try {

        Properties properties = activemq::util::URISupport::parseQuery(location.getQuery());

        Pointer<WireFormat> wireFormat = this->createWireFormat(properties);

        // Create the initial Transport, then wrap it in the normal Filters
        Pointer<Transport> transport(doCreateComposite(location, wireFormat, properties));

        // Create the Transport for response correlator
        transport.reset(new ResponseCorrelator(transport));

        // If command tracing was enabled, wrap the transport with a logging transport.
        if (properties.getProperty("transport.commandTracingEnabled", "false") == "true") {
            // Create the Transport for response correlator
            transport.reset(new LoggingTransport(transport));
        }

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> MockTransportFactory::createComposite(const decaf::net::URI& location) {

    try {

        Properties properties = activemq::util::URISupport::parseQuery(location.getQuery());

        Pointer<WireFormat> wireFormat = this->createWireFormat(properties);

        // Create the initial Transport, then wrap it in the normal Filters
        return doCreateComposite(location, wireFormat, properties);
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Transport> MockTransportFactory::doCreateComposite(const decaf::net::URI& location AMQCPP_UNUSED,
                                                           const Pointer<wireformat::WireFormat> wireFormat,
                                                           const decaf::util::Properties& properties) {

    try {

        std::string wireFormatName = properties.getProperty("wireFormat", "openwire");

        if (properties.getProperty("failOnCreate", "false") == "true") {
            throw IOException(__FILE__, __LINE__, "Failed to Create MockTransport.");
        }

        Pointer<ResponseBuilder> builder;

        if (wireFormatName == "openwire" || wireFormatName == "stomp") {
            builder.reset(new wireformat::openwire::OpenWireResponseBuilder());
        } else {
            throw ActiveMQException(__FILE__, __LINE__,
                "No Response Builder known for this Wireformat, can't create a Mock.");
        }

        Pointer<MockTransport> transport(new MockTransport(wireFormat, builder));

        transport->setFailOnSendMessage(
            Boolean::parseBoolean(properties.getProperty("failOnSendMessage", "false")));
        transport->setNumSentMessageBeforeFail(
            Integer::parseInt(properties.getProperty("numSentMessageBeforeFail", "0")));
        transport->setFailOnReceiveMessage(
            Boolean::parseBoolean(properties.getProperty("failOnReceiveMessage", "false")));
        transport->setNumReceivedMessageBeforeFail(
            Integer::parseInt(properties.getProperty("numReceivedMessageBeforeFail", "0")));
        transport->setFailOnKeepAliveSends(
            Boolean::parseBoolean(properties.getProperty("failOnKeepAliveSends", "false")));
        transport->setNumSentKeepAlivesBeforeFail(
            Integer::parseInt(properties.getProperty("numSentKeepAlivesBeforeFail", "0")));
        transport->setName(properties.getProperty("name", ""));

        return transport;
    }
    AMQ_CATCH_RETHROW(ActiveMQException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, ActiveMQException)
    AMQ_CATCHALL_THROW(ActiveMQException)
}
