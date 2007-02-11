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
#include <activemq/util/Guid.h>
#include <activemq/connector/openwire/OpenWireConnectorException.h>
#include <activemq/connector/openwire/OpenWireFormatFactory.h>

#include <activemq/connector/openwire/commands/ConnectionId.h>
#include <activemq/connector/openwire/commands/RemoveInfo.h>
#include <activemq/connector/openwire/commands/ShutdownInfo.h>
#include <activemq/connector/openwire/commands/SessionInfo.h>
#include <activemq/connector/openwire/commands/MessageDispatch.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/connector/openwire/commands/BrokerInfo.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::util;
using namespace activemq::transport;
using namespace activemq::exceptions;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

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
    this->brokerInfo = NULL;
    this->brokerWireFormatInfo = NULL;
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
        delete brokerInfo;
        delete brokerWireFormatInfo;
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
        state = this->CONNECTING;

        // Fill in our connection info.
        connectionInfo.setUserName( getUsername() );
        connectionInfo.setPassword( getPassword() );

        // Get or Create a Client Id
        string clientId = getClientId();
        if( clientId.length() > 0 ){
            connectionInfo.setClientId( clientId );
        } else {
            connectionInfo.setClientId( Guid::createGUIDString() );
        }

        // Generate a connectionId
        ConnectionId* connectionId = new ConnectionId();
        connectionId->setValue( Guid::createGUIDString() );
        connectionInfo.setConnectionId( connectionId );

        // Now we ping the broker and see if we get an ack / nack
        Response* response = transport->request( &connectionInfo );

        if( dynamic_cast<ExceptionResponse*>( response ) != NULL )
        {
            delete response;

            throw OpenWireConnectorException(
                __FILE__,
                __LINE__,
                "OpenWireConnector::connect - Failed on Connect Request" );
        }

        // Tag us in the Connected State now.
        state = CONNECTED;

        // Clean up the ack
        delete response;
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
        state = DISCONNECTED;

        // Remove our ConnectionId from the Broker
        RemoveInfo remove;
        remove.setObjectId( connectionInfo.getConnectionId() );
        transport->oneway( &remove );

        // Send the disconnect command to the broker.
        ShutdownInfo shutdown;
        transport->oneway( &shutdown );

    } catch( CommandIOException& ex ){
        transport->close();
        throw ex;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException );
}

