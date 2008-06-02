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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPTOPIC_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPTOPIC_H_

#include <activemq/util/Config.h>
#include <activemq/connector/stomp/StompDestination.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <cms/Topic.h>

namespace activemq{
namespace connector{
namespace stomp{
    
    class AMQCPP_API StompTopic : public StompDestination<cms::Topic> {
    public:

        /**
         * Copy Consturctor
         * @param CMS Dest to Copy, must be a compatible type
         */
        StompTopic( const cms::Destination* source ) : 
            StompDestination< cms::Topic >( source ) {}

        /**
         * Custom Constructor
         * @param name string destination name plus any params
         */
        StompTopic( const std::string& name ) : 
            StompDestination< cms::Topic >( name, cms::Destination::TOPIC )
        {}

        virtual ~StompTopic() {}

        /**
         * Gets the name of this queue.
         * @return The queue name.
         */
        virtual std::string getTopicName() const 
            throw( cms::CMSException ) {
                return getName();
        }

        /**
         * Creates a new instance of this destination type that is a
         * copy of this one, and returns it.
         * @returns cloned copy of this object
         */
        virtual cms::Destination* clone() const {
            return new StompTopic( this );
        }

    protected:

        /**
         * Retrieves the proper Stomp Prefix for the specified type
         * of Destination
         * @return string prefix
         */
        virtual std::string getPrefix() const {
            return commands::CommandConstants::topicPrefix;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPTOPIC_H_*/
