/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEMPDESTINATION_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEMPDESTINATION_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQDestination.h>
#include <activemq/connector/Connector.h>
#include <activemq/connector/ConnectorResource.h>
#include <cms/Closeable.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQTempDestination : public ActiveMQDestination,
                                    public ConnectorResource,
                                    public cms::Closeable
    {
    protected:

        /**
         * Connector that we call back on close to allow this resource to
         * be cleaned up correctly at this end and at the Broker End.
         */
        connector::Connector* connector;

    public:

        const static unsigned char ID_ACTIVEMQTEMPDESTINATION = 0;

    public:

        ActiveMQTempDestination();
        ActiveMQTempDestination( const std::string& name );
        virtual ~ActiveMQTempDestination();

        virtual unsigned char getDataStructureType() const;

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQDestination::copyDataStructure( src );
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const {
            std::ostringstream stream;

            stream << "Begin Class = ActiveMQTempDestination" << std::endl;
            stream << ActiveMQDestination::toString();
            stream << "End Class = ActiveMQTempDestination" << std::endl;

            return stream.str();
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {
            return ActiveMQDestination::equals( value );
        }

    public:   // Closeable

        /**
         * Closes this object and deallocates the appropriate resources.
         * The object is generally no longer usable after calling close.
         * @throws CMSException
         */
        virtual void close() throw( cms::CMSException );

    public:    // ActiveMQTempDestination

        /**
         * Gets the Connector that this object will call when its closed
         * @returns pointer to a Connector, or NULL if not set.
         */
        const connector::Connector* getConnector() const {
            return this->connector;
        }

        /**
         * Sets the Connector that this object will call when its closed
         * @param connector - pointer to a Connector or NULL to clear.
         */
        virtual void setConnector( connector::Connector* connector ) {
            this->connector = connector;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEMPDESTINATION_H_*/

