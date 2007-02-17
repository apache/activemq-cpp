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
#include <activemq/transport/Transport.h>
#include <activemq/exceptions/UnsupportedOperationException.h>
#include <activemq/util/Integer.h>
#include <activemq/util/Boolean.h>
#include <activemq/util/Long.h>
#include <activemq/util/Guid.h>
#include <activemq/connector/openwire/OpenWireConnectorException.h>
#include <activemq/connector/openwire/OpenWireSessionInfo.h>
#include <activemq/connector/openwire/OpenWireProducerInfo.h>
#include <activemq/connector/openwire/OpenWireConsumerInfo.h>
#include <activemq/connector/openwire/OpenWireTransactionInfo.h>
#include <activemq/connector/openwire/BrokerException.h>
#include <activemq/connector/openwire/OpenWireFormatFactory.h>

#include <activemq/connector/openwire/commands/ActiveMQTempTopic.h>
#include <activemq/connector/openwire/commands/ActiveMQTempQueue.h>
#include <activemq/connector/openwire/commands/ConnectionId.h>
#include <activemq/connector/openwire/commands/DestinationInfo.h>
#include <activemq/connector/openwire/commands/RemoveInfo.h>
#include <activemq/connector/openwire/commands/ShutdownInfo.h>
#include <activemq/connector/openwire/commands/SessionInfo.h>
#include <activemq/connector/openwire/commands/Message.h>
#include <activemq/connector/openwire/commands/MessageAck.h>
#include <activemq/connector/openwire/commands/MessageDispatch.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/connector/openwire/commands/BrokerInfo.h>
#include <activemq/connector/openwire/commands/BrokerError.h>
#include <activemq/connector/openwire/commands/ActiveMQTopic.h>
#include <activemq/connector/openwire/commands/ActiveMQQueue.h>
#include <activemq/connector/openwire/commands/ExceptionResponse.h>
#include <activemq/connector/openwire/commands/BrokerError.h>

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
    this->brokerInfo = NULL;
    this->brokerWireFormatInfo = NULL;
    this->nextConsumerId = 1;
    this->nextProducerId = 1;
    this->nextTransactionId = 1;
    this->nextSessionId = 1;
    this->nextTempDestinationId = 1;
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
long long OpenWireConnector::getNextConsumerId()
{
    synchronized( &mutex )
    {
        return nextConsumerId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
long long OpenWireConnector::getNextProducerId()
{
    synchronized( &mutex )
    {
        return nextProducerId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
long long OpenWireConnector::getNextTransactionId()
{
    synchronized( &mutex )
    {
        return nextTransactionId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
long long OpenWireConnector::getNextSessionId()
{
    synchronized( &mutex )
    {
        return nextSessionId++;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
long long OpenWireConnector::getNextTempDestinationId()
{
    synchronized( &mutex )
    {
        return nextTempDestinationId++;
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
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::close() throw( cms::CMSException ){

    try
    {
        synchronized( &mutex )
        {
            if( state == CONNECTED )
            {
                // Send the disconnect message to the broker.
                disconnect();

                // Close the transport.
                transport->close();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::connect() throw (ConnectorException)
{
    try
    {
        // Mark this connector as started.
        state = CONNECTING;

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
        commands::ConnectionId* connectionId = new commands::ConnectionId();
        connectionId->setValue( Guid::createGUIDString() );
        connectionInfo.setConnectionId( connectionId );

        // Now we ping the broker and see if we get an ack / nack
        Response* response = syncRequest( &connectionInfo );

        // Tag us in the Connected State now.
        state = CONNECTED;

        // Clean up the ack
        delete response;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::disconnect() throw (ConnectorException)
{
    try
    {
        // Mark state as no longer connected.
        state = DISCONNECTED;

        // Remove our ConnectionId from the Broker
        disposeOf( connectionInfo.getConnectionId() );

        // Send the disconnect command to the broker.
        commands::ShutdownInfo shutdown;
        oneway( &shutdown );

    } catch( ConnectorException& ex ){
        try{
            transport->close();
        } catch( cms::CMSException& e ){}
        throw ex;
    } catch( ... ) {
        try{
            transport->close();
        } catch( cms::CMSException& e ){}

        throw OpenWireConnectorException(__FILE__, __LINE__, "Caught unknown exception" );
    }

}

////////////////////////////////////////////////////////////////////////////////
connector::SessionInfo* OpenWireConnector::createSession(
    cms::Session::AcknowledgeMode ackMode )
        throw( ConnectorException )
{
    try
    {
        enforceConnected();

        // Create and initialize a new SessionInfo object
        commands::SessionInfo* info = new commands::SessionInfo();
        commands::SessionId* sessionId = new commands::SessionId();
        sessionId->setConnectionId( connectionInfo.getConnectionId()->getValue() );
        sessionId->setValue( getNextSessionId() );
        info->setSessionId( sessionId );
        OpenWireSessionInfo* session = new OpenWireSessionInfo();

        try{

            // Send the subscription message to the broker.
            Response* response = syncRequest(info);

            // The broker did not return an error - this is good.
            // Just discard the response.
            delete response;

            // Create the Connector Session Wrapper Object and fill in its
            // data
            session->setSessionInfo( info );
            session->setAckMode( ackMode );

            // Return the session info.
            return session;

        } catch( ConnectorException& ex ) {

            // Something bad happened - free the session info object.
            delete info;
            delete session;

            ex.setMark(__FILE__, __LINE__);
            throw ex;
        }
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* OpenWireConnector::createConsumer(
    const cms::Destination* destination,
    connector::SessionInfo* session,
    const std::string& selector,
    bool noLocal AMQCPP_UNUSED )
        throw ( ConnectorException )
{
    OpenWireConsumerInfo* consumer = NULL;
    commands::ConsumerInfo* consumerInfo = NULL;

    try
    {
        enforceConnected();

        consumer = new OpenWireConsumerInfo();
        consumerInfo = new commands::ConsumerInfo();
        consumer->setConsumerInfo( consumerInfo );

        commands::ConsumerId* consumerId = new commands::ConsumerId();
        consumerInfo->setConsumerId( consumerId );

        consumerId->setConnectionId( session->getConnectionId() );
        consumerId->setSessionId( session->getSessionId() );
        consumerId->setValue( getNextConsumerId() );
        consumerInfo->setSelector( selector );

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const commands::ActiveMQDestination* amqDestination =
            dynamic_cast<const commands::ActiveMQDestination*>(destination);
        if( amqDestination == NULL ) {
            throw ConnectorException( __FILE__, __LINE__,
                "Destination was either NULL or not created by this OpenWireConnector" );
        }


        consumerInfo->setDestination( dynamic_cast<commands::ActiveMQDestination*>(
            amqDestination->cloneDataStructure()) );

        // Get any options specified in the destination and apply them to the
        // ConsumerInfo object.
        const Properties& options = amqDestination->getOptions();
        consumerInfo->setBrowser( Boolean::parseBoolean(
            options.getProperty( "consumer.browser", "false" )) );
        consumerInfo->setPrefetchSize( Integer::parseInt(
            options.getProperty( "consumer.prefetchSize", "0" )) );
        consumerInfo->setMaximumPendingMessageLimit( Integer::parseInt(
            options.getProperty( "consumer.maximumPendingMessageLimit", "0" )) );
        consumerInfo->setDispatchAsync( Boolean::parseBoolean(
            options.getProperty( "consumer.dispatchAsync", "false" )) );
        consumerInfo->setNoLocal( Boolean::parseBoolean(
            options.getProperty( "consumer.noLocal", "false" )) );
        consumerInfo->setExclusive( Boolean::parseBoolean(
            options.getProperty( "consumer.exclusive", "false" )) );
        consumerInfo->setRetroactive( Boolean::parseBoolean(
            options.getProperty( "consumer.retroactive", "false" )) );
        consumerInfo->setPriority( Integer::parseInt(
            options.getProperty( "consumer.priority", "0" )) );
        consumerInfo->setNetworkSubscription( Boolean::parseBoolean(
            options.getProperty( "consumer.networkSubscription", "false" )) );
        consumerInfo->setOptimizedAcknowledge( Boolean::parseBoolean(
            options.getProperty( "consumer.optimizedAcknowledge", "false" )) );
        consumerInfo->setNoRangeAcks( Boolean::parseBoolean(
            options.getProperty( "consumer.noRangeAcks", "false" )) );

        // Send the message to the broker.
        Response* response = syncRequest(consumerInfo);

        // The broker did not return an error - this is good.
        // Just discard the response.
        delete response;

        return consumer;

    } catch( ConnectorException& ex ) {
        delete consumer;
        delete consumerInfo;

        ex.setMark( __FILE__, __LINE__ );
        throw ex;
    } catch( std::exception& ex ) {
        delete consumer;
        delete consumerInfo;

        throw OpenWireConnectorException( __FILE__, __LINE__,
            ex.what() );

    } catch( ... ) {
        delete consumer;
        delete consumerInfo;

        throw OpenWireConnectorException( __FILE__, __LINE__,
            "caught unknown exception" );
    }
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
ProducerInfo* OpenWireConnector::createProducer(
    const cms::Destination* destination,
    connector::SessionInfo* session )
        throw ( ConnectorException )
{
    OpenWireProducerInfo* producer = NULL;
    commands::ProducerInfo* producerInfo = NULL;

    try
    {
        enforceConnected();

        producer = new OpenWireProducerInfo();
        producerInfo = new commands::ProducerInfo();
        producer->setProducerInfo( producerInfo );

        commands::ProducerId* producerId = new commands::ProducerId();
        producerInfo->setProducerId( producerId );

        producerId->setConnectionId( session->getConnectionId() );
        producerId->setSessionId( session->getSessionId() );
        producerId->setValue( getNextProducerId() );

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const commands::ActiveMQDestination* amqDestination =
            dynamic_cast<const commands::ActiveMQDestination*>(destination);
        if( amqDestination == NULL ) {
            throw ConnectorException( __FILE__, __LINE__,
                "Destination was either NULL or not created by this OpenWireConnector" );
        }

        // Get any options specified in the destination and apply them to the
        // ProducerInfo object.
        producerInfo->setDestination( dynamic_cast<commands::ActiveMQDestination*>(
            amqDestination->cloneDataStructure()) );
        const Properties& options = amqDestination->getOptions();
        producerInfo->setDispatchAsync( Boolean::parseBoolean(
            options.getProperty( "producer.dispatchAsync", "false" )) );

        // Send the message to the broker.
        Response* response = syncRequest(producerInfo);

        // The broker did not return an error - this is good.
        // Just discard the response.
        delete response;

        return producer;

    } catch( ConnectorException& ex ) {
        delete producer;
        delete producerInfo;

        ex.setMark( __FILE__, __LINE__ );
        throw ex;
    } catch( std::exception& ex ) {
        delete producer;
        delete producerInfo;

        throw OpenWireConnectorException( __FILE__, __LINE__,
            ex.what() );

    } catch( ... ) {
        delete producer;
        delete producerInfo;

        throw OpenWireConnectorException( __FILE__, __LINE__,
            "caught unknown exception" );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* OpenWireConnector::createTopic( const std::string& name,
                                            connector::SessionInfo* session AMQCPP_UNUSED )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        return new commands::ActiveMQTopic( name );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* OpenWireConnector::createQueue( const std::string& name,
                                            connector::SessionInfo* session AMQCPP_UNUSED )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        return new commands::ActiveMQQueue( name );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* OpenWireConnector::createTemporaryTopic(
    connector::SessionInfo* session AMQCPP_UNUSED )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        commands::ActiveMQTempTopic* topic = new
            commands::ActiveMQTempTopic( createTemporaryDestinationName() );

        // Register it with the Broker
        this->createTemporaryDestination( topic );

        return topic;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* OpenWireConnector::createTemporaryQueue(
    connector::SessionInfo* session AMQCPP_UNUSED )
        throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        commands::ActiveMQTempQueue* queue = new
            commands::ActiveMQTempQueue( createTemporaryDestinationName() );

        // Register it with the Broker
        this->createTemporaryDestination( queue );

        return queue;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::acknowledge( const SessionInfo* session,
                                     const ConsumerInfo* consumer,
                                     const cms::Message* message,
                                     AckType ackType = ConsumedAck )
    throw ( ConnectorException )
{
    try {

        if( session->getAckMode() == cms::Session::CLIENT_ACKNOWLEDGE ||
            session->getAckMode() == cms::Session::SESSION_TRANSACTED ) {

            const commands::Message* amqMessage =
                dynamic_cast<const commands::Message*>( message );

            if( amqMessage == NULL ) {
                throw OpenWireConnectorException(
                    __FILE__, __LINE__,
                    "OpenWireConnector::acknowledge - "
                    "Message was not a commands::Message derivation.");
            }

            const OpenWireConsumerInfo* consumerInfo =
                dynamic_cast<const OpenWireConsumerInfo*>( consumer );

            if( consumerInfo == NULL ) {
                throw OpenWireConnectorException(
                    __FILE__, __LINE__,
                    "OpenWireConnector::acknowledge - "
                    "Consumer was not of the OpenWire flavor.");
            }

            commands::MessageAck ack;
            ack.setAckType( (int)ackType );
            ack.setConsumerId(
                dynamic_cast<commands::ConsumerId*>(
                    consumerInfo->getConsumerInfo()->
                        getConsumerId()->cloneDataStructure() ) );
            ack.setDestination(
                dynamic_cast<commands::ActiveMQDestination*>(
                    amqMessage->getDestination()->cloneDataStructure() ) );
            ack.setFirstMessageId(
                dynamic_cast<commands::MessageId*>(
                    amqMessage->getMessageId()->cloneDataStructure() ) );
            ack.setLastMessageId(
                dynamic_cast<commands::MessageId*>(
                    amqMessage->getMessageId()->cloneDataStructure() ) );
            ack.setMessageCount( 1 );

            if( session->getAckMode() == cms::Session::SESSION_TRANSACTED ) {

                const OpenWireTransactionInfo* transactionInfo =
                    dynamic_cast<const OpenWireTransactionInfo*>(
                        session->getTransactionInfo() );

                if( transactionInfo == NULL ) {
                    throw OpenWireConnectorException(
                        __FILE__, __LINE__,
                        "OpenWireConnector::acknowledge - "
                        "Transacted Session, has no Transaction Info.");
                }

                ack.setTransactionId(
                    dynamic_cast<commands::TransactionId*>(
                        transactionInfo->getTransactionInfo()->
                            getTransactionId()->cloneDataStructure() ) );
            }

            transport->oneway( &ack );
        }
    }
    catch( CommandIOException& ex ){
        transport->close();
        throw ConnectorException( __FILE__, __LINE__,
            ex.what() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::unsubscribe( const std::string& name )
    throw ( ConnectorException )
{
    try
    {
        enforceConnected();

        // TODO
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::destroyResource( ConnectorResource* resource )
    throw ( ConnectorException )
{
    try
    {
        if( resource == NULL ) {
            return;
        }

        commands::DataStructure* dataStructure = NULL;

        OpenWireConsumerInfo* consumer =
            dynamic_cast<OpenWireConsumerInfo*>(resource);
        OpenWireProducerInfo* producer =
            dynamic_cast<OpenWireProducerInfo*>(resource);
        OpenWireSessionInfo* session =
            dynamic_cast<OpenWireSessionInfo*>(resource);
        commands::ActiveMQTempDestination* tempDestination =
            dynamic_cast<commands::ActiveMQTempDestination*>(resource);

        if( consumer != NULL ) {
            dataStructure = consumer->getConsumerInfo();
        } else if( producer != NULL ) {
            dataStructure = producer->getProducerInfo();
        } else if( session != NULL ) {
            dataStructure = session->getSessionInfo();
        } else if( tempDestination != NULL ) {
            // User deletes these
            destroyTemporaryDestination( tempDestination );
            return;
        }

        if( dataStructure == NULL ) {
            throw OpenWireConnectorException(__FILE__,__LINE__,
                "attempting to destroy an invalid resource");
        }

        // Dispose of this data structure at the broker.
        disposeOf( dataStructure );

        // No matter what we end it here.
        delete resource;
    }
    catch( ConnectorException& ex ) {
        delete resource;
        ex.setMark(__FILE__, __LINE__ );
        throw ex;
    }
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::onCommand( transport::Command* command )
{
    try
    {
        commands::MessageDispatch* dispatch =
            dynamic_cast<commands::MessageDispatch*>( command );
        commands::WireFormatInfo* brokerWireFormatInfo =
            dynamic_cast<commands::WireFormatInfo*>( command );
        commands::BrokerInfo* brokerInfo =
            dynamic_cast<commands::BrokerInfo*>( command );
        commands::ShutdownInfo* shutdownInfo =
            dynamic_cast<commands::ShutdownInfo*>( command );

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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::onTransportException(
    transport::Transport* source AMQCPP_UNUSED,
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
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::oneway( Command* command )
    throw ( ConnectorException )
{
    try
    {
        transport->oneway(command);
    }
    AMQ_CATCH_EXCEPTION_CONVERT( CommandIOException, OpenWireConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( UnsupportedOperationException, OpenWireConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
Response* OpenWireConnector::syncRequest( Command* command )
    throw ( ConnectorException )
{
    try
    {
        Response* response = transport->request(command);

        commands::ExceptionResponse* exceptionResponse = dynamic_cast<commands::ExceptionResponse*>(response);
        if( exceptionResponse != NULL )
        {
            // Create an exception to hold the error information.
            commands::BrokerError* brokerError = dynamic_cast<commands::BrokerError*>(exceptionResponse->getException());
            BrokerException exception( __FILE__, __LINE__, brokerError );

            // Free the response command.
            delete response;

            // Throw the exception.
            throw exception;
        }

        // Nothing bad happened - just return the response.
        return response;
    }
    AMQ_CATCH_EXCEPTION_CONVERT( CommandIOException, OpenWireConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( UnsupportedOperationException, OpenWireConnectorException )
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::disposeOf(
    commands::DataStructure* objectId ) throw ( ConnectorException )
{
    try
    {
        commands::RemoveInfo command;
        command.setObjectId( objectId->cloneDataStructure() );
        oneway( &command );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::createTemporaryDestination(
    commands::ActiveMQTempDestination* tempDestination ) throw ( ConnectorException ) {

    try {

        commands::DestinationInfo command;
        command.setConnectionId(
            dynamic_cast<commands::ConnectionId*>(
                connectionInfo.getConnectionId()->cloneDataStructure() ) );
        command.setOperationType( 0 ); // 0 is add
        command.setDestination(
            dynamic_cast<commands::ActiveMQDestination*>(
                tempDestination->cloneDataStructure() ) );

        // Send the message to the broker.
        Response* response = syncRequest(&command);

        // The broker did not return an error - this is good.
        // Just discard the response.
        delete response;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenWireConnector::destroyTemporaryDestination(
    commands::ActiveMQTempDestination* tempDestination ) throw ( ConnectorException ) {

    try {

        commands::DestinationInfo command;
        command.setConnectionId(
            dynamic_cast<commands::ConnectionId*>(
                connectionInfo.getConnectionId()->cloneDataStructure() ) );
        command.setOperationType( 1 ); // 1 is remove
        command.setDestination(
            dynamic_cast<commands::ActiveMQDestination*>(
                tempDestination->cloneDataStructure() ) );

        // Send the message to the broker.
        Response* response = syncRequest(&command);

        // The broker did not return an error - this is good.
        // Just discard the response.
        delete response;
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
std::string OpenWireConnector::createTemporaryDestinationName()
    throw ( ConnectorException )
{
    try {
        return connectionInfo.getConnectionId()->getValue() + ":" +
               util::Long::toString( getNextTempDestinationId() );
    }
    AMQ_CATCH_RETHROW( ConnectorException )
    AMQ_CATCHALL_THROW( OpenWireConnectorException )
}
