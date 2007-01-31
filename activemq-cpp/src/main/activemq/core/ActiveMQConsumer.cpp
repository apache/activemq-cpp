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
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQMessage.h>
#include <cms/ExceptionListener.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
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
    this->listenerThread = NULL;
    this->listener       = NULL;
    this->closed         = false;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer()
{
    try
    {
        close();
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
            
            closed = true;
            
            // Identifies any errors encountered during shutdown.
            bool haveException = false;
            ActiveMQException error; 
            
            // Dispose of the Consumer Info, this should stop us from getting
            // any more messages.  This may result in message traffic
            // going to the connector.  If the socket is broken, this
            // will result in an exception, in which case we catch it
            // and continue to shutdown normally.
            try{
                session->onDestroySessionResource( this );
            } catch( ActiveMQException& ex ){
                if( !haveException ){ 
                    ex.setMark( __FILE__, __LINE__ );                
                    error = ex;
                    haveException = true;
                }
            }
            
            // Stop the asynchronous message processin thread if it's
            // running.
            try{
                stopThread();
            } catch ( ActiveMQException& ex ){
                if( !haveException ){ 
                    ex.setMark( __FILE__, __LINE__ );                
                    error = ex;
                    haveException = true;
                }
            }
            
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

        synchronized( &msgQueue )
        {
            // Check for empty in case of spurious wakeup, or race to
            // queue lock.
            while( !closed && msgQueue.empty() )
            {
                msgQueue.wait();
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
            cms::Message* message = msgQueue.pop();
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

        synchronized( &msgQueue )
        {
            // Check for empty, and wait if its not
            if( !closed && msgQueue.empty() ){
                
                msgQueue.wait(millisecs);

                // if its still empty...bail
                if( msgQueue.empty() ) {
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
            cms::Message* message = msgQueue.pop();
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

        synchronized( &msgQueue )
        {
            if( !msgQueue.empty() )
            {
                // Fetch the Message then copy it so it can be handed off
                // to the user.
                cms::Message* message = msgQueue.pop();
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

        synchronized( &listenerLock )
        {
            this->listener = listener;
        }
        
        // Start the thread if it isn't already running.
        // If it is already running, this method will wake the thread up
        // to notify it that there is a message listener, so that it may
        // get rid of backed up messages.
        startThread();                
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
void ActiveMQConsumer::run()
{
    try
    {
        while( !closed )
        {
            Message* message = NULL;

            synchronized( &msgQueue )
            {
                
                // Gaurd against spurious wakeup or race to sync lock
                // also if the listner has been unregistered we don't
                // have anyone to notify, so we wait till a new one is
                // registered, and then we will deliver the backlog
                while( msgQueue.empty() || listener == NULL )
                {
                    if( closed )
                    {
                        break;
                    }
                    msgQueue.wait();
                }
                
                // don't want to process messages if we are shutting down.
                if( closed )
                {
                    return;
                }
                
                // Dispatch the message
                message = msgQueue.pop();
            }
        
            // Notify the listener
            notifyListener( message );            
            
            // The Message is cleaned up here if the Session is not
            // transacted, otherwise we let the transaction clean up
            // this message as it will have already been ack'd and 
            // stored for later redelivery.
            destroyMessage( message );
        }
    }
    catch( ... )
    {
        cms::ExceptionListener* listener = session->getExceptionListener();
        
        if( listener != NULL )
        {
            listener->onException( ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::run - "
                "MessageListener threw an unknown Exception, recovering..." ) );
        }
    }        
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispatch( ActiveMQMessage* message ) 
    throw ( cms::CMSException )
{
    try
    {
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

        // No listener, so we queue it
        synchronized( &msgQueue ) {
            
            msgQueue.push( dynamic_cast< cms::Message* >( message ) );
            msgQueue.notifyAll();
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
        synchronized( &msgQueue )
        {
            while( !msgQueue.empty() )
            {
                // destroy these messages if this is not a transacted
                // session, if it is then the tranasction will clean 
                // the messages up.
                destroyMessage( msgQueue.pop() );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::onActiveMQMessage( ActiveMQMessage* message )
    throw ( ActiveMQException )
{
    try
    {
        if( message == NULL )
        {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::onActiveMQMessage - Passed a Null Message");
        }

        this->dispatch( message );
    }        
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::notifyListener( Message* message ) throw ( ActiveMQException ){
    
    try
    {
        MessageListener* listener = NULL;
        synchronized( &listenerLock )
        {
            listener = getMessageListener();                
        }
        if(listener != NULL)
        {
            listener->onMessage( message );
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
void ActiveMQConsumer::startThread() throw ( ActiveMQException ) {
    
    try
    {
        // Start the thread, if it's not already started.
        if( listenerThread == NULL )
        {
            listenerThread = new Thread( this );        
            listenerThread->start();                        
        }
        
        // notify the Queue so that any pending messages get delivered
        synchronized( &msgQueue )
        {
            msgQueue.notifyAll();
        }
    }        
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::stopThread() throw ( ActiveMQException ) {
    
    try
    {
        // if the thread is running signal it to quit and then
        // wait for run to return so thread can die
        if( listenerThread != NULL )
        {                        
            synchronized( &msgQueue )
            {
                // Force a wakeup if run is in a wait.
                msgQueue.notifyAll();
            }

            // Wait for it to die and then delete it.
            listenerThread->join();
            delete listenerThread;
            listenerThread = NULL;
        }
    }        
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

