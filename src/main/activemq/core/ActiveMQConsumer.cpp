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
#include "ActiveMQConsumer.h"

#include <activemq/exceptions/NullPointerException.h>
#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/exceptions/IllegalArgumentException.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQMessage.h>
#include <cms/ExceptionListener.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::ActiveMQConsumer( connector::ConsumerInfo* consumerInfo,
                                    ActiveMQSession* session )
{
    if( session == NULL || consumerInfo == NULL )
    {
        throw NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Init with NULL Session");
    }
    
    // Init Producer Data
    this->session        = session;
    this->consumerInfo   = consumerInfo;
    this->listener       = NULL;
    this->closed         = false;

    // Listen for our resource to close
    this->consumerInfo->addListener( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer()
{
    try
    {
        close();

        delete consumerInfo;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::close() 
    throw ( cms::CMSException )
{
    try
    {
        if( !closed ) {
            
            // Identifies any errors encountered during shutdown.
            bool haveException = false;
            ActiveMQException error; 
            
            // Close the ConsumerInfo
            if( !consumerInfo->isClosed() ) {
                try{
                        // We don't want a callback now
                        this->consumerInfo->removeListener( this );
                        this->consumerInfo->close();
                } catch( ActiveMQException& ex ){
                    if( !haveException ){ 
                        ex.setMark( __FILE__, __LINE__ );                
                        error = ex;
                        haveException = true;
                    }
                }
            }
            
            closed = true;
            
            // Purge all the pending messages
            try{
                purgeMessages();
            } catch ( ActiveMQException& ex ){
                if( !haveException ){ 
                    ex.setMark( __FILE__, __LINE__ );                
                    error = ex;
                    haveException = true;
                }
            }
            
            // If we encountered an error, propagate it.
            if( haveException ){
                error.setMark( __FILE__, __LINE__ );
                throw error;
            }
                                  
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConsumer::getMessageSelector() const 
    throw ( cms::CMSException )
{
    try
    {
        // Fetch the Selector
        return consumerInfo->getMessageSelector();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive() throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        synchronized( &unconsumedMessages )
        {
            // Check for empty in case of spurious wakeup, or race to
            // queue lock.
            while( !closed && unconsumedMessages.empty() )
            {
                unconsumedMessages.wait();
            }
            
            // This will only happen when this object is being
            // closed in another thread context - kind of
            // scary.
            if( closed ){
                throw ActiveMQException( __FILE__, __LINE__,
                    "Consumer is being closed in another thread" );
            }
            
            // Fetch the Message then copy it so it can be handed off
            // to the user.
            DispatchData data = unconsumedMessages.pop();
            cms::Message* message = dynamic_cast<cms::Message*>(data.getMessage());
            cms::Message* result = message->clone();

            // The Message is cleaned up here if the Session is not
            // transacted, otherwise we let the transaction clean up
            // this message as it will have already been ack'd and 
            // stored for later redelivery.
            destroyMessage( message );

            return result;
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive( int millisecs ) 
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        synchronized( &unconsumedMessages )
        {
            // Check for empty, and wait if its not
            if( !closed && unconsumedMessages.empty() ){
                
                unconsumedMessages.wait(millisecs);

                // if its still empty...bail
                if( unconsumedMessages.empty() ) {
                    return NULL;
                }
            }
            
            // This will only happen when this object is being
            // closed in another thread context - kind of
            // scary.
            if( closed ){
                throw ActiveMQException( __FILE__, __LINE__,
                    "Consumer is being closed in another thread" );
            }
            
            // Fetch the Message then copy it so it can be handed off
            // to the user.
            DispatchData data = unconsumedMessages.pop();
            cms::Message* message = dynamic_cast<cms::Message*>(data.getMessage());
            cms::Message* result = message->clone();

            // The Message is cleaned up here if the Session is not
            // transacted, otherwise we let the transaction clean up
            // this message as it will have already been ack'd and 
            // stored for later redelivery.
            destroyMessage( message );

            return result;
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receiveNoWait() 
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        synchronized( &unconsumedMessages )
        {
            if( !unconsumedMessages.empty() )
            {
                // Fetch the Message then copy it so it can be handed off
                // to the user.
                DispatchData data = unconsumedMessages.pop();
                cms::Message* message = dynamic_cast<cms::Message*>(data.getMessage());
                cms::Message* result = message->clone();

                // The Message is cleaned up here if the Session is not
                // transacted, otherwise we let the transaction clean up
                // this message as it will have already been ack'd and 
                // stored for later redelivery.
                destroyMessage( message );

                return result;
            }
        }
        
        return NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageListener( cms::MessageListener* listener )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        this->listener = listener;
        
        if( listener != NULL && session != NULL ) {
            
            // Now that we have a valid message listener,
            // redispatch all the messages that it missed.
            
            bool wasStarted = session->isStarted();
            if( wasStarted ) {
                session->stop();
            }
                        
            session->redispatch( unconsumedMessages );
            
            if( wasStarted ) {
                session->start();
            }
        }       
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledgeMessage( const ActiveMQMessage* message )
   throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Delegate the Ack to the Session, we cast away copnstness since
        // in a transactional session we might need to redeliver this
        // message and update its data.
        session->acknowledge( this, const_cast< ActiveMQMessage*>( message ) );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispatch( DispatchData& data )
{
    try
    {
        
        ActiveMQMessage* message = data.getMessage();
        
        // Don't dispatch expired messages, ack it and then destroy it
        if( message->isExpired() ) {
            session->acknowledge( this, message );
            delete message;

            // stop now, don't queue
            return;
        }
        
        // If the Session is in ClientAcknowledge mode, then we set the 
        // handler in the message to this object and send it out.  Otherwise
        // we ack it here for all the other Modes.
        if( session->getAcknowledgeMode() == Session::CLIENT_ACKNOWLEDGE ) {
            
            // Register ourself so that we can handle the Message's
            // acknowledge method.
            message->setAckHandler( this );
            
        } else {
            session->acknowledge( this, message );
        }
        
        // If we have a listener, send the message.
        if( listener != NULL ) {
            cms::Message* message = dynamic_cast<cms::Message*>(data.getMessage());
            listener->onMessage( message );
            destroyMessage( message );
        } else {
            
            // No listener, add it to the unconsumed messages list
            synchronized( &unconsumedMessages ) {
                unconsumedMessages.push( data );
                unconsumedMessages.notifyAll();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::purgeMessages() throw ( ActiveMQException )
{
    try
    {
        synchronized( &unconsumedMessages )
        {
            while( !unconsumedMessages.empty() )
            {
                // destroy these messages if this is not a transacted
                // session, if it is then the tranasction will clean 
                // the messages up.
                destroyMessage( dynamic_cast<cms::Message*>(unconsumedMessages.pop().getMessage()) );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::destroyMessage( Message* message ) throw ( ActiveMQException ){
    
    try
    {
        /**
         * Only destroy the message if the session is NOT transacted.  If
         * it is, the session will take care of it.
         */
        if( message != NULL && !session->isTransacted() )
        {
            delete message;
        }
    }        
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::onConnectorResourceClosed(
    const ConnectorResource* resource ) throw ( cms::CMSException ) {

    try{

        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::onConnectorResourceClosed - "
                "Producer Already Closed");
        }

        if( resource != consumerInfo ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::onConnectorResourceClosed - "
                "Unknown object passed to this callback");
        }

        // If our producer isn't closed already, then lets close
        this->close();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

