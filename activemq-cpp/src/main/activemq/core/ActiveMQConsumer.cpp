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
#include <activemq/core/FifoMessageDispatchChannel.h>
#include <activemq/core/SimplePriorityMessageDispatchChannel.h>
#include <activemq/core/RedeliveryPolicy.h>
#include <activemq/threads/Scheduler.h>
#include <cms/ExceptionListener.h>
#include <memory>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::threads;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace activemq{
namespace core {

    class ActiveMQConsumerMembers {
    private:

        ActiveMQConsumerMembers( const ActiveMQConsumerMembers& );
        ActiveMQConsumerMembers& operator= ( const ActiveMQConsumerMembers& );

    public:

        cms::MessageListener* listener;
        decaf::util::concurrent::Mutex listenerMutex;
        AtomicBoolean deliveringAcks;
        AtomicBoolean started;
        Pointer<MessageDispatchChannel> unconsumedMessages;
        decaf::util::LinkedList< decaf::lang::Pointer<commands::MessageDispatch> > dispatchedMessages;
        long long lastDeliveredSequenceId;
        Pointer<commands::MessageAck> pendingAck;
        int deliveredCounter;
        int additionalWindowSize;
        volatile bool synchronizationRegistered;
        bool clearDispatchList;
        bool inProgressClearRequiredFlag;
        long long redeliveryDelay;
        Pointer<RedeliveryPolicy> redeliveryPolicy;
        Pointer<Exception> failureError;
        Pointer<Scheduler> scheduler;

        ActiveMQConsumerMembers() : listener(NULL),
                                    listenerMutex(),
                                    deliveringAcks(),
                                    started(),
                                    unconsumedMessages(),
                                    dispatchedMessages(),
                                    lastDeliveredSequenceId(0),
                                    pendingAck(),
                                    deliveredCounter(0),
                                    additionalWindowSize(0),
                                    synchronizationRegistered(false),
                                    clearDispatchList(false),
                                    inProgressClearRequiredFlag(false),
                                    redeliveryDelay(0),
                                    redeliveryPolicy(),
                                    failureError(),
                                    scheduler() {
        }

    };

    /**
     * Class used to deal with consumers in an active transaction.  This
     * class calls back into the consumer when the transaction is Committed or
     * Rolled Back to process that event.
     */
    class TransactionSynhcronization : public Synchronization {
    private:

        ActiveMQConsumer* consumer;

    private:

        TransactionSynhcronization( const TransactionSynhcronization& );
        TransactionSynhcronization& operator= ( const TransactionSynhcronization& );

    public:

        TransactionSynhcronization( ActiveMQConsumer* consumer ) : consumer(consumer) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~TransactionSynhcronization() {}

        virtual void beforeEnd() {
            consumer->acknowledge();
            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterCommit() {
            consumer->commit();
            consumer->setSynchronizationRegistered( false );
        }

        virtual void afterRollback() {
            consumer->rollback();
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

    private:

        CloseSynhcronization( const CloseSynhcronization& );
        CloseSynhcronization& operator= ( const CloseSynhcronization& );

    public:

        CloseSynhcronization( ActiveMQConsumer* consumer ) : consumer(consumer) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }
        }

        virtual ~CloseSynhcronization() {}

        virtual void beforeEnd() {
        }

        virtual void afterCommit() {
            consumer->doClose();
        }

        virtual void afterRollback() {
            consumer->doClose();
        }

    };

    /**
     * ActiveMQAckHandler used to support Client Acknowledge mode.
     */
    class ClientAckHandler : public ActiveMQAckHandler {
    private:

        ActiveMQSession* session;

    private:

        ClientAckHandler( const ClientAckHandler& );
        ClientAckHandler& operator= ( const ClientAckHandler& );

    public:

        ClientAckHandler( ActiveMQSession* session ) : session(session) {
            if( session == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Ack Handler Created with NULL Session.");
            }
        }

        void acknowledgeMessage( const commands::Message* message AMQCPP_UNUSED ) {

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

        ActiveMQConsumer* consumer;
        Pointer<commands::MessageDispatch> dispatch;

    private:

        IndividualAckHandler( const IndividualAckHandler& );
        IndividualAckHandler& operator= ( const IndividualAckHandler& );

    public:

        IndividualAckHandler( ActiveMQConsumer* consumer, const Pointer<MessageDispatch>& dispatch ) :
            consumer(consumer), dispatch(dispatch) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Ack Handler Created with NULL consumer.");
            }
        }

