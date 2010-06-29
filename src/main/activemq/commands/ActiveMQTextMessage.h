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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQTEXTMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQTEXTMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>
#include <cms/TextMessage.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQTextMessage :
        public ActiveMQMessageTemplate<cms::TextMessage> {
    public:

        const static unsigned char ID_ACTIVEMQTEXTMESSAGE = 28;

        mutable std::auto_ptr<std::string> text;

    private:

        ActiveMQTextMessage( const ActiveMQTextMessage& );
        ActiveMQTextMessage& operator= ( const ActiveMQTextMessage& );

    public:

        ActiveMQTextMessage();
        virtual ~ActiveMQTextMessage() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQTextMessage* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual void clearBody();

        virtual void beforeMarshal( wireformat::WireFormat* wireFormat );

        virtual unsigned int getSize() const;

    public:   // CMS Message

        virtual cms::TextMessage* clone() const {
            return dynamic_cast<cms::TextMessage*>( this->cloneDataStructure() );
        }

    public:   // cms::TextMessage

        virtual std::string getText() const;

        virtual void setText( const char* msg );

        virtual void setText( const std::string& msg );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQTEXTMESSAGE_H_*/
