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
#include <activemq/wireformat/openwire/marshal/PrimitiveTypesMarshaller.h>

#include <algorithm>

#include <decaf/lang/Math.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::ActiveMQStreamMessage() :
    ActiveMQMessageTemplate< cms::StreamMessage >(), currentPos( 0 ) {

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
ActiveMQStreamMessage* ActiveMQStreamMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQStreamMessage> message( new ActiveMQStreamMessage() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::StreamMessage>::copyDataStructure( src );

    const ActiveMQStreamMessage* srcStream =
        dynamic_cast< const ActiveMQStreamMessage* >( src );

    if( srcStream != NULL && srcStream->list.get() != NULL ) {
        this->list.reset( new util::PrimitiveList( *srcStream->list ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::toString() const{
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQStreamMessage" << std::endl;
    stream << ActiveMQMessageTemplate<cms::StreamMessage>::toString();
    stream << "End Class = ActiveMQStreamMessage" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::StreamMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::checkWriteOnlyBody() const throw ( cms::CMSException ){
    if( !this->isReadOnlyBody() ){
        throw exceptions::ActiveMQException(
            __FILE__, __LINE__,
            "message is in read-only mode and "
            "cannot be written to" ).convertToCMSException();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::beforeMarshal( WireFormat* wireFormat )
    throw ( decaf::io::IOException ) {

    try{

        // Let the base class do its thing.
        ActiveMQMessageTemplate<cms::StreamMessage>::beforeMarshal( wireFormat );

        if( this->list.get() != NULL && !this->list->isEmpty() ) {
            // Marshal as Content.
            PrimitiveTypesMarshaller::marshal( this->list.get(), getContent() );
        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList& ActiveMQStreamMessage::getList() throw ( NullPointerException ) {

    try{

        this->checkListIsUnmarshalled();
        return *( this->list );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
const PrimitiveList& ActiveMQStreamMessage::getList() const
    throw ( decaf::lang::exceptions::NullPointerException ) {

    try{

        this->checkListIsUnmarshalled();
        return *( this->list );
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::checkListIsUnmarshalled() const
    throw ( decaf::lang::exceptions::NullPointerException ) {

    try {

        if( this->list.get() == NULL ) {

            this->list.reset( new PrimitiveList() );

            if( getContent().size() != 0 ){
                PrimitiveTypesMarshaller::unmarshal( this->list.get(), getContent() );
            }

            if( this->list.get() == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__,
                    "ActiveMQStreamMessage::getMap() - All attempts to create a "
                    "list have failed." );
            }
        }
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::reset() throw ( cms::CMSException ) {

    try{
        this->setReadOnlyBody( true );
        this->currentPos = 0;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::readBoolean() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getBool( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBoolean( bool value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::readByte() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getByte( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeByte( unsigned char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( std::vector<unsigned char>& value ) const
    throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        value = this->getList().getByteArray( this->currentPos++ );
        return value.size();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const std::vector<unsigned char>& value )
    throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( unsigned char*& buffer, std::size_t length ) const
    throw ( cms::CMSException ) {

    try{

        if( buffer == NULL ) {
            throw cms::CMSException( "Passed Buffer was NULL", NULL );
        }

        checkWriteOnlyBody();

        std::vector<unsigned char> temp = this->getList().getByteArray( this->currentPos++ );
        std::size_t amount = Math::min( (unsigned int)length, (unsigned int)temp.size() );

        for( std::size_t i = 0; i < amount; ++i ) {
            buffer[i] = temp.at( i );
        }

        return amount;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const unsigned char* value,
                                        std::size_t offset,
                                        std::size_t length ) throw ( cms::CMSException ) {

    try{

        checkReadOnlyBody();

        std::vector<unsigned char> temp;
        for( std::size_t i = offset; i < length; ++i ) {
            temp.push_back( value[i] );
        }

        this->getList().add( temp );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQStreamMessage::readChar() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getChar( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeChar( char value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQStreamMessage::readFloat() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getFloat( this->currentPos++ );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeFloat( float value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQStreamMessage::readDouble() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getDouble( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeDouble( double value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQStreamMessage::readShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getShort( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeShort( short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQStreamMessage::readUnsignedShort() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return (unsigned short)this->getList().getShort( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeUnsignedShort( unsigned short value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( (short)value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readInt() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getInt( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeInt( int value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQStreamMessage::readLong() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getLong( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeLong( long long value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::readString() const throw ( cms::CMSException ) {

    try{
        checkWriteOnlyBody();
        return this->getList().getString( this->currentPos++ );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeString( const std::string& value ) throw ( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        this->getList().add( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
