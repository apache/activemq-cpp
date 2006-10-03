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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQQUEUE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQQUEUE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQDestination.h>
#include <cms/Queue.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQQueue : public ActiveMQDestination,
                          public cms::Queue
    {
    public:

        const static unsigned char ID_ACTIVEMQQUEUE = 100;

    public:

        ActiveMQQueue();
        virtual ~ActiveMQQueue();

        virtual unsigned char getDataStructureType() const;

    public:   // CMS Destination

        /**
         * Retrieve the Destination Type for this Destination
         * @return The Destination Type
         */
        virtual cms::Destination::DestinationType getDestinationType(void) const;
        
        /**
         * Converts the Destination Name into a String 
         * @return string name
         */
        virtual std::string toString(void) const;

        /**
         * Converts the Destination to a String value representing the
         * Provider specific name fot this destination, which is not
         * necessarily equal to the User Supplied name of the Destination
         * @return Provider specific Name
         */
        virtual std::string toProviderString(void) const;
        
        /**
         * Creates a new instance of this destination type that is a
         * copy of this one, and returns it.
         * @returns cloned copy of this object
         */
        virtual cms::Destination* clone(void) const;
      
        /**
         * Copies the contents of the given Destinastion object to this one.
         * @param source The source Destination object.
         */
        virtual void copy( const cms::Destination& source );

        /**
         * Retrieve any properties that might be part of the destination
         * that was specified.  This is a deviation from the JMS spec
         * but necessary due to C++ restrictions.  
         * @return const reference to a properties object.
         */
        virtual const activemq::util::Properties& getProperties(void) const;

    public:  // CMS Queue

        /**
         * Gets the name of this queue.
         * @return The queue name.
         */
        virtual std::string getQueueName() const 
            throw( cms::CMSException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQQUEUE_H_*/

