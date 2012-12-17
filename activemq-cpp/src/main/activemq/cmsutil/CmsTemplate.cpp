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
#include "ProducerCallback.h"
#include "MessageCreator.h"
#include <iostream>

#include <cms/IllegalStateException.h>
#include <cms/CMSException.h>

using namespace cms;
using namespace activemq::cmsutil;
using namespace std;

/**
 * Macro for catching an exception then rethrowing an
 * cms::CMSException.
 * @param type
 *      The type of the exception to throw
 * @param t
 *      The instance of CmsTemplate
 * (e.g. ActiveMQException ).
 */
#define CMSTEMPLATE_CATCH( type, t ) \
    catch( type& ex ){ \
        ex.setMark(__FILE__, __LINE__); \
        try { \
            t->destroy(); \
        } catch(...) {} \
        throw CMSException(ex.what(), NULL); \
    }

/**
 * A catch-all that throws an CMSException.
 * @param t
 *      The instance of CmsTemplate
 */
#define CMSTEMPLATE_CATCHALL(t) \
    catch( cms::CMSException& ex ){ \
        try { \
            t->destroy(); \
        } catch( ... ) {} \
        throw; \
    } catch(...){ \
        try { \
            t->destroy(); \
        } catch( ... ) {} \
        throw CMSException("caught unknown exception", NULL); \
    }

#define CMSTEMPLATE_CATCHALL_NOTHROW( ) \
    catch(...){ \
    }

