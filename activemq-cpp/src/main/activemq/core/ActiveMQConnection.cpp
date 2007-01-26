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

#include <activemq/util/Config.h>

using namespace cms;
using namespace activemq;
using namespace activemq::core;
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
cms::Session* ActiveMQConnection::createSession() 
    throw ( cms::CMSException )
{
    try
    {
        return this->createSession( Session::AUTO_ACKNOWLEDGE );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* ActiveMQConnection::createSession(
   cms::Session::AcknowledgeMode ackMode ) 
      throw ( cms::CMSException )
{
    try
    {
        // Create the session instance.
        ActiveMQSession* session = new ActiveMQSession(
            connectionData->getConnector()->createSession( ackMode ), 
            connectionData->getProperties(),
            this );
        
        // Add the session to the set of active sessions.
        synchronized( &activeSessions ) {
            activeSessions.add( session );
        } 
            
        return session;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
   
////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConnection::getClientId() const
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
        std::vector<cms::Session*> allSessions;
        synchronized( &activeSessions ) {
            allSessions = activeSessions.toArray();
        }
        
        // Close all of the resources.
        for( unsigned int ix=0; ix<allSessions.size(); ++ix ){
            cms::Session* session = allSessions[ix];
            try{
                session->close();
            } catch( cms::CMSException& ex AMQCPP_UNUSED ){
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
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::stop() throw ( cms::CMSException )
{
    // Once current deliveries are done this stops the delivery of any
    // new messages.
    started = false;
}
   
////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::addMessageListener( const unsigned int consumerId,
                                             ActiveMQMessageListener* listener )
{
    // Place in Map
    synchronized( &consumers )
    {
        consumers.setValue( consumerId, listener );
    }
}
  
////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnection::removeMessageListener( const unsigned int consumerId )
{
    // Remove from Map
    synchronized( &consumers )
    {
        consumers.remove( consumerId );
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

        // When not started we drop incomming messages
        if( !started )
        {
            // Indicate to Broker that we received the message, but it
            // was not consumed.
            connectionData->getConnector()->acknowledge(
                consumer->getSessionInfo(), 
                (Message*)message, 
                Connector::DeliveredAck );
                
            // Delete the message here
            delete message;
            
            return;
        }
        
        // Started, so lock map and dispatch the message.
        synchronized( &consumers )
        {
            if( consumers.containsKey(consumer->getConsumerId()) )
            {
                ActiveMQMessageListener* listener = 
                    consumers.getValue(consumer->getConsumerId());
                    
                listener->onActiveMQMessage( message );
            }
        }        
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
        
        // Destroy this sessions resources
        getConnectionData()->
            getConnector()->destroyResource( session->getSessionInfo() );
            
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

                          

