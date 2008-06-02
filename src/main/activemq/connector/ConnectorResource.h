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

#ifndef _ACTIVEMQ_CONNECTOR_CONNECTORRESOURCE_H_
#define _ACTIVEMQ_CONNECTOR_CONNECTORRESOURCE_H_

#include <cms/Closeable.h>
#include <activemq/util/Config.h>
#include <activemq/connector/ConnectorResourceListener.h>

namespace activemq{
namespace connector{

    class Connector;

    /**
     * A Connector Resource is an object that is created in the Connector
     * and must alert the connector to is closing so that the connector
     * can clean up and resouces that are associated with the resouce.
     * The lifetime of the ConnectorResource is still managed by the
     * class that owns it, but it must be close on destruction.
     * <p>
     * The Connector Resouce should notify its owner of its close by
     * calling back to its owner through a registered
     * <code>ConnectorResouceListener</code> from its owner.
     */
    class AMQCPP_API ConnectorResource : public cms::Closeable {
    public:

        virtual ~ConnectorResource() {}

        /**
         * Adds a new listener to this Connector Resource
         * @param listener - Listener of ConnectorResource events to add
         */
        virtual void addListener( ConnectorResourceListener* listener ) = 0;

        /**
         * Removes a new listener to this Connector Resource
         * @param listener - Listener of ConnectorResource events to remove
         */
        virtual void removeListener( ConnectorResourceListener* listener ) = 0;

        /**
         * Gets the Connector that this resource is associated with
         * @returns Connector pointer to this resources Connector
         */
        virtual Connector* getConnector() const = 0;

        /**
         * Sets the Connector that this Resouce is associated with.
         * @param connector - The Connector the resource is associated with.
         */
        virtual void setConnector( Connector* connector ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CONNECTOR_CONNECTORRESOURCE_H_*/
