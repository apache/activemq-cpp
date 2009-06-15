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
#include <decaf/lang/Math.h>
#include <decaf/lang/System.h>
#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <cms/ExceptionListener.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core {

    class TransactionSynhcronization : public Synchronization {
    private:

        ActiveMQConsumer* consumer;

    public:

        TransactionSynhcronization( ActiveMQConsumer* consumer ) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }

            this->consumer = consumer;
        }

        virtual ~TransactionSynhcronization() {}

        virtual void beforeEnd() throw( exceptions::ActiveMQException ) {
            consumer->acknowledge();
            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterCommit() throw( exceptions::ActiveMQException ) {
            consumer->commit();
            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterRollback() throw( exceptions::ActiveMQException ) {
            consumer->rollback();
            consumer->setSynchronizationRegistered( false );
        }

    };

    class CloseSynhcronization : public Synchronization {
    private:

        ActiveMQConsumer* consumer;

    public:

        CloseSynhcronization( ActiveMQConsumer* consumer ) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }

            this->consumer = consumer;
        }

        virtual ~CloseSynhcronization() {}

        virtual void beforeEnd() throw( exceptions::ActiveMQException ) {
        }

        virtual void afterCommit() throw( exceptions::ActiveMQException ) {
            consumer->doClose();
        }

        virtual void afterRollback() throw( exceptions::ActiveMQException ) {
            consumer->doClose();
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::ActiveMQConsumer( const Pointer<ConsumerInfo>& consumerInfo,
                                    ActiveMQSession* session,
                                    const Pointer<ActiveMQTransactionContext>& transaction ) {

    if( session == NULL || consumerInfo == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Init with NULL Session");
    }

    // Initialize Producer Data
    this->session = session;
    this->transaction = transaction;
    this->consumerInfo = consumerInfo;
    this->lastDeliveredSequenceId = 0;
    this->synchronizationRegistered = false;
    this->additionalWindowSize = 0;
    this->redeliveryDelay = 0;
    this->deliveredCounter = 0;
    this->clearDispatchList = false;
    this->listener = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer() {

    try {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::start() {

    if( this->unconsumedMessages.isClosed() ) {
        return;
    }

    this->started.set( true );
    this->unconsumedMessages.start();
    this->session->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::stop() {
    this->started.set( false );
    this->unconsumedMessages.stop();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::close()
    throw ( cms::CMSException ) {

    try{
        if( !this->isClosed() ) {
            if( this->transaction != NULL && this->transaction->isInTransaction() ) {

                // TODO - Currently we can do this since the consumer could be
                // deleted right after the close call so it won't stick around
                // long enough to clean up the transaction data.  For now we
                // just have to close badly.
                //
                //Pointer<Synchronization> sync( new CloseSynhcronization( this ) );
                //this->transaction->addSynchronization( sync );

                doClose();

                throw UnsupportedOperationException(
                    __FILE__, __LINE__,
                    "The Consumer is still in an Active Transaction, commit it first." );

            } else {
                doClose();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::doClose() throw ( ActiveMQException ) {

    try {

        if( !this->isClosed() ) {

            if( !session->isTransacted() ) {
                deliverAcks();
            }

            // Remove this Consumer from the Connections set of Dispatchers and then
            // remove it from the Broker.
            this->session->disposeOf( this->consumerInfo->getConsumerId() );

            this->started.set( false );

            // Identifies any errors encountered during shutdown.
            bool haveException = false;
            ActiveMQException error;

            // Purge all the pending messages
            try{
                unconsumedMessages.clear();
            } catch ( ActiveMQException& ex ){
                if( !haveException ){
                    ex.setMark( __FILE__, __LINE__ );
                    error = ex;
                    haveException = true;
                }
            }

            // Stop and Wakeup all sync consumers.
            unconsumedMessages.close();

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
        return this->consumerInfo->getSelector();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageDispatch> ActiveMQConsumer::dequeue( long long timeout )
    throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        // Calculate the deadline
        long long deadline = 0;
        if( timeout > 0 ) {
            deadline = Date::getCurrentTimeMilliseconds() + timeout;
        }

        // Loop until the time is up or we get a non-expired message
        while( true ) {

            Pointer<MessageDispatch> dispatch = unconsumedMessages.dequeue( timeout );
            if( dispatch == NULL ) {

                if( timeout > 0 && !unconsumedMessages.isClosed() ) {
                    timeout = Math::max( deadline - System::currentTimeMillis(), 0LL );
                } else {
                    return Pointer<MessageDispatch>();
                }

            } else if( dispatch->getMessage()->isExpired() ) {

                beforeMessageIsConsumed( dispatch );
                afterMessageIsConsumed( dispatch, true );
                if( timeout > 0 ) {
                    timeout = Math::max( deadline - System::currentTimeMillis(), 0LL );
                }

                continue;
            }

            // Return the message.
            return dispatch;
        }

        return Pointer<MessageDispatch>();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive() throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        // Send a request for a new message if needed
        this->sendPullRequest( 0 );

        // Wait for the next message.
        Pointer<MessageDispatch> message = dequeue( -1 );
        if( message == NULL ) {
            return NULL;
        }

        // Message pre-processing
        beforeMessageIsConsumed( message );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMessage =
            dynamic_cast<cms::Message*>( message->getMessage()->cloneDataStructure() );

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( message, false );

        // Return the cloned message.
        return clonedMessage;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receive( int millisecs )
    throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        // Send a request for a new message if needed
        this->sendPullRequest( millisecs );

        // Wait for the next message.
        Pointer<MessageDispatch> message = dequeue( millisecs );
        if( message == NULL ) {
            return NULL;
        }

        // Message preprocessing
        beforeMessageIsConsumed( message );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMessage =
            dynamic_cast<cms::Message*>( message->getMessage()->cloneDataStructure() );

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( message, false );

        // Return the cloned message.
        return clonedMessage;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQConsumer::receiveNoWait()
    throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        // Send a request for a new message if needed
        this->sendPullRequest( -1 );

        // Get the next available message, if there is one.
        Pointer<MessageDispatch> message = dequeue( 0 );
        if( message == NULL ) {
            return NULL;
        }

        // Message preprocessing
        beforeMessageIsConsumed( message );

        // Need to clone the message because the user is responsible for freeing
        // its copy of the message.
        cms::Message* clonedMessage =
            dynamic_cast<cms::Message*>( message->getMessage()->cloneDataStructure() );

        // Post processing (may result in the message being deleted)
        afterMessageIsConsumed( message, false );

        // Return the cloned message.
        return clonedMessage;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setMessageListener( cms::MessageListener* listener ) throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( this->consumerInfo->getPrefetchSize() == 0 && listener != NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Cannot deliver async when Prefetch is Zero, set Prefecth to at least One.");
        }

        if( listener != NULL ) {

            // Now that we have a valid message listener,
            // redispatch all the messages that it missed.

            bool wasStarted = session->isStarted();
            if( wasStarted ) {
                session->stop();
            }

            synchronized( &listenerMutex ) {
                this->listener = listener;
            }

            session->redispatch( unconsumedMessages );

            if( wasStarted ) {
                session->start();
            }
        } else {
            synchronized( &listenerMutex ) {
                this->listener = NULL;
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::beforeMessageIsConsumed( const Pointer<MessageDispatch>& dispatch ) {

    // If the Session is in ClientAcknowledge mode, then we set the
    // handler in the message to this object and send it out.  Otherwise
    // we ack it here for all the other Modes.
    if( session->isClientAcknowledge() ) {

        // Register ourself so that we can handle the Message's
        // acknowledge method.
        dispatch->getMessage()->setAckHandler( this );
    }

    this->lastDeliveredSequenceId =
        dispatch->getMessage()->getMessageId()->getBrokerSequenceId();

    synchronized( &dispatchedMessages ) {
        dispatchedMessages.enqueueFront( dispatch );
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

        ackLater( dispatch, ActiveMQConstants::ACK_TYPE_DELIVERED );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::afterMessageIsConsumed( const Pointer<MessageDispatch>& message,
                                               bool messageExpired AMQCPP_UNUSED ) {

    try{

        if( unconsumedMessages.isClosed() ) {
            return;
        }

        if( messageExpired == true ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );
        }

        if( session->isAutoAcknowledge() ) {

            if( this->deliveringAcks.compareAndSet( false, true ) ) {

                synchronized( &dispatchedMessages ) {
                    if( !dispatchedMessages.empty() ) {
                        Pointer<MessageAck> ack = makeAckForAllDeliveredMessages(
                            ActiveMQConstants::ACK_TYPE_CONSUMED );

                        if( ack != NULL ) {
                            dispatchedMessages.clear();
                            session->oneway( ack );
                        }
                    }
                }

                this->deliveringAcks.set( false );
            }

        } else if( session->isClientAcknowledge() ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::deliverAcks()
    throw ( ActiveMQException ) {

    try{

        Pointer<MessageAck> ack;

        if( this->deliveringAcks.compareAndSet( false, true ) ) {

            if( this->session->isAutoAcknowledge() ) {

                synchronized( &dispatchedMessages ) {
                    ack = makeAckForAllDeliveredMessages( ActiveMQConstants::ACK_TYPE_CONSUMED );
                    if( ack != NULL ) {
                        dispatchedMessages.clear();
                    }
                }

            } else if( pendingAck != NULL &&
                       pendingAck->getAckType() == ActiveMQConstants::ACK_TYPE_CONSUMED ) {

                ack.swap( pendingAck );
            }

            if( ack != NULL ) {

                try{
                    this->session->oneway( ack );
                } catch(...) {}

            } else {
                this->deliveringAcks.set( false );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::ackLater( const Pointer<MessageDispatch>& dispatch, int ackType )
    throw ( ActiveMQException ) {

    // Don't acknowledge now, but we may need to let the broker know the
    // consumer got the message to expand the pre-fetch window
    if( session->isTransacted() ) {
        session->doStartTransaction();
        if( !synchronizationRegistered ) {
            synchronizationRegistered = true;

            Pointer<Synchronization> sync( new TransactionSynhcronization( this ) );
            this->transaction->addSynchronization( sync );
        }
    }

    // The delivered message list is only needed for the recover method
    // which is only used with client ack.
    deliveredCounter++;

    Pointer<MessageAck> oldPendingAck = pendingAck;
    pendingAck.reset( new MessageAck() );
    pendingAck->setConsumerId( dispatch->getConsumerId() );
    pendingAck->setAckType( ackType );
    pendingAck->setDestination( dispatch->getDestination() );
    pendingAck->setLastMessageId( dispatch->getMessage()->getMessageId() );
    pendingAck->setMessageCount( deliveredCounter );

    if( oldPendingAck == NULL ) {
        pendingAck->setFirstMessageId( pendingAck->getLastMessageId() );
    } else {
        pendingAck->setFirstMessageId( oldPendingAck->getFirstMessageId() );
    }

    if( session->isTransacted() ) {
        pendingAck->setTransactionId( this->transaction->getTransactionId() );
    }

    if( ( 0.5 * this->consumerInfo->getPrefetchSize() ) <= ( deliveredCounter - additionalWindowSize ) ) {
        session->oneway( pendingAck );
        pendingAck.reset( NULL );
        additionalWindowSize = deliveredCounter;

        // When using DUPS ok, we do a real ack.
        if( ackType == ActiveMQConstants::ACK_TYPE_CONSUMED ) {
            deliveredCounter = 0;
            additionalWindowSize = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageAck> ActiveMQConsumer::makeAckForAllDeliveredMessages( int type ) {

    synchronized( &dispatchedMessages ) {

        if( !dispatchedMessages.empty() ) {

            Pointer<MessageDispatch> dispatched = dispatchedMessages.front();

            Pointer<MessageAck> ack( new MessageAck() );
            ack->setAckType( type );
            ack->setConsumerId( dispatched->getConsumerId() );
            ack->setDestination( dispatched->getDestination() );
            ack->setMessageCount( (int)dispatchedMessages.size() );
            ack->setLastMessageId( dispatched->getMessage()->getMessageId() );
            ack->setFirstMessageId( dispatchedMessages.back()->getMessage()->getMessageId() );

            return ack;
        }
    }

    return Pointer<MessageAck>();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledgeMessage( const commands::Message* message AMQCPP_UNUSED )
   throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( this->session->isClientAcknowledge() ) {
            this->acknowledge();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledge() throw ( cms::CMSException ) {

    try{

        synchronized( &dispatchedMessages ) {

            // Acknowledge all messages so far.
            Pointer<MessageAck> ack =
                makeAckForAllDeliveredMessages( ActiveMQConstants::ACK_TYPE_CONSUMED );

            if( ack == NULL ) {
                return;
            }

            if( session->isTransacted() ) {
                session->doStartTransaction();
                ack->setTransactionId( transaction->getTransactionId() );
            }

            session->oneway( ack );
            pendingAck.reset( NULL );

            // Adjust the counters
            deliveredCounter = Math::max( 0, deliveredCounter - (int)dispatchedMessages.size());
            additionalWindowSize = Math::max(0, additionalWindowSize - (int)dispatchedMessages.size());

            if( !session->isTransacted() ) {
                dispatchedMessages.clear();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::commit() throw( ActiveMQException ) {

    synchronized( &dispatchedMessages ) {
        dispatchedMessages.clear();
    }
    redeliveryDelay = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::rollback() throw( ActiveMQException ) {

    synchronized( &unconsumedMessages ) {

        synchronized( &dispatchedMessages ) {
            if( dispatchedMessages.empty() ) {
                return;
            }

            // Only increase the redelivery delay after the first redelivery..
            Pointer<MessageDispatch> lastMsg = dispatchedMessages.front();
            const int currentRedeliveryCount = lastMsg->getMessage()->getRedeliveryCounter();
            if( currentRedeliveryCount > 0 ) {
                redeliveryDelay = transaction->getRedeliveryDelay();
            }

            Pointer<MessageId> firstMsgId =
                dispatchedMessages.back()->getMessage()->getMessageId();

            std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( dispatchedMessages.iterator() );

            while( iter->hasNext() ) {
                Pointer<Message> message = iter->next()->getMessage();
                message->setRedeliveryCounter( message->getRedeliveryCounter() + 1 );
            }

            if( lastMsg->getRedeliveryCounter() > this->transaction->getMaximumRedeliveries() ) {

                // We need to NACK the messages so that they get sent to the DLQ.
                // Acknowledge the last message.
                Pointer<MessageAck> ack( new MessageAck() );
                ack->setAckType( ActiveMQConstants::ACK_TYPE_POISON );
                ack->setConsumerId( this->consumerInfo->getConsumerId() );
                ack->setDestination( lastMsg->getDestination() );
                ack->setMessageCount( (int)dispatchedMessages.size() );
                ack->setLastMessageId( lastMsg->getMessage()->getMessageId() );
                ack->setFirstMessageId( firstMsgId );

                session->oneway( ack );
                // Adjust the window size.
                additionalWindowSize =
                    Math::max( 0, additionalWindowSize - (int)dispatchedMessages.size() );
                redeliveryDelay = 0;

            } else {

                // only redelivery_ack after first delivery
                if( currentRedeliveryCount > 0 ) {
                    Pointer<MessageAck> ack( new MessageAck() );
                    ack->setAckType( ActiveMQConstants::ACK_TYPE_REDELIVERED );
                    ack->setConsumerId( this->consumerInfo->getConsumerId() );
                    ack->setDestination( lastMsg->getDestination() );
                    ack->setMessageCount( (int)dispatchedMessages.size() );
                    ack->setLastMessageId( lastMsg->getMessage()->getMessageId() );
                    ack->setFirstMessageId( firstMsgId );

                    session->oneway( ack );
                }

                // stop the delivery of messages.
                unconsumedMessages.stop();

                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( dispatchedMessages.iterator() );

                while( iter->hasNext() ) {
                    unconsumedMessages.enqueueFirst( iter->next() );
                }

                if (redeliveryDelay > 0 && !unconsumedMessages.isClosed()) {
                    // TODO
                    // Start up the delivery again a little later.
                    //scheduler.executeAfterDelay(new Runnable() {
                    //    public void run() {
                    //        try {
                    //            if( !started.get() ) {
                    //                start();
                    //            }
                    //        } catch( CMSException& e ) {
                    //            session.connection.onAsyncException(e);
                    //        }
                    //    }
                    //}, redeliveryDelay);
                    start();
                } else {
                    start();
                }

            }
            deliveredCounter -= (int)dispatchedMessages.size();
            dispatchedMessages.clear();
        }
    }

    if( this->listener != NULL ) {
        session->redispatch( unconsumedMessages );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispatch( const Pointer<MessageDispatch>& dispatch ) {

    try {

        synchronized( &unconsumedMessages ) {

            if( this->clearDispatchList ) {
                // we are reconnecting so lets flush the in progress
                // messages
                clearDispatchList = false;
                unconsumedMessages.clear();
            }

            if( !unconsumedMessages.isClosed() ) {

                // Don't dispatch expired messages, ack it and then destroy it
                if( dispatch->getMessage()->isExpired() ) {
                    this->ackLater( dispatch, ActiveMQConstants::ACK_TYPE_CONSUMED );

                    // stop now, don't queue
                    return;
                }

                synchronized( &listenerMutex ) {
                    // If we have a listener, send the message.
                    if( this->listener != NULL && unconsumedMessages.isRunning() ) {

                        // Preprocessing.
                        beforeMessageIsConsumed( dispatch );

                        // Notify the listener
                        this->listener->onMessage(
                            dynamic_cast<cms::Message*>( dispatch->getMessage().get() ) );

                        // Postprocessing
                        afterMessageIsConsumed( dispatch, false );

                    } else {

                        // No listener, add it to the unconsumed messages list
                        this->unconsumedMessages.enqueue( dispatch );
                    }
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::sendPullRequest( long long timeout )
    throw ( activemq::exceptions::ActiveMQException ) {

    try {

        this->checkClosed();

        // There are still local message, consume them first.
        if( !this->unconsumedMessages.isEmpty() ) {
            return;
        }

        if( this->consumerInfo->getPrefetchSize() == 0 ) {

            Pointer<MessagePull> messagePull( new MessagePull() );
            messagePull->setConsumerId( this->consumerInfo->getConsumerId() );
            messagePull->setDestination( this->consumerInfo->getDestination() );
            messagePull->setTimeout( timeout );

            this->session->oneway( messagePull );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::checkClosed() const throw( ActiveMQException ) {
    if( this->isClosed() ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer - Consumer Already Closed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::iterate() {

    synchronized( &listenerMutex ) {

        if( this->listener != NULL ) {

            Pointer<MessageDispatch> dispatch = unconsumedMessages.dequeueNoWait();
            if( dispatch != NULL ) {

                try {
                    beforeMessageIsConsumed( dispatch );
                    this->listener->onMessage(
                        dynamic_cast<cms::Message*>( dispatch->getMessage().get() ) );
                    afterMessageIsConsumed( dispatch, false );
                } catch( ActiveMQException& ex ) {
                    this->session->fire( ex );
                }

                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::clearMessagesInProgress() {
    // we are called from inside the transport reconnection logic
    // which involves us clearing all the connections' consumers
    // dispatch lists and clearing them
    // so rather than trying to grab a mutex (which could be already
    // owned by the message listener calling the send) we will just set
    // a flag so that the list can be cleared as soon as the
    // dispatch thread is ready to flush the dispatch list
    clearDispatchList = true;
}
