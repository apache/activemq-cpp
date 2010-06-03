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
#include <decaf/util/Properties.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <activemq/transport/TransportRegistry.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/policies/DefaultPrefetchPolicy.h>
#include <activemq/core/policies/DefaultRedeliveryPolicy.h>
#include <activemq/util/URISupport.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::core::policies;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const std::string ActiveMQConnectionFactory::DEFAULT_URI = "failover:(tcp://localhost:61616)";

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core{

    class FactorySettings {
    public:

        Pointer<Properties> properties;

        std::string username;
        std::string password;
        std::string clientId;

        std::string brokerURL;

        bool dispatchAsync;
        bool alwaysSyncSend;
        bool useAsyncSend;
        bool useCompression;
        unsigned int sendTimeout;
        unsigned int closeTimeout;
        unsigned int producerWindowSize;

        cms::ExceptionListener* defaultListener;
        std::auto_ptr<PrefetchPolicy> defaultPrefetchPolicy;
        std::auto_ptr<RedeliveryPolicy> defaultRedeliveryPolicy;

        FactorySettings() : brokerURL( ActiveMQConnectionFactory::DEFAULT_URI ),
                            dispatchAsync( true ),
                            alwaysSyncSend( false ),
                            useAsyncSend( false ),
                            useCompression( false ),
                            sendTimeout( 0 ),
                            closeTimeout( 15000 ),
                            producerWindowSize( 0 ),
                            defaultListener( NULL ),
                            defaultPrefetchPolicy( NULL ),
                            defaultRedeliveryPolicy( NULL ) {

            this->properties.reset( new Properties() );
            this->defaultPrefetchPolicy.reset( new DefaultPrefetchPolicy() );
            this->defaultRedeliveryPolicy.reset( new DefaultRedeliveryPolicy() );
        }

        void updateConfiguration( const URI& uri ) {

            this->brokerURL = uri.toString();
            this->properties->clear();
            activemq::util::URISupport::parseQuery( uri.getQuery(), properties.get() );

            // Check the connection options
            this->alwaysSyncSend = Boolean::parseBoolean(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_ALWAYSSYNCSEND ), "false" ) );

            this->useAsyncSend = Boolean::parseBoolean(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_USEASYNCSEND ), "false" ) );

            this->useCompression = Boolean::parseBoolean(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_USECOMPRESSION ), "false" ) );

            this->dispatchAsync = Boolean::parseBoolean(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_DISPATCHASYNC ), "true" ) );

            this->producerWindowSize = decaf::lang::Integer::parseInt(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_PRODUCERWINDOWSIZE ), "0" ) );

            this->sendTimeout = decaf::lang::Integer::parseInt(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_SENDTIMEOUT ), "0" ) );

            this->closeTimeout = decaf::lang::Integer::parseInt(
                properties->getProperty(
                    core::ActiveMQConstants::toString(
                        core::ActiveMQConstants::CONNECTION_CLOSETIMEOUT ), "15000" ) );

            this->clientId = properties->getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_CLIENTID ), "" );

            this->username = properties->getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_USERNAME ), "" );

            this->password = properties->getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_PASSWORD ), "" );
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
cms::ConnectionFactory* cms::ConnectionFactory::createCMSConnectionFactory( const std::string& brokerURI )
    throw ( cms::CMSException ) {

    return new ActiveMQConnectionFactory( brokerURI );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory() : settings( new FactorySettings() ) {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::ActiveMQConnectionFactory( const std::string& url,
                                                      const std::string& username,
                                                      const std::string& password ) : settings( new FactorySettings() ) {

    settings->brokerURL = url;
    settings->username = username;
    settings->password = password;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnectionFactory::~ActiveMQConnectionFactory() {
    try{
        delete this->settings;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection()
    throw ( cms::CMSException ) {

    return createConnection( settings->brokerURL, settings->username, settings->password, settings->clientId );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& username,
    const std::string& password )
        throw ( cms::CMSException ) {

    return createConnection( settings->brokerURL, username, password, settings->clientId );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& username,
    const std::string& password,
    const std::string& clientId )
        throw ( cms::CMSException ) {

    return createConnection( settings->brokerURL, username, password, clientId );
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::doCreateConnection( const std::string& url,
                                                                const std::string& username,
                                                                const std::string& password,
                                                                const std::string& clientId )
    throw ( cms::CMSException ) {

    Pointer<Transport> transport;
    auto_ptr<ActiveMQConnection> connection;

    try{

        // Try to convert the String URL into a valid URI
        URI uri( url );

        // Update configuration with new authentication info if any was provided.
        this->settings->updateConfiguration( uri );

        // Store login data in the properties
        if( !username.empty() ) {
            this->settings->username = username;
        }
        if( !password.empty() ) {
            this->settings->password = password;
        }
        if( !clientId.empty() ) {
            this->settings->clientId = clientId;
        }

        // Use the TransportBuilder to get our Transport
        transport =
            TransportRegistry::getInstance().findFactory( uri.getScheme() )->create( uri );

        if( transport == NULL ){
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConnectionFactory::createConnection - "
                "failed creating new Transport" );
        }

        Pointer<Properties> properties( this->settings->properties->clone() );

        // Create and Return the new connection object.
        connection.reset( new ActiveMQConnection( transport, properties ) );

        // Set all options parsed from the URI.
        configureConnection( connection.get() );

        // Now start the connection since all other configuration is done.
        transport->start();

        if( !this->settings->clientId.empty() ) {
            connection->setDefaultClientId( this->settings->clientId );
        }

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

////////////////////////////////////////////////////////////////////////////////
cms::Connection* ActiveMQConnectionFactory::createConnection(
    const std::string& url,
    const std::string& username,
    const std::string& password,
    const std::string& clientId )
       throw ( cms::CMSException ) {

    ActiveMQConnectionFactory factory;

    return factory.doCreateConnection( url, username, password, clientId );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::configureConnection( ActiveMQConnection* connection ) {

    connection->setUsername( this->settings->username );
    connection->setPassword( this->settings->password );
    connection->setDispatchAsync( this->settings->dispatchAsync );
    connection->setAlwaysSyncSend( this->settings->alwaysSyncSend );
    connection->setUseAsyncSend( this->settings->useAsyncSend );
    connection->setUseCompression( this->settings->useCompression );
    connection->setSendTimeout( this->settings->sendTimeout );
    connection->setCloseTimeout( this->settings->closeTimeout );
    connection->setProducerWindowSize( this->settings->producerWindowSize );
    connection->setPrefetchPolicy( this->settings->defaultPrefetchPolicy->clone() );
    connection->setRedeliveryPolicy( this->settings->defaultRedeliveryPolicy->clone() );

    if( this->settings->defaultListener ) {
        connection->setExceptionListener( this->settings->defaultListener );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUsername( const std::string& username ) {
    settings->username = username;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnectionFactory::getUsername() const {
    return settings->username;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setPassword( const std::string& password ){
    settings->password = password;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnectionFactory::getPassword() const {
    return settings->password;
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnectionFactory::getClientId() const {
    return this->settings->clientId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setClientId( const std::string& clientId ) {
    this->settings->clientId = clientId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setBrokerURL( const std::string& brokerURL ){
    settings->brokerURL = brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& ActiveMQConnectionFactory::getBrokerURL() const {
    return settings->brokerURL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setExceptionListener( cms::ExceptionListener* listener ) {
    this->settings->defaultListener = listener;
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQConnectionFactory::getExceptionListener() const {
    return this->settings->defaultListener;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setPrefetchPolicy( PrefetchPolicy* policy ) {
    this->settings->defaultPrefetchPolicy.reset( policy );
}

////////////////////////////////////////////////////////////////////////////////
PrefetchPolicy* ActiveMQConnectionFactory::getPrefetchPolicy() const {
    return this->settings->defaultPrefetchPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setRedeliveryPolicy( RedeliveryPolicy* policy ) {
    this->settings->defaultRedeliveryPolicy.reset( policy );
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConnectionFactory::getRedeliveryPolicy() const {
    return this->settings->defaultRedeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isDispatchAsync() const {
    return this->settings->dispatchAsync;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setDispatchAsync( bool value ) {
    this->settings->dispatchAsync = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isAlwaysSyncSend() const {
    return this->settings->alwaysSyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setAlwaysSyncSend( bool value ) {
    this->settings->alwaysSyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isUseAsyncSend() const {
    return this->settings->useAsyncSend;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUseAsyncSend( bool value ) {
    this->settings->useAsyncSend = value;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConnectionFactory::isUseCompression() const {
    return this->settings->useCompression;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setUseCompression( bool value ) {
    this->settings->useCompression = value;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getSendTimeout() const {
    return this->settings->sendTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setSendTimeout( unsigned int timeout ) {
    this->settings->sendTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getCloseTimeout() const {
    return this->settings->closeTimeout;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setCloseTimeout( unsigned int timeout ) {
    this->settings->closeTimeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ActiveMQConnectionFactory::getProducerWindowSize() const {
    return this->settings->producerWindowSize;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactory::setProducerWindowSize( unsigned int windowSize ) {
    this->settings->producerWindowSize = windowSize;
}
