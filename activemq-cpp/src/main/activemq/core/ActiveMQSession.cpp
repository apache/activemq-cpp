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

#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/exceptions/NullPointerException.h>

#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQTransaction.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQMessage.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQSessionExecutor.h>
#include <activemq/util/Boolean.h>

#include <activemq/connector/TransactionInfo.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::concurrent;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::ActiveMQSession( SessionInfo* sessionInfo,
                                  const Properties& properties,
                                  ActiveMQConnection* connection )
{
    if( sessionInfo == NULL || connection == NULL )
    {
        throw NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQSession::ActiveMQSession - Init with NULL data");
    }

    this->sessionInfo  = sessionInfo;
    this->transaction  = NULL;
    this->connection   = connection;
    this->closed       = false;
    this->asyncThread  = NULL;
    this->useAsyncSend = Boolean::parseBoolean(
        properties.getProperty( "useAsyncSend", "false" ) );

    // If we are in Async Send Mode we need to start the Thread
    // otherwise we don't need to do anything.
    if( this->useAsyncSend == true ) {
        this->startThread();
    }

    // Create a Transaction object only if the session is transactional
    if( isTransacted() )
    {
        transaction =
            new ActiveMQTransaction(connection, this, properties );
    }

    // Create the session executor object.
    executor = new ActiveMQSessionExecutor( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::~ActiveMQSession()
{
    try
    {
        // Destroy this session's resources
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::fire( exceptions::ActiveMQException& ex ) {
    if( connection != NULL ) {
        connection->fire( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::close() throw ( cms::CMSException )
{
    // If we're already close, just get outta' here.
    if( closed ) {
        return;
    }

    try
    {
        // Stop the dispatch executor.
        stop();

        // Get the complete list of closeable session resources.
        std::vector<cms::Closeable*> allResources;
        synchronized( &closableSessionResources ) {
            allResources = closableSessionResources.toArray();
        }

        // Close all of the resources.
        for( unsigned int ix=0; ix<allResources.size(); ++ix ){
            cms::Closeable* resource = allResources[ix];
            try{
                resource->close();
            } catch( cms::CMSException& ex ){
                /* Absorb */
            }
        }

        // Destroy the Transaction
        if( transaction != NULL ){
            delete transaction;
            transaction = NULL;
        }

        // Remove this sessions from the connector
        connection->removeSession( this );
        delete sessionInfo;
        sessionInfo = NULL;

        // Now indicate that this session is closed.
        closed = true;

        // Stop the Async Thread if its running
        stopThread();

        // Remove any unsent cloned messages.
        purgeMessages();

        delete executor;
        executor = NULL;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::commit() throw ( cms::CMSException )
{
    try
    {
        if( closed || !isTransacted() )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::commit - This Session Can't Commit");
        }

        // Commit the Transaction
        transaction->commit();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::rollback() throw ( cms::CMSException )
{
    try
    {
        if( closed || !isTransacted() )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::rollback - This Session Can't Rollback" );
        }

        // Rollback the Transaction
        transaction->rollback();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination )
        throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        return createConsumer( destination, "", false );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination,
    const std::string& selector )
        throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        return createConsumer( destination, selector, false );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createConsumer(
    const cms::Destination* destination,
    const std::string& selector,
    bool noLocal )
        throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        ConsumerInfo* consumerInfo =
            connection->getConnectionData()->getConnector()->
                createConsumer( destination,
                                sessionInfo,
                                selector,
                                noLocal );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( consumerInfo ) );

        // Create the consumer instance.
        ActiveMQConsumer* consumer = new ActiveMQConsumer(
            consumerInfo, this );


        // Add the consumer to the map.
        synchronized( &consumers ) {
            consumers.setValue( consumerInfo->getConsumerId(), consumer );
        }

        // Register this as a message dispatcher for the consumer.
        connection->addDispatcher( consumerInfo, this );

        // Start the Consumer, we are now ready to receive messages
        try{
            connection->getConnectionData()->getConnector()->startConsumer(
                consumerInfo );
        } catch( ActiveMQException& ex ) {
            synchronized( &consumers ) {
                consumers.remove( consumerInfo->getConsumerId() );
            }
            delete consumer;
            ex.setMark( __FILE__, __LINE__ );
            throw ex;
        }

        return consumer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* ActiveMQSession::createDurableConsumer(
    const cms::Topic* destination,
    const std::string& name,
    const std::string& selector,
    bool noLocal )
        throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createDurableConsumer - Session Already Closed" );
        }

        ConsumerInfo* consumerInfo =
            connection->getConnectionData()->getConnector()->
                createDurableConsumer(
                    destination, sessionInfo, name, selector, noLocal );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( consumerInfo ) );

        // Create the consumer instance.
        ActiveMQConsumer* consumer = new ActiveMQConsumer(
            consumerInfo, this );

        // Add the consumer to the map.
        synchronized( &consumers ) {
            consumers.setValue( consumerInfo->getConsumerId(), consumer );
        }

        // Register this as a message dispatcher for the consumer.
        connection->addDispatcher( consumerInfo, this );

        // Start the Consumer, we are now ready to receive messages
        try{
            connection->getConnectionData()->getConnector()->startConsumer(
                consumerInfo );
        } catch( ActiveMQException& ex ) {
            synchronized( &consumers ) {
                consumers.remove( consumerInfo->getConsumerId() );
            }
            delete consumer;
            ex.setMark( __FILE__, __LINE__ );
            throw ex;
        }

        return consumer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSession::createProducer(
    const cms::Destination* destination )
        throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createProducer - Session Already Closed" );
        }

        ProducerInfo* producerInfo =
            connection->getConnectionData()->getConnector()->
                createProducer( destination, sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( producerInfo ) );

        // Create the producer instance.
        ActiveMQProducer* producer = new ActiveMQProducer(
            producerInfo, this );

        return producer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* ActiveMQSession::createQueue( const std::string& queueName )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createQueue - Session Already Closed" );
        }

        cms::Queue* queue = connection->getConnectionData()->
            getConnector()->createQueue( queueName, sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( queue ) );

        return queue;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* ActiveMQSession::createTopic( const std::string& topicName )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTopic - Session Already Closed");
        }

        cms::Topic* topic = connection->getConnectionData()->
            getConnector()->createTopic( topicName, sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( topic ) );

        return topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* ActiveMQSession::createTemporaryQueue()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTemporaryQueue - "
                "Session Already Closed" );
        }

        // Create the consumer instance.
        cms::TemporaryQueue* queue =
            connection->getConnectionData()->
                getConnector()->createTemporaryQueue( sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( queue ) );

        return queue;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* ActiveMQSession::createTemporaryTopic()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTemporaryTopic - "
                "Session Already Closed" );
        }

        // Create the consumer instance.
        cms::TemporaryTopic* topic =
            connection->getConnectionData()->
                getConnector()->createTemporaryTopic( sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( topic ) );

        return topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSession::createMessage()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createMessage - Session Already Closed" );
        }

        cms::Message* message = connection->getConnectionData()->
            getConnector()->createMessage( sessionInfo, transaction );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( message ) );

        return message;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createBytesMessage - Session Already Closed" );
        }

        cms::BytesMessage* message = connection->getConnectionData()->
            getConnector()->createBytesMessage( sessionInfo, transaction );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( message ) );

        return message;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage(
    const unsigned char* bytes,
    std::size_t bytesSize )
        throw ( cms::CMSException )
{
    try
    {
        BytesMessage* msg = createBytesMessage();

        msg->setBodyBytes( bytes, bytesSize );

        return msg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTextMessage - Session Already Closed" );
        }

        cms::TextMessage* message = connection->getConnectionData()->
            getConnector()->createTextMessage( sessionInfo, transaction );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( message ) );

        return message;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage( const std::string& text )
    throw ( cms::CMSException )
{
    try
    {
        TextMessage* msg = createTextMessage();

        msg->setText( text.c_str() );

        return msg;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MapMessage* ActiveMQSession::createMapMessage()
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createMapMessage - Session Already Closed" );
        }

        cms::MapMessage* message = connection->getConnectionData()->
                getConnector()->createMapMessage( sessionInfo, transaction );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( message ) );

        return message;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session::AcknowledgeMode ActiveMQSession::getAcknowledgeMode() const
{
    return sessionInfo != NULL ?
        sessionInfo->getAckMode() : Session::AUTO_ACKNOWLEDGE;
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isTransacted() const
{
    return sessionInfo != NULL ?
        sessionInfo->getAckMode() == Session::SESSION_TRANSACTED : false;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::acknowledge( ActiveMQConsumer* consumer,
                                   ActiveMQMessage* message )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::acknowledgeMessage - Session Already Closed" );
        }

        // Stores the Message and its consumer in the tranasction, if the
        // session is a transactional one.
        if( isTransacted() )
        {
            transaction->addToTransaction( message, consumer );
        }

        // Delegate to connector to ack this message.
        return connection->getConnectionData()->
            getConnector()->acknowledge(
                sessionInfo,
                consumer->getConsumerInfo(),
                dynamic_cast< cms::Message* >( message ) );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::send( cms::Message* message, ActiveMQProducer* producer )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::onProducerClose - Session Already Closed" );
        }

        if( useAsyncSend ) {

            // Put it in the send queue, thread will dispatch it.  We clone it
            // in case the client deletes their copy before we get a chance to
            // send it.
            synchronized( &msgQueue ) {
                msgQueue.push( make_pair( message->clone(), producer ) );
                msgQueue.notifyAll();
            }

        } else {
            // Send via the connection syncrhronously.
            connection->getConnectionData()->
                getConnector()->send( message, producer->getProducerInfo() );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::onConnectorResourceClosed(
    const ConnectorResource* resource ) throw ( cms::CMSException ) {

    try{

        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::onProducerClose - Session Already Closed");
        }

        const ConsumerInfo* consumer =
            dynamic_cast<const ConsumerInfo*>( resource );

        if( consumer != NULL )
        {
            bool wasStarted = isStarted();
            if( wasStarted ) {
                stop();
            }
            
            // Remove the dispatcher for the Connection
            connection->removeDispatcher( consumer );

            // Remove this consumer from the Transaction if we are
            // transactional
            if( transaction != NULL ) {
                transaction->removeFromTransaction(
                    consumer->getConsumerId() );
            }

            // Remove this consumer from the consumers map.
            synchronized( &consumers ) {
                consumers.remove( consumer->getConsumerId() );
            }
            
            if( wasStarted ) {
                start();
            }
        }

        // Remove the entry from the session resource map if it's there
        const cms::Closeable* closeable =
            dynamic_cast<const cms::Closeable*>( resource );

        if( closeable != NULL ){
            synchronized( &closableSessionResources ) {
                closableSessionResources.remove(
                    const_cast<cms::Closeable*>( closeable ) );
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::ExceptionListener* ActiveMQSession::getExceptionListener()
{
    if( connection != NULL )
    {
        return connection->getExceptionListener();
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::run()
{
    try{

        while( !closed )
        {
            std::pair<Message*, ActiveMQProducer*> messagePair;

            synchronized( &msgQueue )
            {
                // Gaurd against spurious wakeup or race to sync lock
                // also if the listner has been unregistered we don't
                // have anyone to notify, so we wait till a new one is
                // registered, and then we will deliver the backlog
                while( msgQueue.empty() )
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

                // get the data
                messagePair = msgQueue.pop();
            }

            // Dispatch the message
            connection->getConnectionData()->
                getConnector()->send(
                    messagePair.first,
                    messagePair.second->getProducerInfo() );

            // Destroy Our copy of the message
            delete messagePair.first;
        }
    }
    catch(...)
    {
        cms::ExceptionListener* listener = this->getExceptionListener();

        if( listener != NULL )
        {
            listener->onException( ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::run - "
                "Connector threw an unknown Exception, recovering..." ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::startThread() throw ( ActiveMQException ) {

    try
    {
        // Start the thread, if it's not already started.
        if( asyncThread == NULL )
        {
            asyncThread = new Thread( this );
            asyncThread->start();
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::stopThread() throw ( ActiveMQException ) {

    try
    {
        // if the thread is running signal it to quit and then
        // wait for run to return so thread can die
        if( asyncThread != NULL )
        {
            synchronized( &msgQueue )
            {
                // Force a wakeup if run is in a wait.
                msgQueue.notifyAll();
            }

            // Wait for it to die and then delete it.
            asyncThread->join();
            delete asyncThread;
            asyncThread = NULL;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::purgeMessages() throw ( ActiveMQException )
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
                delete msgQueue.pop().first;
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::unsubscribe( const std::string& name )
    throw ( CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        // Delegate to the connector.
        connection->getConnectionData()->getConnector()->unsubscribe( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::checkConnectorResource(
    connector::ConnectorResource* resource ) {

    if( resource == NULL ) {
        return;
    }

    // Add the consumer to the map of closeable session resources.
    synchronized( &closableSessionResources ) {
        closableSessionResources.add( resource );
    }

    // Register as a Listener
    resource->addListener( this );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::dispatch( DispatchData& message ) {

    if( executor != NULL ) {
        executor->execute( message );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::redispatch( util::Queue<DispatchData>& unconsumedMessages )
{
    util::Queue<DispatchData> reversedList;

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
void ActiveMQSession::start()
{
    if( executor != NULL ) {
        executor->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::stop()
{
    if( executor != NULL ) {
        executor->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isStarted() const
{
    if( executor == NULL ) {
        return false;
    }

    return executor->isStarted();
}


