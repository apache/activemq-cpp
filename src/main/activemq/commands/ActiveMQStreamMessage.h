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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>
#include <cms/StreamMessage.h>
#include <cms/MessageNotWriteableException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageEOFException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQStreamMessage :
        public ActiveMQMessageTemplate< cms::StreamMessage > {
    private:

        mutable std::auto_ptr<decaf::io::ByteArrayOutputStream> bytesOut;
        mutable std::auto_ptr<decaf::io::DataInputStream> dataIn;
        mutable std::auto_ptr<decaf::io::DataOutputStream> dataOut;

        mutable int remainingBytes;

    public:

        const static unsigned char ID_ACTIVEMQSTREAMMESSAGE = 27;

    public:

        ActiveMQStreamMessage();
        virtual ~ActiveMQStreamMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ActiveMQStreamMessage* cloneDataStructure() const;

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const;

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const;

        /**
         * Store the Data that was written to the stream before a send
         */
        virtual void onSend();

    public:   // CMS Message

        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::StreamMessage* clone() const {
            return dynamic_cast<cms::StreamMessage*>( this->cloneDataStructure() );
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() throw( cms::CMSException );

    public: // CMS Stream Message

        /**
         * Puts the message body in read-only mode and repositions the stream
         * of bytes to the beginning.
         * @throws CMSException
         */
        virtual void reset() throw ( cms::CMSException );

        /**
         * Reads a Boolean from the Stream message stream
         * @returns boolean value from stream
         *
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual bool readBoolean() const throw ( cms::MessageEOFException,
                                                 cms::MessageFormatException,
                                                 cms::MessageNotReadableException,
                                                 cms::CMSException );

        /**
         * Writes a boolean to the Stream message stream as a 1-byte value.
         * The value true is written as the value (byte)1; the value false
         * is written as the value (byte)0.
         *
         * @param value
         *      boolean to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBoolean( bool value ) throw ( cms::MessageNotWriteableException,
                                                        cms::CMSException );

        /**
         * Reads a Byte from the Stream message stream
         *
         * @returns unsigned char value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual unsigned char readByte() const throw ( cms::MessageEOFException,
                                                       cms::MessageFormatException,
                                                       cms::MessageNotReadableException,
                                                       cms::CMSException );

        /**
         * Writes a byte to the Stream message stream as a 1-byte value
         *
         * @param value
         *      byte to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeByte( unsigned char value ) throw ( cms::MessageNotWriteableException,
                                                              cms::CMSException );

        /**
         * Reads a byte array from the Stream message stream.
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
         * @param value
         *      buffer to place data in
         * @returns the total number of bytes read into the buffer, or -1 if
         *          there is no more data because the end of the stream has
         *          been reached
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual std::size_t readBytes( std::vector<unsigned char>& value ) const
            throw ( cms::MessageEOFException,
                    cms::MessageFormatException,
                    cms::MessageNotReadableException,
                    cms::CMSException );

        /**
         * Writes a byte array to the Stream message stream using the vector
         * size as the number of bytes to write.
         *
         * @param value
         *      bytes to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBytes( const std::vector<unsigned char>& value )
            throw ( cms::MessageNotWriteableException,
                    cms::CMSException );

        /**
         * Reads a portion of the Stream message stream.
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
         * array value, then an CMSException is thrown. No bytes
         * will be read from the stream for this exception case.
         *
         * @param buffer
         *      the buffer into which the data is read
         * @param length
         *      the number of bytes to read; must be less than or equal to
         *      value.length
         * @returns the total number of bytes read into the buffer, or -1 if
         *          there is no more data because the end of the stream has
         *          been reached
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual std::size_t readBytes( unsigned char* buffer, std::size_t length ) const
            throw ( cms::MessageEOFException,
                    cms::MessageFormatException,
                    cms::MessageNotReadableException,
                    cms::CMSException );

        /**
         * Writes a portion of a byte array to the Stream message stream.
         * size as the number of bytes to write.
         *
         * @param value
         *      bytes to write to the stream
         * @param offset
         *      the initial offset within the byte array
         * @param length
         *      the number of bytes to use
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBytes( const unsigned char* value, std::size_t offset, std::size_t length )
            throw ( cms::MessageNotWriteableException,
                    cms::CMSException );

        /**
         * Reads a Char from the Stream message stream
         *
         * @returns char value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual char readChar() const throw ( cms::MessageEOFException,
                                              cms::MessageFormatException,
                                              cms::MessageNotReadableException,
                                              cms::CMSException );

        /**
         * Writes a char to the Stream message stream as a 1-byte value
         *
         * @param value
         *      char to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeChar( char value ) throw ( cms::MessageNotWriteableException,
                                                     cms::CMSException );

        /**
         * Reads a 32 bit float from the Stream message stream
         *
         * @returns double value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual float readFloat() const throw ( cms::MessageEOFException,
                                                cms::MessageFormatException,
                                                cms::MessageNotReadableException,
                                                cms::CMSException );

        /**
         * Writes a float to the Stream message stream as a 4 byte value
         * @param value
         *      float to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeFloat( float value ) throw ( cms::MessageNotWriteableException,
                                                       cms::CMSException );

        /**
         * Reads a 64 bit double from the Stream message stream
         *
         * @returns double value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual double readDouble() const throw ( cms::MessageEOFException,
                                                  cms::MessageFormatException,
                                                  cms::MessageNotReadableException,
                                                  cms::CMSException );

        /**
         * Writes a double to the Stream message stream as a 8 byte value
         * @param value
         *      double to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeDouble( double value ) throw ( cms::MessageNotWriteableException,
                                                         cms::CMSException );

        /**
         * Reads a 16 bit signed short from the Stream message stream
         *
         * @returns short value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual short readShort() const throw ( cms::MessageEOFException,
                                                cms::MessageFormatException,
                                                cms::MessageNotReadableException,
                                                cms::CMSException );

        /**
         * Writes a signed short to the Stream message stream as a 2 byte value
         *
         * @param value
         *      signed short to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeShort( short value ) throw ( cms::MessageNotWriteableException,
                                                       cms::CMSException );

        /**
         * Reads a 16 bit unsigned short from the Stream message stream
         *
         * @returns unsigned short value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual unsigned short readUnsignedShort() const throw ( cms::MessageEOFException,
                                                                 cms::MessageFormatException,
                                                                 cms::MessageNotReadableException,
                                                                 cms::CMSException );

        /**
         * Writes a unsigned short to the Stream message stream as a 2 byte value
         *
         * @param value
         *      unsigned short to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( cms::MessageNotWriteableException,
                                                                        cms::CMSException );

        /**
         * Reads a 32 bit signed integer from the Stream message stream
         *
         * @returns int value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual int readInt() const throw ( cms::MessageEOFException,
                                            cms::MessageFormatException,
                                            cms::MessageNotReadableException,
                                            cms::CMSException );

        /**
         * Writes a signed int to the Stream message stream as a 4 byte value
         *
         * @param value
         *      signed int to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeInt( int value ) throw ( cms::MessageNotWriteableException,
                                                   cms::CMSException );

        /**
         * Reads a 64 bit long from the Stream message stream
         *
         * @returns long long value from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual long long readLong() const throw ( cms::MessageEOFException,
                                                   cms::MessageFormatException,
                                                   cms::MessageNotReadableException,
                                                   cms::CMSException );

        /**
         * Writes a long long to the Stream message stream as a 8 byte value
         *
         * @param value
         *      signed long long to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeLong( long long value ) throw ( cms::MessageNotWriteableException,
                                                          cms::CMSException );

        /**
         * Reads an ASCII String from the Stream message stream
         *
         * @returns String from stream
         * @throw CMSException - if the CMS provider fails to read the message
         *                       due to some internal error.
         * @throw MessageEOFException - if unexpected end of message stream has been reached.
         * @throw MessageFormatException - if this type conversion is invalid.
         * @throw MessageNotReadableException - if the message is in write-only mode.
         */
        virtual std::string readString() const throw ( cms::MessageEOFException,
                                                       cms::MessageFormatException,
                                                       cms::MessageNotReadableException,
                                                       cms::CMSException );

        /**
         * Writes an ASCII String to the Stream message stream
         *
         * @param value
         *      String to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeString( const std::string& value )throw ( cms::MessageNotWriteableException,
                                                                    cms::CMSException );

    private:

        void storeContent();

        void initializeReading() const;

        void initializeWriting();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGE_H_*/
