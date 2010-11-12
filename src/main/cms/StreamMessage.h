/**
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

#ifndef _CMS_STREAMMESSAGE_H_
#define _CMS_STREAMMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>
#include <cms/CMSException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageEOFException.h>

namespace cms {

    /**
     * Interface for a StreamMessage.  The stream Messages provides a Message
     * type whose body is a stream of self describing primitive types.  The primitive
     * values are read and written using accessors specific to the given types.
     *
     * StreamMessage objects support the following conversion table. The marked cases
     * must be supported. The unmarked cases must throw a CMSException. The string-to-
     * primitive conversions may throw a runtime exception if the primitive's valueOf()
     * method does not accept it as a valid String representation of the primitive.
     *
     * A value written as the row type can be read as the column type.
     *
     * <PRE>
     *    |        | boolean byte short char int long float double String byte[]
     *    |----------------------------------------------------------------------
     *    |boolean |    X                                            X
     *    |byte    |          X     X         X   X                  X
     *    |short   |                X         X   X                  X
     *    |char    |                     X                           X
     *    |int     |                          X   X                  X
     *    |long    |                              X                  X
     *    |float   |                                    X     X      X
     *    |double  |                                          X      X
     *    |String  |    X     X     X         X   X     X     X      X
     *    |byte[]  |                                                        X
     *    |----------------------------------------------------------------------
     * </PRE>
     *
     * @since 1.3
     */
    class CMS_API StreamMessage : public Message {
    public:

        virtual ~StreamMessage() throw();

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
        virtual bool readBoolean() const = 0;

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
        virtual void writeBoolean( bool value ) = 0;

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
        virtual unsigned char readByte() const = 0;

        /**
         * Writes a byte to the Stream message stream as a 1-byte value
         *
         * @param value
         *      byte to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeByte( unsigned char value ) = 0;

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
        virtual int readBytes( std::vector<unsigned char>& value ) const = 0;

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
        virtual void writeBytes( const std::vector<unsigned char>& value ) = 0;

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
        virtual int readBytes( unsigned char* buffer, int length ) const = 0;

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
        virtual void writeBytes( const unsigned char* value, int offset, int length ) = 0;

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
        virtual char readChar() const = 0;

        /**
         * Writes a char to the Stream message stream as a 1-byte value
         *
         * @param value
         *      char to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeChar( char value ) = 0;

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
        virtual float readFloat() const = 0;

        /**
         * Writes a float to the Stream message stream as a 4 byte value
         * @param value
         *      float to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeFloat( float value ) = 0;

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
        virtual double readDouble() const = 0;

        /**
         * Writes a double to the Stream message stream as a 8 byte value
         * @param value
         *      double to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeDouble( double value ) = 0;

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
        virtual short readShort() const = 0;

        /**
         * Writes a signed short to the Stream message stream as a 2 byte value
         *
         * @param value
         *      signed short to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeShort( short value ) = 0;

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
        virtual unsigned short readUnsignedShort() const = 0;

        /**
         * Writes a unsigned short to the Stream message stream as a 2 byte value
         *
         * @param value
         *      unsigned short to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeUnsignedShort( unsigned short value ) = 0;

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
        virtual int readInt() const = 0;

        /**
         * Writes a signed int to the Stream message stream as a 4 byte value
         *
         * @param value
         *      signed int to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeInt( int value ) = 0;

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
        virtual long long readLong() const = 0;

        /**
         * Writes a long long to the Stream message stream as a 8 byte value
         *
         * @param value
         *      signed long long to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeLong( long long value ) = 0;

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
        virtual std::string readString() const = 0;

        /**
         * Writes an ASCII String to the Stream message stream
         *
         * @param value
         *      String to write to the stream
         * @throws CMSException - if the CMS provider fails to write the message due
         *                        to some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeString( const std::string& value ) = 0;

    };

}

#endif /*_CMS_STREAMMESSAGE_H_*/
