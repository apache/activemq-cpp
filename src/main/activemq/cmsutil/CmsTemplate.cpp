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

#include "CmsTemplate.h"
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include "ProducerCallback.h"
#include "MessageCreator.h"
#include <iostream>

using namespace activemq::cmsutil;
using namespace activemq::exceptions;
using namespace decaf::lang::exceptions;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate() {
    initDefaults();
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate(cms::ConnectionFactory* connectionFactory) {
    initDefaults();
    setConnectionFactory(connectionFactory);
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::~CmsTemplate() {
    
    destroySessionPools();
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::initDefaults() {
    defaultDestination = NULL;
    defaultDestinationName = "";
    messageIdEnabled = true;
    messageTimestampEnabled = true;
    noLocal = false;
    receiveTimeout = RECEIVE_TIMEOUT_INDEFINITE_WAIT;
    explicitQosEnabled = false;
    deliveryMode = cms::DeliveryMode::PERSISTENT;
    priority = DEFAULT_PRIORITY;
    timeToLive = DEFAULT_TIME_TO_LIVE;
    
    // Initialize the connection object.
    connection = NULL;
    
    // Initialize the session pools.
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix) {
        sessionPools[ix] = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::createSessionPools() {
    
    // Make sure they're destroyed first.
    destroySessionPools();
    
    /**
     * Create the session pools.
     */
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix) {
        sessionPools[ix] = new SessionPool(connection,
                (cms::Session::AcknowledgeMode)ix, 
                getResourceLifecycleManager());
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroySessionPools() {
    
    /**
     * Destroy the session pools.
     */
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix) {
        if( sessionPools[ix] != NULL ) {
            delete sessionPools[ix];
            sessionPools[ix] = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::init() throw (cms::CMSException, IllegalStateException) {

    try {
        
        // Invoke the base class.
        CmsDestinationAccessor::init();
    
        // Make sure we have a valid default destination.
        checkDefaultDestination();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroy() throw (cms::CMSException, IllegalStateException) {
      
    try {
        
        // Destroy the session pools.
        destroySessionPools();
        
        // Clear the connection reference
        connection = NULL;
        
        // Clear the reference to the default destination.
        defaultDestination = NULL;
        
        // Call the base class.
        CmsDestinationAccessor::destroy();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::checkDefaultDestination() throw (IllegalStateException) {
    if (this->defaultDestination == NULL && this->defaultDestinationName.size()==0) {
        throw IllegalStateException(
                __FILE__, __LINE__,
                "No defaultDestination or defaultDestinationName specified. Check configuration of CmsTemplate.");
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::resolveDefaultDestination(cms::Session* session)
throw (cms::CMSException, IllegalStateException) {
    
    try {
        
        // Make sure we have a default - otherwise throw.
        checkDefaultDestination();
        
        // First, check the destination object.
        cms::Destination* dest = getDefaultDestination();
        
        // If no default object was provided, the name was provided.  Resolve
        // the name and then set the destination object so we don't have to
        // do this next time.
        if( dest == NULL ) {
            dest = resolveDestinationName(session, getDefaultDestinationName());
            setDefaultDestination(dest);
        }
        
        return dest;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_RETHROW( IllegalStateException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CmsTemplate::getConnection() 
throw (cms::CMSException) {

    try {
        
        // If we don't have a connection, create one.
        if( connection == NULL ) {
            
            // Invoke the base class to create the connection and add it
            // to the resource lifecycle manager.
            connection = createConnection();
            
            // Start the connection.
            connection->start();
            
            // Create the session pools, passing in this connection.
            createSessionPools();
        }
        
        return connection;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
PooledSession* CmsTemplate::takeSession() 
throw (cms::CMSException) {

    try {
        
        // Get the connection resource to verify that the connection and session
        // pools have been allocated.
        getConnection();
        
        // Take a session from the pool.
        return sessionPools[getSessionAcknowledgeMode()]->takeSession();        
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::returnSession( PooledSession*& session ) 
throw (cms::CMSException) {

    try {

        if( session == NULL ) {
            return;
        }
        
        // Close the session, but do not delete since it's a pooled session
        session->close();
        session = NULL;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* CmsTemplate::createProducer(cms::Session* session,
        cms::Destination* dest) throw (cms::CMSException) {

    try {

        // If no destination was provided, resolve the default.
        if( dest == NULL ) {            
            dest = resolveDefaultDestination(session);
        }
        
        cms::MessageProducer* producer = NULL;
        
        // Try to use a cached producer - requires that we're using a
        // PooledSession
        PooledSession* pooledSession = dynamic_cast<PooledSession*>(session);
        if( pooledSession != NULL ) {
            producer = pooledSession->createCachedProducer(dest);
        } else {
            producer = session->createProducer(dest);
        }
        
        // Set the default values on the producer.
        producer->setDisableMessageID(!isMessageIdEnabled());
        producer->setDisableMessageTimeStamp(!isMessageTimestampEnabled());

        return producer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( IllegalStateException, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* CmsTemplate::createConsumer(cms::Session* session,
        cms::Destination* dest,
        const std::string& selector,
        bool noLocal ) throw (cms::CMSException) {

    try {

        // If no destination was provided, resolve the default.
        if( dest == NULL ) {            
            dest = resolveDefaultDestination(session);
        }
        
        cms::MessageConsumer* consumer = NULL;
        
        // Try to use a cached consumer - requires that we're using a
        // PooledSession
        PooledSession* pooledSession = dynamic_cast<PooledSession*>(session);
        if( pooledSession != NULL ) {
            consumer = pooledSession->createCachedConsumer(dest, selector, noLocal);
        } else {
            consumer = session->createConsumer(dest, selector, noLocal);
        }

        return consumer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( IllegalStateException, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyProducer( cms::MessageProducer*& producer) 
throw (cms::CMSException) {

    if( producer == NULL ) {
        return;
    }
    
    try {        
        
        // Close the producer, then destroy it.
        producer->close();                
    }
    AMQ_CATCH_NOTHROW( cms::CMSException )
    
    // Destroy if it's not a cached producer.
    CachedProducer* cachedProducer = dynamic_cast<CachedProducer*>(producer);
    if( cachedProducer == NULL ) {
        delete producer;
    }
    
    producer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyConsumer( cms::MessageConsumer*& consumer) 
throw (cms::CMSException) {

    if( consumer == NULL ) {
        return;
    }
    
    try {        
        
        // Close the producer, then destroy it.
        consumer->close();                
    }
    AMQ_CATCH_NOTHROW( cms::CMSException )
    
    // Destroy if it's not a cached consumer.
    CachedConsumer* cachedConsumer = dynamic_cast<CachedConsumer*>(consumer);
    if( cachedConsumer == NULL ) {
        delete consumer;
    }
    
    consumer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyMessage( cms::Message*& message) {

    if( message == NULL ) {
        return;
    }
    
    // Destroy the message.      
    delete message;
    message = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(SessionCallback* action) throw (cms::CMSException) {    
    
    PooledSession* pooledSession = NULL;
    
    try {
        
        if( action == NULL ) {
            return;
        }
        
        // Take a session from the pool.
        pooledSession = takeSession();
        
        // Execute the action with the given session.
        action->doInCms(pooledSession);
        
        // Return the session to the pool.
        returnSession(pooledSession);
        
    } catch( ActiveMQException& e ) {
        
        e.setMark(__FILE__, __LINE__);
        
        // Return the session to the pool.
        returnSession(pooledSession);
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(ProducerCallback* action) throw (cms::CMSException) {
    
    try {
        
        // Create the callback with using default destination.
        ProducerExecutor cb(action, this, NULL);
        
        // Execute the action in a session.
        execute(&cb);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(cms::Destination* dest,
        ProducerCallback* action) throw (cms::CMSException) {
    
    try {
        
        // Create the callback.
        ProducerExecutor cb(action, this, dest);
        
        // Execute the action in a session.
        execute(&cb);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(const std::string& destinationName,
        ProducerCallback* action) throw (cms::CMSException) {
    
    try {
        // Create the callback.
        ResolveProducerExecutor cb(action, this, destinationName);
        
        // Execute the action in a session.
        execute(&cb);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::ProducerExecutor::doInCms( cms::Session* session ) 
throw (cms::CMSException) {
        
    cms::MessageProducer* producer = NULL;
    
    try {
    
        if( session == NULL ) {
            return;
        }
        
        // Create the producer.
        producer = parent->createProducer(session, getDestination(session));
        
        // Execute the action.
        action->doInCms(session, producer);
        
        // Destroy the producer.
        parent->destroyProducer(producer);
        
    } catch( ActiveMQException& e) {
        
        e.setMark(__FILE__, __LINE__);
        
        // Destroy the producer.
        parent->destroyProducer(producer);
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::ResolveProducerExecutor::getDestination(
        cms::Session* session ) 
    throw (cms::CMSException) {
    
    try {    
        return parent->resolveDestinationName(session, destinationName);        
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(MessageCreator* messageCreator) 
throw (cms::CMSException, IllegalStateException)  {
    
    try {
        SendExecutor senderExecutor(messageCreator, this);
        execute(&senderExecutor);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_RETHROW( IllegalStateException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(cms::Destination* dest, 
                MessageCreator* messageCreator) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        SendExecutor senderExecutor(messageCreator, this);
        execute(dest, &senderExecutor);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
       
////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(const std::string& destinationName, 
                MessageCreator* messageCreator) 
throw (cms::CMSException, IllegalStateException) {
    
    try {
        SendExecutor senderExecutor(messageCreator, this);
        execute(destinationName, &senderExecutor);
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
        
////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::doSend(cms::Session* session, cms::MessageProducer* producer, 
        MessageCreator* messageCreator) throw (cms::CMSException) {
    
    cms::Message* message = NULL;
        
    try {
    
        if( producer == NULL ) {
            return;
        }        
        
        // Create the message.
        message = messageCreator->createMessage(session);
        
        // Send the message.
        if( isExplicitQosEnabled() ) {
            producer->send(message, getDeliveryMode(), getPriority(), getTimeToLive());
        } else {
            producer->send(message);
        }
        
        // Destroy the resources.
        destroyMessage(message);
        
    } catch( ActiveMQException& e) {
        
        e.setMark(__FILE__, __LINE__ );
        
        // Destroy the resources.
        destroyMessage(message);
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::doReceive(cms::MessageConsumer* consumer ) 
throw (cms::CMSException) {    
            
    try {
    
        if( consumer == NULL ) {
            throw new ActiveMQException(__FILE__, __LINE__, "consumer is NULL");
        }
        
        long long receiveTime = getReceiveTimeout();
        
        switch( receiveTime ) {
        case RECEIVE_TIMEOUT_NO_WAIT: {
            return consumer->receiveNoWait();
        }
        case RECEIVE_TIMEOUT_INDEFINITE_WAIT: {
            return consumer->receive();
        }
        default: {
            return consumer->receive(receiveTime);
        }
        }
        
    } catch( ActiveMQException& e) {
        
        e.setMark(__FILE__, __LINE__ );
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::ReceiveExecutor::doInCms(cms::Session* session) 
    throw (cms::CMSException) {
    
    cms::MessageConsumer* consumer = NULL;
                
    try {
    
        // Create the consumer resource.
        consumer = parent->createConsumer(session, getDestination(session), selector, noLocal);
        
        // Receive the message.
        message = parent->doReceive(consumer); 
            
        // Destroy the consumer resource.
        parent->destroyConsumer(consumer);
        
    } catch( ActiveMQException& e) {
        
        e.setMark(__FILE__, __LINE__ );
        
        // Destroy the message resource.
        parent->destroyMessage(message);
        
        // Destroy the consumer resource.
        parent->destroyConsumer(consumer);
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::ResolveReceiveExecutor::getDestination(
        cms::Session* session ) 
    throw (cms::CMSException) {
    
    try {    
        return parent->resolveDestinationName(session, destinationName);        
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive() 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ReceiveExecutor receiveExecutor(this, NULL,
                "", isNoLocal());
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive(cms::Destination* destination ) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ReceiveExecutor receiveExecutor(this, destination,
                "", isNoLocal());
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive(const std::string& destinationName) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ResolveReceiveExecutor receiveExecutor(this,
                "", isNoLocal(),
                destinationName);
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(const std::string& selector) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ReceiveExecutor receiveExecutor(this, NULL,
                selector, isNoLocal());
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(cms::Destination* destination,
        const std::string& selector) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ReceiveExecutor receiveExecutor(this, destination,
                selector, isNoLocal());
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(const std::string& destinationName,
        const std::string& selector) 
throw (cms::CMSException, IllegalStateException) {
    
    try {        
        ResolveReceiveExecutor receiveExecutor(this,
                selector, isNoLocal(),
                destinationName);
        execute(&receiveExecutor);
        
        return receiveExecutor.getMessage();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

