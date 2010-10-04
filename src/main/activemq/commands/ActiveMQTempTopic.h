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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPTOPIC_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPTOPIC_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <cms/TemporaryTopic.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQTempTopic : public ActiveMQTempDestination,
                                         public cms::TemporaryTopic {
    public:

        const static unsigned char ID_ACTIVEMQTEMPTOPIC = 103;

    private:

        ActiveMQTempTopic( const ActiveMQTempTopic& );
        ActiveMQTempTopic& operator= ( const ActiveMQTempTopic& );

    public:

        ActiveMQTempTopic();
        ActiveMQTempTopic( const std::string& name );
        virtual ~ActiveMQTempTopic() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQTempTopic* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual const cms::Destination* getCMSDestination() const {
            return this;
        }

    public:   // CMS Destination

        virtual cms::Destination::DestinationType getDestinationType() const {
            return cms::Destination::TEMPORARY_TOPIC;
        }

        virtual cms::Destination* clone() const {
            return dynamic_cast<cms::Destination*>(
                this->cloneDataStructure() );
        }

        virtual void copy( const cms::Destination& source ) {
            this->copyDataStructure(
                dynamic_cast<const DataStructure*>( &source ) );
        }

        virtual const cms::CMSProperties& getCMSProperties() const {
            return this->getOptions();
        }

        virtual bool equals( const cms::Destination& other ) const;

    public:  // CMS Temporary Topic

        virtual std::string getTopicName() const {
            return this->getPhysicalName();
        }

        virtual void destroy();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQTEMPTOPIC_H_*/
