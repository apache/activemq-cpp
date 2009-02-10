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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/commands/ActiveMQMessageBase.h>
#include <cms/TextMessage.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class AMQCPP_API ActiveMQTextMessage :
        public ActiveMQMessageBase<cms::TextMessage> {
    public:

        const static unsigned char ID_ACTIVEMQTEXTMESSAGE = 28;

    public:

        ActiveMQTextMessage();
        virtual ~ActiveMQTextMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQTextMessage* cloneDataStructure() const {
            std::auto_ptr<ActiveMQTextMessage> message( new ActiveMQTextMessage() );
            message->copyDataStructure( this );
            return message.release();
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessageBase<cms::TextMessage>::copyDataStructure( src );
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const {
            std::ostringstream stream;

            stream << "Begin Class = ActiveMQTextMessage" << std::endl;
            stream << ActiveMQMessageBase<cms::TextMessage>::toString();
            stream << "End Class = ActiveMQTextMessage" << std::endl;

            return stream.str();
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {
            return ActiveMQMessageBase<cms::TextMessage>::equals( value );
        }

    public:   // CMS Message

        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::TextMessage* clone(void) const {
            return dynamic_cast<cms::TextMessage*>(
                this->cloneDataStructure() );
        }

    public:   // cms::TextMessage

        /**
         * Gets the message character buffer.
         * @return The message character buffer.
         */
        virtual std::string getText() const throw( cms::CMSException );

        /**
         * Sets the message contents, does not take ownership of the passed
         * char*, but copies it instead.
         * @param msg The message buffer.
         */
        virtual void setText( const char* msg ) throw( cms::CMSException );

        /**
         * Sets the message contents
         * @param msg The message buffer.
         */
        virtual void setText( const std::string& msg ) throw( cms::CMSException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_*/

