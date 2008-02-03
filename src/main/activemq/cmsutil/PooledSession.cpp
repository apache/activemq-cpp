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
PooledSession::PooledSession(SessionPool* pool, cms::Session* session) {
    this->session = session;
    this->pool = pool;
}

////////////////////////////////////////////////////////////////////////////////
PooledSession::~PooledSession(){
}

////////////////////////////////////////////////////////////////////////////////
void PooledSession::close() throw( cms::CMSException ) {
    
    if( pool != NULL ) {
        pool->returnSession(this);
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* PooledSession::createCachedProducer( 
        const cms::Destination* destination )
    throw ( cms::CMSException ) {
    
    try {
        
        std::string destName = getUniqueDestName(destination);
        
        // Check the cache - add it if necessary.
        cms::MessageProducer* p = producerCache.getValue(destName);
        if( p == NULL ) {
            
            // No producer exists for this destination - create it.
            p = session->createProducer(destination);           
            
            // Add it to the cache.
            producerCache.setValue(destName, p);
            
            // Add the producer to the resource lifecycle manager.
            pool->getResourceLifecycleManager()->addMessageProducer(p);
        }
        
        return p;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string PooledSession::getUniqueDestName( const cms::Destination* dest ) {
    
    std::string destName;
    const cms::Queue* queue = dynamic_cast<const cms::Queue*>(dest);
    if( queue != NULL ) {
        destName = "q:" + queue->getQueueName();
    } else {
        const cms::Topic* topic = dynamic_cast<const cms::Topic*>(dest);
        if( topic != NULL ) {
            destName = "t:" + topic->getTopicName();
        }
    }
    
    return destName;
}
