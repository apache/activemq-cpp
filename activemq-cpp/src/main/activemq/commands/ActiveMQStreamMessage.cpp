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
#include <activemq/commands/ActiveMQStreamMessage.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::ActiveMQStreamMessage() :
    ActiveMQMessageTemplate< cms::StreamMessage >() {

    this->clearBody();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::~ActiveMQStreamMessage() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::getDataStructureType() const {
    return ActiveMQStreamMessage::ID_ACTIVEMQSTREAMMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::beforeMarshal( WireFormat* wireFormat )
    throw ( decaf::io::IOException ) {

    try{

        // Let the base class do its thing.
        ActiveMQMessageTemplate<cms::StreamMessage>::beforeMarshal( wireFormat );

//        if( map != NULL && !map->isEmpty() ) {
//            // Marshal as Content.
//            PrimitiveMapMarshaller::marshal( map, getContent() );
//        } else {
//            clearBody();
//        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::reset() throw ( cms::CMSException ) {

    try{
        this->setReadOnlyBody( true );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::readBoolean() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBoolean( bool value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::readByte() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeByte( unsigned char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( std::vector<unsigned char>& value ) const
    throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const std::vector<unsigned char>& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( unsigned char*& buffer, std::size_t length ) const
    throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const unsigned char* value,
                                       std::size_t offset,
                                       std::size_t length ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQStreamMessage::readChar() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeChar( char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQStreamMessage::readFloat() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeFloat( float value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQStreamMessage::readDouble() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeDouble( double value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQStreamMessage::readShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeShort( short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQStreamMessage::readUnsignedShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readInt() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeInt( int value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQStreamMessage::readLong() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeLong( long long value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::readString() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return "";
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeString( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
