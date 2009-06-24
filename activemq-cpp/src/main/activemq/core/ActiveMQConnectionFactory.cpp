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

#include <decaf/net/URI.h>
#include <decaf/util/UUID.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/util/URISupport.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
cms::ConnectionFactory* cms::ConnectionFactory::createCMSConnectionFactory( const std::string& brokerURI )
    throw ( cms::CMSException ) {

    return new ActiveMQConnectionFactory( brokerURI );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory() {

    brokerURL = "failover:(tcp://localhost:61616)";
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

    Pointer<Transport> transport;
    Pointer<Properties> properties( new Properties() );
    auto_ptr<ActiveMQConnection> connection;
    std::string clientIdLocal = clientId;

    try{

        // if no Client Id specified, create one
        if( clientIdLocal == "" ) {
            clientIdLocal = UUID::randomUUID().toString();
        }

        // Try to convert the String URL into a valid URI
        URI uri( url );

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

        // Parse out properties so they can be passed to the Connectors.
        activemq::util::URISupport::parseQuery( uri.getQuery(), properties.get() );

        // Use the TransportBuilder to get our Transport
        transport =
            TransportRegistry::getInstance().findFactory( uri.getScheme() )->create( uri );

        if( transport == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConnectionFactory::createConnection - "
                "failed creating new Transport" );
        }

        // Create and Return the new connection object.
        connection.reset( new ActiveMQConnection( transport, properties ) );

        return connection.release();
    }
    catch( cms::CMSException& ex ){
        ex.setMark( __FILE__, __LINE__ );
        throw ex;
    } catch( activemq::exceptions::ActiveMQException& ex ){
        ex.setMark( __FILE__, __LINE__ );
        throw ex.convertToCMSException();
    } catch( decaf::lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
        activemq::exceptions::ActiveMQException amqEx( ex );
        throw amqEx.convertToCMSException();
    } catch( std::exception& ex ) {
        throw cms::CMSException( ex.what(), NULL );
    } catch(...) {
        throw cms::CMSException( "Caught Unknown Exception", NULL );
    }
}
