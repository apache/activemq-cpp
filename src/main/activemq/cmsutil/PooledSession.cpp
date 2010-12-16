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
#include <cms/CMSException.h>

using namespace cms;
using namespace activemq::cmsutil;

/**
 * A catch-all that throws an CMSException.
 */
#define CMSTEMPLATE_CATCHALL() \
    catch( cms::CMSException& ex ){ \
        throw ex; \
    } catch( ... ){ \
        throw CMSException( "caught unknown exception", NULL ); \
    }

////////////////////////////////////////////////////////////////////////////////
PooledSession::PooledSession( SessionPool* pool, cms::Session* session )
    : pool( pool ), session( session ), producerCache(), consumerCache() {
}

////////////////////////////////////////////////////////////////////////////////
PooledSession::~PooledSession() throw() {

    // Destroy cached producers.
    std::vector<CachedProducer*> cachedProducers = producerCache.values();
    for( std::size_t ix = 0; ix < cachedProducers.size(); ++ix ) {
        delete cachedProducers[ix];
    }
    cachedProducers.clear();

    // Destroy cached consumers.
    std::vector<CachedConsumer*> cachedConsumers = consumerCache.values();
    for( std::size_t ix = 0; ix < cachedConsumers.size(); ++ix ) {
        delete cachedConsumers[ix];
    }
    cachedConsumers.clear();
}

////////////////////////////////////////////////////////////////////////////////
void PooledSession::close() {

    if( pool != NULL ) {
        pool->returnSession( this );
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* PooledSession::createBrowser( const cms::Queue* queue ) {
    return session->createBrowser( queue );
}

////////////////////////////////////////////////////////////////////////////////
cms::QueueBrowser* PooledSession::createBrowser( const cms::Queue* queue, const std::string& selector ) {
    return session->createBrowser( queue, selector );
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* PooledSession::createCachedProducer( const cms::Destination* destination ) {

    try {

        if( destination == NULL ) {
            throw CMSException( "destination is NULL", NULL );
        }

        std::string key = getUniqueDestName(destination);

        // Check the cache - add it if necessary.
        CachedProducer* cachedProducer = NULL;
        try {
            cachedProducer = producerCache.get( key );
        } catch( decaf::util::NoSuchElementException& e ) {

            // No producer exists for this destination - start by creating
            // a new producer resource.
            cms::MessageProducer* p = session->createProducer( destination );

            // Add the producer resource to the resource lifecycle manager.
            pool->getResourceLifecycleManager()->addMessageProducer( p );

            // Create the cached producer wrapper.
            cachedProducer = new CachedProducer( p );

            // Add it to the cache.
            producerCache.put( key, cachedProducer );
        }

        return cachedProducer;
    }
    CMSTEMPLATE_CATCHALL()
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* PooledSession::createCachedConsumer( const cms::Destination* destination,
                                                           const std::string& selector,
                                                           bool noLocal ) {

    try {

        if( destination == NULL ) {
            throw CMSException( "destination is NULL", NULL );
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
            cachedConsumer = consumerCache.get( key );
        } catch( decaf::util::NoSuchElementException& e ) {

            // No producer exists for this destination - start by creating
            // a new consumer resource.
            cms::MessageConsumer* c = session->createConsumer( destination, selector, noLocal );

            // Add the consumer resource to the resource lifecycle manager.
            pool->getResourceLifecycleManager()->addMessageConsumer( c );

            // Create the cached consumer wrapper.
            cachedConsumer = new CachedConsumer( c );

            // Add it to the cache.
            consumerCache.put( key, cachedConsumer );
        }

        return cachedConsumer;
    }
    CMSTEMPLATE_CATCHALL()
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
