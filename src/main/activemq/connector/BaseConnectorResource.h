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

#ifndef _ACTIVEMQ_CONNECTOR_BASECONNECTORRESOURCE_H_
#define _ACTIVEMQ_CONNECTOR_BASECONNECTORRESOURCE_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ConnectorResource.h>
#include <activemq/connector/ConnectorResourceListener.h>

#include <set>

namespace activemq{
namespace connector{

    class Connector;

    /**
     * Base Class that implements the common functionality of a
     * <code>ConnectorResource</code>.
     */
    class AMQCPP_API BaseConnectorResource : public ConnectorResource {
    private:

        /** The main Connector to call back when closed */
        Connector* connector;

        /** Set of ConnectorResourceListeners to call back */
        std::set<ConnectorResourceListener*> listeners;

        /** Have we been closed already */
        bool closed;

    public:

        /**
         * Default Constructor
         */
        BaseConnectorResource();

        /**
         * Constructor - Accepts a Connector to call for Close if this
         * resource.
         * @param connector - The Connector to close on.
         */
        BaseConnectorResource( Connector* connector );

        virtual ~BaseConnectorResource() {}

        /**
         * Handles the close of the Resource which calls back to the
         * Connector to close this resource then notifies the listeners
         * that this resource has closed.
         * @throws CMSException
         */
        virtual void close() throw( cms::CMSException );

        /**
         * @ Returns true if this resource has been closed already
         */
        virtual bool isClosed() const {
            return this->closed;
        }

        /**
         * Adds a new listener to this Connector Resource
         * @param listener - Listener of ConnectorResource events to add
         */
        virtual void addListener( ConnectorResourceListener* listener );

        /**
         * Removes a new listener to this Connector Resource
         * @param listener - Listener of ConnectorResource events to remove
         */
        virtual void removeListener( ConnectorResourceListener* listener );

        /**
         * Gets the Connector that this resource is associated with
         * @returns Connector pointer to this resources Connector
         */
        virtual Connector* getConnector() const {
            return this->connector;
        }

        /**
         * Sets the Connector that this Resouce is associated with.
         * @param connector - The Connector the resource is associated with.
         */
        virtual void setConnector( Connector* connector ) {
            this->connector = connector;
        }

    };

}}

#endif /*_ACTIVEMQ_CONNECTOR_BASECONNECTORRESOURCE_H_*/
