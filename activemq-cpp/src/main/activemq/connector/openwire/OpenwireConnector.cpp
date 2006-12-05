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
 
#include <activemq/connector/openwire/OpenwireConnector.h>

#include <activemq/concurrent/Concurrent.h>
#include <activemq/transport/BrokerError.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/ExceptionResponse.h>
#include <activemq/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenwireConnector::OpenwireConnector( Transport* transport, 
                                     const util::Properties& properties )
    throw ( IllegalArgumentException )
{
    if( transport == NULL )
    {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "OpenwireConnector::OpenwireConnector - Transport cannot be NULL");
    }
    
    this->transport = transport;
    this->state = DISCONNECTED;
    this->exceptionListener = NULL;
    this->messageListener = NULL;
    this->nextProducerId = 1;
    this->nextTransactionId = 1;
    this->properties.copy( &properties );
    
    // Observe the transport for events.
    this->transport->setCommandListener( this );
    this->transport->setTransportExceptionListener( this );

    // Setup the reader and writer in the transport.
    this->transport->setCommandReader( &reader );
    this->transport->setCommandWriter( &writer );
}

////////////////////////////////////////////////////////////////////////////////
OpenwireConnector::~OpenwireConnector(void)
{
    try
    {
        close();

        // TODO - Add any cleanup code here        
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
unsigned int OpenwireConnector::getNextProducerId(void)
{
    synchronized( &mutex )
    {
        return nextProducerId++;
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int OpenwireConnector::getNextTransactionId(void)
{
    synchronized( &mutex )
    {
        return nextTransactionId++;
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::enforceConnected( void ) throw ( ConnectorException )
{
    if( state != CONNECTED )
    {
        throw OpenwireConnectorException(
            __FILE__, __LINE__,
            "OpenwireConnector::enforceConnected - Not Connected!" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::start(void) throw( cms::CMSException )
{
    try
    {
        synchronized( &mutex )
        {
            if( state == CONNECTED )
            {
                throw ActiveMQException( 
                    __FILE__, __LINE__, 
                    "OpenwireConnector::start - already started" );
            }
                
            // Start the transport - this establishes the socket.
            transport->start();

            // Send the connect message to the broker.
            connect();         
        }        
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::close(void) throw( cms::CMSException ){
    
    try
    {
        synchronized( &mutex )
        {  
            if( state == this->CONNECTED )
            {
                // Send the disconnect message to the broker.
                disconnect();

                // Close the transport.
                transport->close();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::connect(void)
{
    try
    {
        // Mark this connector as started.
        state = this->CONNECTING;

        // TODO - Create a Connect Command        

        // Encode User Name and Password and Client ID
        string login = getUsername();
        if( login.length() > 0 ){
            cmd.setLogin( login );
        }        
        string password = getPassword();
        if( password.length() > 0 ){
            cmd.setPassword( password );
        }        
        string clientId = getClientId();
        if( clientId.length() > 0 ){
            cmd.setClientId( clientId );
        }

//        Response* response = transport->request( &cmd );
//        
//        if( dynamic_cast< ExceptionResponse* >( response ) != NULL )
//        {
//            delete response;
//            
//            throw OpenwireConnectorException(
//                __FILE__, __LINE__,
//                "OpenwireConnector::connect - Failed on Connect Request" );
//        }

//        ConnectedCommand* connected = 
//            dynamic_cast< ConnectedCommand* >( response );
//
//        if( connected == NULL )
//        {
//            delete response;
//
//            throw OpenwireConnectorException(
//                __FILE__, __LINE__,
//                "OpenwireConnector::connect - "
//                "Response not a connected response" );            
//        }

        // TODO        

        // Tag us in the Connected State now.
        state = CONNECTED;
        
        // Clean up
        delete response;
    }
    AMQ_CATCH_RETHROW( BrokerError )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::disconnect(void)
{
    try
    {
        // Mark state as no longer connected.
        state = this->DISCONNECTED;

        // TODO        
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException );
}

////////////////////////////////////////////////////////////////////////////////
SessionInfo* OpenwireConnector::createSession(
    cms::Session::AcknowledgeMode ackMode ) 
        throw( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* OpenwireConnector::createConsumer(
    const cms::Destination* destination, 
    SessionInfo* session,
    const std::string& selector,
    bool noLocal )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* OpenwireConnector::createDurableConsumer(
    const cms::Topic* topic, 
    SessionInfo* session,
    const std::string& name,
    const std::string& selector,
    bool noLocal )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ProducerInfo* OpenwireConnector::createProducer(
    const cms::Destination* destination, 
    SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* OpenwireConnector::createTopic( const std::string& name, 
                                            SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* OpenwireConnector::createQueue( const std::string& name, 
                                            SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* OpenwireConnector::createTemporaryTopic(
    SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* OpenwireConnector::createTemporaryQueue(
    SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::send( cms::Message* message, 
                              ProducerInfo* producerInfo ) 
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::send( std::list<cms::Message*>& messages,
                              ProducerInfo* producerInfo ) 
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        list< cms::Message* >::const_iterator itr = messages.begin();
        
        for( ; itr != messages.end(); ++itr )
        {
            this->send( *itr, producerInfo );
        }
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::acknowledge( const SessionInfo* session,
                                     const cms::Message* message,
                                     AckType ackType = ConsumedAck )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
TransactionInfo* OpenwireConnector::startTransaction(
    SessionInfo* session ) 
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::commit( TransactionInfo* transaction, 
                                SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::rollback( TransactionInfo* transaction, 
                                  SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* OpenwireConnector::createMessage(
    SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* OpenwireConnector::createBytesMessage(
    SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();
        
        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* OpenwireConnector::createTextMessage(
    SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO        
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* OpenwireConnector::createMapMessage(
    SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::unsubscribe( const std::string& name )
    throw ( ConnectorException )
{
    try
    {
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::destroyResource( ConnectorResource* resource )
    throw ( ConnectorException )
{
    try
    {
        ConsumerInfo* consumer = 
            dynamic_cast<ConsumerInfo*>(resource);
        SessionInfo* session = 
            dynamic_cast<SessionInfo*>(resource);

        // TODO

        // No matter what we end it here.
        delete resource;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::onCommand( transport::Command* command )
{
    try
    {
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireConnector::onTransportException( 
    transport::Transport* source, 
    const exceptions::ActiveMQException& ex )
{
    try
    {
        // Inform the user.
        fire( ex );
        
        // Close down.
        close();
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}
