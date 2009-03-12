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

#include "ActiveMQConnection.h"

#include <cms/Session.h>

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/BrokerException.h>

#include <decaf/lang/Boolean.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/UUID.h>

#include <activemq/commands/Command.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/BrokerInfo.h>
#include <activemq/commands/BrokerError.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/KeepAliveInfo.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/commands/ProducerAck.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection( const Pointer<transport::Transport>& transport,
                                        const Pointer<decaf::util::Properties>& properties ) :
    ActiveMQConnectionSupport( transport, properties ),
    connectionMetaData( new ActiveMQConnectionMetaData() ),
    connectionInfo( new ConnectionInfo() ),
    started( false ),
    closed( false ),
    exceptionListener( NULL ) {

    // Register for messages and exceptions from the connector.
    transport->setTransportListener( this );

    // Now Start the Transport
    transport->start();

    // Attempt to register with the Broker
    this->connect();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::~ActiveMQConnection() {
    try {
        this->close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addDispatcher(
    const decaf::lang::Pointer<ConsumerId>& consumer, Dispatcher* dispatcher )
        throw ( cms::CMSException ) {

    // Add the consumer to the map.
    synchronized( &dispatchers ) {
        dispatchers.put( consumer, dispatcher );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher(
    const decaf::lang::Pointer<ConsumerId>& consumer )
        throw ( cms::CMSException ) {

    // Remove the consumer from the map.
    synchronized( &dispatchers ) {
        dispatchers.remove( consumer );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession() throw ( cms::CMSException ) {
    try {
        return createSession( Session::AUTO_ACKNOWLEDGE );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession(
    cms::Session::AcknowledgeMode ackMode ) throw ( cms::CMSException ) {

    try {

        enforceConnected();

        // Create and initialize a new SessionInfo object
        Pointer<SessionInfo> sessionInfo( new SessionInfo() );
        decaf::lang::Pointer<SessionId> sessionId( new SessionId() );
        sessionId->setConnectionId( connectionInfo->getConnectionId()->getValue() );
        sessionId->setValue( this->getNextSessionId() );
        sessionInfo->setSessionId( sessionId );

        // Send the subscription message to the broker.
        syncRequest( sessionInfo );

        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            sessionInfo, ackMode, this->getProperties(), this );

        // Add the session to the set of active sessions.
        synchronized( &activeSessions ) {
            activeSessions.add( session );
        }

        // If we're already started, start the session.
        if( started ) {
            session->start();
        }

        return session;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeSession( ActiveMQSession* session )
    throw ( cms::CMSException ) {

    try {

        // Remove this session from the set of active sessions.
        synchronized( &activeSessions ) {
            activeSessions.remove( session );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addProducer( ActiveMQProducer* producer )
    throw ( cms::CMSException ) {

    try {

        // Add this producer from the set of active consumer.
        synchronized( &activeProducers ) {
            activeProducers.put( producer->getProducerInfo().getProducerId(), producer );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeProducer( const decaf::lang::Pointer<ProducerId>& producerId )
    throw ( cms::CMSException ) {

    try {

        // Remove this producer from the set of active consumer.
        synchronized( &activeProducers ) {
            activeProducers.remove( producerId );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getClientID() const {

    if( this->isClosed() ) {
        return "";
    }

    return this->getClientId();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::close() throw ( cms::CMSException )
{
    try {

        if( this->isClosed() ) {
            return;
        }

        // Get the complete list of active sessions.
        std::vector<ActiveMQSession*> allSessions;
        synchronized( &activeSessions ) {
            allSessions = activeSessions.toArray();
        }

        // Close all of the resources.
        for( unsigned int ix=0; ix<allSessions.size(); ++ix ){
            cms::Session* session = allSessions[ix];
            try{
                session->close();
            } catch( cms::CMSException& ex ){
                /* Absorb */
            }
        }

        // Now inform the Broker we are shutting down.
        this->disconnect();

        // Once current deliveries are done this stops the delivery
        // of any new messages.
        this->started = false;
        this->closed = true;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::start() throw ( cms::CMSException ) {
    try{

        enforceConnected();

        // This starts or restarts the delivery of all incomming messages
        // messages delivered while this connection is stopped are dropped
        // and not acknowledged.
        started = true;

        // Start all the sessions.
        std::vector<ActiveMQSession*> sessions = activeSessions.toArray();
        for( unsigned int ix=0; ix<sessions.size(); ++ix ) {
            sessions[ix]->start();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::stop() throw ( cms::CMSException ) {

    try {

        enforceConnected();

        // Once current deliveries are done this stops the delivery of any
        // new messages.
        started = false;

        std::auto_ptr< Iterator<ActiveMQSession*> > iter( activeSessions.iterator() );

        while( iter->hasNext() ){
            iter->next()->stop();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::connect() throw ( activemq::exceptions::ActiveMQException ) {

    try{

        // Start the Transport
        this->startupTransport();

        // Fill in our connection info.
        connectionInfo->setUserName( this->getUsername() );
        connectionInfo->setPassword( this->getPassword() );

        // Get or Create a Client Id
        string clientId = this->getClientId();
        if( clientId.length() > 0 ){
            connectionInfo->setClientId( clientId );
        } else {
            connectionInfo->setClientId( UUID::randomUUID().toString() );
        }

        // Generate a connectionId
        decaf::lang::Pointer<ConnectionId> connectionId( new ConnectionId() );
        connectionId->setValue( UUID::randomUUID().toString() );
        connectionInfo->setConnectionId( connectionId );

        // Now we ping the broker and see if we get an ack / nack
        syncRequest( connectionInfo );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disconnect() throw ( activemq::exceptions::ActiveMQException ) {

    try{

        // Remove our ConnectionId from the Broker
        disposeOf( connectionInfo->getConnectionId(), this->getCloseTimeout() );

        // Send the disconnect command to the broker.
        Pointer<ShutdownInfo> shutdown( new ShutdownInfo() );
        oneway( shutdown );

        // Allow the Support class to shutdown its resources, including the Transport.
        this->shutdownTransport();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::sendPullRequest(
    const ConsumerInfo* consumer, long long timeout ) throw ( ActiveMQException ) {

    try {

         if( consumer->getPrefetchSize() == 0 ) {

             Pointer<MessagePull> messagePull( new MessagePull() );
             messagePull->setConsumerId( consumer->getConsumerId() );
             messagePull->setDestination( consumer->getDestination() );
             messagePull->setTimeout( timeout );

             this->oneway( messagePull );
         }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination( const ActiveMQDestination* destination )
    throw( decaf::lang::exceptions::NullPointerException,
           decaf::lang::exceptions::IllegalStateException,
           decaf::lang::exceptions::UnsupportedOperationException,
           activemq::exceptions::ActiveMQException ) {

    try{

        if( destination == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Destination passed was NULL" );
        }

        enforceConnected();

        Pointer<DestinationInfo> command( new DestinationInfo() );

        command->setConnectionId( connectionInfo->getConnectionId() );
        command->setOperationType( ActiveMQConstants::DESTINATION_REMOVE_OPERATION );
        command->setDestination( Pointer<ActiveMQDestination>( destination->cloneDataStructure() ) );

        // Send the message to the broker.
        syncRequest( command );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination( const cms::Destination* destination )
    throw( decaf::lang::exceptions::NullPointerException,
           decaf::lang::exceptions::IllegalStateException,
           decaf::lang::exceptions::UnsupportedOperationException,
           activemq::exceptions::ActiveMQException ) {

    try{

        if( destination == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "Destination passed was NULL" );
        }

        enforceConnected();

        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        this->destroyDestination( amqDestination );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onCommand( const Pointer<Command>& command ) {

    try{

        if( command->isMessageDispatch() ) {

            MessageDispatch* dispatch =
                dynamic_cast<MessageDispatch*>( command.get() );

            // Check fo an empty Message, shouldn't ever happen but who knows.
            if( dispatch->getMessage() == NULL ) {
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "ActiveMQConnection::onCommand - "
                    "Received unsupported dispatch message" );
            }

            // Look up the dispatcher.
            Dispatcher* dispatcher = NULL;
            synchronized( &dispatchers ) {

                dispatcher = dispatchers.get( dispatch->getConsumerId() );

                // If we have no registered dispatcher, the consumer was probably
                // just closed.
                if( dispatcher != NULL ) {

                    Pointer<commands::Message> message = dispatch->getMessage();
                    message->setReadOnlyBody( true );
                    message->setReadOnlyProperties( true );

                    // Dispatch the message.
                    DispatchData data( dispatch->getConsumerId(), message );
                    dispatcher->dispatch( data );
                }
            }

        } else if( command->isProducerAck() ) {

            ProducerAck* producerAck =
                dynamic_cast<ProducerAck*>( command.get() );

            // Get the consumer info object for this consumer.
            ActiveMQProducer* producer = NULL;
            synchronized( &this->activeProducers ) {
                producer = this->activeProducers.get( producerAck->getProducerId() );
                if( producer != NULL ){
                    producer->onProducerAck( *producerAck );
                }
            }

        } else if( command->isWireFormatInfo() ) {
            this->brokerWireFormatInfo =
                command.dynamicCast<WireFormatInfo>();
        } else if( command->isBrokerInfo() ) {
            this->brokerInfo =
                command.dynamicCast<BrokerInfo>();
        } else if( command->isKeepAliveInfo() ) {

            if( command->isResponseRequired() ) {
                command->setResponseRequired( false );

                oneway( command );
            }

        } else if( command->isShutdownInfo() ) {

            try {
                if( !this->isClosed() ) {
                    fire( ActiveMQException(
                        __FILE__, __LINE__,
                        "ActiveMQConnection::onCommand - "
                        "Broker closed this connection."));
                }
            } catch( ... ) { /* do nothing */ }

        } else {
            //LOGDECAF_WARN( logger, "Received an unknown command" );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onException( const decaf::lang::Exception& ex ) {

    try {

        // We're disconnected - the asynchronous error is expected.
        if( this->isClosed() ){
            return;
        }

        // Inform the user of the error.
        fire( exceptions::ActiveMQException( ex ) );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::oneway( Pointer<Command> command )
    throw ( ActiveMQException ) {

    try {
        enforceConnected();
        this->getTransport().oneway( command );
    }
    AMQ_CATCH_EXCEPTION_CONVERT( transport::CommandIOException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( UnsupportedOperationException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::syncRequest( Pointer<Command> command, unsigned int timeout )
    throw ( ActiveMQException ) {

    try {

        enforceConnected();

        Pointer<Response> response;

        if( timeout == 0 ) {
            response = this->getTransport().request( command );
        } else {
            response = this->getTransport().request( command, timeout );
        }

        commands::ExceptionResponse* exceptionResponse =
            dynamic_cast<ExceptionResponse*>( response.get() );

        if( exceptionResponse != NULL ) {

            // Create an exception to hold the error information.
            BrokerError* brokerError =
                exceptionResponse->getException()->cloneDataStructure();
            BrokerException exception( __FILE__, __LINE__, brokerError );

            // Throw the exception.
            throw exception;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( transport::CommandIOException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( UnsupportedOperationException, ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disposeOf( const Pointer<DataStructure>& objectId )
    throw ( ActiveMQException ) {

    try{
        Pointer<RemoveInfo> command( new RemoveInfo() );
        command->setObjectId( objectId );
        oneway( command );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disposeOf( const Pointer<DataStructure>& objectId,
                                    unsigned int timeout )
    throw ( ActiveMQException ) {

    try{
        Pointer<RemoveInfo> command( new RemoveInfo() );
        command->setObjectId( objectId );
        this->syncRequest( command, timeout );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::enforceConnected() const throw ( ActiveMQException ) {
    if( this->isClosed() ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConnection::enforceConnected - Not Connected!" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::fire( const exceptions::ActiveMQException& ex ) {
    if( exceptionListener != NULL ) {
        try {
            exceptionListener->onException( ex );
        }
        catch(...){}
    }
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionInfo& ActiveMQConnection::getConnectionInfo() const
    throw( exceptions::ActiveMQException ) {

    enforceConnected();

    return *this->connectionInfo;
}

////////////////////////////////////////////////////////////////////////////////
const ConnectionId& ActiveMQConnection::getConnectionId() const
    throw( exceptions::ActiveMQException ) {

    enforceConnected();

    return *( this->connectionInfo->getConnectionId() );
}
