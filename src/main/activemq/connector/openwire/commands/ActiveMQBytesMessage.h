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

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/commands/ActiveMQMessageBase.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <cms/BytesMessage.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class AMQCPP_API ActiveMQBytesMessage :
        public ActiveMQMessageBase< cms::BytesMessage > {

    public:

        const static unsigned char ID_ACTIVEMQBYTESMESSAGE = 24;

    public:

        ActiveMQBytesMessage();
        virtual ~ActiveMQBytesMessage() {}

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQBytesMessage* cloneDataStructure() const {
            std::auto_ptr<ActiveMQBytesMessage> message( new ActiveMQBytesMessage() );
            message->copyDataStructure( this );
            return message.release();
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessageBase<cms::BytesMessage>::copyDataStructure( src );
            this->reset();
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const{
            std::ostringstream stream;

            stream << "Begin Class = ActiveMQBytesMessage" << std::endl;
            stream << ActiveMQMessageBase<cms::BytesMessage>::toString();
            stream << "End Class = ActiveMQBytesMessage" << std::endl;

            return stream.str();
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {
            return ActiveMQMessageBase<cms::BytesMessage>::equals( value );
        }

    public:   // CMS Message

        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::BytesMessage* clone() const {
            return dynamic_cast<cms::BytesMessage*>(
                this->cloneDataStructure() );
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody(){

            // Invoke base class's version.
            ActiveMQMessageBase<cms::BytesMessage>::clearBody();

            outputStream.setBuffer( getContent() );
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
            std::size_t numBytes )
                throw( cms::CMSException );

        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes() const;

        /**
         * Returns the number of bytes contained in the body of this message.
         * @return number of bytes.
         */
        virtual std::size_t getBodyLength() const;

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
        virtual bool readBoolean() const throw ( cms::CMSException );

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
        virtual unsigned char readByte() const throw ( cms::CMSException );

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
        virtual std::size_t readBytes( std::vector<unsigned char>& value ) const
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
        virtual std::size_t readBytes( unsigned char*& buffer, std::size_t length ) const
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
                                 std::size_t offset,
                                 std::size_t length ) throw ( cms::CMSException );

        /**
         * Reads a Char from the Bytes message stream
         * @returns char value from stream
         * @throws CMSException
         */
        virtual char readChar() const throw ( cms::CMSException );

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
        virtual float readFloat() const throw ( cms::CMSException );

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
        virtual double readDouble() const throw ( cms::CMSException );

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
        virtual short readShort() const throw ( cms::CMSException );

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
        virtual unsigned short readUnsignedShort() const throw ( cms::CMSException );

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
        virtual int readInt() const throw ( cms::CMSException );

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
        virtual long long readLong() const throw ( cms::CMSException );

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
        virtual std::string readString() const throw ( cms::CMSException );

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
        virtual std::string readUTF() const throw ( cms::CMSException );

        /**
         * Writes an UTF String to the BytesMessage stream
         * @param value - String to write to the stream
         * @throws CMSException
         */
        virtual void writeUTF( const std::string& value ) throw ( cms::CMSException );

    protected:

        /**
         * Throws an exception if in write-only mode.
         * @throws CMSException.
         */
        void checkWriteOnlyBody() const throw (cms::CMSException){
            if( !isReadOnlyBody() ){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "message is in write-only mode and cannot be read from" );
            }
        }

    private:

        /**
         * InputStream that wraps around the command's content when in
         * read-only mode.
         */
        mutable decaf::io::ByteArrayInputStream inputStream;

        /**
         * OutputStream that wraps around the command's content when in
         * write-only mode.
         */
        decaf::io::ByteArrayOutputStream outputStream;

        /**
         * DataInputStream wrapper around the input stream.
         */
        mutable decaf::io::DataInputStream dataInputStream;

        /**
         * DataOutputStream wrapper around the output stream.
         */
        decaf::io::DataOutputStream dataOutputStream;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_*/

