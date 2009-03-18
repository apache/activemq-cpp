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

#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQTransaction.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQMessage.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQSessionExecutor.h>
#include <decaf/lang/Boolean.h>
#include <decaf/util/Queue.h>

#include <activemq/connector/TransactionInfo.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQSession::ActiveMQSession( SessionInfo* sessionInfo,
                                  const Properties& properties,
                                  ActiveMQConnection* connection ) {

    if( sessionInfo == NULL || connection == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQSession::ActiveMQSession - Init with NULL data");
    }

    this->sessionInfo = sessionInfo;
    this->transaction = NULL;
    this->connection = connection;
    this->closed = false;

    // Create a Transaction object only if the session is transacted
    if( isTransacted() ) {
        transaction =
            new ActiveMQTransaction(connection, this, properties );
    }

    // Create the session executor object.
    executor = new ActiveMQSessionExecutor( this );
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
void ActiveMQSession::fire( activemq::exceptions::ActiveMQException& ex ) {
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

        // Get the complete list of closeable session resources.
        // Get the complete list of closeable session resources.
        synchronized( &closableSessionResources ) {

            Iterator<cms::Closeable*>* iter = closableSessionResources.iterator();
            while( iter->hasNext() ) {
                cms::Closeable* resource = iter->next();
                try{
                    resource->close();
                } catch( cms::CMSException& ex ){
                    /* Absorb */
                }
            }
            delete iter;
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

        delete executor;
        executor = NULL;
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::commit() throw ( cms::CMSException ) {

    try {

        if( closed || !isTransacted() ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::commit - This Session Can't Commit");
        }

        // Commit the Transaction
        transaction->commit();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::rollback() throw ( cms::CMSException ) {

    try{

        if( closed || !isTransacted() ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::rollback - This Session Can't Rollback" );
        }

        // Rollback the Transaction
        transaction->rollback();
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

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        return createConsumer( destination, "", false );
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
        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        return createConsumer( destination, selector, false );
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

        if( closed ) {
            throw ActiveMQException(
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
            consumerInfo, this, this->transaction );

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

        if( closed ) {
            throw ActiveMQException(
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
            consumerInfo, this, this->transaction );

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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* ActiveMQSession::createProducer(
    const cms::Destination* destination )
        throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* ActiveMQSession::createQueue( const std::string& queueName )
    throw ( cms::CMSException ) {

    try{
        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createQueue - Session Already Closed" );
        }

        if( queueName == "" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createQueue - Destination name cannot be empty.");
        }

        cms::Queue* queue = connection->getConnectionData()->
            getConnector()->createQueue( queueName, sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( queue ) );

        return queue;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Topic* ActiveMQSession::createTopic( const std::string& topicName )
    throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTopic - Session Already Closed");
        }

        if( topicName == "" ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createTopic - Destination name cannot be empty.");
        }

        cms::Topic* topic = connection->getConnectionData()->
            getConnector()->createTopic( topicName, sessionInfo );

        // Add to Session Closeables and Monitor for close, if needed.
        checkConnectorResource(
            dynamic_cast<ConnectorResource*>( topic ) );

        return topic;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryQueue* ActiveMQSession::createTemporaryQueue()
    throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TemporaryTopic* ActiveMQSession::createTemporaryTopic()
    throw ( cms::CMSException ) {
    try{

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* ActiveMQSession::createMessage()
    throw ( cms::CMSException ) {

    try{
        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage()
    throw ( cms::CMSException ) {

    try{
        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::BytesMessage* ActiveMQSession::createBytesMessage(
    const unsigned char* bytes,
    std::size_t bytesSize )
        throw ( cms::CMSException ) {

    try{
        BytesMessage* msg = createBytesMessage();

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

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::TextMessage* ActiveMQSession::createTextMessage( const std::string& text )
    throw ( cms::CMSException ) {

    try {

        TextMessage* msg = createTextMessage();

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

        if( closed ) {
            throw ActiveMQException(
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
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
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
void ActiveMQSession::send( cms::Message* message, ActiveMQProducer* producer )
    throw ( cms::CMSException ) {

    try {

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::onProducerClose - Session Already Closed" );
        }

        // Send via the connection synchronously.
        connection->getConnectionData()->
            getConnector()->send( message, producer->getProducerInfo() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::onConnectorResourceClosed(
    const ConnectorResource* resource ) throw ( cms::CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::onProducerClose - Session Already Closed");
        }

        const ConsumerInfo* consumer =
            dynamic_cast<const ConsumerInfo*>( resource );

        if( consumer != NULL ) {

            // If the executor thread is currently running, stop it.
            bool wasStarted = isStarted();
            if( wasStarted ) {
                stop();
            }

            // Remove the dispatcher for the Connection
            connection->removeDispatcher( consumer );

            // Remove this consumer from the Transaction if we are transacted
            if( transaction != NULL ) {
                transaction->removeFromTransaction( consumer->getConsumerId() );
            }

            ActiveMQConsumer* obj = NULL;
            synchronized( &consumers ) {

                if( consumers.containsKey( consumer->getConsumerId() ) ) {

                    // Get the consumer reference
                    obj = consumers.getValue( consumer->getConsumerId() );

                    // Remove this consumer from the map.
                    consumers.remove( consumer->getConsumerId() );
                }
            }

            // Clean up any resources in the executor for this consumer
            if( obj != NULL && executor != NULL ) {

                // Purge any pending messages for this consumer.
                vector<ActiveMQMessage*> messages =
                    executor->purgeConsumerMessages(obj);

                // Destroy the messages.
                for( unsigned int ix=0; ix<messages.size(); ++ix ) {
                    delete messages[ix];
                }
            }

            // If the executor thread was previously running, start it back
            // up.
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
    throw ( CMSException ) {

    try{

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        // Delegate to the connector.
        connection->getConnectionData()->getConnector()->unsubscribe( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::sendPullRequest( const connector::ConsumerInfo* consumer, long long timeout )
    throw ( activemq::exceptions::ActiveMQException ) {

    try {

        if( closed ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQSession::createConsumer - Session Already Closed" );
        }

        this->connection->sendPullRequest( consumer, timeout );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
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
void ActiveMQSession::redispatch( decaf::util::Queue<DispatchData>& unconsumedMessages ) {

    decaf::util::Queue<DispatchData> reversedList;

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

    if( executor != NULL ) {
        executor->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSession::stop() {

    if( executor != NULL ) {
        executor->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQSession::isStarted() const {

    if( executor == NULL ) {
        return false;
    }

    return executor->isStarted();
}
