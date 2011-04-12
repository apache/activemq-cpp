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

#include <cms/CMSException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>
#include <cms/MessageEOFException.h>
#include <cms/MessageFormatException.h>

namespace cms{

    /**
     * A BytesMessage object is used to send a message containing a stream of unsigned
     * bytes. It inherits from the Message interface and adds a bytes message body. The
     * receiver of the message supplies the interpretation of the bytes using the methods
     * added by the BytesMessage interface.
     *
     * The BytesMessage methods are based largely on those found in decaf.io.DataInputStream
     * and decaf.io.DataOutputStream.
     *
     * Although the CMS API allows the use of message properties with byte messages, they are
     * typically not used, since the inclusion of properties may affect the format.
     *
     * The primitive types can be written explicitly using methods for each type.  Because the
     * C++ language is more limited when dealing with primitive types the JMS equivalent generic
     * read and write methods that take Java objects cannot be provided in the CMS API.
     *
     * When the message is first created, and when clearBody is called, the body of the message
     * is in write-only mode. After the first call to reset has been made, the message body is
     * in read-only mode. After a message has been sent, the client that sent it can retain and
     * modify it without affecting the message that has been sent. The same message object can
     * be sent multiple times. When a message has been received, the provider has called reset
     * so that the message body is in read-only mode for the client.
     *
     * If clearBody is called on a message in read-only mode, the message body is cleared and
     * the message is in write-only mode.
     *
     * If a client attempts to read a message in write-only mode, a MessageNotReadableException
     * is thrown.
     *
     * If a client attempts to write a message in read-only mode, a MessageNotWriteableException
     * is thrown.
     *
     * @since 1.0
     */
    class CMS_API BytesMessage : public Message{
    public:

        virtual ~BytesMessage() throw();

        /**
         * sets the bytes given to the message body.
         *
         * @param buffer
         *      Byte Buffer to copy
         * @param numBytes
         *      Number of bytes in Buffer to copy
         *
         * @throws CMSException - If an internal error occurs.
         * @throws MessageNotWriteableException - if in Read Only Mode.
         */
        virtual void setBodyBytes( const unsigned char* buffer, int numBytes ) = 0;

        /**
         * Gets the bytes that are contained in this message and returns them in a newly
         * allocated array that becomes the property of the caller.  This is a copy of the
         * data contained in this message, changing the value contained in this array has
         * no effect on the data contained in this message.
         *
         * @return pointer to a byte buffer that the call owns upon completion of this
         *         method.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws MessageNotReadableException - If the message is in Write Only Mode.
         */
        virtual unsigned char* getBodyBytes() const = 0;

        /**
         * Returns the number of bytes contained in the body of this message.
         *
         * @return number of bytes.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws MessageNotReadableException - If the message is in Write Only Mode.
         */
        virtual int getBodyLength() const = 0;

        /**
         * Puts the message body in read-only mode and repositions the stream
         * of bytes to the beginning.
         *
         * @throws CMSException - If the provider fails to perform the reset operation.
         * @throws MessageFormatException - If the Message has an invalid format.
         */
        virtual void reset() = 0;

        /**
         * Reads a Boolean from the Bytes message stream
         * @returns boolean value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual bool readBoolean() const = 0;

        /**
         * Writes a boolean to the bytes message stream as a 1-byte value.
         * The value true is written as the value (byte)1; the value false
         * is written as the value (byte)0.
         *
         * @param value
         *      boolean to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBoolean( bool value ) = 0;

        /**
         * Reads a Byte from the Bytes message stream
         *
         * @returns unsigned char value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual unsigned char readByte() const = 0;

        /**
         * Writes a byte to the bytes message stream as a 1-byte value
         *
         * @param value
         *      byte to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeByte( unsigned char value ) = 0;

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
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual int readBytes( std::vector<unsigned char>& value ) const = 0;

        /**
         * Writes a byte array to the bytes message stream using the vector
         * size as the number of bytes to write.
         *
         * @param value
         *      bytes to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBytes( const std::vector<unsigned char>& value ) = 0;

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
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual int readBytes( unsigned char* buffer, int length ) const = 0;

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
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeBytes( const unsigned char* value, int offset, int length ) = 0;

        /**
         * Reads a Char from the Bytes message stream
         *
         * @returns char value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual char readChar() const = 0;

        /**
         * Writes a char to the bytes message stream as a 1-byte value
         *
         * @param value
         *      char to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeChar( char value ) = 0;

        /**
         * Reads a 32 bit float from the Bytes message stream
         *
         * @returns double value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual float readFloat() const = 0;

        /**
         * Writes a float to the bytes message stream as a 4 byte value
         * @param value
         *      float to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeFloat( float value ) = 0;

        /**
         * Reads a 64 bit double from the Bytes message stream
         *
         * @returns double value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual double readDouble() const = 0;

        /**
         * Writes a double to the bytes message stream as a 8 byte value
         * @param value
         *      double to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeDouble( double value ) = 0;

        /**
         * Reads a 16 bit signed short from the Bytes message stream
         *
         * @returns short value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual short readShort() const = 0;

        /**
         * Writes a signed short to the bytes message stream as a 2 byte value
         *
         * @param value
         *      signed short to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeShort( short value ) = 0;

        /**
         * Reads a 16 bit unsigned short from the Bytes message stream
         *
         * @returns unsigned short value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual unsigned short readUnsignedShort() const = 0;

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         *
         * @param value
         *      unsigned short to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeUnsignedShort( unsigned short value ) = 0;

        /**
         * Reads a 32 bit signed integer from the Bytes message stream
         *
         * @returns int value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual int readInt() const = 0;

        /**
         * Writes a signed int to the bytes message stream as a 4 byte value
         *
         * @param value
         *      signed int to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeInt( int value ) = 0;

        /**
         * Reads a 64 bit long from the Bytes message stream
         *
         * @returns long long value from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual long long readLong() const = 0;

        /**
         * Writes a long long to the bytes message stream as a 8 byte value
         *
         * @param value
         *      signed long long to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeLong( long long value ) = 0;

        /**
         * Reads an ASCII String from the Bytes message stream
         *
         * @returns String from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual std::string readString() const = 0;

        /**
         * Writes an ASCII String to the Bytes message stream
         *
         * @param value
         *      String to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to write the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeString( const std::string& value ) = 0;

        /**
         * Reads an UTF String from the BytesMessage stream
         *
         * @returns String from stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageEOFException - if unexpected end of bytes stream has been reached.
         * @throws MessageNotReadableException - if the message is in write-only mode.
         */
        virtual std::string readUTF() const = 0;

        /**
         * Writes an UTF String to the BytesMessage stream
         *
         * @param value
         *      String to write to the stream
         *
         * @throws CMSException - if the CMS provider fails to read the message due to
         *                        some internal error.
         * @throws MessageNotWriteableException - if the message is in read-only mode.
         */
        virtual void writeUTF( const std::string& value ) = 0;

        /**
         * Clones this message.
         *
         * @return a deep copy of this message.
         *
         * @throws CMSException - if an internal error occurs while cloning the Message.
         */
        virtual BytesMessage* clone() const = 0;

   };
}

#endif /*_CMS_BYTESMESSAGE_H_*/
