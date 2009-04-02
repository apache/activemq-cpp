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
#include "ActiveMQSession.h"

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQTransactionContext.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQSessionExecutor.h>
#include <activemq/util/ActiveMQProperties.h>

#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/DestinationInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQQueue.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQBytesMessage.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ActiveMQMapMessage.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Queue.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::ActiveMQSession( const Pointer<SessionInfo>& sessionInfo,
                                  cms::Session::AcknowledgeMode ackMode,
                                  const Properties& properties,
                                  ActiveMQConnection* connection ) {

    if( sessionInfo == NULL || connection == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQSession::ActiveMQSession - Init with NULL data");
    }

    this->sessionInfo = sessionInfo;
    this->connection = connection;
    this->closed = false;
    this->ackMode = ackMode;

    // Create a Transaction object only if the session is transacted
    if( this->isTransacted() ) {
        this->transaction.reset( new ActiveMQTransactionContext( this, properties ) );
    }

    // Create the session executor object.
    this->executor.reset( new ActiveMQSessionExecutor( this ) );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::~ActiveMQSession() {
    try{
        // Destroy this session's resources
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::fire( const activemq::exceptions::ActiveMQException& ex ) {
    if( connection != NULL ) {
        connection->fire( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::close() throw ( cms::CMSException )
{
    // If we're already closed, just return.
    if( closed ) {
        return;
    }

    try {

        // Stop the dispatch executor.
        stop();

        // Roll Back the transaction since we were closed without an explicit call
        // to commit it.
        if( this->transaction.get() != NULL && this->transaction->isInTransaction() ){
            this->transaction->rollback();
        }

        // Close all Consumers
        synchronized( &this->consumers ) {

            std::vector<ActiveMQConsumer*> closables = this->consumers.values();

            for( std::size_t i = 0; i < closables.size(); ++i ) {
                try{
                    closables[i]->close();
                } catch( cms::CMSException& ex ){
                    /* Absorb */
                }
            }
        }

        // Close all Producers
        synchronized( &this->producers ) {

            std::vector<ActiveMQProducer*> closables = this->producers.values();

            for( std::size_t i = 0; i < closables.size(); ++i ) {
                try{
                    closables[i]->close();
                } catch( cms::CMSException& ex ){
                    /* Absorb */
                }
            }
        }

        // Remove this session from the Broker.
        this->connection->disposeOf( this->sessionInfo->getSessionId() );

        // Remove this sessions from the connector
        this->connection->removeSession( this );

        // Now indicate that this session is closed.
        closed = true;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::commit() throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        if( !this->isTransacted() ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::commit - This Session is not Transacted");
        }

        // Commit the Transaction
        this->transaction->commit();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::rollback() throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( !this->isTransacted() ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::rollback - This Session is not Transacted" );
        }

        bool started = this->executor->isStarted();

        if( started ) {
            this->executor->stop();
        }

        // Roll back the Transaction
        this->transaction->rollback();

        if( started ) {
            this->executor->start();
        }
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        return this->createConsumer( destination, "", false );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination,
    const std::string& selector )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        return this->createConsumer( destination, selector, false );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination,
    const std::string& selector,
    bool noLocal )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        Pointer<ConsumerInfo> consumerInfo( createConsumerInfo( destination ) );

        consumerInfo->setSelector( selector );
        consumerInfo->setNoLocal( noLocal );

        // Override default options with uri-encoded parameters.
        this->applyDestinationOptions( consumerInfo );

        // Register this as a message dispatcher for the consumer since we
        // could start receiving messages from the broker right away once we
        // send the ConsumerInfo command.
        this->connection->addDispatcher( consumerInfo->getConsumerId(), this );

        // Create the consumer instance.
        std::auto_ptr<ActiveMQConsumer> consumer(
            new ActiveMQConsumer( consumerInfo, this, this->transaction ) );

        // Add the consumer to the map.
        synchronized( &this->consumers ) {
            this->consumers.put( consumer->getConsumerInfo().getConsumerId(), consumer.get() );
        }

        // Send our info to the Broker.
        this->syncRequest( consumerInfo );

        return consumer.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createDurableConsumer(
    const cms::Topic* destination,
    const std::string& name,
    const std::string& selector,
    bool noLocal )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        Pointer<ConsumerInfo> consumerInfo( createConsumerInfo( destination ) );

        consumerInfo->setSelector( selector );
        consumerInfo->setNoLocal( noLocal );
        consumerInfo->setSubscriptionName( name );

        // Override default options with uri-encoded parameters.
        this->applyDestinationOptions( consumerInfo );

        // Register this as a message dispatcher for the consumer since we
        // could start receiving messages from the broker right away once we
        // send the ConsumerInfo command.
        this->connection->addDispatcher( consumerInfo->getConsumerId(), this );

        // Create the consumer instance.
        std::auto_ptr<ActiveMQConsumer> consumer(
            new ActiveMQConsumer( consumerInfo, this, this->transaction ) );

        // Add the consumer to the map.
        synchronized( &this->consumers ) {
            this->consumers.put( consumer->getConsumerInfo().getConsumerId(), consumer.get() );
        }

        // Send our info to the Broker.
        this->syncRequest( consumerInfo );

        return consumer.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSession::createProducer(
    const cms::Destination* destination )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        Pointer<cms::Destination> clonedDestination(
            destination != NULL ? destination->clone() : NULL );

        decaf::lang::Pointer<commands::ProducerId> producerId( new commands::ProducerId() );
        producerId->setConnectionId( this->sessionInfo->getSessionId()->getConnectionId() );
        producerId->setSessionId( this->sessionInfo->getSessionId()->getValue() );
        producerId->setValue( this->getNextProducerId() );

        Pointer<commands::ProducerInfo> producerInfo( new commands::ProducerInfo() );
        producerInfo->setProducerId( producerId );
        producerInfo->setWindowSize( this->connection->getProducerWindowSize() );

        // Producers are allowed to have NULL destinations.  In this case, the
        // destination is specified by the messages as they are sent.
        if( clonedDestination != NULL ) {

            // Cast the destination to an OpenWire destination, so we can
            // get all the goodies.
            Pointer<commands::ActiveMQDestination> amqDestination =
                clonedDestination.dynamicCast<commands::ActiveMQDestination>();

            // Get any options specified in the destination and apply them to the
            // ProducerInfo object.
            producerInfo->setDestination( amqDestination );
            const ActiveMQProperties& options = amqDestination->getOptions();
            producerInfo->setDispatchAsync( Boolean::parseBoolean(
                options.getProperty( "producer.dispatchAsync", "false" )) );
        }

        // Create the producer instance.
        std::auto_ptr<ActiveMQProducer> producer(
            new ActiveMQProducer( producerInfo, clonedDestination, this ) );

        producer->setSendTimeout( this->connection->getSendTimeout() );

        synchronized( &this->producers ) {
            // Place the Producer into the Map.
            this->producers.put( producerId, producer.get() );
        }

        // Add to the Connections list
        this->connection->addProducer( producer.get() );

        this->syncRequest( producerInfo );

        return producer.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* ActiveMQSession::createQueue( const std::string& queueName )
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( queueName == "" ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Destination Name cannot be the Empty String." );
        }

        return new commands::ActiveMQQueue( queueName );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* ActiveMQSession::createTopic( const std::string& topicName )
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        if( topicName == "" ) {
            throw IllegalArgumentException(
                __FILE__, __LINE__, "Destination Name cannot be the Empty String." );
        }

        return new commands::ActiveMQTopic( topicName );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* ActiveMQSession::createTemporaryQueue()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        std::auto_ptr<commands::ActiveMQTempQueue> queue( new
            commands::ActiveMQTempQueue( this->createTemporaryDestinationName() ) );

        // Register it with the Broker
        this->createTemporaryDestination( queue.get() );

        return queue.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* ActiveMQSession::createTemporaryTopic()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        std::auto_ptr<commands::ActiveMQTempTopic> topic( new
            commands::ActiveMQTempTopic( createTemporaryDestinationName() ) );

        // Register it with the Broker
        this->createTemporaryDestination( topic.get() );

        return topic.release();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSession::createMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        return new commands::ActiveMQMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        return new commands::ActiveMQBytesMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage(
    const unsigned char* bytes,
    std::size_t bytesSize )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        cms::BytesMessage* msg = createBytesMessage();
        msg->setBodyBytes( bytes, bytesSize );
        return msg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        return new commands::ActiveMQTextMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage( const std::string& text )
    throw ( cms::CMSException ) {

    try {

        this->checkClosed();
        cms::TextMessage* msg = createTextMessage();
        msg->setText( text.c_str() );
        return msg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* ActiveMQSession::createMapMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        return new commands::ActiveMQMapMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session::AcknowledgeMode ActiveMQSession::getAcknowledgeMode() const {
    return this->ackMode;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isTransacted() const {
    return this->ackMode == Session::SESSION_TRANSACTED;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::send(
    cms::Message* message, ActiveMQProducer* producer, util::Usage* usage )
        throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        commands::Message* amqMessage =
            dynamic_cast< commands::Message* >( message );

        if( amqMessage == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::send - "
                "Message is not a valid Open Wire type.");
        }

        // Clear any old data that might be in the message object
        amqMessage->getMessageId().reset( NULL );
        amqMessage->getProducerId().reset( NULL );
        amqMessage->getTransactionId().reset( NULL );

        // Always assign the message ID, regardless of the disable
        // flag.  Not adding a message ID will cause an NPE at the broker.
        decaf::lang::Pointer<commands::MessageId> id( new commands::MessageId() );
        id->setProducerId( producer->getProducerInfo().getProducerId() );
        id->setProducerSequenceId( this->getNextProducerSequenceId() );

        amqMessage->setMessageId( id );

        if( this->getAcknowledgeMode() == cms::Session::SESSION_TRANSACTED ) {

            // Ensure that a new transaction is started if this is the first message
            // sent since the last commit.
            doStartTransaction();

            if( this->transaction.get() == NULL ) {
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "ActiveMQException::send - "
                    "Transacted Session, has no Transaction Info.");
            }

            amqMessage->setTransactionId( this->transaction->getTransactionId() );
        }

        // NOTE:
        // Now we copy the message before sending, this allows the user to reuse the
        // message object without interfering with the copy that's being sent.  We
        // could make this step optional to increase performance but for now we won't.
        // To not do this implies that the user must never reuse the message object, or
        // know that the configuration of Transports doesn't involve the message hanging
        // around beyond the point that send returns.
        Pointer<commands::Message> msgCopy( amqMessage->cloneDataStructure() );

        msgCopy->setProducerId( producer->getProducerInfo().getProducerId() );

        if( this->connection->getSendTimeout() <= 0 &&
            !msgCopy->isResponseRequired() &&
            !this->connection->isAlwaysSyncSend() &&
            ( !msgCopy->isPersistent() || this->connection->isUseAsyncSend() ||
                msgCopy->getTransactionId() != NULL ) ) {

            if( usage != NULL ) {
                usage->enqueueUsage( msgCopy->getSize() );
            }

            // No Response Required.
            this->connection->oneway( msgCopy );

        } else {

            // Send the message to the broker.
            this->connection->syncRequest( msgCopy, this->connection->getSendTimeout() );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQSession::getExceptionListener()
{
    if( connection != NULL ) {
        return connection->getExceptionListener();
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::unsubscribe( const std::string& name )
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        Pointer<RemoveSubscriptionInfo> rsi( new RemoveSubscriptionInfo() );

        rsi->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        rsi->setSubcriptionName( name );
        rsi->setClientId( this->connection->getConnectionInfo().getClientId() );

        // Send the message to the broker.
        this->connection->syncRequest( rsi );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::dispatch( DispatchData& message ) {

    if( this->executor.get() != NULL ) {
        this->executor->execute( message );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::redispatch( decaf::util::StlQueue<DispatchData>& unconsumedMessages ) {

    decaf::util::StlQueue<DispatchData> reversedList;

    // Copy the list in reverse order then clear the original list.
    synchronized( &unconsumedMessages ) {
        unconsumedMessages.reverse( reversedList );
        unconsumedMessages.clear();
    }

    // Add the list to the front of the executor.
    while( !reversedList.empty() ) {
        DispatchData data = reversedList.pop();
        executor->executeFirst( data );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::start() {

    if( this->executor.get() != NULL ) {
        this->executor->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::stop() {

    if( this->executor.get() != NULL ) {
        this->executor->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isStarted() const {

    if( this->executor.get() == NULL ) {
        return false;
    }

    return this->executor->isStarted();
}

////////////////////////////////////////////////////////////////////////////////
ConsumerInfo* ActiveMQSession::createConsumerInfo(
    const cms::Destination* destination ) throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        std::auto_ptr<ConsumerInfo> consumerInfo( new commands::ConsumerInfo() );
        decaf::lang::Pointer<ConsumerId> consumerId( new commands::ConsumerId() );

        consumerId->setConnectionId(
            this->connection->getConnectionId().getValue() );
        consumerId->setSessionId( this->sessionInfo->getSessionId()->getValue() );
        consumerId->setValue( this->getNextConsumerId() );

        consumerInfo->setConsumerId( consumerId );

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        if( amqDestination == NULL ) {
            throw activemq::exceptions::ActiveMQException( __FILE__, __LINE__,
                "Destination was either NULL or not created by this OpenWireConnector" );
        }

        consumerInfo->setDestination(
            Pointer<ActiveMQDestination>( amqDestination->cloneDataStructure() ) );

        return consumerInfo.release();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::applyDestinationOptions( const Pointer<ConsumerInfo>& info ) {

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
        info->setPriority( Integer::parseInt( options.getProperty( priorityStr ) ) );
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

    std::string browserStr = "consumer.browser";

    if( options.hasProperty( browserStr ) ) {
        info->setBrowser(
            Boolean::parseBoolean(
                options.getProperty( browserStr ) ) );
    }

    std::string networkSubscriptionStr = "consumer.networkSubscription";

    if( options.hasProperty( networkSubscriptionStr ) ) {
        info->setNetworkSubscription(
            Boolean::parseBoolean(
                options.getProperty( networkSubscriptionStr ) ) );
    }

    std::string optimizedAcknowledgeStr = "consumer.optimizedAcknowledge";

    if( options.hasProperty( optimizedAcknowledgeStr ) ) {
        info->setOptimizedAcknowledge(
            Boolean::parseBoolean(
                options.getProperty( optimizedAcknowledgeStr ) ) );
    }

    std::string noRangeAcksStr = "consumer.noRangeAcks";

    if( options.hasProperty( noRangeAcksStr ) ) {
        info->setNoRangeAcks(
            Boolean::parseBoolean(
                options.getProperty( noRangeAcksStr ) ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::createTemporaryDestination(
    commands::ActiveMQTempDestination* tempDestination )
        throw ( activemq::exceptions::ActiveMQException ) {

    try {

        Pointer<DestinationInfo> command( new DestinationInfo() );
        command->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        command->setOperationType( ActiveMQConstants::DESTINATION_ADD_OPERATION );
        command->setDestination(
            Pointer<ActiveMQTempDestination>( tempDestination->cloneDataStructure() ) );

        // Send the message to the broker.
        this->syncRequest( command );

        // Now that its setup, link it to this Connection so it can be closed.
        tempDestination->setConnection( this->connection );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::destroyTemporaryDestination(
    commands::ActiveMQTempDestination* tempDestination )
        throw ( activemq::exceptions::ActiveMQException ) {

    try {

        Pointer<DestinationInfo> command( new DestinationInfo() );

        command->setConnectionId( this->connection->getConnectionInfo().getConnectionId() );
        command->setOperationType( ActiveMQConstants::DESTINATION_REMOVE_OPERATION );
        command->setDestination(
            Pointer<ActiveMQTempDestination>( tempDestination->cloneDataStructure() ) );

        // Send the message to the broker.
        this->connection->syncRequest( command );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQSession::createTemporaryDestinationName()
    throw ( activemq::exceptions::ActiveMQException )
{
    try {
        return this->connection->getConnectionId().getValue() + ":" +
               Long::toString( this->connection->getNextTempDestinationId() );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::oneway( Pointer<Command> command )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{
        this->checkClosed();
        this->connection->oneway( command );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::syncRequest( Pointer<Command> command, unsigned int timeout )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{
        this->checkClosed();
        this->connection->syncRequest( command, timeout );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::checkClosed() const throw( activemq::exceptions::ActiveMQException ) {
    if( closed ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQSession - Session Already Closed" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::disposeOf( Pointer<ConsumerId> id )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        synchronized( &this->consumers ) {

            if( this->consumers.containsKey( id ) ) {

                // If the executor thread is currently running, stop it.
                bool wasStarted = isStarted();
                if( wasStarted ) {
                    stop();
                }

                // Remove this Id both from the Sessions Map of Consumers and from
                // the Connection.
                this->connection->removeDispatcher( id );
                this->connection->disposeOf( id );
                this->consumers.remove( id );

                // Clean up any resources in the executor for this consumer
                if( this->executor.get() != NULL ) {

                    // Purge any pending messages for this consumer.
                    this->executor->purgeConsumerMessages( id );
                }

                if( wasStarted ) {
                    start();
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::disposeOf( Pointer<ProducerId> id )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        synchronized( &this->producers ) {

            if( this->producers.containsKey( id ) ) {

                this->connection->removeProducer( id );
                this->connection->disposeOf( id );
                this->producers.remove( id );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQConsumer* ActiveMQSession::getConsumer( const Pointer<ConsumerId>& id ) {

    synchronized( &this->consumers ) {
        if( this->consumers.containsKey( id ) ) {
            return this->consumers.get( id );
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::doStartTransaction() throw ( ActiveMQException ) {

    if( !this->isTransacted() ) {
        throw ActiveMQException( __FILE__, __LINE__, "Not a Transacted Session" );
    }

    this->transaction->begin();
}
