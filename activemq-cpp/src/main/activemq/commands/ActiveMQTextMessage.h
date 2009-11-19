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

    public:

        ActiveMQTextMessage();
        virtual ~ActiveMQTextMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQTextMessage* cloneDataStructure() const;

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const;

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const;

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() throw( cms::CMSException );

        /**
         * Performs any cleanup or other tasks that must be done before the Message is
         * marshalled to its binary WireFormat version.
         *
         * @param wireFormat the WireFormat instance that is marshalling this message.
         */
        virtual void beforeMarshal( wireformat::WireFormat* wireFormat )
            throw ( decaf::io::IOException );

        /**
         * Returns the Size of this message in Bytes.
         * @returns number of bytes this message equates to.
         */
        virtual unsigned int getSize() const;

    public:   // CMS Message

        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::TextMessage* clone() const {
            return dynamic_cast<cms::TextMessage*>( this->cloneDataStructure() );
        }

    public:   // cms::TextMessage

        /**
         * Gets the message character buffer.
         *
         * @return The message character buffer.
         *
         * @throws CMSException - if an internal error occurs.
         */
        virtual std::string getText() const throw( cms::CMSException );

        /**
         * Sets the message contents, does not take ownership of the passed
         * char*, but copies it instead.
         *
         * @param msg
         *      The message buffer.
         *
         * @throws CMSException - if an internal error occurs.
         * @throws MessageNotWriteableException - if the message is in read-only mode..
         */
        virtual void setText( const char* msg ) throw( cms::MessageNotWriteableException,
                                                       cms::CMSException );

        /**
         * Sets the message contents
         *
         * @param msg
         *      The message buffer.
         *
         * @throws CMSException - if an internal error occurs.
         * @throws MessageNotWriteableException - if the message is in read-only mode..
         */
        virtual void setText( const std::string& msg ) throw( cms::MessageNotWriteableException,
                                                              cms::CMSException );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQTEXTMESSAGE_H_*/
