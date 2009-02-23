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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_STOMPMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_STOMPMESSAGE_H_

#include <activemq/core/ActiveMQMessage.h>
#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/transport/Command.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/util/Config.h>

#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/Date.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>

#include <string>
#include <sstream>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Base class for Stomp Commands that represent the Active MQ message
     * types.  This class is templated and expects the Template type to be
     * a cms::Message type, Message, TextMessage etc.  This class will
     * implement all the general cms:Message methods
     *
     * This class implement AbsractCommand<StompCommnd> and the
     * ActiveMQMessage interface.
     */
    template<typename T>
    class AMQCPP_API StompMessage :
        public AbstractCommand< transport::Command >,
        public T,
        public core::ActiveMQMessage
    {
    private:

        // Core API defined Acknowedge Handler.
        core::ActiveMQAckHandler* ackHandler;

        // Cached Destination
        cms::Destination* dest;

        // Cached Destination
        cms::Destination* replyTo;

        bool readOnlyBody;
        bool readOnlyProperties;

    public:

        StompMessage() :
            AbstractCommand< transport::Command >(),
            ackHandler( NULL ),
            dest( NULL ),
            replyTo( NULL ),
            readOnlyBody( false ),
            readOnlyProperties( false ) {
        }

        StompMessage( StompFrame* frame ) :
            AbstractCommand< transport::Command >( frame ),
            ackHandler( NULL ),
            dest( NULL ),
            replyTo( NULL ),
            readOnlyBody( false ),
            readOnlyProperties( false ) {

            const std::string& destHeader = CommandConstants::toString(
                CommandConstants::HEADER_DESTINATION );
            const std::string& replyToHeader = CommandConstants::toString(
                CommandConstants::HEADER_REPLYTO );

            dest = CommandConstants::toDestination(
                getPropertyValue( destHeader, "" ) );

            std::string replyToValue = getPropertyValue( replyToHeader, "null" );
            if( replyToValue != "null" ) {
                replyTo = CommandConstants::toDestination( replyToValue );
            }
        }

        virtual ~StompMessage() {

            if( dest != NULL ){
                delete dest;
            }

            if( replyTo != NULL ){
                delete replyTo;
            }
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody(){
            this->readOnlyBody = false;
            getFrame().getBody().clear();
        }

        /**
         * Clears the message properties.  Does not clear the body or
         * header values.
         */
        virtual void clearProperties(){

            this->readOnlyProperties = false;
            decaf::util::Properties& props = getFrame().getProperties();
            std::vector< std::pair< std::string, std::string > > propArray = props.toArray();
            for( unsigned int ix=0; ix<propArray.size(); ++ix ){

                const std::string& name = propArray[ix].first;

                // Only clear properties that aren't Stomp headers.
                if( !CommandConstants::isStompHeader(name) ){
                    props.remove( name );
                }
            }
        }

        /**
         * Retrieves the property names.
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const{
            std::vector<std::string> names;

            const decaf::util::Properties& props = getFrame().getProperties();
            std::vector< std::pair< std::string, std::string > > propArray = props.toArray();
            for( unsigned int ix=0; ix<propArray.size(); ++ix ){

                const std::string& name = propArray[ix].first;

                // Only clear properties that aren't Stomp headers.
                if( !CommandConstants::isStompHeader(name) ){
                    names.push_back( name );
                }
            }

            return names;
        }

        /**
         * Indicates whether or not a given property exists.
         * @param name The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const{
            if( CommandConstants::isStompHeader( name ) ){
                return false;
            }

            return getFrame().getProperties().hasProperty( name );
        }

        virtual bool getBooleanProperty( const std::string& name ) const
            throw( cms::CMSException ){

            try{
                std::string value = getStrictPropertyValue<std::string>( name );
                return value == "true";
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual unsigned char getByteProperty( const std::string& name ) const
            throw( cms::CMSException ){

            try{
                return getStrictPropertyValue<unsigned char>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual double getDoubleProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<double>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual float getFloatProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<float>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual int getIntProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<int>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual long long getLongProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<long long>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual short getShortProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<short>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual std::string getStringProperty( const std::string& name ) const
            throw( cms::CMSException ){
            try{
                return getStrictPropertyValue<std::string>(name);
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setBooleanProperty( const std::string& name,
                                         bool value ) throw( cms::CMSException ){

            try{
                checkReadOnlyProperties();
                testProperty( name );
                std::string strvalue = value? "true" : "false";
                setPropertyValue( name, strvalue );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setByteProperty( const std::string& name,
            unsigned char value ) throw( cms::CMSException ){

            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<unsigned char>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setDoubleProperty( const std::string& name,
                                        double value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<double>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setFloatProperty( const std::string& name,
                                       float value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<float>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setIntProperty( const std::string& name,
                                     int value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<int>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setLongProperty( const std::string& name,
                                      long long value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<long long>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setShortProperty( const std::string& name,
                                       short value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                setStrictPropertyValue<short>( name, value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        virtual void setStringProperty( const std::string& name,
                                        const std::string& value ) throw( cms::CMSException ){
            try{
                checkReadOnlyProperties();
                testProperty( name );
                setPropertyValue( name, value );
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
            std::string correlationId = getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_CORRELATIONID ), "null" );
            if( correlationId == "null" ){
                return "";
            }
            return correlationId;
        }

        /**
         * Sets the Correlation Id used by this message
         * @param correlationId String representing the correlation id.
         */
        virtual void setCMSCorrelationID(const std::string& correlationId) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_CORRELATIONID ) ,
                correlationId );
        }

        /**
         * Acknowledges all consumed messages of the session
         * of this consumed message.
         * @throws CMSException
         */
        virtual void acknowledge() const throw( cms::CMSException ) {
            try{
                if( ackHandler != NULL ) {
                    ackHandler->acknowledgeMessage( this );
                }
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Sets the DeliveryMode for this message
         * @return DeliveryMode enumerated value.
         */
        virtual int getCMSDeliveryMode() const {

            if(!getFrame().getProperties().hasProperty(
                   CommandConstants::toString(
                       CommandConstants::HEADER_PERSISTENT ) ) ) {
                return cms::DeliveryMode::PERSISTENT;
            }

            if( decaf::lang::Boolean::parseBoolean( getPropertyValue(
                       CommandConstants::toString(
                           CommandConstants::HEADER_PERSISTENT ) ) ) == true ) {
                return (int)cms::DeliveryMode::PERSISTENT;
            }

            return cms::DeliveryMode::NON_PERSISTENT;
        }

        /**
         * Sets the DeliveryMode for this message
         * @param mode DeliveryMode enumerated value.
         */
        virtual void setCMSDeliveryMode( int mode ) {
            std::string persistant = "true";

            if( mode == (int)cms::DeliveryMode::NON_PERSISTENT ) {
                persistant = "false";
            }

            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_PERSISTENT ) ,
                persistant );
        }

        /**
         * Gets the Destination for this Message
         * @return Destination object can be NULL
         */
        virtual const cms::Destination* getCMSDestination() const{
            return dest;
        }

        /**
         * Sets the Destination for this message
         * @param destination Destination Object
         */
        virtual void setCMSDestination( const cms::Destination* destination ) {

            if( destination != NULL ) {
                delete dest;
                dest = destination->clone();
                setPropertyValue(
                    CommandConstants::toString(
                        CommandConstants::HEADER_DESTINATION ),
                    dest->toProviderString() );
            }
        }

        /**
         * Gets the Expiration Time for this Message
         * @return time value
         */
        virtual long long getCMSExpiration() const {
            return decaf::lang::Long::parseLong( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_EXPIRES ), "0" ) );
        }

        /**
         * Sets the Expiration Time for this message
         * @param expireTime time value
         */
        virtual void setCMSExpiration( long long expireTime ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_EXPIRES) ,
                decaf::lang::Long::toString( expireTime ) );
        }

        /**
         * Gets the CMS Message Id for this Message
         * @return time value
         */
        virtual std::string getCMSMessageID() const {
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGEID ), "" );
        }

        /**
         * Sets the CMS Message Id for this message
         * @param id time value
         */
        virtual void setCMSMessageID( const std::string& id ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGEID ),
                id );
        }

        /**
         * Gets the Priority Value for this Message
         * @return priority value
         */
        virtual int getCMSPriority() const {
            return decaf::lang::Integer::parseInt( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_JMSPRIORITY ), "0" ) );
        }

        /**
         * Sets the Priority Value for this message
         * @param priority priority value
         */
        virtual void setCMSPriority( int priority ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_JMSPRIORITY),
                decaf::lang::Integer::toString( priority ) );
        }

        /**
         * Gets the Redelivered Flag for this Message
         * @return redelivered value
         */
        virtual bool getCMSRedelivered() const {
            return decaf::lang::Boolean::parseBoolean( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_REDELIVERED ),
                "false" ) );
        }

        /**
         * Sets the Redelivered Flag for this message
         * @param redelivered redelivered value
         */
        virtual void setCMSRedelivered( bool redelivered ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_REDELIVERED ),
                decaf::lang::Boolean::toString( redelivered ) );
        }

        /**
         * Gets the CMS Reply To Address for this Message
         * @return Reply To Value
         */
        virtual const cms::Destination* getCMSReplyTo() const {
            return replyTo;
        }

        /**
         * Sets the CMS Reply To Address for this message
         * @param id Reply To value
         */
        virtual void setCMSReplyTo( const cms::Destination* destination ) {
            if( destination != NULL )
            {
                delete replyTo;
                replyTo = destination->clone();
                setPropertyValue(
                    CommandConstants::toString(
                        CommandConstants::HEADER_REPLYTO ),
                    replyTo->toProviderString() );
            }
        }

        /**
         * Gets the Time Stamp for this Message
         * @return time stamp value
         */
        virtual long long getCMSTimestamp() const {
            return decaf::lang::Long::parseLong( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_TIMESTAMP ), "0" ) );
        }

        /**
         * Sets the Time Stamp for this message
         * @param timeStamp time stamp value
         */
        virtual void setCMSTimestamp( long long timeStamp ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_TIMESTAMP ),
                decaf::lang::Long::toString( timeStamp ) );
        }

        /**
         * Gets the CMS Message Type for this Message
         * @return type value
         */
        virtual std::string getCMSType() const {
            std::string type = getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_TYPE ), "null" );
            if( type == "null" ){
                return "";
            }
            return type;
        }

        /**
         * Sets the CMS Message Type for this message
         * @param type type value
         */
        virtual void setCMSType( const std::string& type ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_TYPE ),
                type );
        }

    public:    // ActiveMQMessage

        /**
         * Gets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @returns handler ActiveMQAckHandler
         */
        virtual core::ActiveMQAckHandler* getAckHandler() const {
            return this->ackHandler;
        }

        /**
         * Sets the Acknowledgment Handler that this Message will use
         * when the Acknowledge method is called.
         * @param handler ActiveMQAckHandler
         */
        virtual void setAckHandler( core::ActiveMQAckHandler* handler ) {
            this->ackHandler = handler;
        }

        /**
         * Gets the number of times this message has been redelivered.
         * @return redelivery count
         */
        virtual int getRedeliveryCount() const {
            return decaf::lang::Integer::parseInt( getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_REDELIVERYCOUNT ),
                "0" ) );
        }

        /**
         * Sets the count of the number of times this message has been
         * redelivered
         * @param count redelivery count
         */
        virtual void setRedeliveryCount( int count ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_REDELIVERYCOUNT ),
                decaf::lang::Integer::toString( count ) );
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

    protected:

        /**
         * Checks to see if the given property has the name of a
         * pre-defined header.  If so, throws an exception.
         */
        virtual void testProperty( const std::string& name ) const
            throw( cms::CMSException ){
            if( CommandConstants::isStompHeader( name ) ){
                throw exceptions::ActiveMQException(
                    __FILE__, __LINE__,
                    "searching for property with name of pre-defined header" );
            }
        }

        /**
         * Attempts to get a property from the frame's property
         * map.
         */
        template <typename TYPE>
        TYPE getStrictPropertyValue( const std::string& name ) const
            throw( cms::CMSException ){

            try{
                testProperty( name );

                if( !getProperties().hasProperty( name ) ){
                    throw exceptions::ActiveMQException(
                        __FILE__, __LINE__,
                        "property not available in message" );
                }

                const char* strProp = getPropertyValue( name );
                std::istringstream stream( strProp );
                TYPE value;
                stream >> value;

                if( stream.fail() ){
                    throw exceptions::ActiveMQException(
                        __FILE__, __LINE__,
                        "Error extracting property from string" );
                }

                return value;
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Attempts to set the property in the frame.  If an error occurs or
         * the property name is that of a pre-defined header, an exception
         * is thrown.
         */
        template <typename TYPE>
        void setStrictPropertyValue( const std::string& name, TYPE value )
            throw( cms::CMSException ){
            try{
                testProperty( name );

                std::ostringstream stream;
                stream << value;

                setPropertyValue( name, stream.str() );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::SEND ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {

            if(frame.getCommand() ==
               CommandConstants::toString( CommandConstants::SEND ) ) {

                if(frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_DESTINATION ) ) ) {

                    return true;
                }

            } else if( frame.getCommand() ==
                       CommandConstants::toString( CommandConstants::MESSAGE ) ) {

                if(frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_DESTINATION ) ) &&
                   frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_MESSAGEID ) ) ) {
                    return true;
                }
            }

            return false;
        }

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

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_STOMPMESSAGE_H_*/
