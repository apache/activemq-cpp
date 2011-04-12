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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <cms/Message.h>
#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQMessage :
        public ActiveMQMessageTemplate<cms::Message> {

    public:

        static const unsigned char ID_ACTIVEMQMESSAGE = 23;

    private:

        ActiveMQMessage( const ActiveMQMessage& );
        ActiveMQMessage& operator= ( const ActiveMQMessage& );

    public:

        ActiveMQMessage();

        virtual ~ActiveMQMessage() throw() {}

        virtual unsigned char getDataStructureType() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual ActiveMQMessage* cloneDataStructure() const;

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

    public:  // cms::Message

        virtual cms::Message* clone() const {
            return dynamic_cast<cms::Message*>( this->cloneDataStructure() );
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGE_H_*/
