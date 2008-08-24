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

#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Date.h>
#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQTransaction.h>
#include <activemq/core/ActiveMQMessage.h>
#include <cms/ExceptionListener.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::ActiveMQConsumer( connector::ConsumerInfo* consumerInfo,
                                    ActiveMQSession* session,
                                    ActiveMQTransaction* transaction ) {

    if( session == NULL || consumerInfo == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Init with NULL Session");
    }

    // Init Producer Data
    this->session = session;
    this->transaction = transaction;
    this->consumerInfo = consumerInfo;
    this->listener = NULL;
    this->closed = false;

    // Listen for our resource to close
    this->consumerInfo->addListener( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer() {

    try {
        close();
        delete consumerInfo;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::close()
    throw ( cms::CMSException ) {

    try{

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

            // Wakeup any synchronous consumers.
            synchronized( &unconsumedMessages ) {
                unconsumedMessages.notifyAll();
            }

            // If we encountered an error, propagate it.
            if( haveException ){
                error.setMark( __FILE__, __LINE__ );
                throw error;
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQConsumer::getMessageSelector() const
    throw ( cms::CMSException ) {

    try {
        // Fetch the Selector
        return consumerInfo->getMessageSelector();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMessage* ActiveMQConsumer::dequeue( int timeout )
    throw ( cms::CMSException ) {

    try {

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        synchronized( &unconsumedMessages ) {

            // Calculate the deadline
            long long deadline = 0;
            if (timeout > 0) {
                deadline = Date::getCurrentTimeMilliseconds() + timeout;
            }

            // Loop until the time is up or we get a non-expired message
            while( true ) {

                // Wait until either the deadline is met, a message arrives, or
                // we've closed.
                while( !closed && unconsumedMessages.empty() && timeout != 0 ) {

                    if( timeout < 0 ) {
                        unconsumedMessages.wait();
                    } else if( timeout > 0 ) {
                        unconsumedMessages.wait(timeout);
                        timeout = std::max((int)(deadline - Date::getCurrentTimeMilliseconds()), 0);
                    }
                }

                if( unconsumedMessages.empty() ) {
                    return NULL;
                }

                // Fetch the Message then copy it so it can be handed off
                // to the user.
                DispatchData data = unconsumedMessages.pop();

                // Get the message.
                ActiveMQMessage* message = data.getMessage();

                // If it's expired, process the message and then go back to waiting.
                if( message->isExpired() ) {

                    beforeMessageIsConsumed(message);
                    afterMessageIsConsumed(message, true);
                    if (timeout > 0) {
                        timeout = std::max((int)(deadline - Date::getCurrentTimeMilliseconds()), 0);
                    }

                    // Go back to waiting for a non-expired message.
                    continue;
                }

                // Return the message.
                return message;
            }
        }

        return NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive() throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Send a request for a new message if needed
        this->sendPullRequest( 0 );

        // Wait for the next message.
        ActiveMQMessage* msg = dequeue( -1 );
        if( msg == NULL ) {
            return NULL;
        }

        // Message preprocessing
        beforeMessageIsConsumed( msg );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMsg = dynamic_cast<cms::Message*>(msg)->clone();

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( msg, false );

        // Return the cloned message.
        return clonedMsg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive( int millisecs )
    throw ( cms::CMSException ) {

    try {

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Send a request for a new message if needed
        this->sendPullRequest( millisecs );

        // Wait for the next message.
        ActiveMQMessage* msg = dequeue( millisecs );
        if( msg == NULL ) {
            return NULL;
        }

        // Message preprocessing
        beforeMessageIsConsumed( msg );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMsg = dynamic_cast<cms::Message*>(msg)->clone();

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( msg, false );

        // Return the cloned message.
        return clonedMsg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receiveNoWait()
    throw ( cms::CMSException ) {

    try {

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Send a request for a new message if needed
        this->sendPullRequest( -1 );

        // Get the next available message, if there is one.
        ActiveMQMessage* msg = dequeue( 0 );
        if( msg == NULL ) {
            return NULL;
        }

        // Message preprocessing
        beforeMessageIsConsumed( msg );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMsg = dynamic_cast<cms::Message*>(msg)->clone();

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( msg, false );

        // Return the cloned message.
        return clonedMsg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageListener( cms::MessageListener* listener ) {

    try{

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::beforeMessageIsConsumed( ActiveMQMessage* message ) {

    // If the Session is in ClientAcknowledge mode, then we set the
    // handler in the message to this object and send it out.  Otherwise
    // we ack it here for all the other Modes.
    if( session->isClientAcknowledge() ) {

        // Register ourself so that we can handle the Message's
        // acknowledge method.
        message->setAckHandler( this );
    }

    // If the session is transacted then we hand off the message to it to
    // be stored for later redelivery.  We do need to check and see if we
    // are approaching the prefetch limit and send an Delivered ack just so
    // we continue to receive messages, otherwise we'd stall.
    if( session->isTransacted() ) {

        if( transaction == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "In a Transacted Session but no Transaction Context set." );
        }

        // Store the message in the transaction, we clone the message into the
        // transaction so that there is a copy to commit if commit is called in
        // the async onMessage method and also so we know that our copy can
        // be deleted.
        transaction->addToTransaction(
            dynamic_cast<cms::Message*>( message )->clone(), this );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::afterMessageIsConsumed( ActiveMQMessage* message,
                                               bool messageExpired AMQCPP_UNUSED ) {

    try{

        if( session->isAutoAcknowledge() || messageExpired ) {
            this->acknowledge( message, Connector::ACK_TYPE_CONSUMED );
        }

        // The Message is cleaned up here.
        delete message;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledgeMessage( const ActiveMQMessage* message )
   throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Send an ack indicating that the client has consumed the message
        this->acknowledge( message, Connector::ACK_TYPE_CONSUMED );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledge( const ActiveMQMessage* message, int ackType )
    throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::receive - This Consumer is closed" );
        }

        // Delegate the Ack to the Session.
        // Delegate to connector to ack this message.
        session->getConnection()->getConnectionData()->
            getConnector()->acknowledge(
                session->getSessionInfo(),
                this->getConsumerInfo(),
                dynamic_cast<const cms::Message*>( message ),
                (Connector::AckType)ackType );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispatch( DispatchData& data ) {

    try {

        ActiveMQMessage* message = data.getMessage();

        // Don't dispatch expired messages, ack it and then destroy it
        if( message->isExpired() ) {
            this->acknowledge( message, Connector::ACK_TYPE_CONSUMED );
            delete message;

            // stop now, don't queue
            return;
        }

        // If we have a listener, send the message.
        if( listener != NULL ) {
            ActiveMQMessage* message = data.getMessage();

            // Preprocessing.
            beforeMessageIsConsumed( message );

            // Notify the listener
            listener->onMessage( dynamic_cast<cms::Message*>(message) );

            // Postprocessing
            afterMessageIsConsumed( message, false );

        } else {

            // No listener, add it to the unconsumed messages list
            synchronized( &unconsumedMessages ) {
                unconsumedMessages.push( data );
                unconsumedMessages.notifyAll();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::purgeMessages() throw ( ActiveMQException ) {

    try {

        synchronized( &unconsumedMessages ) {

            while( !unconsumedMessages.empty() ) {
                delete unconsumedMessages.pop().getMessage();
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::sendPullRequest( long long timeout )
    throw ( ActiveMQException ) {

    try {

        // There are still local message, consume them first.
        if( !unconsumedMessages.empty() ) {
            return;
        }

        this->session->sendPullRequest( this->consumerInfo, timeout );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::onConnectorResourceClosed(
    const ConnectorResource* resource ) throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::onConnectorResourceClosed - "
                "Producer Already Closed");
        }

        if( resource != consumerInfo ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQConsumer::onConnectorResourceClosed - "
                "Unknown object passed to this callback");
        }

        // If our producer isn't closed already, then lets close
        this->close();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
