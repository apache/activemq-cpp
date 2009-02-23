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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGEBASE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGEBASE_H_

#include <activemq/connector/openwire/commands/Message.h>
#include <activemq/core/ActiveMQMessage.h>
#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h>
#include <activemq/connector/openwire/utils/MessagePropertyInterceptor.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <decaf/util/Date.h>
#include <activemq/util/PrimitiveMap.h>
#include <cms/DeliveryMode.h>
#include <activemq/util/Config.h>
#include <memory>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    /**
     * Base class for all Messages tagged with the ActiveMQXXXMessage
     * type.  The class implements the basic wrapper around the generated
     * Message class as well as satisfying the contract of the
     * core::ActiveMQMessage interface and the cms::Message method of which
     * T must be a derivation of.
     */
    template< typename T>
    class AMQCPP_API ActiveMQMessageBase : public T,
                                           public Message,
                                           public core::ActiveMQMessage {
    private:

        core::ActiveMQAckHandler* ackHandler;
        int redeliveryCount;
        util::PrimitiveMap properties;
        std::auto_ptr<utils::MessagePropertyInterceptor> propertiesInterceptor;
        bool readOnlyBody;
        bool readOnlyProperties;

    public:

        ActiveMQMessageBase() {
            this->ackHandler = NULL;
            this->redeliveryCount = 0;
            this->readOnlyBody = false;
            this->readOnlyProperties = false;
            this->propertiesInterceptor.reset(
                new utils::MessagePropertyInterceptor( this, &this->properties ) );
        }

        virtual ~ActiveMQMessageBase() {}

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            const ActiveMQMessageBase<T>* message =
                dynamic_cast< const ActiveMQMessageBase<T>* >( src );

            this->properties.copy( message->properties );
            this->setAckHandler( message->getAckHandler() );
            this->setRedeliveryCount( message->getRedeliveryCount() );
            this->setReadOnlyBody( message->isReadOnlyBody() );
            this->setReadOnlyProperties( message->isReadOnlyProperties() );

            openwire::commands::Message::copyDataStructure( src );
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
         * internal byte array before the object is marshalled to the
         * wire
         * @param wireFormat - the wireformatting controller
         */
        virtual void beforeMarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                marshalledProperties.clear();
                if( !properties.isEmpty() )
                {
                    marshal::PrimitiveMapMarshaller::marshal(
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
        virtual void afterUnmarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED )
            throw ( decaf::io::IOException ) {

            try{

                marshal::PrimitiveMapMarshaller::unmarshal(
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
            stream << " Value of redeliveryCount = " << this->redeliveryCount << std::endl;
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

            const ActiveMQMessageBase<T>* valuePtr =
                dynamic_cast< const ActiveMQMessageBase<T>* >( value );

            if( value == NULL || valuePtr == NULL ) {
                return false;
            }

            // Check this class's fields
            if( ackHandler != valuePtr->getAckHandler() ){
                return false;
            }
            if( redeliveryCount != valuePtr->getRedeliveryCount() ) {
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

    public:   // core::ActiveMQMessage

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
         * Gets the number of times this message has been redelivered.
         * @return redelivery count
         */
        virtual int getRedeliveryCount(void) const {
            return redeliveryCount;
        }

        /**
         * Sets the count of the number of times this message has been
         * redelivered
         * @param count the redelivery count
         */
        virtual void setRedeliveryCount( int count ) {
            this->redeliveryCount = count;
        }

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const {
            long long expireTime = this->getCMSExpiration();
            long long currentTime = decaf::util::Date::getCurrentTimeMilliseconds();
            if( expireTime > 0 && currentTime > expireTime ) {
                return true;
            }
            return false;
        }

        /**
         * Returns if the Message Body is Read Only.
         * @returns true if the Message Body is Read Only.
         */
        virtual bool isReadOnlyBody() const {
             return this->readOnlyBody;
        }

        /**
         * Sets the Read Only status of a Message Body
         * @param value - true if the Message Body is Read Only.
         */
        virtual void setReadOnlyBody( bool value ) {
            this->readOnlyBody = value;
        }

        /**
         * Returns if the Message Properties are Read Only.
         * @returns true if the Message properties are Read Only.
         */
        virtual bool isReadOnlyProperties() const {
            return this->readOnlyProperties;
       }

        /**
         * Sets the Read Only status of a Message's Properties
         * @param value - true if the Message Properties are Read Only.
         */
        virtual void setReadOnlyProperties( bool value )  {
            this->readOnlyProperties = value;
        }

    public:   // CMS Message

        /**
         * Acknowledges all consumed messages of the session
         * of this consumed message.
         */
        virtual void acknowledge() const throw( cms::CMSException ) {
            try{
                this->getAckHandler()->acknowledgeMessage( this );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() {
            this->readOnlyBody = false;
            this->setContent( std::vector<unsigned char>() );
        }

        /**
         * Clears the message properties.  Does not clear the body or
         * header values.
         */
        virtual void clearProperties() {
            this->readOnlyProperties = false;
            this->properties.clear();
        }

        /**
         * Retrieves the property names.
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const {
            return properties.getKeys();
        }

        /**
         * Indicates whether or not a given property exists.
         * @param name The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const {
            return properties.containsKey( name );
        }

        /**
         * Gets a boolean property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual bool getBooleanProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getBooleanProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a byte property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getByteProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a double property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual double getDoubleProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getDoubleProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a float property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual float getFloatProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getFloatProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a int property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual int getIntProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getIntProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a long property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual long long getLongProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getLongProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a short property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual short getShortProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getShortProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Gets a string property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual std::string getStringProperty( const std::string& name ) const
            throw( cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getStringProperty( name );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a boolean property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setBooleanProperty( const std::string& name,
                                         bool value )
                                            throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setBooleanProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a byte property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setByteProperty( const std::string& name,
                                      unsigned char value )
                                        throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setByteProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a double property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setDoubleProperty( const std::string& name,
                                        double value )
                                            throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setDoubleProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a float property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setFloatProperty( const std::string& name,
                                       float value )
                                        throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setFloatProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a int property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setIntProperty( const std::string& name,
                                     int value )
                                        throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setIntProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a long property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setLongProperty( const std::string& name,
                                      long long value )
                                        throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setLongProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a short property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setShortProperty( const std::string& name,
                                       short value )
                                        throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setShortProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets a string property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setStringProperty( const std::string& name,
                                        const std::string& value )
                                            throw( cms::CMSException ) {

            try{
                checkReadOnlyProperties();
                this->propertiesInterceptor->setStringProperty( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Get the Correlation Id for this message
         * @return string representation of the correlation Id
         */
        virtual std::string getCMSCorrelationID() const {
            return this->getCorrelationId();
        }

        /**
         * Sets the Correlation Id used by this message
         * @param correlationId - String representing the correlation id.
         */
        virtual void setCMSCorrelationID( const std::string& correlationId ) {
            this->setCorrelationId( correlationId );
        }

        /**
         * Gets the DeliveryMode for this message
         * @return DeliveryMode enumerated value.
         */
        virtual int getCMSDeliveryMode() const {
            return this->isPersistent();
        }

        /**
         * Sets the DeliveryMode for this message
         * @param mode - DeliveryMode enumerated value.
         */
        virtual void setCMSDeliveryMode( int mode ) {
            this->setPersistent( mode == (int)cms::DeliveryMode::PERSISTENT );
        }

        /**
         * Gets the Destination for this Message, returns a
         * @return Destination object
         */
        virtual const cms::Destination* getCMSDestination(void) const {
            return dynamic_cast<const cms::Destination*>(
                this->getDestination() );
        }

        /**
         * Sets the Destination for this message
         * @param destination - Destination Object
         */
        virtual void setCMSDestination( const cms::Destination* destination ) {

            delete getDestination();
            if( destination != NULL ) {
                this->setDestination(
                    dynamic_cast<ActiveMQDestination*>(
                        destination->clone() ) );
            } else {
                this->setDestination( NULL );
            }
        }

        /**
         * Gets the Expiration Time for this Message
         * @return time value
         */
        virtual long long getCMSExpiration() const {
            return this->getExpiration();
        }

        /**
         * Sets the Expiration Time for this message
         * @param expireTime - time value
         */
        virtual void setCMSExpiration( long long expireTime ) {
            this->setExpiration( expireTime );
        }

        /**
         * Gets the CMS Message Id for this Message
         * @return time value
         */
        virtual std::string getCMSMessageID() const {
            return marshal::BaseDataStreamMarshaller::toString( this->getMessageId() );
        }

        /**
         * Sets the CMS Message Id for this message
         * @param id - time value
         */
        virtual void setCMSMessageID( const std::string& id AMQCPP_UNUSED) {}

        /**
         * Gets the Priority Value for this Message
         * @return priority value
         */
        virtual int getCMSPriority() const {
            return this->getPriority();
        }

        /**
         * Sets the Priority Value for this message
         * @param priority - priority value for this message
         */
        virtual void setCMSPriority( int priority ) {
            this->setPriority( priority );
        }

        /**
         * Gets the Redelivered Flag for this Message
         * @return redelivered value
         */
        virtual bool getCMSRedelivered() const {
            return this->getRedeliveryCounter() != 0;
        }

        /**
         * Sets the Redelivered Flag for this message
         * @param redelivered - boolean redelivered value
         */
        virtual void setCMSRedelivered( bool redelivered AMQCPP_UNUSED) {}

        /**
         * Gets the CMS Reply To Address for this Message
         * @return Reply To Value
         */
        virtual const cms::Destination* getCMSReplyTo(void) const {
            return dynamic_cast< const cms::Destination* >(
                this->getReplyTo() );
        }

        /**
         * Sets the CMS Reply To Address for this message
         * @param id - Reply To value
         */
        virtual void setCMSReplyTo( const cms::Destination* destination ) {

            delete getReplyTo();
            if( destination != NULL ) {
                this->setReplyTo(
                    dynamic_cast<ActiveMQDestination*>(
                        destination->clone() ) );
            } else {
                this->setReplyTo( NULL );
            }
        }

        /**
         * Gets the Time Stamp for this Message
         * @return time stamp value
         */
        virtual long long getCMSTimestamp() const {
            return this->getTimestamp();
        }

        /**
         * Sets the Time Stamp for this message
         * @param timeStamp - integer time stamp value
         */
        virtual void setCMSTimestamp( long long timeStamp ) {
            this->setTimestamp( timeStamp );
        }

        /**
         * Gets the CMS Message Type for this Message
         * @return type value
         */
        virtual std::string getCMSType() const {
            return this->getType();
        }

        /**
         * Sets the CMS Message Type for this message
         * @param type - message type value string
         */
        virtual void setCMSType( const std::string& type ) {
            this->setType( type );
        }

    protected:

        void checkReadOnlyBody() {
            if( this->isReadOnlyBody() ) {
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Message Body is in Read Only Mode." );
            }
        }

        void checkReadOnlyProperties() {
            if( this->isReadOnlyProperties() ) {
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Message Properties are in Read Only Mode." );
            }
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMESSAGEBASE_H_*/
