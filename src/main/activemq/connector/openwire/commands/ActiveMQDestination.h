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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATION_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATION_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/commands/BaseDataStructure.h>
#include <activemq/util/ActiveMQProperties.h>
#include <cms/Destination.h>

#include <vector>
#include <string>
#include <map>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class AMQCPP_API ActiveMQDestination : public BaseDataStructure {
    protected:

        enum DESTINATION_TYPE_IDS {
            ACTIVEMQ_TOPIC = 1,
            ACTIVEMQ_TEMPORARY_TOPIC = 2,
            ACTIVEMQ_QUEUE = 3,
            ACTIVEMQ_TEMPORARY_QUEUE = 4
        };

        struct DestinationFilter{
            static const std::string ANY_CHILD;
            static const std::string ANY_DESCENDENT;
        };

        /**
         * prefix for Advisory message destinations
         */
        static const std::string ADVISORY_PREFIX;

        /**
         * prefix for consumer advisory destinations
         */
        static const std::string CONSUMER_ADVISORY_PREFIX;

        /**
         * prefix for producer advisory destinations
         */
        static const std::string PRODUCER_ADVISORY_PREFIX;

        /**
         * prefix for connection advisory destinations
         */
        static const std::string CONNECTION_ADVISORY_PREFIX;

        /**
         * The default target for ordered destinations
         */
        static const std::string DEFAULT_ORDERED_TARGET;

        static const std::string TEMP_PREFIX;
        static const std::string TEMP_POSTFIX;
        static const std::string COMPOSITE_SEPARATOR;

        // Cached transient data
        bool exclusive;
        bool ordered;
        bool advisory;
        std::string orderedTarget;

        std::string physicalName;
        util::ActiveMQProperties options;

    public:

        const static unsigned char ID_ACTIVEMQDESTINATION = 0;

    public:

        ActiveMQDestination();
        ActiveMQDestination( const std::string& physicalName );
        virtual ~ActiveMQDestination() {}

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQDestination* cloneDataStructure() const {
            return NULL;
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        virtual unsigned char getDataStructureType() const;

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const;

        /**
         * Fetch this destination's physical name
         * @returns const string containing the name
         */
        virtual const std::string& getPhysicalName() const {
            return this->physicalName;
        }
        virtual std::string& getPhysicalName() {
            return this->physicalName;
        }

        /**
         * Set this destination's physical name
         * @returns const string containing the name
         */
        virtual void setPhysicalName( const std::string& physicalName );

        /**
         * @return Returns the advisory.
         */
        virtual bool isAdvisory() const {
            return advisory;
        }

        /**
         * @param advisory The advisory to set.
         */
        virtual void setAdvisory( bool advisory ){
            this->advisory = advisory;
        }

        /**
         * @return true if this is a destination for Consumer advisories
         */
        virtual bool isConsumerAdvisory() const {
            return isAdvisory() &&
                   physicalName.find(CONSUMER_ADVISORY_PREFIX) == 0;
        }

        /**
         * @return true if this is a destination for Producer advisories
         */
        virtual bool isProducerAdvisory() const {
            return isAdvisory() &&
                   physicalName.find(PRODUCER_ADVISORY_PREFIX) == 0;
        }

        /**
         * @return true if this is a destination for Connection advisories
         */
        virtual bool isConnectionAdvisory() const {
            return isAdvisory() &&
                   physicalName.find(CONNECTION_ADVISORY_PREFIX) == 0;
        }

        /**
         * @return Returns the exclusive.
         */
        virtual bool isExclusive() const {
            return exclusive;
        }

        /**
         * @param exclusive The exclusive to set.
         */
        virtual void setExclusive( bool exclusive ) {
            this->exclusive = exclusive;
        }

        /**
         * @return Returns the ordered.
         */
        virtual bool isOrdered() const {
            return ordered;
        }

        /**
         * @param ordered The ordered to set.
         */
        virtual void setOrdered( bool ordered ) {
            this->ordered = ordered;
        }

        /**
         * @return Returns the orderedTarget.
         */
        virtual std::string getOrderedTarget() const {
            return orderedTarget;
        }

        /**
         * @param orderedTarget The orderedTarget to set.
         */
        virtual void setOrderedTarget( const std::string& orderedTarget) {
            this->orderedTarget = orderedTarget;
        }

    public:  // Statics

        /**
         * Create a temporary name from the clientId
         *
         * @param clientId
         * @return
         */
        static std::string createTemporaryName(
            const std::string& clientId ) {

            return TEMP_PREFIX + clientId + TEMP_POSTFIX;
        }

        /**
         * From a temporary destination find the clientId of the Connection that created it
         *
         * @param destination
         * @return the clientId or null if not a temporary destination
         */
        static std::string getClientId( const ActiveMQDestination* destination );

    public:

        /**
         * Returns the Type of Destination that this object represents
         * @returns int type qualifier.
         */
        virtual cms::Destination::DestinationType getDestinationType() const = 0;

        /**
         * Returns true if a temporary Destination
         * @return true/false
         */
        virtual bool isTemporary() const {
            return getDestinationType() == cms::Destination::TEMPORARY_TOPIC ||
                   getDestinationType() == cms::Destination::TEMPORARY_QUEUE;
        }

        /**
         * Returns true if a Topic Destination
         * @return true/false
         */
        virtual bool isTopic() const {
            return getDestinationType() == cms::Destination::TOPIC ||
                   getDestinationType() == cms::Destination::TEMPORARY_TOPIC;
        }

        /**
         * Returns true if a Queue Destination
         * @return true/false
         */
        virtual bool isQueue() const {
            return !isTopic();
        }

        /**
         * Returns true if this destination represents a collection of
         * destinations; allowing a set of destinations to be published to or
         * subscribed from in one CMS operation.
         * <p/>
         * If this destination is a composite then you can call
         * {@link #getChildDestinations()}
         * to return the list of child destinations.
         * @return true if this destination represents a collection of child
         *         destinations.
         */
        virtual bool isComposite() const {
            return physicalName.find_first_of( COMPOSITE_SEPARATOR ) > 0;
        }

        /**
         * @return true if the destination matches multiple possible destinations
         */
        virtual bool isWildcard() const {
            return physicalName.find_first_of( DestinationFilter::ANY_CHILD ) != std::string::npos ||
                   physicalName.find_first_of( DestinationFilter::ANY_DESCENDENT ) != std::string::npos;
        }

        /**
         * @returns a reference (const) to the options properties for this Dest.
         */
        const activemq::util::ActiveMQProperties& getOptions() const {
            return options;
        }

        /**
         * @returns the cms::Destination interface pointer that the
         *          objects that derive from this class implement.
         */
        virtual const cms::Destination* getCMSDestination() const {
            return NULL;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATION_H_*/

