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
using namespace activemq::exceptions;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::ActiveMQBytesMessage() :
    ActiveMQMessageBase< cms::BytesMessage >(),
    dataInputStream( &inputStream ),
    dataOutputStream( &outputStream ) {

    clearBody();
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::getDataStructureType() const {
    return ActiveMQBytesMessage::ID_ACTIVEMQBYTESMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::setBodyBytes( const unsigned char* buffer,
                                         std::size_t numBytes )
    throw( cms::CMSException ) {

    try{

        clearBody();
        std::vector<unsigned char>& content = getContent();
        content.insert( content.end(), buffer, buffer + numBytes );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char* ActiveMQBytesMessage::getBodyBytes(void) const {

    try{

        if( getContent().size() > 0 ) {
            return reinterpret_cast<const unsigned char*>( &( getContent()[0] ) );
        } else {
            return NULL;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::getBodyLength(void) const {

    try{
        return getContent().size();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::reset() throw ( cms::CMSException ) {

    try{
        this->setReadOnlyBody( true );
        inputStream.setBuffer( getContent() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBytesMessage::readBoolean() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readBoolean();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBoolean( bool value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeBoolean( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::readByte() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readByte();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeByte( unsigned char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeByte( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( std::vector<unsigned char>& value ) const
    throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.read( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const std::vector<unsigned char>& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.write( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( unsigned char*& buffer, std::size_t length ) const
    throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.read( buffer, 0, length );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const unsigned char* value,
                         std::size_t offset,
                         std::size_t length ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.write( value, offset, length );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQBytesMessage::readChar() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readChar();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeChar( char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeChar( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQBytesMessage::readFloat() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readFloat();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeFloat( float value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeFloat( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQBytesMessage::readDouble() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readDouble();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeDouble( double value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeDouble( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQBytesMessage::readShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readShort();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeShort( short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeShort( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQBytesMessage::readUnsignedShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readUnsignedShort();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeUnsignedShort( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQBytesMessage::readInt() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readInt();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeInt( int value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeInt( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQBytesMessage::readLong() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readLong();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeLong( long long value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeLong( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readString() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readString();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeString( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeChars( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readUTF() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return dataInputStream.readUTF();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUTF( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        dataOutputStream.writeUTF( value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
