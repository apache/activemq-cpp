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

#ifndef _CMS_BYTESMESSAGE_H_
#define _CMS_BYTESMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>

namespace cms{

    /**
     * A message used for sending a stream of uninterpreted bytes.
     */
    class CMS_API BytesMessage : public Message{
    public:

        virtual ~BytesMessage(){}

        /**
         * sets the bytes given to the message body.
         * 
         * @param buffer 
         *      Byte Buffer to copy
         * @param numBytes 
         *      Number of bytes in Buffer to copy
         * @throws CMSException
         */
        virtual void setBodyBytes(
            const unsigned char* buffer, std::size_t numBytes )
                throw( CMSException ) = 0;

        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * 
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes() const = 0;

        /**
         * Returns the number of bytes contained in the body of this message.
         * 
         * @return number of bytes.
         */
        virtual std::size_t getBodyLength() const = 0;

        /**
         * Puts the message body in read-only mode and repositions the stream
         * of bytes to the beginning.
         * 
         * @throws CMSException
         */
        virtual void reset() throw ( cms::CMSException ) = 0;

        /**
         * Reads a Boolean from the Bytes message stream
         * @returns boolean value from stream
         * 
         * @throws CMSException
         */
        virtual bool readBoolean() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a boolean to the bytes message stream as a 1-byte value.
         * The value true is written as the value (byte)1; the value false
         * is written as the value (byte)0.
         * 
         * @param value 
         *      boolean to write to the stream
         * @throws CMSException
         */
        virtual void writeBoolean( bool value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a Byte from the Bytes message stream
         * 
         * @returns unsigned char value from stream
         * @throws CMSException
         */
        virtual unsigned char readByte() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a byte to the bytes message stream as a 1-byte value
         * 
         * @param value 
         *      byte to write to the stream
         * @throws CMSException
         */
        virtual void writeByte( unsigned char value ) throw ( cms::CMSException ) = 0;

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
         * @param value 
         *      buffer to place data in
         * @returns the total number of bytes read into the buffer, or -1 if
         *          there is no more data because the end of the stream has
         *          been reached
         * @throws CMSException if an error occurs.
         */
        virtual std::size_t readBytes( std::vector<unsigned char>& value ) const
            throw ( cms::CMSException ) = 0;

        /**
         * Writes a byte array to the bytes message stream using the vector
         * size as the number of bytes to write.
         * 
         * @param value 
         *      bytes to write to the stream
         * @throws CMSException
         */
        virtual void writeBytes( const std::vector<unsigned char>& value )
            throw ( cms::CMSException ) = 0;

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
         * @param buffer 
         *      the buffer into which the data is read
         * @param length 
         *      the number of bytes to read; must be less than or equal to 
         *      value.length
         * @returns the total number of bytes read into the buffer, or -1 if
         *          there is no more data because the end of the stream has
         *          been reached
         * @throws CMSException
         */
        virtual std::size_t readBytes( unsigned char*& buffer, std::size_t length ) const
            throw ( cms::CMSException ) = 0;

        /**
         * Writes a portion of a byte array to the bytes message stream.
         * size as the number of bytes to write.
         * 
         * @param value 
         *      bytes to write to the stream
         * @param offset 
         *      the initial offset within the byte array
         * @param length 
         *      the number of bytes to use
         * @throws CMSException
         */
        virtual void writeBytes( const unsigned char* value,
                                 std::size_t offset,
                                 std::size_t length ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a Char from the Bytes message stream
         * 
         * @returns char value from stream
         * @throws CMSException
         */
        virtual char readChar() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a char to the bytes message stream as a 1-byte value
         * 
         * @param value
         *      char to write to the stream
         * @throws CMSException
         */
        virtual void writeChar( char value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 32 bit float from the Bytes message stream
         * 
         * @returns double value from stream
         * @throws CMSException
         */
        virtual float readFloat() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a float to the bytes message stream as a 4 byte value
         * @param value 
         *      float to write to the stream
         * @throws CMSException
         */
        virtual void writeFloat( float value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 64 bit double from the Bytes message stream
         * 
         * @returns double value from stream
         * @throws CMSException
         */
        virtual double readDouble() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a double to the bytes message stream as a 8 byte value
         * @param value 
         *      double to write to the stream
         * @throws CMSException
         */
        virtual void writeDouble( double value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 16 bit signed short from the Bytes message stream
         * 
         * @returns short value from stream
         * @throws CMSException
         */
        virtual short readShort() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a signed short to the bytes message stream as a 2 byte value
         * 
         * @param value 
         *      signed short to write to the stream
         * @throws CMSException
         */
        virtual void writeShort( short value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 16 bit unsigned short from the Bytes message stream
         * 
         * @returns unsigned short value from stream
         * @throws CMSException
         */
        virtual unsigned short readUnsignedShort() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         * 
         * @param value 
         *      unsigned short to write to the stream
         * @throws CMSException
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 32 bit signed intger from the Bytes message stream
         * 
         * @returns int value from stream
         * @throws CMSException
         */
        virtual int readInt() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a signed int to the bytes message stream as a 4 byte value
         * 
         * @param value 
         *      signed int to write to the stream
         * @throws CMSException
         */
        virtual void writeInt( int value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads a 64 bit long from the Bytes message stream
         * 
         * @returns long long value from stream
         * @throws CMSException
         */
        virtual long long readLong() const throw ( cms::CMSException ) = 0;

        /**
         * Writes a long long to the bytes message stream as a 8 byte value
         * 
         * @param value 
         *      signed long long to write to the stream
         * @throws CMSException
         */
        virtual void writeLong( long long value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads an ASCII String from the Bytes message stream
         * 
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readString() const throw ( cms::CMSException ) = 0;

        /**
         * Writes an ASCII String to the Bytes message stream
         * 
         * @param value 
         *      String to write to the stream
         * @throws CMSException
         */
        virtual void writeString( const std::string& value ) throw ( cms::CMSException ) = 0;

        /**
         * Reads an UTF String from the BytesMessage stream
         * 
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readUTF() const throw ( cms::CMSException ) = 0;

        /**
         * Writes an UTF String to the BytesMessage stream
         * 
         * @param value 
         *      String to write to the stream
         * @throws CMSException
         */
        virtual void writeUTF( const std::string& value ) throw ( cms::CMSException ) = 0;

        /**
         * Clones this message.
         * @return a deep copy of this message.
         */
        virtual BytesMessage* clone() const = 0;

   };
}

#endif /*_CMS_BYTESMESSAGE_H_*/
