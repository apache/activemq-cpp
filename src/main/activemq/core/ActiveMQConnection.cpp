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
#include <activemq/transport/Response.h>
#include <activemq/exceptions/BrokerException.h>

#include <decaf/lang/Boolean.h>
#include <decaf/util/Iterator.h>
#include <decaf/util/UUID.h>

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
using namespace activemq::exceptions;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection( transport::Transport* transport,
                                        decaf::util::Properties* properties )
 :  ActiveMQConnectionSupport( transport, properties ){

    this->started = false;
    this->closed = false;
    this->exceptionListener = NULL;
    this->connectionMetaData.reset( new ActiveMQConnectionMetaData() );

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
void ActiveMQConnection::addDispatcher( commands::ConsumerInfo* consumer,
    Dispatcher* dispatcher ) {

    // Add the consumer to the map.
    synchronized( &dispatchers ) {
        dispatchers.setValue( consumer->getConsumerId()->getValue(), dispatcher );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher( const commands::ConsumerInfo* consumer ) {

    // Remove the consumer from the map.
    synchronized( &dispatchers ) {
        dispatchers.remove( consumer->getConsumerId()->getValue() );
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
        std::auto_ptr<commands::SessionInfo> sessionInfo( new commands::SessionInfo() );
        std::auto_ptr<commands::SessionId> sessionId( new commands::SessionId() );
        sessionId->setConnectionId( connectionInfo.getConnectionId()->getValue() );
        sessionId->setValue( this->getNextSessionId() );
        sessionInfo->setSessionId( sessionId.release() );

        // Send the subscription message to the broker.
        syncRequest( sessionInfo.get() );

        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            sessionInfo.release(), ackMode, this->getProperties(), this );

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
            activeProducers.setValue(
                producer->getProducerInfo()->getProducerId()->getValue(), producer );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeProducer( ActiveMQProducer* producer )
    throw ( cms::CMSException ) {

    try {

        // Remove this producer from the set of active consumer.
        synchronized( &activeProducers ) {
            activeProducers.remove(
                producer->getProducerInfo()->getProducerId()->getValue() );
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
        connectionInfo.setUserName( this->getUsername() );
        connectionInfo.setPassword( this->getPassword() );

        // Get or Create a Client Id
        string clientId = this->getClientId();
        if( clientId.length() > 0 ){
            connectionInfo.setClientId( clientId );
        } else {
            connectionInfo.setClientId( UUID::randomUUID().toString() );
        }

        // Generate a connectionId
        commands::ConnectionId* connectionId = new commands::ConnectionId();
        connectionId->setValue( UUID::randomUUID().toString() );
        connectionInfo.setConnectionId( connectionId );

        // Now we ping the broker and see if we get an ack / nack
        syncRequest( &connectionInfo );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disconnect() throw ( activemq::exceptions::ActiveMQException ) {

    try{

        // Remove our ConnectionId from the Broker
        disposeOf( connectionInfo.getConnectionId(), this->getCloseTimeout() );

        // Send the disconnect command to the broker.
        commands::ShutdownInfo shutdown;
        oneway( &shutdown );

        // Allow the Support class to shutdown its resources, including the Transport.
        this->shutdownTransport();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::sendPullRequest(
    const commands::ConsumerInfo* consumer, long long timeout ) throw ( ActiveMQException ) {

    try {

         if( consumer->getPrefetchSize() == 0 ) {

             commands::MessagePull messagePull;
             messagePull.setConsumerId( consumer->getConsumerId()->cloneDataStructure() );
             messagePull.setDestination( consumer->getDestination()->cloneDataStructure() );
             messagePull.setTimeout( timeout );

             this->oneway( &messagePull );
         }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::destroyDestination( const commands::ActiveMQDestination* destination )
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

        commands::DestinationInfo command;

        command.setConnectionId( connectionInfo.getConnectionId()->cloneDataStructure() );
        command.setOperationType( ActiveMQConstants::DESTINATION_REMOVE_OPERATION );
        command.setDestination( destination->cloneDataStructure() );

        // Send the message to the broker.
        syncRequest( &command );
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

        const commands::ActiveMQDestination* amqDestination =
            dynamic_cast<const commands::ActiveMQDestination*>( destination );

        this->destroyDestination( amqDestination );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onCommand( transport::Command* command ) {

    try{

        if( typeid( *command ) == typeid( commands::MessageDispatch ) ) {

            commands::MessageDispatch* dispatch =
                dynamic_cast<commands::MessageDispatch*>( command );

            // Due to the severe suckiness of C++, in order to cast to
            // a type that is in a different branch of the inheritance hierarchy
            // we have to cast to the type at the "crotch" of the branch and then
            // we can implicitly cast up the other branch.
            core::ActiveMQMessage* message =
                dynamic_cast<core::ActiveMQMessage*>( dispatch->getMessage() );
            if( message == NULL ) {
                delete command;
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "ActiveMQConnection::onCommand - "
                    "Received unsupported dispatch message" );
            }

            // Look up the dispatcher.
            Dispatcher* dispatcher = NULL;
            synchronized( &dispatchers ) {

                dispatcher = dispatchers.getValue( dispatch->getConsumerId()->getValue() );

                // If we have no registered dispatcher, the consumer was probably
                // just closed.  Just delete the message.
                if( dispatcher == NULL ) {
                    delete message;
                } else {

                    // Dispatch the message.
                    DispatchData data( dispatch->getConsumerId(), message );
                    dispatcher->dispatch( data );
                }
            }

            // Clear the Message as we've passed it onto the
            // listener, who is responsible for deleting it at
            // the appropriate time, which depends on things like
            // the session being transacted etc.
            dispatch->setMessage( NULL );
            dispatch->setConsumerId( NULL );

            delete command;

        } else if( typeid( *command ) == typeid( commands::ProducerAck ) ) {

            commands::ProducerAck* producerAck =
                dynamic_cast<commands::ProducerAck*>( command );

            // Get the consumer info object for this consumer.
            ActiveMQProducer* producer = NULL;
            synchronized( &this->activeProducers ) {
                producer = this->activeProducers.getValue( producerAck->getProducerId()->getValue() );
                if( producer != NULL ){
                    producer->onProducerAck( *producerAck );
                }
            }

            delete command;

        } else if( typeid( *command ) == typeid( commands::WireFormatInfo ) ) {
            this->brokerWireFormatInfo.reset(
                dynamic_cast<commands::WireFormatInfo*>( command ) );
        } else if( typeid( *command ) == typeid( commands::BrokerInfo ) ) {
            this->brokerInfo.reset(
                dynamic_cast<commands::BrokerInfo*>( command ) );
        } else if( typeid( *command ) == typeid( commands::KeepAliveInfo ) ) {

            if( command->isResponseRequired() ) {
                command->setResponseRequired( false );

                oneway( command );
            }

            delete command;

        } else if( typeid( *command ) == typeid( commands::ShutdownInfo ) ) {

            try {
                if( !this->isClosed() ) {
                    fire( ActiveMQException(
                        __FILE__, __LINE__,
                        "ActiveMQConnection::onCommand - "
                        "Broker closed this connection."));
                }
            } catch( ... ) { /* do nothing */ }

            delete command;

        } else {
            //LOGDECAF_WARN( logger, "Received an unknown command" );
            delete command;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onTransportException( transport::Transport* source AMQCPP_UNUSED,
                                               const decaf::lang::Exception& ex ) {

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
void ActiveMQConnection::oneway( transport::Command* command )
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
void ActiveMQConnection::syncRequest( transport::Command* command, unsigned int timeout )
    throw ( ActiveMQException ) {

    try {

        enforceConnected();

        std::auto_ptr<transport::Response> response;

        if( timeout == 0 ) {
            response.reset( this->getTransport().request( command ) );
        } else {
            response.reset( this->getTransport().request( command, timeout ) );
        }

        commands::ExceptionResponse* exceptionResponse =
            dynamic_cast<commands::ExceptionResponse*>( response.get() );

        if( exceptionResponse != NULL ) {

            // Create an exception to hold the error information.
            commands::BrokerError* brokerError =
                dynamic_cast<commands::BrokerError*>(
                        exceptionResponse->getException() );
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
void ActiveMQConnection::disposeOf( commands::DataStructure* objectId )
    throw ( ActiveMQException ) {

    try{
        commands::RemoveInfo command;
        command.setObjectId( objectId->cloneDataStructure() );
        oneway( &command );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::disposeOf( commands::DataStructure* objectId,
                                    unsigned int timeout )
    throw ( ActiveMQException ) {

    try{
        commands::RemoveInfo command;
        command.setObjectId( objectId->cloneDataStructure() );
        this->syncRequest( &command, timeout );
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
const commands::ConnectionInfo* ActiveMQConnection::getConnectionInfo() const
    throw( exceptions::ActiveMQException ) {

    enforceConnected();

    return &this->connectionInfo;
}

////////////////////////////////////////////////////////////////////////////////
const commands::ConnectionId* ActiveMQConnection::getConnectionId() const
    throw( exceptions::ActiveMQException ) {

    enforceConnected();

    return this->connectionInfo.getConnectionId();
}
