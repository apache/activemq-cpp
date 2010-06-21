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
#include <activemq/core/ActiveMQQueueBrowser.h>
#include <activemq/core/ActiveMQSessionExecutor.h>
#include <activemq/core/PrefetchPolicy.h>
#include <activemq/util/ActiveMQProperties.h>
#include <activemq/util/CMSExceptionSupport.h>

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
#include <activemq/commands/ActiveMQStreamMessage.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/ActiveMQTempQueue.h>
#include <activemq/commands/MessagePull.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Math.h>
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
    this->lastDeliveredSequenceId = -1;

    // Create a Transaction object only if the session is transacted
    if( this->isTransacted() ) {
        this->transaction.reset( new ActiveMQTransactionContext( this, properties ) );
    }

    // Create the session executor object.
    this->executor.reset( new ActiveMQSessionExecutor( this ) );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::~ActiveMQSession() throw() {
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

        // Now indicate that this session is closed.
        closed = true;

        // Remove this sessions from the connection
        this->connection->removeSession( this );

        // Remove this session from the Broker.
        Pointer<RemoveInfo> info( new RemoveInfo() );
        info->setObjectId( this->sessionInfo->getSessionId() );
        info->setLastDeliveredSequenceId( this->lastDeliveredSequenceId );
        this->connection->oneway( info );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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

        // Roll back the Transaction
        this->transaction->rollback();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::recover() throw( cms::CMSException ) {

    try{

        throw UnsupportedOperationException(
            __FILE__, __LINE__,
            "Recover Method is not yet supported." );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::clearMessagesInProgress() {

    if( this->executor.get() != NULL ) {
        this->executor->clearMessagesInProgress();
    }

    synchronized( &this->consumers ) {
        std::vector< ActiveMQConsumer* > consumers = this->consumers.values();

        std::vector< ActiveMQConsumer* >::iterator iter = consumers.begin();
        for( ; iter != consumers.end(); ++iter ) {
            (*iter)->inProgressClearRequired();
            // Todo - This should occur asynchronously.
            (*iter)->clearMessagesInProgress();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::acknowledge() {

    synchronized( &this->consumers ) {
        std::vector< ActiveMQConsumer* > consumers = this->consumers.values();

        std::vector< ActiveMQConsumer* >::iterator iter = consumers.begin();
        for( ; iter != consumers.end(); ++iter ) {
            (*iter)->acknowledge();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::deliverAcks() {

    synchronized( &this->consumers ) {
        std::vector< ActiveMQConsumer* > consumers = this->consumers.values();

        std::vector< ActiveMQConsumer* >::iterator iter = consumers.begin();
        for( ; iter != consumers.end(); ++iter ) {
            (*iter)->deliverAcks();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        return this->createConsumer( destination, "", false );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination,
    const std::string& selector,
    bool noLocal )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        if( amqDestination == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Destination was either NULL or not created by this CMS Client" );
        }

        Pointer<ActiveMQDestination> dest( amqDestination->cloneDataStructure() );

        int prefetch = 0;
        if( dest->isTopic() ) {
            prefetch = this->connection->getPrefetchPolicy()->getTopicPrefetch();
        } else {
            prefetch = this->connection->getPrefetchPolicy()->getQueuePrefetch();
        }

        // Create the consumer instance.
        std::auto_ptr<ActiveMQConsumer> consumer(
            new ActiveMQConsumer( this, this->getNextConsumerId(),
                                  dest, "", selector, prefetch, 0, noLocal,
                                  false, this->connection->isDispatchAsync(), NULL ) );

        try{
            this->addConsumer( consumer.get() );
            this->connection->syncRequest( consumer->getConsumerInfo() );
        } catch( Exception& ex ) {
            this->removeConsumer( consumer->getConsumerId() );
            throw ex;
        }

        if( this->connection->isStarted() ) {
            consumer->start();
        }

        return consumer.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( destination );

        if( amqDestination == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Destination was either NULL or not created by this CMS Client" );
        }

        Pointer<ActiveMQDestination> dest( amqDestination->cloneDataStructure() );

        // Create the consumer instance.
        std::auto_ptr<ActiveMQConsumer> consumer(
            new ActiveMQConsumer( this, this->getNextConsumerId(),
                                  dest, name, selector,
                                  this->connection->getPrefetchPolicy()->getDurableTopicPrefetch(),
                                  0, noLocal, false, this->connection->isDispatchAsync(), NULL ) );

        try{
            this->addConsumer( consumer.get() );
            this->connection->syncRequest( consumer->getConsumerInfo() );
        } catch( Exception& ex ) {
            this->removeConsumer( consumer->getConsumerId() );
            throw ex;
        }

        if( this->connection->isStarted() ) {
            consumer->start();
        }

        return consumer.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSession::createProducer(
    const cms::Destination* destination )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();

        Pointer<commands::ActiveMQDestination> dest;

        // Producers are allowed to have NULL destinations.  In this case, the
        // destination is specified by the messages as they are sent.
        if( destination != NULL ) {

            const ActiveMQDestination* amqDestination =
                dynamic_cast<const ActiveMQDestination*>( destination );

            if( amqDestination == NULL ) {
                throw ActiveMQException(
                    __FILE__, __LINE__,
                    "Destination was either NULL or not created by this CMS Client" );
            }

            // Cast the destination to an OpenWire destination, so we can
            // get all the goodies.
            dest.reset( amqDestination->cloneDataStructure() );
        }

        // Create the producer instance.
        std::auto_ptr<ActiveMQProducer> producer( new ActiveMQProducer(
            this, this->getNextProducerId(), dest, this->connection->getSendTimeout() ) );

        try{
            this->addProducer( producer.get() );
            this->connection->oneway( producer->getProducerInfo() );
        } catch( Exception& ex ) {
            this->removeProducer( producer->getProducerId() );
            throw ex;
        }

        return producer.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSession::createBrowser( const cms::Queue* queue )
    throw( cms::CMSException ) {

    try{
        return ActiveMQSession::createBrowser( queue, "" );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* ActiveMQSession::createBrowser( const cms::Queue* queue,
                                                   const std::string& selector )
    throw( cms::CMSException ) {

    try{

        this->checkClosed();

        // Cast the destination to an OpenWire destination, so we can
        // get all the goodies.
        const ActiveMQDestination* amqDestination =
            dynamic_cast<const ActiveMQDestination*>( queue );

        if( amqDestination == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "Destination was either NULL or not created by this CMS Client" );
        }

        Pointer<ActiveMQDestination> dest( amqDestination->cloneDataStructure() );

        // Create the QueueBrowser instance
        std::auto_ptr<ActiveMQQueueBrowser> browser(
            new ActiveMQQueueBrowser( this, this->getNextConsumerId(), dest,
                                      selector, this->connection->isDispatchAsync() ) );

        return browser.release();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSession::createMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        commands::ActiveMQMessage* message = new commands::ActiveMQMessage();
        message->setConnection( this->connection );
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        commands::ActiveMQBytesMessage* message = new commands::ActiveMQBytesMessage();
        message->setConnection( this->connection );
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage( const unsigned char* bytes, int bytesSize )
        throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        cms::BytesMessage* msg = createBytesMessage();
        msg->setBodyBytes( bytes, bytesSize );
        return msg;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::StreamMessage* ActiveMQSession::createStreamMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        commands::ActiveMQStreamMessage* message = new commands::ActiveMQStreamMessage();
        message->setConnection( this->connection );
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        commands::ActiveMQTextMessage* message = new commands::ActiveMQTextMessage();
        message->setConnection( this->connection );
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* ActiveMQSession::createMapMessage()
    throw ( cms::CMSException ) {

    try{

        this->checkClosed();
        commands::ActiveMQMapMessage* message = new commands::ActiveMQMapMessage();
        message->setConnection( this->connection );
        return message;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Session::AcknowledgeMode ActiveMQSession::getAcknowledgeMode() const throw ( cms::CMSException ) {
    return this->ackMode;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isTransacted() const throw ( cms::CMSException ) {
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
        id->setProducerId( producer->getProducerInfo()->getProducerId() );
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

        msgCopy->onSend();
        msgCopy->setProducerId( producer->getProducerInfo()->getProducerId() );

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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::dispatch( const Pointer<MessageDispatch>& dispatch ) {

    if( this->executor.get() != NULL ) {
        this->executor->execute( dispatch );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::redispatch( MessageDispatchChannel& unconsumedMessages ) {

    std::vector< Pointer<MessageDispatch> > messages = unconsumedMessages.removeAll();
    std::vector< Pointer<MessageDispatch> >::reverse_iterator iter = messages.rbegin();

    for( ; iter != messages.rend(); ++iter ) {
        executor->executeFirst( *iter );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::start() {

    synchronized( &this->consumers ) {
        std::vector< ActiveMQConsumer* > consumers = this->consumers.values();

        std::vector< ActiveMQConsumer*>::iterator iter = consumers.begin();
        for( ; iter != consumers.end(); ++iter ) {
            (*iter)->start();
        }
    }

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

    return this->executor->isRunning();
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
void ActiveMQSession::addConsumer( ActiveMQConsumer* consumer )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        // Add the consumer to the map.
        synchronized( &this->consumers ) {
            this->consumers.put( consumer->getConsumerInfo()->getConsumerId(), consumer );
        }

        // Register this as a message dispatcher for the consumer.
        this->connection->addDispatcher( consumer->getConsumerInfo()->getConsumerId(), this );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::removeConsumer( const Pointer<ConsumerId>& consumerId, long long lastDeliveredSequenceId )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        synchronized( &this->consumers ) {

            if( this->consumers.containsKey( consumerId ) ) {

                // Remove this Id both from the Sessions Map of Consumers and from
                // the Connection.
                this->connection->removeDispatcher( consumerId );
                this->consumers.remove( consumerId );
                this->lastDeliveredSequenceId =
                    Math::max( this->lastDeliveredSequenceId, lastDeliveredSequenceId );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::addProducer( ActiveMQProducer* producer )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        synchronized( &this->producers ) {
            // Place the Producer into the Map.
            this->producers.put( producer->getProducerInfo()->getProducerId(), producer );
        }

        // Add to the Connections list
        this->connection->addProducer( producer );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::removeProducer( const Pointer<ProducerId>& producerId )
    throw ( activemq::exceptions::ActiveMQException ) {

    try{

        this->checkClosed();

        synchronized( &this->producers ) {

            if( this->producers.containsKey( producerId ) ) {

                this->connection->removeProducer( producerId );
                this->producers.remove( producerId );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::doStartTransaction() throw ( ActiveMQException ) {

    if( !this->isTransacted() ) {
        throw ActiveMQException( __FILE__, __LINE__, "Not a Transacted Session" );
    }

    this->transaction->begin();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::wakeup() {

    if( this->executor.get() != NULL ) {
        this->executor->wakeup();
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::ConsumerId> ActiveMQSession::getNextConsumerId() {
    Pointer<ConsumerId> consumerId( new commands::ConsumerId() );

    consumerId->setConnectionId(
        this->connection->getConnectionId().getValue() );
    consumerId->setSessionId( this->sessionInfo->getSessionId()->getValue() );
    consumerId->setValue( this->consumerIds.getNextSequenceId() );

    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::ProducerId> ActiveMQSession::getNextProducerId() {
    Pointer<ProducerId> producerId( new ProducerId() );

    producerId->setConnectionId(
        this->connection->getConnectionId().getValue() );
    producerId->setSessionId( this->sessionInfo->getSessionId()->getValue() );
    producerId->setValue( this->producerIds.getNextSequenceId() );

    return producerId;
}
