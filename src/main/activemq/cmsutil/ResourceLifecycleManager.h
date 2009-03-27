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

#ifndef ACTIVEMQ_CMSUTIL_RESOURCELIFECYCLEMANAGER_H_
#define ACTIVEMQ_CMSUTIL_RESOURCELIFECYCLEMANAGER_H_

#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    /**
     * Manages the lifecycle of a set of CMS resources.  A call to
     * <code>destroy</code> will close and destroy all of the contained
     * resources in the appropriate manner.
     */
    class AMQCPP_API ResourceLifecycleManager {
    private:

        std::vector<cms::Connection*> connections;
        std::vector<cms::Session*> sessions;
        std::vector<cms::Destination*> destinations;
        std::vector<cms::MessageProducer*> producers;
        std::vector<cms::MessageConsumer*> consumers;

    public:

        ResourceLifecycleManager();

        /**
         * Destructor - calls <code>destroy</code>
         */
        virtual ~ResourceLifecycleManager();

        /**
         * Adds a connection so that its life will be managed by
         * this object.
         *
         * @param connection
         *         the object to be managed
         */
        void addConnection( cms::Connection* connection ) {
            connections.push_back( connection );
        }

        /**
         * Adds a session so that its life will be managed by
         * this object.
         *
         * @param session
         *         the object to be managed
         */
        void addSession( cms::Session* session ) {
            sessions.push_back( session );
        }

        /**
         * Adds a destination so that its life will be managed by
         * this object.
         *
         * @param dest
         *         the object to be managed
         */
        void addDestination( cms::Destination* dest ) {
            destinations.push_back( dest );
        }

        /**
         * Adds a message producer so that its life will be managed by
         * this object.
         *
         * @param producer
         *         the object to be managed
         */
        void addMessageProducer( cms::MessageProducer* producer ) {
            producers.push_back( producer );
        }

        /**
         * Adds a message consumer so that its life will be managed by
         * this object.
         *
         * @param consumer
         *         the object to be managed
         */
        void addMessageConsumer( cms::MessageConsumer* consumer ) {
            consumers.push_back( consumer );
        }

        /**
         * Closes and destroys the contained CMS resources.
         * @throws cms::CMSException thrown if an error occurs.
         */
        void destroy() throw ( cms::CMSException );

        /**
         * Releases all of the contained resources so that this
         * object will no longer control their lifetimes.
         */
        void releaseAll();

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_RESOURCELIFECYCLEMANAGER_H_*/
