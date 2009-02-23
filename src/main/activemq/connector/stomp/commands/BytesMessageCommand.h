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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_

#include <cms/BytesMessage.h>
#include <activemq/util/Config.h>
#include <activemq/connector/stomp/commands/StompMessage.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Long.h>
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Implements the interface for a cms::BytesMessage.  Uses the template
     * class StompMessage to implement all cms::Message type functionality
     * and implements the BytesMessage interface here.
     */
    class AMQCPP_API BytesMessageCommand : public StompMessage< cms::BytesMessage > {
    private:

        /**
         * InputStream that wraps around the frame's body when in read-only
         * mode.
         */
        mutable decaf::io::ByteArrayInputStream inputStream;

        /**
         * OutputStream that wraps around the frame's body when in write-only
         * mode.
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

    public:

        /**
         * Default constructor.  Initializes in write-only mode.
         */
        BytesMessageCommand() :
            StompMessage< cms::BytesMessage >(),
            dataInputStream(&inputStream),
            dataOutputStream(&outputStream) {

            initialize( getFrame() );
            clearBody();
        }

        /**
         * Constructor for initialization in read-only mode.
         * @param frame The stomp frame that was received from the broker.
         */
        BytesMessageCommand( StompFrame* frame ) :
            StompMessage< cms::BytesMessage >( frame ),
            dataInputStream(&inputStream),
            dataOutputStream(&outputStream) {

            validate( getFrame() );
            reset();
        }

        virtual ~BytesMessageCommand() {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return dynamic_cast<StompCommand*>( this->clone() );
        }

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody(){

            // Invoke base class's version.
            StompMessage<cms::BytesMessage>::clearBody();

            outputStream.setBuffer( getBytes() );
        }

        /**
         * Marshals the command to a stomp frame.
         * @returns the stomp frame representation of this
         * command.
         * @throws MarshalException if the command is not
         * in a state that can be marshaled.
         */
        virtual const StompFrame& marshal() throw ( marshal::MarshalException ) {

            try{
                // Before we send out the frame tag it with the content length
                // as this is a bytes message and we can't ensure we have only
                // a trailing NULL.
                setPropertyValue(
                    CommandConstants::toString(
                        CommandConstants::HEADER_CONTENTLENGTH),
                        decaf::lang::Long::toString( getFrame().getBodyLength() ) );

                return StompMessage<cms::BytesMessage>::marshal();
            }
            AMQ_CATCH_RETHROW( marshal::MarshalException )
            AMQ_CATCHALL_THROW( marshal::MarshalException )
        }

        /**
         * Puts the message body in read-only mode and repositions the stream
         * of bytes to the beginning.
         * @throws CMSException
         */
        virtual void reset() throw ( cms::CMSException ){
            this->setReadOnlyBody( true );
            inputStream.setBuffer( getBytes() );
        }

        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::BytesMessage* clone() const {

            BytesMessageCommand* command =
                new BytesMessageCommand( getFrame().clone() );
            command->setAckHandler( this->getAckHandler() );
            command->setReadOnlyBody( this->isReadOnlyBody() );
            command->setReadOnlyProperties( this->isReadOnlyProperties() );

            return command;
        }

        /**
         * sets the bytes given to the message body.
         * @param buffer Byte Buffer to copy
         * @param numBytes Number of bytes in Buffer to copy
         * @throws CMSException
         */
        virtual void setBodyBytes( const unsigned char* buffer,
                                   std::size_t numBytes )
            throw( cms::CMSException ) {

            checkReadOnlyBody();
            this->setBytes( buffer, numBytes );
        }

        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes() const {
            const std::vector<unsigned char>& bytes = getBytes();
            if( bytes.size() == 0 ){
                return NULL;
            }

            return &this->getBytes()[0];
        }

        /**
         * Returns the number of bytes contained in the body of this message.
         * @return number of bytes.
         */
        virtual std::size_t getBodyLength() const {
            return this->getNumBytes();
        }

        /**
         * Reads a Boolean from the Bytes message stream
         * @returns boolean value from stream
         * @throws CMSException
         */
        virtual bool readBoolean() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readBoolean();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a boolean to the bytes message stream as a 1-byte value.
         * The value true is written as the value (byte)1; the value false
         * is written as the value (byte)0.
         * @param value - boolean to write to the stream
         * @throws CMSException
         */
        virtual void writeBoolean( bool value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeBoolean( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a Byte from the Bytes message stream
         * @returns unsigned char value from stream
         * @throws CMSException
         */
        virtual unsigned char readByte() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readByte();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a byte to the bytes message stream as a 1-byte value
         * @param value - byte to write to the stream
         * @throws CMSException
         */
        virtual void writeByte( unsigned char value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeByte( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

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
            throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.read( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a byte array to the bytes message stream using the vector
         * size as the number of bytes to write.
         * @param value - bytes to write to the stream
         * @throws CMSException
         */
        virtual void writeBytes( const std::vector<unsigned char>& value )
            throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.write( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

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
            throw ( cms::CMSException )
        {
            try{
                checkWriteOnlyBody();
                return dataInputStream.read( buffer, 0, length );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

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
                                 std::size_t length ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.write( value, offset, length );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a Char from the Bytes message stream
         * @returns char value from stream
         * @throws CMSException
         */
        virtual char readChar() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readChar();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a char to the bytes message stream as a 1-byte value
         * @param value - char to write to the stream
         * @throws CMSException
         */
        virtual void writeChar( char value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeChar( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 32 bit float from the Bytes message stream
         * @returns double value from stream
         * @throws CMSException
         */
        virtual float readFloat() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readFloat();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a float to the bytes message stream as a 4 byte value
         * @param value - float to write to the stream
         * @throws CMSException
         */
        virtual void writeFloat( float value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeFloat( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 64 bit double from the Bytes message stream
         * @returns double value from stream
         * @throws CMSException
         */
        virtual double readDouble() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readDouble();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a double to the bytes message stream as a 8 byte value
         * @param value - double to write to the stream
         * @throws CMSException
         */
        virtual void writeDouble( double value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeDouble( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 16 bit signed short from the Bytes message stream
         * @returns short value from stream
         * @throws CMSException
         */
        virtual short readShort() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readShort();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a signed short to the bytes message stream as a 2 byte value
         * @param value - signed short to write to the stream
         * @throws CMSException
         */
        virtual void writeShort( short value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeShort( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 16 bit unsigned short from the Bytes message stream
         * @returns unsigned short value from stream
         * @throws CMSException
         */
        virtual unsigned short readUnsignedShort() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readUnsignedShort();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a unsigned short to the bytes message stream as a 2 byte value
         * @param value - unsigned short to write to the stream
         * @throws CMSException
         */
        virtual void writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeUnsignedShort( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 32 bit signed integer from the Bytes message stream
         * @returns int value from stream
         * @throws CMSException
         */
        virtual int readInt() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readInt();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a signed int to the bytes message stream as a 4 byte value
         * @param value - signed int to write to the stream
         * @throws CMSException
         */
        virtual void writeInt( int value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeInt( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads a 64 bit long from the Bytes message stream
         * @returns long long value from stream
         * @throws CMSException
         */
        virtual long long readLong() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readLong();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes a long long to the bytes message stream as a 8 byte value
         * @param value - signed long long to write to the stream
         * @throws CMSException
         */
        virtual void writeLong( long long value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeLong( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads an ASCII String from the Bytes message stream
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readString() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readString();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes an ASCII String to the Bytes message stream
         * @param value The string to be written to the stream.
         * @throws CMSException
         */
        virtual void writeString( const std::string& value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeChars( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Reads an UTF String from the BytesMessage stream
         * @returns String from stream
         * @throws CMSException
         */
        virtual std::string readUTF() const throw ( cms::CMSException ){
            try{
                checkWriteOnlyBody();
                return dataInputStream.readUTF();
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

        /**
         * Writes an UTF String to the BytesMessage stream
         * @param value The string to be written to the stream.
         * @throws CMSException
         */
        virtual void writeUTF( const std::string& value ) throw ( cms::CMSException ){
            try{
                checkReadOnlyBody();
                dataOutputStream.writeUTF( value );
            }
            AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
            AMQ_CATCH_EXCEPTION_CONVERT( decaf::lang::Exception, exceptions::ActiveMQException )
            AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
        }

    protected:

        /**
         * Throws an exception if not in write-only mode.
         * @throws CMSException.
         */
        void checkWriteOnlyBody() const throw ( cms::CMSException ){
            if( !this->isReadOnlyBody() ){
                throw activemq::exceptions::ActiveMQException( __FILE__, __LINE__,
                    "message is in write-only mode and cannot be read from" );
            }
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_*/
