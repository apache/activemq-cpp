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
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/exceptions/NullPointerException.h>
#include <activemq/util/Boolean.h>

using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection(ActiveMQConnectionData* connectionData)
{
    this->connectionData = connectionData;
    this->started = false;
    this->closed = false;
    this->exceptionListener = NULL;

    alwaysSessionAsync = Boolean::parseBoolean(
        connectionData->getProperties().getProperty( "alwaysSessionAsync", "true" ) );

    // Register for messages and exceptions from the connector.
    Connector* connector = connectionData->getConnector();
    connector->setConsumerMessageListener( this );
    connector->setExceptionListener( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::~ActiveMQConnection()
{
    try
    {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addDispatcher( connector::ConsumerInfo* consumer,
    Dispatcher* dispatcher )
{
    // Add the consumer to the map.
    synchronized( &dispatchers )
    {
        dispatchers.setValue( consumer->getConsumerId(), dispatcher );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher( const connector::ConsumerInfo* consumer ) {

    // Remove the consumer from the map.
    synchronized( &dispatchers )
    {
        dispatchers.remove( consumer->getConsumerId() );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession() throw ( cms::CMSException )
{
    try
    {
        return createSession( Session::AUTO_ACKNOWLEDGE );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession(
    cms::Session::AcknowledgeMode ackMode ) throw ( cms::CMSException )
{
    try
    {
        // Determine whether or not to make dispatch for this session asynchronous
        bool doSessionAsync = alwaysSessionAsync || !activeSessions.isEmpty() ||
            ackMode==Session::SESSION_TRANSACTED || ackMode==Session::CLIENT_ACKNOWLEDGE;

        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            connectionData->getConnector()->createSession( ackMode ),
            connectionData->getProperties(),
            this,
            doSessionAsync );

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
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getClientID() const
{
   return connectionData->getConnector()->getClientId();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::close() throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
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

        // Once current deliveries are done this stops the delivery
        // of any new messages.
        started = false;
        closed = true;

        // Destroy the connection data.  This will close the connector
        // and transports.
        if( connectionData != NULL ){
            delete connectionData;
            connectionData = NULL;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::start() throw ( cms::CMSException )
{
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

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::stop() throw ( cms::CMSException )
{
    // Once current deliveries are done this stops the delivery of any
    // new messages.
    started = false;

    // Stop all the sessions.
    std::vector<ActiveMQSession*> sessions = activeSessions.toArray();
    for( unsigned int ix=0; ix<sessions.size(); ++ix ) {
        sessions[ix]->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onConsumerMessage( connector::ConsumerInfo* consumer,
                                            core::ActiveMQMessage* message )
{
    try
    {
        if( connectionData == NULL)
        {
            NullPointerException ex(
                __FILE__, __LINE__,
                "ActiveMQConnection::onConsumerMessage - "
                "Connection Data Null, could be closed." );

            fire( ex );

            return;
        }        

        // Look up the dispatcher.
        Dispatcher* dispatcher = NULL;
        synchronized( &dispatchers )
        {
            dispatcher = dispatchers.getValue(consumer->getConsumerId());
        }
        
        // If we have no registered dispatcher, this is bad!! (should never happen)
        if( dispatcher == NULL )
        {
            // Indicate to Broker that we received the message, but it
            // was not consumed.
            connectionData->getConnector()->acknowledge(
                consumer->getSessionInfo(),
                consumer,
                dynamic_cast<Message*>( message ),
                Connector::DeliveredAck );

            // Delete the message here
            delete message;

            throw ActiveMQException(__FILE__, __LINE__, "no dispatcher registered for consumer" );
        }

        // Dispatch the message.
        DispatchData data( consumer, message );
        dispatcher->dispatch( data );
    }
    catch( exceptions::ActiveMQException& ex )
    {
        ex.setMark( __FILE__, __LINE__ );
        fire( ex );
    }
    catch( ... )
    {
        exceptions::ActiveMQException ex(
           __FILE__, __LINE__,
           "IOTransport::run - caught unknown exception" );
        fire( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onException( const CMSException& ex ){

    if( exceptionListener != NULL ){
        exceptionListener->onException( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeSession( ActiveMQSession* session )
    throw ( cms::CMSException )
{
    try
    {
        // Remove this session from the set of active sessions.
        synchronized( &activeSessions ) {
            activeSessions.remove( session );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
