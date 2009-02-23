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
#include <decaf/lang/Boolean.h>
#include <decaf/util/Iterator.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::ActiveMQConnection(ActiveMQConnectionData* connectionData) {
    this->connectionData = connectionData;
    this->started = false;
    this->closed = false;
    this->exceptionListener = NULL;

    // Register for messages and exceptions from the connector.
    Connector* connector = connectionData->getConnector();
    connector->setConsumerMessageListener( this );
    connector->setExceptionListener( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConnection::~ActiveMQConnection() {
    try {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addDispatcher( connector::ConsumerInfo* consumer,
    Dispatcher* dispatcher ) {

    // Add the consumer to the map.
    synchronized( &dispatchers ) {
        dispatchers.setValue( consumer->getConsumerId(), dispatcher );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeDispatcher( const connector::ConsumerInfo* consumer ) {

    // Remove the consumer from the map.
    synchronized( &dispatchers ) {
        dispatchers.remove( consumer->getConsumerId() );
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

        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            connectionData->getConnector()->createSession( ackMode ),
            connectionData->getProperties(),
            this );

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
std::string ActiveMQConnection::getClientID() const {

    if( closed ) {
        return "";
    }

    return connectionData->getConnector()->getClientId();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::close() throw ( cms::CMSException )
{
    try {

        if( closed ) {
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::start() throw ( cms::CMSException ) {
    try{

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

        // Once current deliveries are done this stops the delivery of any
        // new messages.
        started = false;

        Iterator<ActiveMQSession*>* iter = activeSessions.iterator();
        while( iter->hasNext() ){
            iter->next()->stop();
        }
        delete iter;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::onConsumerMessage( connector::ConsumerInfo* consumer,
                                            core::ActiveMQMessage* message ) {
    try {

        if( connectionData == NULL) {
            ActiveMQException ex(
                __FILE__, __LINE__,
                "ActiveMQConnection::onConsumerMessage - "
                "Connection Data Null, could be closed." );

            fire( ex );

            return;
        }

        // Look up the dispatcher.
        Dispatcher* dispatcher = NULL;
        synchronized( &dispatchers ) {

            dispatcher = dispatchers.getValue(consumer->getConsumerId());

            // If we have no registered dispatcher, the consumer was probably
            // just closed.  Just delete the message.
            if( dispatcher == NULL ) {
                delete message;
            } else {

                message->setReadOnlyBody( true );
                message->setReadOnlyProperties( true );

                // Dispatch the message.
                DispatchData data( consumer, message );
                dispatcher->dispatch( data );
            }
        }
    }
    catch( exceptions::ActiveMQException& ex ) {
        ex.setMark( __FILE__, __LINE__ );
        fire( ex );
    }
    catch( ... ) {
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
void ActiveMQConnection::sendPullRequest( const connector::ConsumerInfo* consumer, long long timeout )
    throw ( ActiveMQException ) {

    try {

        if( !this->connectionData->getConnector()->isMessagePullSupported() ) {
            return;
        }

        this->connectionData->getConnector()->pullMessage( consumer, timeout );
    }
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

        if( this->isClosed() ) {
            throw IllegalStateException(
                __FILE__, __LINE__, "Connection Closed" );
        }

        // Ask the connector to perform a remove.
        this->connectionData->getConnector()->destroyDestination( destination );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
