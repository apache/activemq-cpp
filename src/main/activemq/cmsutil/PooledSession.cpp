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

#include "PooledSession.h"
#include "SessionPool.h"
#include "ResourceLifecycleManager.h"
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/exceptions/ExceptionDefines.h>

using namespace activemq::cmsutil;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
PooledSession::PooledSession( SessionPool* pool, cms::Session* session ) {
    this->session = session;
    this->pool = pool;
}

////////////////////////////////////////////////////////////////////////////////
PooledSession::~PooledSession(){

    // Destroy cached producers.
    std::vector<CachedProducer*> cachedProducers = producerCache.getValues();
    for( std::size_t ix = 0; ix < cachedProducers.size(); ++ix ) {
        delete cachedProducers[ix];
    }
    cachedProducers.clear();

    // Destroy cached consumers.
    std::vector<CachedConsumer*> cachedConsumers = consumerCache.getValues();
    for( std::size_t ix = 0; ix < cachedConsumers.size(); ++ix ) {
        delete cachedConsumers[ix];
    }
    cachedConsumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
void PooledSession::close() throw( cms::CMSException ) {

    if( pool != NULL ) {
        pool->returnSession( this );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* PooledSession::createCachedProducer(
        const cms::Destination* destination )
    throw ( cms::CMSException ) {

    try {

        if( destination == NULL ) {
            throw ActiveMQException(__FILE__, __LINE__, "destination is NULL");
        }

        std::string key = getUniqueDestName(destination);

        // Check the cache - add it if necessary.
        CachedProducer* cachedProducer = NULL;
        try {
            cachedProducer = producerCache.getValue( key );
        } catch( decaf::lang::exceptions::NoSuchElementException& e ) {

            // No producer exists for this destination - start by creating
            // a new producer resource.
            cms::MessageProducer* p = session->createProducer( destination );

            // Add the producer resource to the resource lifecycle manager.
            pool->getResourceLifecycleManager()->addMessageProducer( p );

            // Create the cached producer wrapper.
            cachedProducer = new CachedProducer( p );

            // Add it to the cache.
            producerCache.setValue( key, cachedProducer );
        }

        return cachedProducer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* PooledSession::createCachedConsumer(
    const cms::Destination* destination,
    const std::string& selector,
    bool noLocal ) throw ( cms::CMSException ) {

    try {

        if( destination == NULL ) {
            throw ActiveMQException( __FILE__, __LINE__, "destination is NULL" );
        }

        // Append the selector and noLocal flag onto the key.
        std::string key = getUniqueDestName( destination );
        key += "s=";
        key += selector;
        key += ",nl=";
        key += ( noLocal? "t" : "f" );

        // Check the cache - add it if necessary.
        CachedConsumer* cachedConsumer = NULL;
        try {
            cachedConsumer = consumerCache.getValue( key );
        } catch( decaf::lang::exceptions::NoSuchElementException& e ) {

            // No producer exists for this destination - start by creating
            // a new consumer resource.
            cms::MessageConsumer* c = session->createConsumer( destination, selector, noLocal );

            // Add the consumer resource to the resource lifecycle manager.
            pool->getResourceLifecycleManager()->addMessageConsumer( c );

            // Create the cached consumer wrapper.
            cachedConsumer = new CachedConsumer( c );

            // Add it to the cache.
            consumerCache.setValue( key, cachedConsumer );
        }

        return cachedConsumer;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string PooledSession::getUniqueDestName( const cms::Destination* dest ) {

    std::string destName = "[";
    const cms::Queue* queue = dynamic_cast<const cms::Queue*>( dest );
    if( queue != NULL ) {
        destName += "q:" + queue->getQueueName();
    } else {
        const cms::Topic* topic = dynamic_cast<const cms::Topic*>( dest );
        if( topic != NULL ) {
            destName += "t:" + topic->getTopicName();
        }
    }

    destName += "]";

    return destName;
}
