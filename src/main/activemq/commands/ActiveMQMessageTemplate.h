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
        virtual void acknowledge() const throw( cms::CMSException ) {
            try{
                this->getAckHandler()->acknowledgeMessage( this );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() throw( cms::CMSException ) {
            this->setContent( std::vector<unsigned char>() );
            this->setReadOnlyBody( false );
        }

        /**
         * Clears the message properties.  Does not clear the body or
         * header values.
         */
        virtual void clearProperties() throw( cms::CMSException ) {
            this->getMessageProperties().clear();
            this->setReadOnlyProperties( false );
        }

        /**
         * Retrieves the property names.
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const throw( cms::CMSException ) {
            return getMessageProperties().keySet();
        }

        /**
         * Indicates whether or not a given property exists.
         * @param name The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const throw( cms::CMSException ) {
            return getMessageProperties().containsKey( name );
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
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
            return this->isPersistent();
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

            if( destination != NULL ) {
                this->setDestination(
                    decaf::lang::Pointer<ActiveMQDestination>(
                        dynamic_cast<ActiveMQDestination*>( destination->clone() ) ) );
            } else {
                this->getDestination().reset( NULL );
            }
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
            return wireformat::openwire::marshal::BaseDataStreamMarshaller::toString( this->getMessageId().get() );
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
            return dynamic_cast< const cms::Destination* >(
                this->getReplyTo().get() );
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

            if( destination != NULL ) {
                this->setReplyTo(
                    decaf::lang::Pointer<ActiveMQDestination>(
                        dynamic_cast<ActiveMQDestination*>( destination->clone() ) ) );
            } else {
                this->setReplyTo( decaf::lang::Pointer<ActiveMQDestination>() );
            }
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

        void checkReadOnlyBody() {
            if( this->isReadOnlyBody() ) {
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Message Body is Read-Only." );
            }
        }

        void checkReadOnlyProperties() {
            if( this->isReadOnlyProperties() ) {
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__, "Message Properties are Read-Only." );
            }
        }

    };

}}

#endif /* _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEMPLATE_H_ */
