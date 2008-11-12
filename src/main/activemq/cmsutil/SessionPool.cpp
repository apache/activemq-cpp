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

#include "SessionPool.h"
#include "ResourceLifecycleManager.h"

using namespace activemq::cmsutil;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
SessionPool::SessionPool( cms::Connection* connection,
                          cms::Session::AcknowledgeMode ackMode,
                          ResourceLifecycleManager* resourceLifecycleManager) {

    this->connection = connection;
    this->acknowledgeMode = ackMode;
    this->resourceLifecycleManager = resourceLifecycleManager;
}

////////////////////////////////////////////////////////////////////////////////
SessionPool::~SessionPool() {

    // Destroy all of the pooled session objects.
    list<PooledSession*>::iterator iter = sessions.begin();
    for( ; iter != sessions.end(); ++iter ) {
        delete *iter;
    }
    sessions.clear();
    available.clear();
}

////////////////////////////////////////////////////////////////////////////////
PooledSession* SessionPool::takeSession() throw ( cms::CMSException ){

    synchronized(&mutex) {

        PooledSession* pooledSession = NULL;

        // If there are no sessions available, create a new one and return it.
        if( available.size() == 0 ) {

            // No sessions were available - create a new one.
            cms::Session* session = connection->createSession( acknowledgeMode );

            // Give this resource to the life-cycle manager to manage. The pool
            // will not be in charge of destroying this resource.
            resourceLifecycleManager->addSession( session );

            // Now wrap the session with a pooled session.
            pooledSession = new PooledSession( this, session );

            // Add to the sessions list.
            sessions.push_back( pooledSession );

        } else {

            // There are sessions available - use the one at the head of the
            // list, and remove it from the available list.
            pooledSession = available.front();
            available.pop_front();
        }

        // Return the session.
        return pooledSession;
    }

    // Fixes compiler warnings.
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void SessionPool::returnSession( PooledSession* session ) {

    synchronized( &mutex ) {

        // Add to the available list.
        available.push_back( session );
    }
}
