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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQDESTINATION_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQDESTINATION_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/BaseDataStructure.h>
#include <activemq/util/ActiveMQProperties.h>
#include <cms/Destination.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Comparable.h>
#include <decaf/util/ArrayList.h>

#include <vector>
#include <string>
#include <map>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    class AMQCPP_API ActiveMQDestination : public BaseDataStructure,
                                           public decaf::lang::Comparable<ActiveMQDestination> {
    protected:

        struct DestinationFilter{
            static const std::string ANY_CHILD;
            static const std::string ANY_DESCENDENT;
        };

        /**
         * The default target for ordered destinations
         */
        static const std::string DEFAULT_ORDERED_TARGET;

        static const std::string TEMP_PREFIX;
        static const std::string TEMP_POSTFIX;
        static const std::string COMPOSITE_SEPARATOR;

        static const std::string QUEUE_QUALIFIED_PREFIX;
        static const std::string TOPIC_QUALIFIED_PREFIX;
        static const std::string TEMP_QUEUE_QUALIFED_PREFIX;
        static const std::string TEMP_TOPIC_QUALIFED_PREFIX;

        // Cached transient data
        bool exclusive;
        bool ordered;
        bool advisory;
        mutable decaf::util::ArrayList< Pointer<ActiveMQDestination> > compositeDestinations;
        std::string orderedTarget;

        std::string physicalName;
        util::ActiveMQProperties options;

    public:

        const static unsigned char ID_ACTIVEMQDESTINATION = 0;

        typedef decaf::lang::PointerComparator<ActiveMQDestination> COMPARATOR;

    private:

        ActiveMQDestination(const ActiveMQDestination&);
        ActiveMQDestination& operator=(const ActiveMQDestination&);

    public:

        ActiveMQDestination();

        ActiveMQDestination(const std::string& physicalName);

        virtual ~ActiveMQDestination() throw();

        virtual ActiveMQDestination* cloneDataStructure() const {
            return NULL;
        }

        virtual void copyDataStructure(const DataStructure* src);

        virtual bool equals(const DataStructure* value) const;

        virtual unsigned char getDataStructureType() const;

        virtual std::string toString() const;

    public:

        /**
         * Fetch this destination's physical name
         * @returns const string containing the name
         */
        virtual std::string getPhysicalName() const {
            return this->physicalName;
        }

        /**
         * Set this destination's physical name
         * @returns const string containing the name
         */
        virtual void setPhysicalName(const std::string& physicalName);

        /**
         * @return Returns the advisory.
         */
        virtual bool isAdvisory() const {
            return advisory;
        }

        /**
         * @param advisory The advisory to set.
         */
        virtual void setAdvisory(bool advisory) {
            this->advisory = advisory;
        }

//        /**
//         * @return true if this is a destination for Consumer advisories
//         */
//        virtual bool isConsumerAdvisory() const {
//            return isAdvisory() && physicalName.find(CONSUMER_ADVISORY_PREFIX) == 0;
//        }
//
//        /**
//         * @return true if this is a destination for Producer advisories
//         */
//        virtual bool isProducerAdvisory() const {
//            return isAdvisory() && physicalName.find(PRODUCER_ADVISORY_PREFIX) == 0;
//        }
//
//        /**
//         * @return true if this is a destination for Connection advisories
//         */
//        virtual bool isConnectionAdvisory() const {
//            return isAdvisory() && physicalName.find(CONNECTION_ADVISORY_PREFIX) == 0;
//        }

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
        virtual void setOrdered(bool ordered) {
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
        virtual void setOrderedTarget(const std::string& orderedTarget) {
            this->orderedTarget = orderedTarget;
        }

        /**
         * Returns the Type of Destination that this object represents
         * @returns int type qualifier.
         */
        virtual cms::Destination::DestinationType getDestinationType() const = 0;

        /**
         * Returns the type of Destination that this object represents as a string, the
         * available string values are, "Queue", "Topic", "TempQueue" and "TempTopic".
         *
         * @returns The string value that represents the type of this destination.
         */
        std::string getDestinationTypeAsString() const;

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
         *
         * @return true if this destination represents a collection of child
         *         destinations.
         */
        virtual bool isComposite() const {
            return physicalName.find_first_of(COMPOSITE_SEPARATOR) != std::string::npos;
        }

        /**
         * Returns an ArrayList containing all the ActiveMQDestinations that comprise this
         * Composite destination, if this destination is composite, otherwise it returns an
         * empty list.
         */
        decaf::util::ArrayList< Pointer<ActiveMQDestination> > getCompositeDestinations() const;

        /**
         * @return true if the destination matches multiple possible destinations
         */
        virtual bool isWildcard() const {
            return physicalName.find_first_of(DestinationFilter::ANY_CHILD) != std::string::npos||
                   physicalName.find_first_of(DestinationFilter::ANY_DESCENDENT) != std::string::npos;
        }

        /**
         * @returns a reference (const) to the options properties for this Destination.
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

        /**
         * Create a new Destination that's of the same type as this one but with
         * the given destination name.
         *
         * @param name
         *      The name
         *
         * @return Pointer to a new ActiveMQDestination.
         */
        Pointer<ActiveMQDestination> createDestination(const std::string& name) const {
            return ActiveMQDestination::createDestination(getDestinationType(), name);
        }

        virtual int compareTo(const ActiveMQDestination& value) const;

        virtual bool equals(const ActiveMQDestination& value) const;

        virtual bool operator==(const ActiveMQDestination& value) const;

        virtual bool operator<(const ActiveMQDestination& value) const;

    public:  // Statics

        /**
         * Create a temporary name from the clientId
         *
         * @param clientId
         * @return
         */
        static std::string createTemporaryName(const std::string& clientId) {
            return TEMP_PREFIX + clientId + TEMP_POSTFIX;
        }

        /**
         * From a temporary destination find the clientId of the Connection that created it
         *
         * @param destination
         * @return the clientId or null if not a temporary destination
         */
        static std::string getClientId(const ActiveMQDestination* destination);

        /**
         * Creates a Destination given the String Name to use and a Type.
         *
         * @param type - The Type of Destination to Create
         * @param name - The Name to use in the creation of the Destination
         *
         * @return Pointer to a new ActiveMQDestination instance.
         */
        static Pointer<ActiveMQDestination> createDestination(int type, const std::string& name);

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQDESTINATION_H_*/
