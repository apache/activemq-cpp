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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>
#include <cms/BytesMessage.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQBytesMessage : public ActiveMQMessage,
                                 public cms::BytesMessage
    {
    public:

        const static unsigned char ID_ACTIVEMQBYTESMESSAGE = 24;

    public:

        ActiveMQBytesMessage();
        virtual ~ActiveMQBytesMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual DataStructure* cloneDataStructure() const { 
            ActiveMQBytesMessage* message = new ActiveMQBytesMessage();
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
        virtual cms::BytesMessage* clone(void) const {
            return dynamic_cast<cms::BytesMessage*>( 
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
        virtual void setCMSDestination( const cms::Destination* destination ) {
 //           this->setDestination( destination );
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
        virtual void setCMSMessageId( const std::string& id ) {}
      
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
        virtual void setCMSRedelivered( bool redelivered ) {}

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

    public:   // CMS BytesMessage
    
        /**
         * sets the bytes given to the message body.  
         * @param Byte Buffer to copy
         * @param Number of bytes in Buffer to copy
         * @throws CMSException
         */
        virtual void setBodyBytes( 
            const unsigned char* buffer, 
            const unsigned long long numBytes ) 
                throw( cms::CMSException );
            
        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call 
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes(void) const;
      
        /**
         * Returns the number of bytes contained in the body of this message.
         * @return number of bytes.
         */
        virtual unsigned long long getBodyLength(void) const;
        
        /**
         * Puts the message body in read-only mode and repositions the stream 
         * of bytes to the beginning.
         * @throws CMSException
         */
        virtual void reset() throw ( cms::CMSException );
        
        /**
         * Reads a Boolean from the Bytes message stream
         * @returns boolean value from stream
         * @throws CMSException
         */
        virtual bool readBoolean() throw ( cms::CMSException );
        
        /**
         * Writes a boolean to the bytes message stream as a 1-byte value. 
         * The value true is written as the value (byte)1; the value false 
         * is written as the value (byte)0.
         * @param value - boolean to write to the stream
         * @throws CMSException
         */
        virtual void writeBoolean( bool value ) throw ( cms::CMSException );

        /**
         * Reads a Byte from the Bytes message stream
         * @returns unsigned char value from stream
         * @throws CMSException
         */
        virtual unsigned char readByte() throw ( cms::CMSException );

        /**
         * Writes a byte to the bytes message stream as a 1-byte value
         * @param value - byte to write to the stream
         * @throws CMSException
         */
        virtual void writeByte( unsigned char value ) throw ( cms::CMSException );


        /**
         * Reads a byte array from the bytes message stream.
         *
         * If the length of vector value is less than the number of bytes
         * remaining to be read from the stream, the vector should be filled. A 
         * subsequent call reads the next increment, and so on.
         *
         * If the number of bytes remaining in the stream is less than the 
         * length of vector value, the bytes should be read into the vector. The 
         * return value of the total number of bytes read will be less than the
         * length of the vector, indicating that there are no more bytes left to 
         * be read from the stream. The next read of the stream returns -1.
         * 
         * @param value - buffer to place data in
         * @returns the total number of bytes read into the buffer, or -1 if 
         *          there is no more data because the end of the stream has 
         *          been reached
         * @throws CMSException if an error occurs.
         */
        virtual int readBytes( std::vector<unsigned char>& value ) 
            throw ( cms::CMSException );

        /**
         * Writes a byte array to the bytes message stream using the vector
         * size as the number of bytes to write.
         * @param value - bytes to write to the stream
         * @throws CMSException
         */
        virtual void writeBytes( const std::vector<unsigned char>& value ) throw ( cms::CMSException );

        /**
         * Reads a portion of the bytes message stream.
         * 
         * If the length of array value is less than the number of bytes 
         * remaining to be read from the stream, the array should be filled. A 
         * subsequent call reads the next increment, and so on.
         * 
         * If the number of bytes remaining in the stream is less than the 
         * length of array value, the bytes should be read into the array. The 
         * return value of the total number of bytes read will be less than the 
         * length of the array, indicating that there are no more bytes left to 
         * be read from the stream. The next read of the stream returns -1.
         * 
         * If length is negative, or length is greater than the length of the 
         * array value, then an IndexOutOfBoundsException is thrown. No bytes 
         * will be read from the stream for this exception case.
         * 
         * @param value - the buffer into which the data is read
         * @param length - the number of bytes to read; must be less than or 
         *                 equal to value.length
         * @returns the total number of bytes read into the buffer, or -1 if 
         *          there is no more data because the end of the stream has 
         *          been reached
         * @throws CMSException
         */
        virtual int readBytes( unsigned char*& buffer, int length ) 
            throw ( cms::CMSException );

        /**
         * Writes a portion of a byte array to the bytes message stream.
         * size as the number of bytes to write.
         * @param value - bytes to write to the stream
         * @param offset - the initial offset within the byte array
         * @param length - the number of bytes to use
         * @throws CMSException
         */
        virtual void writeBytes( const unsigned char* value,
                                 int offset,
                                 int length ) throw ( cms::CMSException );

        /**
         * Reads a Char from the Bytes message stream
         * @returns char value from stream
         * @throws CMSException
         */
        virtual char readChar() throw ( cms::CMSException );

        /**
         * Writes a char to the bytes message stream as a 1-byte value
         * @param value - char to write to the stream
         * @throws CMSException
         */
        virtual void writeChar( char value ) throw ( cms::CMSException );

        /**
         * Reads a 32 bit float from the Bytes message stream
         * @returns double value from stream
         * @throws CMSException
         */
        virtual float readFloat() throw ( cms::CMSException );

        /**
         * Writes a float to the bytes message stream as a 4 byte value
         * @param value - float to write to the stream
         * @throws CMSException
         */
        virtual void writeFloat( float value ) throw ( cms::CMSException );

        /**
         * Reads a 64 bit double from the Bytes message stream
         * @returns double value from stream
         * @throws CMSException
         */
        virtual double readDouble() throw ( cms::CMSException );

        /**
         * Writes a double to the bytes message stream as a 8 byte value
         * @param value - double to write to the stream
         * @throws CMSException
         */
        virtual void writeDouble( double value ) throw ( cms::CMSException );

        /**
         * Reads a 16 bit signed short from the Bytes message stream
         * @returns short value from stream
         * @throws CMSException
         */
        virtual short readShort() throw ( cms::CMSException );

        /**
         * Writes a signed short to the bytes message stream as a 2 byte value
         * @param value - signed short to write to the stream
         * @throws CMSException
         */
        virtual void writeShort( short value ) throw ( cms::CMSException );

        /**
         * Reads a 16 bit unsigned short from the Bytes message stream
         * @returns unsigned short value from stream
         * @throws CMSException
         */
        virtual unsigned short readUnsignedShort() throw ( cms::CMSException );

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         * @param value - unsigned short to write to the stream
         * @throws CMSException
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( cms::CMSException );

        /**
         * Reads a 32 bit signed intger from the Bytes message stream
         * @returns int value from stream
         * @throws CMSException
         */
        virtual int readInt() throw ( cms::CMSException );

        /**
         * Writes a signed int to the bytes message stream as a 4 byte value
         * @param value - signed int to write to the stream
         * @throws CMSException
         */
        virtual void writeInt( int value ) throw ( cms::CMSException );

        /**
         * Reads a 64 bit long from the Bytes message stream
         * @returns long long value from stream
         * @throws CMSException
         */
        virtual long long readLong() throw ( cms::CMSException );

        /**
         * Writes a long long to the bytes message stream as a 8 byte value
         * @param value - signed long long to write to the stream
         * @throws CMSException
         */
        virtual void writeLong( long long value ) throw ( cms::CMSException );

        /**
         * Reads an ASCII String from the Bytes message stream
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readString() throw ( cms::CMSException );

        /**
         * Writes an ASCII String to the Bytes message stream
         * @param value - String to write to the stream
         * @throws CMSException
         */
        virtual void writeString( const std::string& value ) throw ( cms::CMSException );

        /**
         * Reads an UTF String from the BytesMessage stream
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readUTF() throw ( cms::CMSException );

        /**
         * Writes an UTF String to the BytesMessage stream
         * @param value - String to write to the stream
         * @throws CMSException
         */
        virtual void writeUTF( const std::string& value ) throw ( cms::CMSException );

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

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_*/

