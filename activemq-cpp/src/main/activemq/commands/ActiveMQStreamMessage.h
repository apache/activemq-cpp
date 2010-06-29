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

        decaf::io::ByteArrayOutputStream* bytesOut;
        mutable std::auto_ptr<decaf::io::DataInputStream> dataIn;
        mutable std::auto_ptr<decaf::io::DataOutputStream> dataOut;

        mutable int remainingBytes;

    public:

        const static unsigned char ID_ACTIVEMQSTREAMMESSAGE = 27;

    private:

        ActiveMQStreamMessage( const ActiveMQStreamMessage& );
        ActiveMQStreamMessage& operator= ( const ActiveMQStreamMessage& );

    public:

        ActiveMQStreamMessage();
        virtual ~ActiveMQStreamMessage() throw();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQStreamMessage* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual void onSend();

    public:   // CMS Message

        virtual cms::StreamMessage* clone() const {
            return dynamic_cast<cms::StreamMessage*>( this->cloneDataStructure() );
        }

        virtual void clearBody();

    public: // CMS Stream Message

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

    private:

        void storeContent();

        void initializeReading() const;

        void initializeWriting();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGE_H_*/
