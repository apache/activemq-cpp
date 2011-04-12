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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQBYTESMESSAGE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQBYTESMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQMessageTemplate.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <cms/BytesMessage.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq{
namespace commands{

    class AMQCPP_API ActiveMQBytesMessage :
        public ActiveMQMessageTemplate< cms::BytesMessage > {
    private:

        /**
         * OutputStream that wraps around the command's content when in
         * write-only mode.
         */
        decaf::io::ByteArrayOutputStream* bytesOut;

        /**
         * DataInputStream wrapper around the input stream.
         */
        mutable std::auto_ptr<decaf::io::DataInputStream> dataIn;

        /**
         * DataOutputStream wrapper around the output stream.
         */
        std::auto_ptr<decaf::io::DataOutputStream> dataOut;

        /**
         * Tracks the actual length of the Message when compressed.
         */
        mutable int length;

    public:

        const static unsigned char ID_ACTIVEMQBYTESMESSAGE = 24;

    private:

        ActiveMQBytesMessage( const ActiveMQBytesMessage& );
        ActiveMQBytesMessage& operator= ( const ActiveMQBytesMessage& );

    public:

        ActiveMQBytesMessage();

        virtual ~ActiveMQBytesMessage() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQBytesMessage* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

    public:   // CMS Message

        virtual cms::BytesMessage* clone() const {
            return dynamic_cast<cms::BytesMessage*>( this->cloneDataStructure() );
        }

        virtual void clearBody();

        virtual void onSend();

    public:   // CMS BytesMessage

        virtual void setBodyBytes( const unsigned char* buffer, int numBytes );

        virtual unsigned char* getBodyBytes() const;

        virtual int getBodyLength() const;

        virtual void reset();

        virtual bool readBoolean() const;

        virtual void writeBoolean( bool value );

        virtual unsigned char readByte() const;

        virtual void writeByte( unsigned char value );

        virtual int readBytes( std::vector<unsigned char>& value ) const;

        virtual void writeBytes( const std::vector<unsigned char>& value );

        virtual int readBytes( unsigned char* buffer, int length ) const;

        virtual void writeBytes( const unsigned char* value, int offset, int length );

        virtual char readChar() const;

        virtual void writeChar( char value );

        virtual float readFloat() const;

        virtual void writeFloat( float value );

        virtual double readDouble() const;

        virtual void writeDouble( double value );

        virtual short readShort() const;

        virtual void writeShort( short value );

        virtual unsigned short readUnsignedShort() const;

        virtual void writeUnsignedShort( unsigned short value );

        virtual int readInt() const;

        virtual void writeInt( int value );

        virtual long long readLong() const;

        virtual void writeLong( long long value );

        virtual std::string readString() const;

        virtual void writeString( const std::string& value );

        virtual std::string readUTF() const;

        virtual void writeUTF( const std::string& value );

    private:

        void storeContent();

        void initializeReading() const;

        void initializeWriting();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQBYTESMESSAGE_H_*/