////////////////////////////////////////////////////////////////////////////////
connector::SessionInfo* OpenWireConnector::createSession(
    cms::Session::AcknowledgeMode ackMode )
        throw( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* OpenWireConnector::createConsumer(
    const cms::Destination* destination,
    connector::SessionInfo* session,
    const std::string& selector,
    bool noLocal )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* OpenWireConnector::createDurableConsumer(
    const cms::Topic* topic,
    connector::SessionInfo* session,
    const std::string& name,
    const std::string& selector,
    bool noLocal )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
ProducerInfo* OpenWireConnector::createProducer(
    const cms::Destination* destination,
    connector::SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* OpenWireConnector::createTopic( const std::string& name,
                                            connector::SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* OpenWireConnector::createQueue( const std::string& name,
                                            connector::SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* OpenWireConnector::createTemporaryTopic(
    connector::SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* OpenWireConnector::createTemporaryQueue(
    connector::SessionInfo* session )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
        return NULL;
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

//        const SessionInfo* session = producerInfo->getSessionInfo();
//        Command* command = dynamic_cast< transport::Command* >( message );
//
//        if( command == NULL )
//        {
//            throw StompConnectorException(
//                __FILE__, __LINE__,
//                "StompConnector::send - "
//                "Message is not a valid stomp type.");
//        }
//
//        if( session->getAckMode() == cms::Session::SESSION_TRANSACTED )
//        {
//            StompCommand* stompCommand =
//                dynamic_cast< StompCommand* >( message );
//
//            if( stompCommand == NULL )
//            {
//                throw StompConnectorException(
//                    __FILE__, __LINE__,
//                    "StompConnector::send - "
//                    "Message is not a valid stomp type.");
//            }
//
//            stompCommand->setTransactionId(
//                Integer::toString(
//                    session->getTransactionInfo()->getTransactionId() ) );
//        }
//
//        // Send it
//        transport->oneway( command );
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
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
void OpenWireConnector::acknowledge( const connector::SessionInfo* session,
                                     const cms::Message* message,
                                     AckType ackType = ConsumedAck )
    throw ( ConnectorException )
{
    try {

//        // Auto to Stomp means don't do anything, so we drop it here
//        // for client acknowledge we have to send and ack.
//        if( session->getAckMode() == cms::Session::CLIENT_ACKNOWLEDGE ||
//            session->getAckMode() == cms::Session::SESSION_TRANSACTED )
//        {
//            AckCommand cmd;
//
//            if( message->getCMSMessageId() == "" )
//            {
//                throw StompConnectorException(
//                    __FILE__, __LINE__,
//                    "StompConnector::send - "
//                    "Message has no Message Id, cannot ack.");
//            }
//
//            cmd.setMessageId( message->getCMSMessageId() );
//
//            if( session->getAckMode() == cms::Session::SESSION_TRANSACTED )
//            {
//                cmd.setTransactionId(
//                    Integer::toString(
//                        session->getTransactionInfo()->getTransactionId() ) );
//            }
//
//            transport->oneway( &cmd );
//        }
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
TransactionInfo* OpenWireConnector::startTransaction(
    connector::SessionInfo* session )
        throw ( ConnectorException )
{
    try {

        enforceConnected();

//        TransactionInfo* transaction = new StompTransactionInfo();
//
//        transaction->setTransactionId( getNextTransactionId() );
//
//        session->setTransactionInfo( transaction );
//
//        BeginCommand cmd;
//
//        cmd.setTransactionId(
//                Integer::toString( transaction->getTransactionId() ) );
//
//        transport->oneway( &cmd );
//
//        return transaction;

        return NULL;
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::commit( TransactionInfo* transaction,
                                connector::SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

//        CommitCommand cmd;
//
//        cmd.setTransactionId(
//                Integer::toString( transaction->getTransactionId() ) );
//
//        transport->oneway( &cmd );
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::rollback( TransactionInfo* transaction,
                                  connector::SessionInfo* session )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

//        AbortCommand cmd;
//
//        cmd.setTransactionId(
//                Integer::toString( transaction->getTransactionId() ) );
//
//        transport->oneway( &cmd );
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* OpenWireConnector::createMessage(
    connector::SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO

        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* OpenWireConnector::createBytesMessage(
    connector::SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO

        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* OpenWireConnector::createTextMessage(
    connector::SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO

        return NULL;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* OpenWireConnector::createMapMessage(
    connector::SessionInfo* session,
    TransactionInfo* transaction )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO

        return NULL;
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
        connector::ConsumerInfo* consumer =
            dynamic_cast<connector::ConsumerInfo*>(resource);
        connector::SessionInfo* session =
            dynamic_cast<connector::SessionInfo*>(resource);

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
        MessageDispatch* dispatch =
            dynamic_cast<MessageDispatch*>( command );
        WireFormatInfo* brokerWireFormatInfo =
            dynamic_cast<WireFormatInfo*>( command );
        BrokerInfo* brokerInfo =
            dynamic_cast<BrokerInfo*>( command );
        ShutdownInfo* shutdownInfo =
            dynamic_cast<ShutdownInfo*>( command );

        if( dispatch != NULL ) {

//            ConsumerId* consumerId = dispatch->getConsumerId();
//            MessageConsumer consumer = (MessageConsumer) consumers[consumerId];
//            if (consumer == null)
//            {
//                Tracer.Error("No such consumer active: " + consumerId);
//            }
//            else
//            {
//                ActiveMQMessage message = (ActiveMQMessage) dispatch.Message;
//                consumer.Dispatch(message);
//            }

            delete command;

        } else if( brokerWireFormatInfo != NULL ) {
            this->brokerWireFormatInfo = brokerWireFormatInfo;
        } else if( brokerInfo != NULL ) {
            this->brokerInfo = brokerInfo;
        } else if( shutdownInfo != NULL ) {

            if( state != DISCONNECTED ) {
                fire( CommandIOException(
                    __FILE__,
                    __LINE__,
                    "OpenWireConnector::onCommand - "
                    "Broker closed this connection."));
            }

            delete command;
        }
        else
        {
            //LOGCMS_WARN( logger, "Received an unknown command" );
            delete command;
        }
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
        // We're disconnected - the asynchronous error is expected.
        if( state == DISCONNECTED ){
            return;
        }

        // We were not closing - log the stack trace.
        //LOGCMS_WARN( logger, ex.getStackTraceString() );

        // Inform the user of the error.
        fire( ex );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( ConnectorException );
}
