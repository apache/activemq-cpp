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
#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::ActiveMQBytesMessage() : 
    ActiveMQMessageBase< cms::BytesMessage >()
{
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
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char* ActiveMQBytesMessage::getBodyBytes(void) const {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::getBodyLength(void) const {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::reset() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBytesMessage::readBoolean() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBoolean( bool value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::readByte() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeByte( unsigned char value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( std::vector<unsigned char>& value ) 
    throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const std::vector<unsigned char>& value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( unsigned char*& buffer, std::size_t length ) 
    throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const unsigned char* value,
                         std::size_t offset,
                         std::size_t length ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
                         
////////////////////////////////////////////////////////////////////////////////
char ActiveMQBytesMessage::readChar() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeChar( char value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQBytesMessage::readFloat() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeFloat( float value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQBytesMessage::readDouble() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeDouble( double value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQBytesMessage::readShort() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeShort( short value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQBytesMessage::readUnsignedShort() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQBytesMessage::readInt() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeInt( int value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQBytesMessage::readLong() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeLong( long long value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readString() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeString( const std::string& value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readUTF() throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUTF( const std::string& value ) throw ( cms::CMSException ) {

    try{
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
