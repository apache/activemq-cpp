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

#include <activemq/connector/openwire/OpenWireConnector.h>

#include <activemq/concurrent/Concurrent.h>
#include <activemq/transport/BrokerError.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/ExceptionResponse.h>
#include <activemq/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Integer.h>
#include <activemq/connector/openwire/OpenWireConnectorException.h>
#include <activemq/connector/openwire/OpenWireFormatFactory.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;

////////////////////////////////////////////////////////////////////////////////
OpenWireConnector::OpenWireConnector( Transport* transport,
                                      const util::Properties& properties )
    throw ( IllegalArgumentException )
{
    if( transport == NULL )
    {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "OpenWireConnector::OpenWireConnector - Transport cannot be NULL");
    }

    // Create our WireFormatFactory on the stack, only need it once.
    OpenWireFormatFactory wireFormatFactory;

    this->state = DISCONNECTED;
    this->exceptionListener = NULL;
    this->messageListener = NULL;
    this->nextProducerId = 1;
    this->nextTransactionId = 1;
    this->properties.copy( &properties );
    this->wireFormat = dynamic_cast<OpenWireFormat*>(
        wireFormatFactory.createWireFormat( properties ) );
    this->transport = new OpenWireFormatNegotiator( wireFormat, transport, false );

    // Observe the transport for events.
    this->transport->setCommandListener( this );
    this->transport->setTransportExceptionListener( this );

    // Setup the Reader and Writer with a Wire Format pointer.
    this->reader.setOpenWireFormat( wireFormat );
    this->writer.setOpenWireFormat( wireFormat );

    // Setup the reader and writer in the transport.
    this->transport->setCommandReader( &reader );
    this->transport->setCommandWriter( &writer );
}

////////////////////////////////////////////////////////////////////////////////
OpenWireConnector::~OpenWireConnector()
{
    try
    {
        close();

        delete transport;
        delete wireFormat;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
unsigned int OpenWireConnector::getNextProducerId()
{
    synchronized( &mutex )
    {
        return nextProducerId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int OpenWireConnector::getNextTransactionId()
{
    synchronized( &mutex )
    {
        return nextTransactionId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::enforceConnected() throw ( ConnectorException )
{
    if( state != CONNECTED )
    {
        throw OpenWireConnectorException(
            __FILE__, __LINE__,
            "OpenWireConnector::enforceConnected - Not Connected!" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::start() throw( cms::CMSException )
{
    try
    {
        synchronized( &mutex )
        {
            if( state == CONNECTED )
            {
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "OpenWireConnector::start - already started" );
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
void OpenWireConnector::close() throw( cms::CMSException ){

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
void OpenWireConnector::connect()
{
    try
    {
        // Mark this connector as started.
        /*state = this->CONNECTING;

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
//            throw OpenWireConnectorException(
//                __FILE__, __LINE__,
//                "OpenWireConnector::connect - Failed on Connect Request" );
//        }

//        ConnectedCommand* connected =
//            dynamic_cast< ConnectedCommand* >( response );
//
//        if( connected == NULL )
//        {
//            delete response;
//
//            throw OpenWireConnectorException(
//                __FILE__, __LINE__,
//                "OpenWireConnector::connect - "
//                "Response not a connected response" );
//        }

        // TODO

        // Tag us in the Connected State now.
        state = CONNECTED;

        // Clean up
        delete response;*/
    }
    AMQ_CATCH_RETHROW( BrokerError )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::disconnect()
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
SessionInfo* OpenWireConnector::createSession(
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
ConsumerInfo* OpenWireConnector::createConsumer(
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
ConsumerInfo* OpenWireConnector::createDurableConsumer(
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
ProducerInfo* OpenWireConnector::createProducer(
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
cms::Topic* OpenWireConnector::createTopic( const std::string& name,
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
cms::Queue* OpenWireConnector::createQueue( const std::string& name,
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
cms::TemporaryTopic* OpenWireConnector::createTemporaryTopic(
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
cms::TemporaryQueue* OpenWireConnector::createTemporaryQueue(
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
void OpenWireConnector::send( cms::Message* message,
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
void OpenWireConnector::send( std::list<cms::Message*>& messages,
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
void OpenWireConnector::acknowledge( const SessionInfo* session,
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
TransactionInfo* OpenWireConnector::startTransaction(
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
void OpenWireConnector::commit( TransactionInfo* transaction,
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
void OpenWireConnector::rollback( TransactionInfo* transaction,
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
cms::Message* OpenWireConnector::createMessage(
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
cms::BytesMessage* OpenWireConnector::createBytesMessage(
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
cms::TextMessage* OpenWireConnector::createTextMessage(
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
cms::MapMessage* OpenWireConnector::createMapMessage(
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
void OpenWireConnector::unsubscribe( const std::string& name )
    throw ( ConnectorException )
{
    try
    {
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::destroyResource( ConnectorResource* resource )
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
void OpenWireConnector::onCommand( transport::Command* command )
{
    try
    {
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::onTransportException(
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
