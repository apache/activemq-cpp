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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGEBASE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGEBASE_H_

#include <activemq/commands/Message.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/util/Config.h>
#include <activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/wireformat/openwire/marshal/PrimitiveMapMarshaller.h>
#include <cms/DeliveryMode.h>
#include <decaf/util/Date.h>
#include <memory>

namespace activemq{
namespace commands{

    /**
     * Base class for all Messages tagged with the ActiveMQXXXMessage
     * type.  The class implements the basic wrapper around the generated
     * Message class as well as satisfying the contract of the
     * core::ActiveMQMessage interface and the cms::Message method of which
     * T must be a derivation of.
     */
    class AMQCPP_API ActiveMQMessageBase : public Message {
    private:

        core::ActiveMQAckHandler* ackHandler;

    protected:  // TODO - Ugly Hack, fix this, protected data is a no-no.

        activemq::util::PrimitiveMap properties;

    public:

        ActiveMQMessageBase() : ackHandler( NULL ) {}

        virtual ~ActiveMQMessageBase() {}

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            const ActiveMQMessageBase* message =
                dynamic_cast< const ActiveMQMessageBase* >( src );

            this->properties.copy( message->properties );
            this->setAckHandler( message->getAckHandler() );

            commands::Message::copyDataStructure( src );
        }

        /**
         * Indicates that this command is aware of Marshaling, and needs
         * to have its Marshaling methods invoked.
         * @returns boolean indicating desire to be in marshaling stages
         */
        virtual bool isMarshalAware() const {
            return true;
        }

        /**
         * Handles the marshaling of the objects properties into the
         * internal byte array before the object is marshaled to the
         * wire
         * @param wireFormat - the wireformat controller
         */
        virtual void beforeMarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                marshalledProperties.clear();
                if( !properties.isEmpty() )
                {
                    wireformat::openwire::marshal::PrimitiveMapMarshaller::marshal(
                        &properties, marshalledProperties );
                }
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

        /**
         * Called after unmarshaling is started to cleanup the object being
         * unmarshaled.
         * @param wireFormat - the wireformat object to control unmarshaling
         */
        virtual void afterUnmarshal( wireformat::WireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                wireformat::openwire::marshal::PrimitiveMapMarshaller::unmarshal(
                    &properties, marshalledProperties );
            }
            AMQ_CATCH_RETHROW( decaf::io::IOException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, decaf::io::IOException )
            AMQ_CATCHALL_THROW( decaf::io::IOException )
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const {
            std::ostringstream stream;

            stream << "Begin Class = ActiveMQMessageBase" << std::endl;
            stream << " Value of ackHandler = " << ackHandler << std::endl;
            stream << " Value of properties = " << this->properties.toString() << std::endl;
            stream << commands::Message::toString();
            stream << "End Class = ActiveMQMessageBase" << std::endl;

            return stream.str();
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {

            const ActiveMQMessageBase* valuePtr =
                dynamic_cast< const ActiveMQMessageBase* >( value );

            if( value == NULL || valuePtr == NULL ) {
                return false;
            }

            // Check this class's fields
            if( ackHandler != valuePtr->getAckHandler() ){
                return false;
            }
            if( !properties.equals( valuePtr->properties ) ) {
                return false;
            }

            // Now check the base class
            if( !commands::Message::equals( value ) ) {
                return false;
            }

            return true;
        }

        /**
         * Sets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @param handler ActiveMQAckHandler to call
         */
        virtual void setAckHandler( core::ActiveMQAckHandler* handler ) {
            this->ackHandler = handler;
        }

        /**
         * Gets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @returns handler ActiveMQAckHandler to call or NULL if not set
         */
        virtual core::ActiveMQAckHandler* getAckHandler() const {
            return this->ackHandler;
        }

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const {
            long long expireTime = this->getExpiration();
            long long currentTime = decaf::util::Date::getCurrentTimeMilliseconds();
            if( expireTime > 0 && currentTime > expireTime ) {
                return true;
            }
            return false;
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGEBASE_H_*/
