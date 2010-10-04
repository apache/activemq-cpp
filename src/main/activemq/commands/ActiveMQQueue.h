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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQQUEUE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQQUEUE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Exception.h>
#include <cms/Queue.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQQueue : public ActiveMQDestination,
                                     public cms::Queue {
    public:

        const static unsigned char ID_ACTIVEMQQUEUE = 100;

    private:

        ActiveMQQueue( const ActiveMQQueue& );
        ActiveMQQueue& operator= ( const ActiveMQQueue& );

    public:

        ActiveMQQueue();

        ActiveMQQueue( const std::string& name );

        virtual ~ActiveMQQueue() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQQueue* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual const cms::Destination* getCMSDestination() const {
            return this;
        }

    public:   // CMS Destination

        virtual cms::Destination::DestinationType getDestinationType() const {
            return cms::Destination::QUEUE;
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

    public:  // CMS Queue

        virtual std::string getQueueName() const {
            return this->getPhysicalName();
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQQUEUE_H_*/
