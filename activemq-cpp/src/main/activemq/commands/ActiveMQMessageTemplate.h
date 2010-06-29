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
#include <activemq/core/ActiveMQConnection.h>
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

        ActiveMQMessageTemplate() : commands::Message(), propertiesInterceptor() {
            this->propertiesInterceptor.reset(
                new wireformat::openwire::utils::MessagePropertyInterceptor(
                    this, &this->getMessageProperties() ) );
        }

        virtual ~ActiveMQMessageTemplate() throw() {}

    public:  // cms::Message related methods

        virtual void acknowledge() const {
            try{
                this->getAckHandler()->acknowledgeMessage( this );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void onSend() {
            this->setReadOnlyBody(true);
            this->setReadOnlyProperties(true);
        }

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

        virtual void clearBody() {
            try{
                this->setContent( std::vector<unsigned char>() );
                this->setReadOnlyBody( false );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void clearProperties() {
            try{
                this->getMessageProperties().clear();
                this->setReadOnlyProperties( false );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual std::vector<std::string> getPropertyNames() const {
            try{
                return getMessageProperties().keySet();
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual bool propertyExists( const std::string& name ) const {
            try{
                return getMessageProperties().containsKey( name );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual bool getBooleanProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getBooleanProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual unsigned char getByteProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getByteProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual double getDoubleProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getDoubleProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual float getFloatProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getFloatProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual int getIntProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getIntProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual long long getLongProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getLongProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual short getShortProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getShortProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual std::string getStringProperty( const std::string& name ) const {

            try{
                return this->propertiesInterceptor->getStringProperty( name );
            } catch( decaf::lang::exceptions::UnsupportedOperationException& ex ) {
                throw activemq::util::CMSExceptionSupport::createMessageFormatException( ex );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setBooleanProperty( const std::string& name, bool value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setBooleanProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setByteProperty( const std::string& name, unsigned char value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setByteProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setDoubleProperty( const std::string& name, double value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setDoubleProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setFloatProperty( const std::string& name, float value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setFloatProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setIntProperty( const std::string& name, int value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setIntProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setLongProperty( const std::string& name, long long value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setLongProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setShortProperty( const std::string& name, short value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setShortProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setStringProperty( const std::string& name, const std::string& value ) {

            if( name == "" ) {
                throw cms::CMSException( "Message Property names must not be empty", NULL );
            }

            failIfReadOnlyProperties();
            try{
                this->propertiesInterceptor->setStringProperty( name, value );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual std::string getCMSCorrelationID() const {
            return this->getCorrelationId();
        }

        virtual void setCMSCorrelationID( const std::string& correlationId ) {
            this->setCorrelationId( correlationId );
        }

        virtual int getCMSDeliveryMode() const {
            return !this->isPersistent();
        }

        virtual void setCMSDeliveryMode( int mode ) {
            this->setPersistent( mode == (int)cms::DeliveryMode::PERSISTENT );
        }

        virtual const cms::Destination* getCMSDestination() const {
            return dynamic_cast<const cms::Destination*>( this->getDestination().get() );
        }

        virtual void setCMSDestination( const cms::Destination* destination ) {

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

        virtual long long getCMSExpiration() const {
            return this->getExpiration();
        }

        virtual void setCMSExpiration( long long expireTime ) {
            this->setExpiration( expireTime );
        }

        virtual std::string getCMSMessageID() const {
            try{
                return wireformat::openwire::marshal::BaseDataStreamMarshaller::toString( this->getMessageId().get() );
            }
            AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
        }

        virtual void setCMSMessageID( const std::string& id AMQCPP_UNUSED ) {}

        virtual int getCMSPriority() const {
            return this->getPriority();
        }

        virtual void setCMSPriority( int priority ) {
            this->setPriority( (unsigned char)priority );
        }

        virtual bool getCMSRedelivered() const {
            return this->getRedeliveryCounter() != 0;
        }

        virtual void setCMSRedelivered( bool redelivered AMQCPP_UNUSED ) {}

        virtual const cms::Destination* getCMSReplyTo() const {
            return dynamic_cast< const cms::Destination* >( this->getReplyTo().get() );
        }

        virtual void setCMSReplyTo( const cms::Destination* destination ) {

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

        virtual long long getCMSTimestamp() const {
            return this->getTimestamp();
        }

        virtual void setCMSTimestamp( long long timeStamp ) {
            this->setTimestamp( timeStamp );
        }

        virtual std::string getCMSType() const {
            return this->getType();
        }

        virtual void setCMSType( const std::string& type ) {
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
