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

#ifndef ACTIVEMQ_CMSUTIL_SESSIONPOOL_H_
#define ACTIVEMQ_CMSUTIL_SESSIONPOOL_H_

#include <activemq/cmsutil/PooledSession.h>
#include <decaf/util/concurrent/Mutex.h>
#include <cms/Connection.h>
#include <list>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    // Forward declarations.
    class ResourceLifecycleManager;

    /**
     * A pool of CMS sessions from the same connection and with the same
     * acknowledge mode.  Internal session resources are managed through a
     * provided <code>ResourceLifecycleManager</code>, not by this pool.  This
     * class is thread-safe.
     */
    class AMQCPP_API SessionPool {
    private:

        cms::Connection* connection;

        ResourceLifecycleManager* resourceLifecycleManager;

        decaf::util::concurrent::Mutex mutex;

        std::list<PooledSession*> available;

        std::list<PooledSession*> sessions;

        cms::Session::AcknowledgeMode acknowledgeMode;

    private:

        SessionPool( const SessionPool& );
        SessionPool& operator= ( const SessionPool& );

    public:

        /**
         * Constructs a session pool.
         * @param connection
         *          the connection to be used for creating all sessions.
         * @param ackMode
         *          the acknowledge mode to be used for all sessions
         * @param resourceLifecycleManager
         *          the object responsible for managing the lifecycle of
         *          any allocated cms::Session resources.
         */
        SessionPool( cms::Connection* connection,
                     cms::Session::AcknowledgeMode ackMode,
                     ResourceLifecycleManager* resourceLifecycleManager );

        /**
         * Destroys the pooled session objects, but not the underlying session
         * resources.  That is the job of the ResourceLifecycleManager.
         */
        virtual ~SessionPool();

        /**
         * Takes a session from the pool, creating one if necessary.
         *
         * @return the pooled session object
         *
         * @throws CMSException if an error occurred
         */
        virtual PooledSession* takeSession();

        /**
         * Returns a session to the pool.
         * @param session
         *         the session to be returned.
         */
        virtual void returnSession( PooledSession* session );

        ResourceLifecycleManager* getResourceLifecycleManager() {
            return resourceLifecycleManager;
        }

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_SESSIONPOOL_H_*/
