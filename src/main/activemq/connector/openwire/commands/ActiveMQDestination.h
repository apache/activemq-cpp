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

#include <activemq/connector/openwire/commands/BaseDataStructure.h>

#include <vector>
#include <string>
#include <map>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQDestination : public BaseDataStructure
    {
    protected:

        enum DESTINATION_TYPE_IDS {
            ACTIVEMQ_TOPIC = 1,
            ACTIVEMQ_TEMPORARY_TOPIC = 2,
            ACTIVEMQ_QUEUE = 3,
            ACTIVEMQ_TEMPORARY_QUEUE = 4
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
        std::map<std::string, std::string> options;

    public:

        const static unsigned char ID_ACTIVEMQDESTINATION = 0;

    public:

        ActiveMQDestination();
        virtual ~ActiveMQDestination();

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        virtual unsigned char getDataStructureType() const;

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
        virtual void setPhysicalName( const std::string& physicalName ){
            this->physicalName = physicalName;
        }

        /**
         * @return Returns the advisory.
         */
        virtual bool isAdvisory() {
            return advisory;
        }
        
        /**
         * @param advisory The advisory to set.
         */
        void setAdvisory( bool advisory ){
            this->advisory = advisory;
        }
        
        /**
         * @return true if this is a destination for Consumer advisories
         */
        bool isConsumerAdvisory() {
            return isAdvisory() && 
                   physicalName.find(CONSUMER_ADVISORY_PREFIX) == 0;
        }
        
        /**
         * @return true if this is a destination for Producer advisories
         */
        bool isProducerAdvisory() {
            return isAdvisory() && 
                   physicalName.find(PRODUCER_ADVISORY_PREFIX) == 0;
        }
        
        /**
         * @return true if this is a destination for Connection advisories
         */
        bool isConnectionAdvisory() {
            return isAdvisory() && 
                   physicalName.find(CONNECTION_ADVISORY_PREFIX) == 0;
        }
        
        /**
         * @return Returns the exclusive.
         */
        bool isExclusive() {
            return exclusive;
        }
        
        /**
         * @param exclusive The exclusive to set.
         */
        void setExclusive( bool exclusive ) {
            this->exclusive = exclusive;
        }
        
        /**
         * @return Returns the ordered.
         */
        bool isOrdered() {
            return ordered;
        }

        /**
         * @param ordered The ordered to set.
         */
        void setOrdered( bool ordered ) {
            this->ordered = ordered;
        }

        /**
         * @return Returns the orderedTarget.
         */
        std::string getOrderedTarget() {
            return orderedTarget;
        }

        /**
         * @param orderedTarget The orderedTarget to set.
         */
        void setOrderedTarget( const std::string& orderedTarget) {
            this->orderedTarget = orderedTarget;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATION_H_*/

