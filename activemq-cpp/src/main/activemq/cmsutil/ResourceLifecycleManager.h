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
#include <decaf/util/LinkedList.h>

namespace activemq {
namespace cmsutil {

    /**
     * Manages the lifecycle of a set of CMS resources.  A call to
     * <code>destroy</code> will close and destroy all of the contained
     * resources in the appropriate manner.
     */
    class AMQCPP_API ResourceLifecycleManager {
    private:

        typedef decaf::util::LinkedList< cms::Connection* > ConnectionList;
        typedef decaf::util::LinkedList< cms::Session* > SessionList;
        typedef decaf::util::LinkedList< cms::Destination* > DestinationList;
        typedef decaf::util::LinkedList< cms::MessageProducer* > ProducerList;
        typedef decaf::util::LinkedList< cms::MessageConsumer* > ConsumerList;

        ConnectionList connections;
        SessionList sessions;
        DestinationList destinations;
        ProducerList producers;
        ConsumerList consumers;

    protected:

        ResourceLifecycleManager( const ResourceLifecycleManager& );
        ResourceLifecycleManager& operator= ( const ResourceLifecycleManager& );

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
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        void addConnection( cms::Connection* connection );

        /**
         * Adds a session so that its life will be managed by
         * this object.
         *
         * @param session
         *         the object to be managed
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        void addSession( cms::Session* session );

        /**
         * Adds a destination so that its life will be managed by
         * this object.
         *
         * @param dest
         *         the object to be managed
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        void addDestination( cms::Destination* dest );

        /**
         * Adds a message producer so that its life will be managed by
         * this object.
         *
         * @param producer
         *         the object to be managed
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        void addMessageProducer( cms::MessageProducer* producer );

        /**
         * Adds a message consumer so that its life will be managed by
         * this object.
         *
         * @param consumer
         *         the object to be managed
         *
         * @throws CMSException if an error occurs while performing this operation.
         */
        void addMessageConsumer( cms::MessageConsumer* consumer );

        /**
         * Closes and destroys the contained CMS resources.
         *
         * @throws cms::CMSException thrown if an error occurs.
         */
        void destroy();

        /**
         * Releases all of the contained resources so that this
         * object will no longer control their lifetimes.
         */
        void releaseAll();

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_RESOURCELIFECYCLEMANAGER_H_*/