        void acknowledgeMessage( const commands::Message* message AMQCPP_UNUSED ) {

            try {

                if( this->dispatch != NULL ) {
                    this->consumer->acknowledge( this->dispatch );
                    this->dispatch.reset( NULL );
                }
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }
    };

    /**
     * Class used to Start a Consumer's dispatch queue asynchronously from the
     * configured Scheduler.
     */
    class StartConsumerTask : public Runnable {
    private:

        ActiveMQConsumer* consumer;

    private:

        StartConsumerTask( const StartConsumerTask& );
        StartConsumerTask& operator= ( const StartConsumerTask& );

    public:

        StartConsumerTask( ActiveMQConsumer* consumer ) : Runnable(), consumer(NULL) {

            if( consumer == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__, "Synchronization Created with NULL Consumer.");
            }

            this->consumer = consumer;
        }

        virtual ~StartConsumerTask() {}

        virtual void run() {
            try{
                if(!this->consumer->isClosed()) {
                    this->consumer->start();
                }
            } catch(cms::CMSException& ex) {
                // TODO - Need Connection onAsyncException method.
            }
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
                                    cms::MessageListener* listener ) : internal(NULL), session(NULL), consumerInfo() {

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

    this->internal = new ActiveMQConsumerMembers();

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
    this->internal->lastDeliveredSequenceId = -1;
    this->internal->synchronizationRegistered = false;
    this->internal->additionalWindowSize = 0;
    this->internal->deliveredCounter = 0;
    this->internal->clearDispatchList = false;
    this->internal->inProgressClearRequiredFlag = false;
    this->internal->listener = NULL;
    this->internal->redeliveryDelay = 0;
    this->internal->redeliveryPolicy.reset( this->session->getConnection()->getRedeliveryPolicy()->clone() );
    this->internal->scheduler = this->session->getScheduler();

    if( this->session->getConnection()->isMessagePrioritySupported() ) {
        this->internal->unconsumedMessages.reset( new SimplePriorityMessageDispatchChannel() );
    } else {
        this->internal->unconsumedMessages.reset( new FifoMessageDispatchChannel() );
    }

    if( listener != NULL ) {
        this->setMessageListener( listener );
    }

    applyDestinationOptions(this->consumerInfo);
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer::~ActiveMQConsumer() throw() {

    try {

        try{
            this->close();
        } catch(...) {}

        delete this->internal;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::start() {

    if( this->internal->unconsumedMessages->isClosed() ) {
        return;
    }

    this->internal->started.set( true );
    this->internal->unconsumedMessages->start();
    this->session->wakeup();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::stop() {
    this->internal->started.set( false );
    this->internal->unconsumedMessages->stop();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isClosed() const {
    return this->internal->unconsumedMessages->isClosed();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::close() {

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
void ActiveMQConsumer::doClose() {

    try {

        dispose();
        // Remove at the Broker Side, consumer has been removed from the local
        // Session and Connection objects so if the remote call to remove throws
        // it is okay to propagate to the client.
        Pointer<RemoveInfo> info( new RemoveInfo );
        info->setObjectId( this->consumerInfo->getConsumerId() );
        info->setLastDeliveredSequenceId( this->internal->lastDeliveredSequenceId );
        this->session->oneway( info );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispose() {

    try{
        if( !this->isClosed() ) {

            if( !session->isTransacted() ) {
                deliverAcks();
            }

            this->internal->started.set( false );

            // Identifies any errors encountered during shutdown.
            bool haveException = false;
            ActiveMQException error;

            // Purge all the pending messages
            try{
                this->internal->unconsumedMessages->clear();
            } catch ( ActiveMQException& ex ){
                if( !haveException ){
                    ex.setMark( __FILE__, __LINE__ );
                    error = ex;
                    haveException = true;
                }
            }

            // Stop and Wakeup all sync consumers.
            this->internal->unconsumedMessages->close();

            if( this->session->isIndividualAcknowledge() ) {
                // For IndividualAck Mode we need to unlink the ack handler to remove a
                // cyclic reference to the MessageDispatch that brought the message to us.
                synchronized( &internal->dispatchedMessages ) {
                    std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->internal->dispatchedMessages.iterator() );
                    while( iter->hasNext() ) {
                        iter->next()->getMessage()->setAckHandler( Pointer<ActiveMQAckHandler>() );
                    }

                    this->internal->dispatchedMessages.clear();
                }
            }

            // Remove this Consumer from the Connections set of Dispatchers
            this->session->removeConsumer( this->consumerInfo->getConsumerId() );

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
std::string ActiveMQConsumer::getMessageSelector() const {

    try {
        // Fetch the Selector
        return this->consumerInfo->getSelector();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageDispatch> ActiveMQConsumer::dequeue( long long timeout ) {

    try {

        this->checkClosed();

        // Calculate the deadline
        long long deadline = 0;
        if( timeout > 0 ) {
            deadline = System::currentTimeMillis() + timeout;
        }

        // Loop until the time is up or we get a non-expired message
        while( true ) {

            Pointer<MessageDispatch> dispatch = this->internal->unconsumedMessages->dequeue( timeout );
            if( dispatch == NULL ) {

                if( timeout > 0 && !this->internal->unconsumedMessages->isClosed() ) {
                    timeout = Math::max( deadline - System::currentTimeMillis(), 0LL );
                } else {
                    if( this->internal->failureError != NULL ) {
                        throw CMSExceptionSupport::create(*this->internal->failureError);
                    } else {
                        return Pointer<MessageDispatch>();
                    }
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
cms::Message* ActiveMQConsumer::receive() {

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
cms::Message* ActiveMQConsumer::receive( int millisecs ) {

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
cms::Message* ActiveMQConsumer::receiveNoWait() {

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
void ActiveMQConsumer::setMessageListener( cms::MessageListener* listener ) {

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

            synchronized( &(this->internal->listenerMutex) ) {
                this->internal->listener = listener;
            }

            this->session->redispatch( *(this->internal->unconsumedMessages) );

            if( wasStarted ) {
                this->session->start();
            }
        } else {
            synchronized( &(this->internal->listenerMutex) ) {
                this->internal->listener = NULL;
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

    this->internal->lastDeliveredSequenceId =
        dispatch->getMessage()->getMessageId()->getBrokerSequenceId();

    if( !isAutoAcknowledgeBatch() ) {

        // When not in an Auto
        synchronized( &this->internal->dispatchedMessages ) {
            this->internal->dispatchedMessages.addFirst( dispatch );
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

        if( this->internal->unconsumedMessages->isClosed() ) {
            return;
        }

        if( messageExpired == true ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );
        }

        if( session->isTransacted() ) {
            return;
        } else if( isAutoAcknowledgeEach() ) {

            if( this->internal->deliveringAcks.compareAndSet( false, true ) ) {

                synchronized( &this->internal->dispatchedMessages ) {
                    if( !this->internal->dispatchedMessages.isEmpty() ) {
                        Pointer<MessageAck> ack = makeAckForAllDeliveredMessages(
                            ActiveMQConstants::ACK_TYPE_CONSUMED );

                        if( ack != NULL ) {
                            this->internal->dispatchedMessages.clear();
                            session->oneway( ack );
                        }
                    }
                }

                this->internal->deliveringAcks.set( false );
            }

        } else if( isAutoAcknowledgeBatch() ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_CONSUMED );
        } else if( session->isClientAcknowledge() || session->isIndividualAcknowledge() ) {
            ackLater( message, ActiveMQConstants::ACK_TYPE_DELIVERED );

            bool messageUnackedByConsumer = false;

            synchronized( &this->internal->dispatchedMessages ) {
                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->internal->dispatchedMessages.iterator() );
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
void ActiveMQConsumer::deliverAcks() {

    try{

        Pointer<MessageAck> ack;

        if( this->internal->deliveringAcks.compareAndSet( false, true ) ) {

            if( isAutoAcknowledgeEach() ) {

                synchronized( &this->internal->dispatchedMessages ) {

                    ack = makeAckForAllDeliveredMessages( ActiveMQConstants::ACK_TYPE_CONSUMED );

                    if( ack != NULL ) {
                        this->internal->dispatchedMessages.clear();
                    } else {
                        ack.swap( internal->pendingAck );
                    }
                }

            } else if( this->internal->pendingAck != NULL &&
                       this->internal->pendingAck->getAckType() == ActiveMQConstants::ACK_TYPE_CONSUMED ) {

                ack.swap( this->internal->pendingAck );
            }

            if( ack != NULL ) {

                try{
                    this->session->oneway( ack );
                } catch(...) {}

            } else {
                this->internal->deliveringAcks.set( false );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::ackLater( const Pointer<MessageDispatch>& dispatch, int ackType ) {

    // Don't acknowledge now, but we may need to let the broker know the
    // consumer got the message to expand the pre-fetch window
    if( session->isTransacted() ) {
        session->doStartTransaction();
        if( !this->internal->synchronizationRegistered ) {
            this->internal->synchronizationRegistered = true;

            Pointer<Synchronization> sync( new TransactionSynhcronization( this ) );
            this->session->getTransactionContext()->addSynchronization( sync );
        }
    }

    // The delivered message list is only needed for the recover method
    // which is only used with client ack.
    this->internal->deliveredCounter++;

    Pointer<MessageAck> oldPendingAck = this->internal->pendingAck;
    this->internal->pendingAck.reset( new MessageAck() );
    this->internal->pendingAck->setConsumerId( dispatch->getConsumerId() );
    this->internal->pendingAck->setAckType( (unsigned char)ackType );
    this->internal->pendingAck->setDestination( dispatch->getDestination() );
    this->internal->pendingAck->setLastMessageId( dispatch->getMessage()->getMessageId() );
    this->internal->pendingAck->setMessageCount( internal->deliveredCounter );

    if( oldPendingAck == NULL ) {
        this->internal->pendingAck->setFirstMessageId( this->internal->pendingAck->getLastMessageId() );
    } else if ( oldPendingAck->getAckType() == this->internal->pendingAck->getAckType() ) {
        this->internal->pendingAck->setFirstMessageId( oldPendingAck->getFirstMessageId() );
    } else {
        // old pending ack being superseded by ack of another type, if is is not a delivered
        // ack and hence important, send it now so it is not lost.
        if( oldPendingAck->getAckType() != ActiveMQConstants::ACK_TYPE_DELIVERED ) {
            session->oneway( oldPendingAck );
        }
    }

    if( session->isTransacted() ) {
        this->internal->pendingAck->setTransactionId( this->session->getTransactionContext()->getTransactionId() );
    }

    if( ( 0.5 * this->consumerInfo->getPrefetchSize() ) <= ( internal->deliveredCounter - internal->additionalWindowSize ) ) {
        session->oneway( this->internal->pendingAck );
        this->internal->pendingAck.reset( NULL );
        this->internal->deliveredCounter = 0;
        this->internal->additionalWindowSize = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageAck> ActiveMQConsumer::makeAckForAllDeliveredMessages( int type ) {

    synchronized( &this->internal->dispatchedMessages ) {

        if( !this->internal->dispatchedMessages.isEmpty() ) {

            Pointer<MessageDispatch> dispatched = this->internal->dispatchedMessages.getFirst();

            Pointer<MessageAck> ack( new MessageAck() );
            ack->setAckType( (unsigned char)type );
            ack->setConsumerId( dispatched->getConsumerId() );
            ack->setDestination( dispatched->getDestination() );
            ack->setMessageCount( (int)this->internal->dispatchedMessages.size() );
            ack->setLastMessageId( dispatched->getMessage()->getMessageId() );
            ack->setFirstMessageId( this->internal->dispatchedMessages.getLast()->getMessage()->getMessageId() );

            return ack;
        }
    }

    return Pointer<MessageAck>();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::acknowledge( const Pointer<commands::MessageDispatch>& dispatch ) {

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

            synchronized( &this->internal->dispatchedMessages ) {
                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->internal->dispatchedMessages.iterator() );
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
void ActiveMQConsumer::acknowledge() {

    try{

        synchronized( &this->internal->dispatchedMessages ) {

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
            this->internal->pendingAck.reset( NULL );

            // Adjust the counters
            this->internal->deliveredCounter =
                Math::max( 0, this->internal->deliveredCounter - (int)this->internal->dispatchedMessages.size());
            this->internal->additionalWindowSize =
                Math::max(0, this->internal->additionalWindowSize - (int)this->internal->dispatchedMessages.size());

            if( !session->isTransacted() ) {
                this->internal->dispatchedMessages.clear();
            }
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::commit() {

    synchronized( &(this->internal->dispatchedMessages) ) {
        this->internal->dispatchedMessages.clear();
    }
    this->internal->redeliveryDelay = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::rollback() {

    synchronized( this->internal->unconsumedMessages.get() ) {

        synchronized( &this->internal->dispatchedMessages ) {
            if( this->internal->dispatchedMessages.isEmpty() ) {
                return;
            }

            // Only increase the redelivery delay after the first redelivery..
            Pointer<MessageDispatch> lastMsg = this->internal->dispatchedMessages.getFirst();
            const int currentRedeliveryCount = lastMsg->getMessage()->getRedeliveryCounter();
            if( currentRedeliveryCount > 0 ) {
                this->internal->redeliveryDelay = this->internal->redeliveryPolicy->getNextRedeliveryDelay( internal->redeliveryDelay );
            } else {
                this->internal->redeliveryDelay = this->internal->redeliveryPolicy->getInitialRedeliveryDelay();
            }

            Pointer<MessageId> firstMsgId =
                this->internal->dispatchedMessages.getLast()->getMessage()->getMessageId();

            std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( internal->dispatchedMessages.iterator() );

            while( iter->hasNext() ) {
                Pointer<Message> message = iter->next()->getMessage();
                message->setRedeliveryCounter( message->getRedeliveryCounter() + 1 );
            }

            if( this->internal->redeliveryPolicy->getMaximumRedeliveries() != RedeliveryPolicy::NO_MAXIMUM_REDELIVERIES &&
                lastMsg->getMessage()->getRedeliveryCounter() > this->internal->redeliveryPolicy->getMaximumRedeliveries() ) {

                // We need to NACK the messages so that they get sent to the DLQ.
                // Acknowledge the last message.
                Pointer<MessageAck> ack( new MessageAck() );
                ack->setAckType( ActiveMQConstants::ACK_TYPE_POISON );
                ack->setConsumerId( this->consumerInfo->getConsumerId() );
                ack->setDestination( lastMsg->getDestination() );
                ack->setMessageCount( (int)this->internal->dispatchedMessages.size() );
                ack->setLastMessageId( lastMsg->getMessage()->getMessageId() );
                ack->setFirstMessageId( firstMsgId );

                session->oneway( ack );
                // Adjust the window size.
                this->internal->additionalWindowSize =
                    Math::max( 0, this->internal->additionalWindowSize - (int)this->internal->dispatchedMessages.size() );
                this->internal->redeliveryDelay = 0;

            } else {

                // only redelivery_ack after first delivery
                if( currentRedeliveryCount > 0 ) {
                    Pointer<MessageAck> ack( new MessageAck() );
                    ack->setAckType( ActiveMQConstants::ACK_TYPE_REDELIVERED );
                    ack->setConsumerId( this->consumerInfo->getConsumerId() );
                    ack->setDestination( lastMsg->getDestination() );
                    ack->setMessageCount( (int)this->internal->dispatchedMessages.size() );
                    ack->setLastMessageId( lastMsg->getMessage()->getMessageId() );
                    ack->setFirstMessageId( firstMsgId );

                    session->oneway( ack );
                }

                // stop the delivery of messages.
                this->internal->unconsumedMessages->stop();

                std::auto_ptr< Iterator< Pointer<MessageDispatch> > > iter( this->internal->dispatchedMessages.iterator() );

                while( iter->hasNext() ) {
                    this->internal->unconsumedMessages->enqueueFirst( iter->next() );
                }

                if( internal->redeliveryDelay > 0 && !this->internal->unconsumedMessages->isClosed() ) {
                    // TODO - Can't do this until we can control object lifetime.
                    // Start up the delivery again a little later.
                    // this->internal->scheduler->executeAfterDelay(
                    //    new StartConsumerTask(this), internal->redeliveryDelay);
                    start();
                } else {
                    start();
                }

            }
            this->internal->deliveredCounter -= (int)internal->dispatchedMessages.size();
            this->internal->dispatchedMessages.clear();
        }
    }

    if( this->internal->listener != NULL ) {
        session->redispatch( *this->internal->unconsumedMessages );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::dispatch( const Pointer<MessageDispatch>& dispatch ) {

    try {

        synchronized( this->internal->unconsumedMessages.get() ) {

            clearMessagesInProgress();
            if( this->internal->clearDispatchList ) {
                // we are reconnecting so lets flush the in progress
                // messages
                this->internal->clearDispatchList = false;
                this->internal->unconsumedMessages->clear();
            }

            if( !this->internal->unconsumedMessages->isClosed() ) {

                // Don't dispatch expired messages, ack it and then destroy it
                if( dispatch->getMessage() != NULL && dispatch->getMessage()->isExpired() ) {
                    this->ackLater( dispatch, ActiveMQConstants::ACK_TYPE_CONSUMED );

                    // stop now, don't queue
                    return;
                }

                synchronized( &this->internal->listenerMutex ) {
                    // If we have a listener, send the message.
                    if( this->internal->listener != NULL && internal->unconsumedMessages->isRunning() ) {

                        // Preprocessing.
                        beforeMessageIsConsumed( dispatch );

                        // Notify the listener
                        this->internal->listener->onMessage(
                            dynamic_cast<cms::Message*>( dispatch->getMessage().get() ) );

                        // Postprocessing
                        afterMessageIsConsumed( dispatch, false );

                    } else {

                        // No listener, add it to the unconsumed messages list
                        this->internal->unconsumedMessages->enqueue( dispatch );
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
void ActiveMQConsumer::sendPullRequest( long long timeout ) {

    try {

        this->checkClosed();

        // There are still local message, consume them first.
        if( !this->internal->unconsumedMessages->isEmpty() ) {
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
void ActiveMQConsumer::checkClosed() const {
    if( this->isClosed() ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQConsumer - Consumer Already Closed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::iterate() {

    synchronized( &this->internal->listenerMutex ) {

        if( this->internal->listener != NULL ) {

            Pointer<MessageDispatch> dispatch = internal->unconsumedMessages->dequeueNoWait();
            if( dispatch != NULL ) {

                try {
                    beforeMessageIsConsumed( dispatch );
                    this->internal->listener->onMessage(
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

    this->internal->inProgressClearRequiredFlag = true;
    // Clears dispatched messages async to avoid lock contention with inprogress acks.
    this->internal->clearDispatchList = true;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::clearMessagesInProgress() {
    if( this->internal->inProgressClearRequiredFlag ) {
        synchronized( this->internal->unconsumedMessages.get() ) {
            if( this->internal->inProgressClearRequiredFlag ) {

                // TODO - Rollback duplicates.

                // allow dispatch on this connection to resume
                this->session->getConnection()->setTransportInterruptionProcessingComplete();
                this->internal->inProgressClearRequiredFlag = false;
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
    return this->internal->unconsumedMessages->size();
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

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setRedeliveryPolicy( RedeliveryPolicy* policy ) {
    if( policy != NULL ) {
        this->internal->redeliveryPolicy.reset( policy );
    }
}

////////////////////////////////////////////////////////////////////////////////
RedeliveryPolicy* ActiveMQConsumer::getRedeliveryPolicy() const {
    return this->internal->redeliveryPolicy.get();
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageListener* ActiveMQConsumer::getMessageListener() const {
    return this->internal->listener;
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerInfo>& ActiveMQConsumer::getConsumerInfo() const {
    this->checkClosed();
    return this->consumerInfo;
}

////////////////////////////////////////////////////////////////////////////////
const Pointer<commands::ConsumerId>& ActiveMQConsumer::getConsumerId() const {
    this->checkClosed();
    return this->consumerInfo->getConsumerId();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQConsumer::isSynchronizationRegistered() const {
    return this->internal->synchronizationRegistered;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setSynchronizationRegistered( bool value ) {
    this->internal->synchronizationRegistered = value;
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQConsumer::getLastDeliveredSequenceId() const {
    return this->internal->lastDeliveredSequenceId;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setLastDeliveredSequenceId( long long value ) {
    this->internal->lastDeliveredSequenceId = value;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConsumer::setFailureError( decaf::lang::Exception* error ) {
    if( error != NULL ) {
        this->internal->failureError.reset( error->clone() );
    }
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception* ActiveMQConsumer::getFailureError() const {
    if( this->internal->failureError == NULL ) {
        return NULL;
    }

    return this->internal->failureError.get();
}
