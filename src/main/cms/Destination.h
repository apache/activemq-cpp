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

#ifndef _CMS_DESTINATION_H_
#define _CMS_DESTINATION_H_

#include <cms/CMSProperties.h>
#include <cms/Config.h>

#include <string>

namespace cms{
	
    /**
     * A Destination object encapsulates a provider-specific address. 
     */
    class CMS_API Destination{
    public:
   
        enum DestinationType
        {
            TOPIC,
            QUEUE,
            TEMPORARY_TOPIC,
            TEMPORARY_QUEUE
        };
		
	public:

        /**
         * Destructor
         */	
        virtual ~Destination(){}
      
        /**
         * Retrieve the Destination Type for this Destination
         * 
         * @return The Destination Type
         */
        virtual DestinationType getDestinationType() const = 0;

        /**
         * Converts the Destination to a String value representing the
         * Provider specific name for this destination.  This name must
         * uniquely identify a particular destination.  For example, a topic
         * and a queue both named "FOO" must not have equivalent provider
         * strings.
         * 
         * @return Provider specific Name
         */
        virtual std::string toProviderString() const = 0;
        
        /**
         * Creates a new instance of this destination type that is a
         * copy of this one, and returns it.
         * 
         * @returns cloned copy of this object
         */
        virtual cms::Destination* clone() const = 0;
      
        /**
         * Copies the contents of the given Destinastion object to this one.
         * 
         * @param source 
         *      The source Destination object.
         */
        virtual void copy( const cms::Destination& source ) = 0;

        /**
         * Retrieve any properties that might be part of the destination
         * that was specified.  This is a deviation from the JMS spec
         * but necessary due to C++ restrictions.
         * 
         * @return const reference to a properties object.
         */
        virtual const CMSProperties& getCMSProperties() const = 0;
        
    };
}

#endif /*_CMS_DESTINATION_H_*/