////////////////////////////////////////////////////////////////////////////////
const long long CmsTemplate::RECEIVE_TIMEOUT_NO_WAIT = -1;
const long long CmsTemplate::RECEIVE_TIMEOUT_INDEFINITE_WAIT = 0;
const int CmsTemplate::DEFAULT_PRIORITY = 4;
const long long CmsTemplate::DEFAULT_TIME_TO_LIVE = 0;

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate() : CmsDestinationAccessor(),
                             connection(NULL),
                             sessionPools(),
                             defaultDestination(NULL),
                             defaultDestinationName(""),
                             messageIdEnabled(false),
                             messageTimestampEnabled(false),
                             noLocal(false),
                             receiveTimeout(0),
                             explicitQosEnabled(false),
                             deliveryMode(0),
                             priority(0),
                             timeToLive(0),
                             initialized(false) {

    initDefaults();
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate( cms::ConnectionFactory* connectionFactory ) : CmsDestinationAccessor(),
                                                                        connection(NULL),
                                                                        sessionPools(),
                                                                        defaultDestination(NULL),
                                                                        defaultDestinationName(""),
                                                                        messageIdEnabled(false),
                                                                        messageTimestampEnabled(false),
                                                                        noLocal(false),
                                                                        receiveTimeout(0),
                                                                        explicitQosEnabled(false),
                                                                        deliveryMode(0),
                                                                        priority(0),
                                                                        timeToLive(0),
                                                                        initialized(false) {

    initDefaults();
    setConnectionFactory(connectionFactory);
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::~CmsTemplate() {

    try {
        destroy();
    } catch( ... ) { /* Absorb */ }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::initDefaults() {
    initialized = false;
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
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix ) {
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
    for (int ix = 0; ix < NUM_SESSION_POOLS; ++ix) {
        sessionPools[ix] = new SessionPool(connection, (cms::Session::AcknowledgeMode) ix, getResourceLifecycleManager());
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroySessionPools() {

    /**
     * Destroy the session pools.
     */
    for (int ix = 0; ix < NUM_SESSION_POOLS; ++ix) {
        if (sessionPools[ix] != NULL) {
            delete sessionPools[ix];
            sessionPools[ix] = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::init() {

    try {

        if (!initialized) {

            // Invoke the base class.
            CmsDestinationAccessor::init();

            initialized = true;
        }
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroy() {

    try {

        // Mark as not initialized.
        initialized = false;

        // Clear the connection reference
        connection = NULL;

        // Clear the reference to the default destination.
        defaultDestination = NULL;

        // Destroy the session pools.
        destroySessionPools();

        // Call the base class.
        CmsDestinationAccessor::destroy();
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::checkDefaultDestination() {
    if (this->defaultDestination == NULL && this->defaultDestinationName.size() == 0) {
        throw IllegalStateException("No defaultDestination or defaultDestinationName specified."
                "Check configuration of CmsTemplate.", NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::resolveDefaultDestination(cms::Session* session) {

    try {

        // Make sure we have a default - otherwise throw.
        checkDefaultDestination();

        // First, check the destination object.
        cms::Destination* dest = getDefaultDestination();

        // If no default object was provided, the name was provided.  Resolve
        // the name and then set the destination object so we don't have to
        // do this next time.
        if (dest == NULL) {
            dest = resolveDestinationName(session, getDefaultDestinationName());
            setDefaultDestination(dest);
        }

        return dest;
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CmsTemplate::getConnection() {

    try {

        // If we don't have a connection, create one.
        if (connection == NULL) {

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
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
PooledSession* CmsTemplate::takeSession() {

    try {

        // Get the connection resource to verify that the connection and session
        // pools have been allocated.
        getConnection();

        // Take a session from the pool.
        return sessionPools[getSessionAcknowledgeMode()]->takeSession();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::returnSession(PooledSession*& session) {

    try {

        if (session == NULL) {
            return;
        }

        // Close the session, but do not delete since it's a pooled session
        session->close();
        session = NULL;
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* CmsTemplate::createProducer(cms::Session* session, cms::Destination* dest) {

    try {

        // If no destination was provided, resolve the default.
        if (dest == NULL) {
            dest = resolveDefaultDestination(session);
        }

        cms::MessageProducer* producer = NULL;

        // Try to use a cached producer - requires that we're using a
        // PooledSession
        PooledSession* pooledSession = dynamic_cast<PooledSession*>(session);
        if (pooledSession != NULL) {
            producer = pooledSession->createCachedProducer(dest);
        } else {
            producer = session->createProducer(dest);
        }

        // Set the default values on the producer.
        producer->setDisableMessageID(!isMessageIdEnabled());
        producer->setDisableMessageTimeStamp(!isMessageTimestampEnabled());

        return producer;
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* CmsTemplate::createConsumer(cms::Session* session, cms::Destination* dest, const std::string& selector, bool noLocal) {

    try {

        // If no destination was provided, resolve the default.
        if (dest == NULL) {
            dest = resolveDefaultDestination(session);
        }

        cms::MessageConsumer* consumer = NULL;

        // Try to use a cached consumer - requires that we're using a
        // PooledSession
        PooledSession* pooledSession = dynamic_cast<PooledSession*>(session);
        if (pooledSession != NULL) {
            consumer = pooledSession->createCachedConsumer(dest, selector, noLocal);
        } else {
            consumer = session->createConsumer(dest, selector, noLocal);
        }

        return consumer;
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyProducer(cms::MessageProducer*& producer) {

    if (producer == NULL) {
        return;
    }

    try {
        // Close the producer, then destroy it.
        producer->close();
    }
    CMSTEMPLATE_CATCHALL_NOTHROW()

    // Destroy if it's not a cached producer.
    CachedProducer* cachedProducer = dynamic_cast<CachedProducer*>(producer);
    if (cachedProducer == NULL) {
        delete producer;
    }

    producer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyConsumer(cms::MessageConsumer*& consumer) {

    if (consumer == NULL) {
        return;
    }

    try {
        // Close the producer, then destroy it.
        consumer->close();
    }
    CMSTEMPLATE_CATCHALL_NOTHROW()

    // Destroy if it's not a cached consumer.
    CachedConsumer* cachedConsumer = dynamic_cast<CachedConsumer*>(consumer);
    if (cachedConsumer == NULL) {
        delete consumer;
    }

    consumer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyMessage(cms::Message*& message) {

    if (message == NULL) {
        return;
    }

    // Destroy the message.
    delete message;
    message = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(SessionCallback* action) {

    PooledSession* pooledSession = NULL;

    try {

        if (action == NULL) {
            return;
        }

        // Verify that we are initialized
        init();

        // Take a session from the pool.
        pooledSession = takeSession();

        // Execute the action with the given session.
        action->doInCms(pooledSession);

        // Return the session to the pool.
        returnSession(pooledSession);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(ProducerCallback* action) {

    try {

        // Verify that we are initialized
        init();

        // Create the callback with using default destination.
        ProducerExecutor cb(action, this, NULL);

        // Execute the action in a session.
        execute(&cb);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(cms::Destination* dest, ProducerCallback* action) {

    try {

        // Verify that we are initialized
        init();

        // Create the callback.
        ProducerExecutor cb(action, this, dest);

        // Execute the action in a session.
        execute(&cb);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(const std::string& destinationName, ProducerCallback* action) {

    try {

        // Verify that we are initialized
        init();

        // Create the callback.
        ResolveProducerExecutor cb(action, this, destinationName);

        // Execute the action in a session.
        execute(&cb);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, this)
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::ProducerExecutor::doInCms(cms::Session* session) {

    cms::MessageProducer* producer = NULL;

    try {

        if (session == NULL) {
            return;
        }

        // Create the producer.
        producer = parent->createProducer(session, getDestination(session));

        // Execute the action.
        action->doInCms(session, producer);

        // Destroy the producer.
        parent->destroyProducer(producer);

    }
    CMSTEMPLATE_CATCHALL(parent)
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::ResolveProducerExecutor::getDestination(cms::Session* session) {

    try {
        return parent->resolveDestinationName(session, destinationName);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, parent)
    CMSTEMPLATE_CATCHALL(parent)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(MessageCreator* messageCreator) {

    try {
        SendExecutor senderExecutor(messageCreator, this);
        execute(&senderExecutor);
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(cms::Destination* dest, MessageCreator* messageCreator) {

    try {
        SendExecutor senderExecutor(messageCreator, this);
        execute(dest, &senderExecutor);
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::send(const std::string& destinationName, MessageCreator* messageCreator) {

    try {
        SendExecutor senderExecutor(messageCreator, this);
        execute(destinationName, &senderExecutor);
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::doSend(cms::Session* session, cms::MessageProducer* producer, MessageCreator* messageCreator) {

    cms::Message* message = NULL;

    try {

        if (producer == NULL) {
            return;
        }

        // Create the message.
        message = messageCreator->createMessage(session);

        // Send the message.
        if (isExplicitQosEnabled()) {
            producer->send(message, getDeliveryMode(), getPriority(), getTimeToLive());
        } else {
            producer->send(message);
        }

        // Destroy the resources.
        destroyMessage(message);

    } catch (CMSException& e) {

        e.setMark(__FILE__, __LINE__);

        // Destroy the resources.
        destroyMessage(message);

        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::doReceive(cms::MessageConsumer* consumer) {

    try {

        if (consumer == NULL) {
            throw CMSException("consumer is NULL", NULL);
        }

        long long receiveTime = getReceiveTimeout();

        switch (receiveTime) {
            case RECEIVE_TIMEOUT_NO_WAIT: {
                return consumer->receiveNoWait();
            }
            case RECEIVE_TIMEOUT_INDEFINITE_WAIT: {
                return consumer->receive();
            }
            default: {
                return consumer->receive((int) receiveTime);
            }
        }

    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::ReceiveExecutor::doInCms(cms::Session* session) {

    cms::MessageConsumer* consumer = NULL;
    message = NULL;

    try {

        // Create the consumer resource.
        consumer = parent->createConsumer(session, getDestination(session), selector, noLocal);

        // Receive the message.
        message = parent->doReceive(consumer);

        // Destroy the consumer resource.
        parent->destroyConsumer(consumer);

    } catch (CMSException& e) {

        e.setMark(__FILE__, __LINE__);

        // Destroy the message resource.
        parent->destroyMessage(message);

        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CmsTemplate::ResolveReceiveExecutor::getDestination(cms::Session* session) {

    try {
        return parent->resolveDestinationName(session, destinationName);
    }
    CMSTEMPLATE_CATCH(IllegalStateException, parent)
    CMSTEMPLATE_CATCHALL(parent)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive() {

    try {

        ReceiveExecutor receiveExecutor(this, NULL, "", isNoLocal());
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive(cms::Destination* destination) {

    try {

        ReceiveExecutor receiveExecutor(this, destination, "", isNoLocal());
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receive(const std::string& destinationName) {

    try {

        ResolveReceiveExecutor receiveExecutor(this, "", isNoLocal(), destinationName);
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(const std::string& selector) {

    try {

        ReceiveExecutor receiveExecutor(this, NULL, selector, isNoLocal());
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(cms::Destination* destination, const std::string& selector) {

    try {

        ReceiveExecutor receiveExecutor(this, destination, selector, isNoLocal());
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}

////////////////////////////////////////////////////////////////////////////////
cms::Message* CmsTemplate::receiveSelected(const std::string& destinationName, const std::string& selector) {

    try {

        ResolveReceiveExecutor receiveExecutor(this, selector, isNoLocal(), destinationName);
        execute(&receiveExecutor);

        return receiveExecutor.getMessage();
    }
    CMSTEMPLATE_CATCHALL(this)
}
