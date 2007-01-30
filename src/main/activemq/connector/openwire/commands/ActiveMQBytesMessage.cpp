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
#include <activemq/connector/openwire/commands/ActiveMQBytesMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::ActiveMQBytesMessage() : 
    ActiveMQMessageBase< cms::BytesMessage >(),
    dataInputStream( &inputStream ),
    dataOutputStream( &outputStream )
{
    clearBody();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::~ActiveMQBytesMessage()
{
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::getDataStructureType() const
{
    return ActiveMQBytesMessage::ID_ACTIVEMQBYTESMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::setBodyBytes( const unsigned char* buffer, 
                                         std::size_t numBytes ) 
    throw( cms::CMSException ) {

    try{
        
        clearBody();
        for( size_t i = 0; i < numBytes; ++i ) {
            getContent().push_back( buffer[i] );
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char* ActiveMQBytesMessage::getBodyBytes(void) const {

    try{
        return reinterpret_cast<const unsigned char*>( &( getContent()[0] ) );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::getBodyLength(void) const {

    try{
        return getContent().size();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::reset() throw ( cms::CMSException ) {

    try{
        readOnly = true;
        inputStream.setBuffer( getContent() );            
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBytesMessage::readBoolean() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readBoolean();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBoolean( bool value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeBoolean( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::readByte() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readByte();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeByte( unsigned char value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeByte( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( std::vector<unsigned char>& value ) 
    throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.read( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const std::vector<unsigned char>& value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.write( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( unsigned char*& buffer, std::size_t length ) 
    throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.read( buffer, 0, length );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const unsigned char* value,
                         std::size_t offset,
                         std::size_t length ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.write( value, offset, length );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
                         
////////////////////////////////////////////////////////////////////////////////
char ActiveMQBytesMessage::readChar() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readChar();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeChar( char value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeChar( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQBytesMessage::readFloat() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readFloat();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeFloat( float value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeFloat( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQBytesMessage::readDouble() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readDouble();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeDouble( double value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeDouble( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQBytesMessage::readShort() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readShort();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeShort( short value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeShort( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQBytesMessage::readUnsignedShort() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readUnsignedShort();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeUnsignedShort( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQBytesMessage::readInt() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readInt();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeInt( int value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeInt( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQBytesMessage::readLong() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readLong();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeLong( long long value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeLong( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readString() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readString();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeString( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeBytes( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readUTF() throw ( cms::CMSException ) {

    try{
        checkReadOnly();
        return dataInputStream.readUTF();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUTF( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkWriteOnly();
        dataOutputStream.writeUTF( value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
