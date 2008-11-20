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
#include "ActiveMQConnectionFactory.h"

#include <decaf/util/UUID.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <activemq/transport/TransportBuilder.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/support/LibraryInit.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace decaf::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
cms::ConnectionFactory* cms::ConnectionFactory::createCMSConnectionFactory( const std::string& brokerURI )
    throw ( cms::CMSException ) {

    return new ActiveMQConnectionFactory( brokerURI );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory() {

    brokerURL = "tcp://localhost:61616";
    this->username = "";
    this->password = "";
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory(
    const std::string& url,
    const std::string& username,
    const std::string& password ) {

    brokerURL = url;
    this->username = username;
    this->password = password;
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection()
    throw ( cms::CMSException ) {

    return createConnection( brokerURL, username, password, UUID::randomUUID().toString() );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& username,
    const std::string& password )
        throw ( cms::CMSException ) {

    return createConnection( brokerURL, username, password, UUID::randomUUID().toString() );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& username,
    const std::string& password,
    const std::string& clientId )
        throw ( cms::CMSException ) {

    return createConnection( brokerURL, username, password, clientId );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& url,
    const std::string& username,
    const std::string& password,
    const std::string& clientId )
       throw ( cms::CMSException ) {

    // Declared here so that they can be deleted in the catch block
    auto_ptr<Properties> properties( new Properties() );
    auto_ptr<Transport> transport;
    auto_ptr<Connector> connector;
    auto_ptr<ActiveMQConnectionData> connectionData;
    auto_ptr<ActiveMQConnection> connection;
    std::string clientIdLocal = clientId;
    TransportBuilder transportBuilder;

    try{

        // if no Client Id specified, create one
        if( clientIdLocal == "" ) {
            clientIdLocal = UUID::randomUUID().toString();
        }

        // Store login data in the properties
        properties->setProperty(
            ActiveMQConstants::toString(
                ActiveMQConstants::PARAM_USERNAME ), username );
        properties->setProperty(
            ActiveMQConstants::toString(
                ActiveMQConstants::PARAM_PASSWORD ), password );
        properties->setProperty(
            ActiveMQConstants::toString(
                ActiveMQConstants::PARAM_CLIENTID ), clientIdLocal );

        // Use the TransportBuilder to get our Transport
        transport.reset( transportBuilder.buildTransport( url, *properties ) );

        if( transport.get() == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConnectionFactory::createConnection - "
                "failed creating new Transport" );
        }

        // What wire format are we using, defaults to Stomp
        std::string wireFormat =
            properties->getProperty( "wireFormat", "openwire" );

        // Now try and find a factory to create the Connector
        ConnectorFactory* connectorfactory =
            ConnectorFactoryMap::getInstance()->lookup( wireFormat );

        if( connectorfactory == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConnectionFactory::createConnection - "
                "Connector for Wire Format not registered in Map" );
        }

        // Create the Connector.
        connector.reset( connectorfactory->createConnector( *properties, transport.get() ) );

        if( connector.get() == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConnectionFactory::createConnection - "
                "Failed to Create the Connector" );
        }

        // Start the Connector
        connector->start();

        // Create Holder and store the data for the Connection
        connectionData.reset( new ActiveMQConnectionData(
            connector.release(), transport.release(), properties.release() ) );

        // Create and Return the new connection object.
        connection.reset( new ActiveMQConnection( connectionData.release() ) );

        return connection.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
