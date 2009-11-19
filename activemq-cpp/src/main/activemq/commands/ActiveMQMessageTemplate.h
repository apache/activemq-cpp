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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEMPLATE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEMPLATE_H_

#include <cms/DeliveryMode.h>
#include <activemq/util/Config.h>
#include <activemq/commands/Message.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/wireformat/openwire/utils/MessagePropertyInterceptor.h>
#include <activemq/wireformat/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <cms/IllegalStateException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>

namespace activemq {
namespace commands {

    template< typename T >
    class AMQCPP_API ActiveMQMessageTemplate : public T, public Message {
    private:

        std::auto_ptr<wireformat::openwire::utils::MessagePropertyInterceptor> propertiesInterceptor;

    public:

        ActiveMQMessageTemplate() : commands::Message() {
            this->propertiesInterceptor.reset(
                new wireformat::openwire::utils::MessagePropertyInterceptor(
                    this, &this->getMessageProperties() ) );
        }

        virtual ~ActiveMQMessageTemplate() {}

    public:  // cms::Message related methods

        /**
         * Acknowledges all consumed messages of the session
         * of this consumed message.
         */
        virtual void acknowledge() const throw( cms::IllegalStateException, cms::CMSException ) {
            try{
                this->getAckHandler()->acknowledgeMessage( this );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Resets the Message to a Read-Only state.
         */
        virtual void onSend() {
            this->setReadOnlyBody(true);
            this->setReadOnlyProperties(true);
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {
            try{

                if( this == value ) {
                    return true;
                }

                const ActiveMQMessageTemplate<T>* object =
                    dynamic_cast< const ActiveMQMessageTemplate<T>* >( value );

                if( object == NULL ) {
                    return false;
                }

                decaf::lang::Pointer<MessageId> thisMsgId = this->getMessageId();
                decaf::lang::Pointer<MessageId> otherMsgId = object->getMessageId();

                return thisMsgId != NULL && otherMsgId != NULL && otherMsgId->equals( thisMsgId.get() );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() throw( cms::CMSException ) {
            try{
                this->setContent( std::vector<unsigned char>() );
                this->setReadOnlyBody( false );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Clears the message properties.  Does not clear the body or
         * header values.
         */
        virtual void clearProperties() throw( cms::CMSException ) {
            try{
                this->getMessageProperties().clear();
                this->setReadOnlyProperties( false );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Retrieves the property names.
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const throw( cms::CMSException ) {
            try{
                return getMessageProperties().keySet();
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Indicates whether or not a given property exists.
         * @param name The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const throw( cms::CMSException ) {
            try{
                return getMessageProperties().containsKey( name );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a boolean property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual bool getBooleanProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getBooleanProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a byte property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getByteProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a double property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual double getDoubleProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getDoubleProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a float property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual float getFloatProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getFloatProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a int property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual int getIntProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getIntProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a long property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual long long getLongProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getLongProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a short property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual short getShortProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getShortProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets a string property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @return The value for the named property.
         *
         * @throws CMSException if the property does not exist.
         * @throws MessageFormatException - if this type conversion is invalid.
         */
        virtual std::string getStringProperty( const std::string& name ) const
            throw( cms::MessageFormatException, cms::CMSException ) {

            try{
                return this->propertiesInterceptor->getStringProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a boolean property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setBooleanProperty( const std::string& name, bool value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setBooleanProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a byte property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setByteProperty( const std::string& name, unsigned char value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setByteProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a double property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setDoubleProperty( const std::string& name, double value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setDoubleProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a float property.
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setFloatProperty( const std::string& name, float value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setFloatProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a int property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setIntProperty( const std::string& name, int value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setIntProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a long property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setLongProperty( const std::string& name, long long value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setLongProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a short property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setShortProperty( const std::string& name, short value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setShortProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets a string property.
         *
         * @param name
         *      The name of the property to retrieve.
         * @param value
         *      The value for the named property.
         *
         * @throws CMSException - if the name is an empty string.
         * @throws MessageNotWriteableException - if properties are read-only
         */
        virtual void setStringProperty( const std::string& name, const std::string& value )
            throw( cms::MessageNotWriteableException, cms::CMSException ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setStringProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Get the Correlation Id for this message
         * @return string representation of the correlation Id
         * @throw CMSException
         */
        virtual std::string getCMSCorrelationID() const throw( cms::CMSException ) {
            return this->getCorrelationId();
        }

        /**
         * Sets the Correlation Id used by this message
         * @param correlationId - String representing the correlation id.
         * @throw CMSException
         */
        virtual void setCMSCorrelationID( const std::string& correlationId ) throw( cms::CMSException ) {
            this->setCorrelationId( correlationId );
        }

        /**
         * Gets the DeliveryMode for this message
         * @return DeliveryMode enumerated value.
         * @throw CMSException
         */
        virtual int getCMSDeliveryMode() const throw( cms::CMSException ) {
            return !this->isPersistent();
        }

        /**
         * Sets the DeliveryMode for this message
         * @param mode - DeliveryMode enumerated value.
         * @throw CMSException
         */
        virtual void setCMSDeliveryMode( int mode ) throw( cms::CMSException ) {
            this->setPersistent( mode == (int)cms::DeliveryMode::PERSISTENT );
        }

        /**
         * Gets the Destination for this Message, returns a
         * @return Destination object
         * @throw CMSException
         */
        virtual const cms::Destination* getCMSDestination() const throw( cms::CMSException ) {
            return dynamic_cast<const cms::Destination*>( this->getDestination().get() );
        }

        /**
         * Sets the Destination for this message
         * @param destination - Destination Object
         * @throw CMSException
         */
        virtual void setCMSDestination( const cms::Destination* destination )
            throw( cms::CMSException ) {

            try{
                if( destination != NULL ) {
                    this->setDestination(
                        decaf::lang::Pointer<ActiveMQDestination>(
                            dynamic_cast<ActiveMQDestination*>( destination->clone() ) ) );
                } else {
                    this->getDestination().reset( NULL );
                }
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets the Expiration Time for this Message
         * @return time value
         * @throw CMSException
         */
        virtual long long getCMSExpiration() const throw( cms::CMSException ) {
            return this->getExpiration();
        }

        /**
         * Sets the Expiration Time for this message
         * @param expireTime - time value
         * @throw CMSException
         */
        virtual void setCMSExpiration( long long expireTime ) throw( cms::CMSException ) {
            this->setExpiration( expireTime );
        }

        /**
         * Gets the CMS Message Id for this Message
         * @return time value
         * @throw CMSException
         */
        virtual std::string getCMSMessageID() const throw( cms::CMSException ) {
            try{
                return wireformat::openwire::marshal::BaseDataStreamMarshaller::toString( this->getMessageId().get() );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Sets the CMS Message Id for this message
         * @param id - time value
         * @throw CMSException
         */
        virtual void setCMSMessageID( const std::string& id AMQCPP_UNUSED )
            throw( cms::CMSException ) {
        }

        /**
         * Gets the Priority Value for this Message
         * @return priority value
         * @throw CMSException
         */
        virtual int getCMSPriority() const throw( cms::CMSException ) {
            return this->getPriority();
        }

        /**
         * Sets the Priority Value for this message
         * @param priority - priority value for this message
         * @throw CMSException
         */
        virtual void setCMSPriority( int priority ) throw( cms::CMSException ) {
            this->setPriority( priority );
        }

        /**
         * Gets the Redelivered Flag for this Message
         * @return redelivered value
         * @throw CMSException
         */
        virtual bool getCMSRedelivered() const throw( cms::CMSException ) {
            return this->getRedeliveryCounter() != 0;
        }

        /**
         * Sets the Redelivered Flag for this message
         * @param redelivered - boolean redelivered value
         * @throw CMSException
         */
        virtual void setCMSRedelivered( bool redelivered AMQCPP_UNUSED ) throw( cms::CMSException )  {}

        /**
         * Gets the CMS Reply To Address for this Message
         * @return Reply To Value
         * @throw CMSException
         */
        virtual const cms::Destination* getCMSReplyTo() const throw( cms::CMSException ) {
            return dynamic_cast< const cms::Destination* >( this->getReplyTo().get() );
        }

        /**
         * Sets the CMS Reply To Address for this message
         *
         * @param destination
         *      Pointer to the CMS Destination that is the Reply-To value.
         *
         * @throw CMSException
         */
        virtual void setCMSReplyTo( const cms::Destination* destination ) throw( cms::CMSException ) {

            try{
                if( destination != NULL ) {
                    this->setReplyTo(
                        decaf::lang::Pointer<ActiveMQDestination>(
                            dynamic_cast<ActiveMQDestination*>( destination->clone() ) ) );
                } else {
                    this->setReplyTo( decaf::lang::Pointer<ActiveMQDestination>() );
                }
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Gets the Time Stamp for this Message
         * @return time stamp value
         * @throw CMSException
         */
        virtual long long getCMSTimestamp() const throw( cms::CMSException ) {
            return this->getTimestamp();
        }

        /**
         * Sets the Time Stamp for this message
         * @param timeStamp - integer time stamp value
         * @throw CMSException
         */
        virtual void setCMSTimestamp( long long timeStamp ) throw( cms::CMSException ) {
            this->setTimestamp( timeStamp );
        }

        /**
         * Gets the CMS Message Type for this Message
         * @return type value
         * @throw CMSException
         */
        virtual std::string getCMSType() const throw( cms::CMSException ) {
            return this->getType();
        }

        /**
         * Sets the CMS Message Type for this message
         * @param type - message type value string
         * @throw CMSException
         */
        virtual void setCMSType( const std::string& type ) throw( cms::CMSException ) {
            this->setType( type );
        }

    protected:

        void failIfWriteOnlyBody() const {
            if( !this->isReadOnlyBody() ){
                throw cms::MessageNotReadableException(
                    "message is in write-only mode and cannot be read from", NULL );
            }
        }

        void failIfReadOnlyBody() const {
            if( this->isReadOnlyBody() ) {
                throw cms::MessageNotWriteableException(
                    "Message Body is Read-Only.", NULL );
            }
        }

        void failIfReadOnlyProperties() const {
            if( this->isReadOnlyProperties() ) {
                throw cms::MessageNotWriteableException(
                    "Message Properties are Read-Only.", NULL );
            }
        }

    };

}}

#endif /* _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEMPLATE_H_ */
