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
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <activemq/util/Config.h>
#include <activemq/util/CMSExceptionSupport.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/TransactionId.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <cms/ExceptionListener.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::util;
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

    /**
     * Class used to deal with consumers in an active transaction.  This
     * class calls back into the consumer when the transaction is Committed or
     * Rolled Back to process that event.
     */
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
            try {
                consumer->acknowledge();
            } catch( cms::CMSException& ex ) {
                throw ActiveMQException( __FILE__, __LINE__, ex.getMessage().c_str() );
            }

            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterCommit() throw( exceptions::ActiveMQException ) {
            try {
                consumer->commit();
            } catch( cms::CMSException& ex ) {
                throw ActiveMQException( __FILE__, __LINE__, ex.getMessage().c_str() );
            }

            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterRollback() throw( exceptions::ActiveMQException ) {
            try {
                consumer->rollback();
            } catch( cms::CMSException& ex ) {
                throw ActiveMQException( __FILE__, __LINE__, ex.getMessage().c_str() );
            }

            consumer->setSynchronizationRegistered( false );
        }

    };

    /**
     * Class used to Hook a consumer that has been closed into the Transaction
     * it is currently a part of.  Once the Transaction has been Committed or
     * Rolled back this Synchronization can finish the Close of the consumer.
     */
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

    /**
     * ActiveMQAckHandler used to support Client Acknowledge mode.
     */
    class ClientAckHandler : public ActiveMQAckHandler {
    private:

        ActiveMQSession* session;

    public:

        ClientAckHandler( ActiveMQSession* session ) {
            this->session = session;
        }

        void acknowledgeMessage( const commands::Message* message AMQCPP_UNUSED )
            throw ( cms::CMSException ) {

            try {
                this->session->acknowledge();
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }
    };

    /**
     * ActiveMQAckHandler used to enable the Individual Acknowledge mode.
     */
    class IndividualAckHandler : public ActiveMQAckHandler {
    private:

        Pointer<commands::MessageDispatch> dispatch;
        ActiveMQConsumer* consumer;

    public:

        IndividualAckHandler( ActiveMQConsumer* consumer, const Pointer<MessageDispatch>& dispatch ) {
            this->consumer = consumer;
            this->dispatch = dispatch;
        }

        void acknowledgeMessage( const commands::Message* message AMQCPP_UNUSED )
            throw ( cms::CMSException ) {

            try {

                if( this->dispatch != NULL ) {
                    this->consumer->acknowledge( this->dispatch );
                    this->dispatch.reset( NULL );
                }
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::ActiveMQConsumer( ActiveMQSession* session,
                                    const Pointer<ConsumerId>& id,
                                    const Pointer<ActiveMQDestination>& destination,
                                    const std::string& name,
                                    const std::string& selector,
                                    int prefetch,
                                    int maxPendingMessageCount,
                                    bool noLocal,
                                    bool browser,
                                    bool dispatchAsync,
                                    cms::MessageListener* listener ) {

    if( session == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Init with NULL Session" );
    }

    if( destination == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Init with NULL Destination" );
    }

    if( destination->getPhysicalName() == "" ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer::ActiveMQConsumer - Destination given has no Physical Name." );
    }

    Pointer<ConsumerInfo> consumerInfo( new ConsumerInfo() );

    consumerInfo->setConsumerId( id );
    consumerInfo->setDestination( destination );
    consumerInfo->setSubscriptionName( name );
    consumerInfo->setSelector( selector );
    consumerInfo->setPrefetchSize( prefetch );
    consumerInfo->setMaximumPendingMessageLimit( maxPendingMessageCount );
    consumerInfo->setBrowser( browser );
    consumerInfo->setDispatchAsync( dispatchAsync );
    consumerInfo->setNoLocal( noLocal );

    // Initialize Consumer Data
    this->session = session;
    this->consumerInfo = consumerInfo;
    this->lastDeliveredSequenceId = -1;
    this->synchronizationRegistered = false;
    this->additionalWindowSize = 0;
    this->deliveredCounter = 0;
    this->clearDispatchList = false;
    this->inProgressClearRequiredFlag = false;
    this->listener = NULL;
    this->redeliveryDelay = 0;
    this->redeliveryPolicy.reset( this->session->getConnection()->getRedeliveryPolicy()->clone() );

    if( listener != NULL ) {
        this->setMessageListener( listener );
    }

    applyDestinationOptions(this->consumerInfo);
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
            if( this->session->getTransactionContext() != NULL &&
                this->session->getTransactionContext()->isInTransaction() ) {

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

            if( this->session->isIndividualAcknowledge() ) {
                // For IndividualAck Mode we need to unlink the ack handler to remove a
                // cyclic reference to the MessageDispatch that brought the message to us.
                synchronized( &dispatchedMessages ) {
                    std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->dispatchedMessages.iterator() );
                    while( iter->hasNext() ) {
                        iter->next()->getMessage()->setAckHandler( Pointer<ActiveMQAckHandler>() );
                    }

                    dispatchedMessages.clear();
                }
            }

            // Remove this Consumer from the Connections set of Dispatchers
            this->session->removeConsumer( this->consumerInfo->getConsumerId(), lastDeliveredSequenceId );

            // Remove at the Broker Side, consumer has been removed from the local
            // Session and Connection objects so if the remote call to remove throws
            // it is okay to propagate to the client.
            Pointer<RemoveInfo> info( new RemoveInfo );
            info->setObjectId( this->consumerInfo->getConsumerId() );
            info->setLastDeliveredSequenceId( lastDeliveredSequenceId );
            this->session->oneway( info );

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
            deadline = System::currentTimeMillis() + timeout;
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

            } else if( dispatch->getMessage() == NULL ) {

                return Pointer<MessageDispatch>();

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

    // If the Session is in ClientAcknowledge or IndividualAcknowledge mode, then
    // we set the handler in the message to this object and send it out.
    if( session->isClientAcknowledge() ) {
        Pointer<ActiveMQAckHandler> ackHandler( new ClientAckHandler( this->session ) );
        dispatch->getMessage()->setAckHandler( ackHandler );
    } else if( session->isIndividualAcknowledge() ) {
        Pointer<ActiveMQAckHandler> ackHandler( new IndividualAckHandler( this, dispatch ) );
        dispatch->getMessage()->setAckHandler( ackHandler );
    }

    this->lastDeliveredSequenceId =
        dispatch->getMessage()->getMessageId()->getBrokerSequenceId();

    if( !isAutoAcknowledgeBatch() ) {

        // When not in an Auto
        synchronized( &dispatchedMessages ) {
            dispatchedMessages.enqueueFront( dispatch );
        }

        if( this->session->isTransacted() ) {
            ackLater( dispatch, ActiveMQConstants::ACK_TYPE_DELIVERED );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::afterMessageIsConsumed( const Pointer<MessageDispatch>& message,
                                               bool messageExpired ) {

    try{

        if( unconsumedMessages.isClosed() ) {
            return;
        }

        if( messageExpired == true ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );
        }

        if( session->isTransacted() ) {
            return;
        } else if( isAutoAcknowledgeEach() ) {

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

        } else if( isAutoAcknowledgeBatch() ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_CONSUMED );
        } else if( session->isClientAcknowledge() || session->isIndividualAcknowledge() ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );

            bool messageUnackedByConsumer = false;

            synchronized( &dispatchedMessages ) {
                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->dispatchedMessages.iterator() );
                while( iter->hasNext() ) {
                    if( iter->next() == message ) {
                        messageUnackedByConsumer = true;
                        break;
                    }
                }
            }

            if( messageUnackedByConsumer ) {
                this->ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );
            }

        } else {
            throw IllegalStateException( __FILE__, __LINE__, "Invalid Session State" );
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

            if( isAutoAcknowledgeEach() ) {

                synchronized( &dispatchedMessages ) {

                    ack = makeAckForAllDeliveredMessages( ActiveMQConstants::ACK_TYPE_CONSUMED );

                    if( ack != NULL ) {
                        dispatchedMessages.clear();
                    } else {
                        ack.swap( pendingAck );
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
            this->session->getTransactionContext()->addSynchronization( sync );
        }
    }

    // The delivered message list is only needed for the recover method
    // which is only used with client ack.
    deliveredCounter++;

    Pointer<MessageAck> oldPendingAck = pendingAck;
    pendingAck.reset( new MessageAck() );
    pendingAck->setConsumerId( dispatch->getConsumerId() );
    pendingAck->setAckType( (unsigned char)ackType );
    pendingAck->setDestination( dispatch->getDestination() );
    pendingAck->setLastMessageId( dispatch->getMessage()->getMessageId() );
    pendingAck->setMessageCount( deliveredCounter );

    if( oldPendingAck == NULL ) {
        pendingAck->setFirstMessageId( pendingAck->getLastMessageId() );
    } else if ( oldPendingAck->getAckType() == pendingAck->getAckType() ) {
        pendingAck->setFirstMessageId( oldPendingAck->getFirstMessageId() );
    } else {
        // old pending ack being superseded by ack of another type, if is is not a delivered
        // ack and hence important, send it now so it is not lost.
        if( oldPendingAck->getAckType() != ActiveMQConstants::ACK_TYPE_DELIVERED ) {
            session->oneway( oldPendingAck );
        }
    }

    if( session->isTransacted() ) {
        pendingAck->setTransactionId( this->session->getTransactionContext()->getTransactionId() );
    }

    if( ( 0.5 * this->consumerInfo->getPrefetchSize() ) <= ( deliveredCounter - additionalWindowSize ) ) {
        session->oneway( pendingAck );
        pendingAck.reset( NULL );
        deliveredCounter = 0;
        additionalWindowSize = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageAck> ActiveMQConsumer::makeAckForAllDeliveredMessages( int type ) {

    synchronized( &dispatchedMessages ) {

        if( !dispatchedMessages.empty() ) {

            Pointer<MessageDispatch> dispatched = dispatchedMessages.front();

            Pointer<MessageAck> ack( new MessageAck() );
            ack->setAckType( (unsigned char)type );
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
void ActiveMQConsumer::acknowledge( const Pointer<commands::MessageDispatch>& dispatch )
   throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( this->session->isIndividualAcknowledge() ) {

            Pointer<MessageAck> ack( new MessageAck() );
            ack->setAckType( ActiveMQConstants::ACK_TYPE_CONSUMED );
            ack->setConsumerId( this->consumerInfo->getConsumerId() );
            ack->setDestination( this->consumerInfo->getDestination() );
            ack->setMessageCount( 1 );
            ack->setLastMessageId( dispatch->getMessage()->getMessageId() );
            ack->setFirstMessageId( dispatch->getMessage()->getMessageId() );

            session->oneway( ack );

            synchronized( &dispatchedMessages ) {
                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->dispatchedMessages.iterator() );
                while( iter->hasNext() ) {
                    if( iter->next() == dispatch ) {
                        iter->remove();
                        break;
                    }
                }
            }

        } else {
            throw IllegalStateException(
                __FILE__, __LINE__,
                "Session is not in IndividualAcknowledge mode." );
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
                ack->setTransactionId( session->getTransactionContext()->getTransactionId() );
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
                redeliveryDelay = this->redeliveryPolicy->getRedeliveryDelay( redeliveryDelay );
            }

            Pointer<MessageId> firstMsgId =
                dispatchedMessages.back()->getMessage()->getMessageId();

            std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( dispatchedMessages.iterator() );

            while( iter->hasNext() ) {
                Pointer<Message> message = iter->next()->getMessage();
                message->setRedeliveryCounter( message->getRedeliveryCounter() + 1 );
            }

            if( this->redeliveryPolicy->getMaximumRedeliveries() != RedeliveryPolicy::NO_MAXIMUM_REDELIVERIES &&
                lastMsg->getMessage()->getRedeliveryCounter() > this->redeliveryPolicy->getMaximumRedeliveries() ) {

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

            clearMessagesInProgress();
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
void ActiveMQConsumer::inProgressClearRequired() {

    inProgressClearRequiredFlag = true;
    // Clears dispatched messages async to avoid lock contention with inprogress acks.
    clearDispatchList = true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::clearMessagesInProgress() {
    if( inProgressClearRequiredFlag ) {
        synchronized( &unconsumedMessages ) {
            if( inProgressClearRequiredFlag ) {

                // TODO - Rollback duplicates.

                // allow dispatch on this connection to resume
                this->session->getConnection()->setTransportInterruptionProcessingComplete();
                inProgressClearRequiredFlag = false;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isAutoAcknowledgeEach() const {
    return this->session->isAutoAcknowledge() ||
           ( this->session->isDupsOkAcknowledge() && this->consumerInfo->getDestination()->isQueue() );
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isAutoAcknowledgeBatch() const {
    return this->session->isDupsOkAcknowledge() && !this->consumerInfo->getDestination()->isQueue();
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQConsumer::getMessageAvailableCount() const {
    return this->unconsumedMessages.size();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::applyDestinationOptions( const Pointer<ConsumerInfo>& info ) {

    decaf::lang::Pointer<commands::ActiveMQDestination> amqDestination = info->getDestination();

    // Get any options specified in the destination and apply them to the
    // ConsumerInfo object.
    const ActiveMQProperties& options = amqDestination->getOptions();

    std::string noLocalStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_NOLOCAL );
    if( options.hasProperty( noLocalStr ) ) {
        info->setNoLocal( Boolean::parseBoolean(
            options.getProperty( noLocalStr ) ) );
    }

    std::string selectorStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_SELECTOR );
    if( options.hasProperty( selectorStr ) ) {
        info->setSelector( options.getProperty( selectorStr ) );
    }

    std::string priorityStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_PRIORITY );
    if( options.hasProperty( priorityStr ) ) {
        info->setPriority( (unsigned char)Integer::parseInt( options.getProperty( priorityStr ) ) );
    }

    std::string dispatchAsyncStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_DISPATCHASYNC );
    if( options.hasProperty( dispatchAsyncStr ) ) {
        info->setDispatchAsync(
            Boolean::parseBoolean( options.getProperty( dispatchAsyncStr ) ) );
    }

    std::string exclusiveStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_EXCLUSIVE );
    if( options.hasProperty( exclusiveStr ) ) {
        info->setExclusive(
            Boolean::parseBoolean( options.getProperty( exclusiveStr ) ) );
    }

    std::string maxPendingMsgLimitStr =
        core::ActiveMQConstants::toString(
            core::ActiveMQConstants::CUNSUMER_MAXPENDINGMSGLIMIT );

    if( options.hasProperty( maxPendingMsgLimitStr ) ) {
        info->setMaximumPendingMessageLimit(
            Integer::parseInt(
                options.getProperty( maxPendingMsgLimitStr ) ) );
    }

    std::string prefetchSizeStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_PREFECTCHSIZE );
    if( info->getPrefetchSize() <= 0 || options.hasProperty( prefetchSizeStr )  ) {
        info->setPrefetchSize(
            Integer::parseInt( options.getProperty( prefetchSizeStr, "1000" ) ) );
    }

    std::string retroactiveStr =
        core::ActiveMQConstants::toString( core::ActiveMQConstants::CONSUMER_RETROACTIVE );
    if( options.hasProperty( retroactiveStr ) ) {
        info->setRetroactive(
            Boolean::parseBoolean( options.getProperty( retroactiveStr ) ) );
    }

    std::string networkSubscriptionStr = "consumer.networkSubscription";

    if( options.hasProperty( networkSubscriptionStr ) ) {
        info->setNetworkSubscription(
            Boolean::parseBoolean(
                options.getProperty( networkSubscriptionStr ) ) );
    }
}
