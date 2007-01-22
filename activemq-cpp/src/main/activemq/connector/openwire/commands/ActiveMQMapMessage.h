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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMAPMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMAPMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>
#include <activemq/util/Config.h>
#include <cms/MapMessage.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQMapMessage : public ActiveMQMessage,
                               public cms::MapMessage
    {
    public:

        const static unsigned char ID_ACTIVEMQMAPMESSAGE = 25;

    public:

        ActiveMQMapMessage();
        virtual ~ActiveMQMapMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual DataStructure* cloneDataStructure() const { 
            ActiveMQMapMessage* message = new ActiveMQMapMessage();
            message->copyDataStructure( this );            
            return message;
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessage::copyDataStructure( src );
        }
        
    public:   // CMS Message
    
        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::Message* clone(void) const {
            return dynamic_cast<cms::Message*>( 
                this->cloneDataStructure() );
        }
        
        /**
         * Acknowledges all consumed messages of the session 
         * of this consumed message.
         */
        virtual void acknowledge(void) const throw( cms::CMSException );

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody();
        
        /**
         * Clears the message properties.  Does not clear the body or
         * header values.
         */
        virtual void clearProperties();
        
        /**
         * Retrieves the propery names.
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const ;
        
        /**
         * Indicates whether or not a given property exists.
         * @param name The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const;
        
        /**
         * Gets a boolean property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual bool getBooleanProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a byte property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a double property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual double getDoubleProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a float property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual float getFloatProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a int property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual int getIntProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a long property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual long long getLongProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a short property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual short getShortProperty( const std::string& name ) const 
            throw( cms::CMSException );
            
        /**
         * Gets a string property.
         * @param name The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual std::string getStringProperty( const std::string& name ) const 
            throw( cms::CMSException );
        
        /**
         * Sets a boolean property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setBooleanProperty( const std::string& name,
            bool value ) throw( cms::CMSException );
            
        /**
         * Sets a byte property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setByteProperty( const std::string& name,
            unsigned char value ) throw( cms::CMSException );
            
        /**
         * Sets a double property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setDoubleProperty( const std::string& name,
            double value ) throw( cms::CMSException );
            
        /**
         * Sets a float property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setFloatProperty( const std::string& name,
            float value ) throw( cms::CMSException );
            
        /**
         * Sets a int property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setIntProperty( const std::string& name,
            int value ) throw( cms::CMSException );
            
        /**
         * Sets a long property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setLongProperty( const std::string& name,
            long long value ) throw( cms::CMSException );
            
        /**
         * Sets a short property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setShortProperty( const std::string& name,
            short value ) throw( cms::CMSException );
            
        /**
         * Sets a string property.
         * @param name The name of the property to retrieve.
         * @param value The value for the named property.
         * @throws CMSException
         */
        virtual void setStringProperty( const std::string& name,
            const std::string& value ) throw( cms::CMSException );
      
        /**
         * Get the Correlation Id for this message
         * @return string representation of the correlation Id
         */
        virtual std::string getCMSCorrelationId(void) const {
            return this->getCorrelationId();
        }

        /**
         * Sets the Correlation Id used by this message
         * @param correlationId - String representing the correlation id.
         */
        virtual void setCMSCorrelationId( const std::string& correlationId ) {
            this->setCorrelationId( correlationId );
        }

        /**
         * Gets the DeliveryMode for this message
         * @return DeliveryMode enumerated value.
         */
        virtual int getCMSDeliveryMode(void) const {
            return this->isPersistent();
        }

        /**
         * Sets the DeliveryMode for this message
         * @param mode - DeliveryMode enumerated value.
         */
        virtual void setCMSDeliveryMode( int mode ) {
            this->setPersistent( mode != 0 );
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
        virtual void setCMSDestination( const cms::Destination* destination AMQCPP_UNUSED ) {
//            this->setDestination( destination );
        }
      
        /**
         * Gets the Expiration Time for this Message
         * @return time value
         */
        virtual long long getCMSExpiration(void) const {
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
        virtual std::string getCMSMessageId(void) const;
      
        /**
         * Sets the CMS Message Id for this message
         * @param id - time value
         */
        virtual void setCMSMessageId( const std::string& id AMQCPP_UNUSED ) {}
      
        /**
         * Gets the Priority Value for this Message
         * @return priority value
         */
        virtual int getCMSPriority(void) const {
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
        virtual bool getCMSRedelivered(void) const {
            return this->getRedeliveryCounter() != 0;
        }
      
        /**
         * Sets the Redelivered Flag for this message
         * @param redelivered - boolean redelivered value
         */
        virtual void setCMSRedelivered( bool redelivered AMQCPP_UNUSED ) {}

        /**
         * Gets the CMS Reply To Address for this Message
         * @return Reply To Value
         */
        virtual const cms::Destination* getCMSReplyTo(void) const;
      
        /**
         * Sets the CMS Reply To Address for this message
         * @param id - Reply To value
         */
        virtual void setCMSReplyTo( const cms::Destination* destination );

        /**
         * Gets the Time Stamp for this Message
         * @return time stamp value
         */
        virtual long long getCMSTimeStamp(void) const {
            return this->getTimestamp();
        }
      
        /**
         * Sets the Time Stamp for this message
         * @param timeStamp - integer time stamp value
         */
        virtual void setCMSTimeStamp( long long timeStamp ) {
            this->setTimestamp( timeStamp );
        }

        /**
         * Gets the CMS Message Type for this Message
         * @return type value
         */
        virtual std::string getCMSMessageType(void) const {
            return this->getType();
        }
      
        /**
         * Sets the CMS Message Type for this message
         * @param type - message type value string
         */
        virtual void setCMSMessageType( const std::string& type ) {
            this->setType( type );
        }
    public:   // CMS MapMessage
    
        /**
         * Returns an Enumeration of all the names in the MapMessage 
         * object.
         * @return STL Vector of String values, each of which is the 
         *         name of an item in the MapMessage
         * @throws CMSException
         */
        virtual std::vector< std::string > getMapNames(void) const;

        /**
         * Indicates whether an item exists in this MapMessage object.
         * @param name - String name of the Object in question
         * @return boolean value indicating if the name is in the map
         */
        virtual bool itemExists( const std::string& name ) const;
        
        /**
         * Returns the Boolean value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual bool getBoolean( const std::string& name ) const;
        
        /** 
         * Sets a boolean value with the specified name into the Map.
         * @param name - the name of the boolean
         * @param value - the boolean value to set in the Map
         * @throws CMSException
         */
        virtual void setBoolean( const std::string& name,
                                 bool value );

        /**
         * Returns the Byte value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual unsigned char getByte( const std::string& name ) const;
        
        /** 
         * Sets a Byte value with the specified name into the Map.
         * @param name - the name of the Byte
         * @param value - the Byte value to set in the Map
         * @throws CMSException
         */
        virtual void setByte( const std::string& name,
                              unsigned char value );

        /**
         * Returns the Bytes value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual const unsigned char* getBytes( const std::string& name ) const;
        
        /** 
         * Sets a Bytes value with the specified name into the Map.
         * @param name - the name of the Bytes
         * @param value - the Bytes value to set in the Map
         * @throws CMSException
         */
        virtual void setBytes( const std::string& name,
                               unsigned char* value );

        /**
         * Returns the Char value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual char getChar( const std::string& name ) const;
        
        /** 
         * Sets a Char value with the specified name into the Map.
         * @param name - the name of the Char
         * @param value - the Char value to set in the Map
         * @throws CMSException
         */
        virtual void setChar( const std::string& name, char value );

        /**
         * Returns the Double value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual double getDouble( const std::string& name ) const;
        
        /** 
         * Sets a Double value with the specified name into the Map.
         * @param name - the name of the Double
         * @param value - the Double value to set in the Map
         * @throws CMSException
         */
        virtual void setDouble( const std::string& name,
                                double value );

        /**
         * Returns the Float value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual float getFloat( const std::string& name ) const;
        
        /** 
         * Sets a Float value with the specified name into the Map.
         * @param name - the name of the Float
         * @param value - the Float value to set in the Map
         * @throws CMSException
         */
        virtual void setFloat( const std::string& name, float value );

        /**
         * Returns the Int value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual int getInt( const std::string& name ) const;
        
        /** 
         * Sets a Int value with the specified name into the Map.
         * @param name - the name of the Int
         * @param value - the Int value to set in the Map
         * @throws CMSException
         */
        virtual void setInt( const std::string& name, int value );

        /**
         * Returns the Long value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual long long getLong( const std::string& name ) const;
        
        /** 
         * Sets a Long value with the specified name into the Map.
         * @param name - the name of the Long
         * @param value - the Long value to set in the Map
         * @throws CMSException
         */
        virtual void setLong( const std::string& name, long long value );

        /**
         * Returns the Short value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual short getShort( const std::string& name ) const;
        
        /** 
         * Sets a Short value with the specified name into the Map.
         * @param name - the name of the Short
         * @param value - the Short value to set in the Map
         * @throws CMSException
         */
        virtual void setShort( const std::string& name, short value );

        /**
         * Returns the String value of the Specified name
         * @param name of the value to fetch from the map
         * @throws CMSException
         */
        virtual std::string getString( const std::string& name ) const;
        
        /** 
         * Sets a String value with the specified name into the Map.
         * @param name - the name of the String
         * @param value - the String value to set in the Map
         * @throws CMSException
         */
        virtual void setString( const std::string& name, 
                                const std::string& value );

    public:  // ActiveMQMessage

        /**
         * Returns if this message has expired, meaning that its
         * Expiration time has elapsed.
         * @returns true if message is expired.
         */
        virtual bool isExpired() const {
            long long expireTime = this->getCMSExpiration();
            long long currentTime = util::Date::getCurrentTimeMilliseconds();
            if( expireTime > 0 && currentTime > expireTime ) {
                return true;
            }
            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQMAPMESSAGE_H_*/

