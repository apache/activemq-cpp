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
#include <activemq/commands/ActiveMQBytesMessage.h>

#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/EOFException.h>
#include <decaf/io/IOException.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::ActiveMQBytesMessage() :
    ActiveMQMessageTemplate< cms::BytesMessage >(), length( 0 ) {

    this->clearBody();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage::~ActiveMQBytesMessage() {
    this->reset();
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::getDataStructureType() const {
    return ActiveMQBytesMessage::ID_ACTIVEMQBYTESMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQBytesMessage* ActiveMQBytesMessage::cloneDataStructure() const {
    std::auto_ptr<ActiveMQBytesMessage> message( new ActiveMQBytesMessage() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::copyDataStructure( const DataStructure* src ) {

    // Protect against invalid self assignment.
    if( this == src ) {
        return;
    }

    const ActiveMQBytesMessage* srcPtr = dynamic_cast<const ActiveMQBytesMessage*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQBytesMessage::copyDataStructure - src is NULL or invalid" );
    }

    ActiveMQBytesMessage* nonConstSrc = const_cast<ActiveMQBytesMessage*>( srcPtr );
    nonConstSrc->storeContent();

    ActiveMQMessageTemplate<cms::BytesMessage>::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::toString() const{
    return ActiveMQMessageTemplate<cms::BytesMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBytesMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::BytesMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::setBodyBytes( const unsigned char* buffer, std::size_t numBytes )
    throw( cms::MessageNotWriteableException, cms::CMSException ) {

    try{

        initializeWriting();
        dataOut->write( buffer, numBytes, 0, numBytes );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char* ActiveMQBytesMessage::getBodyBytes() const
    throw( cms::MessageNotReadableException, cms::CMSException ) {

    try{

        initializeReading();

        std::size_t length = this->getBodyLength();

        if( length != 0 ) {
            unsigned char* buffer = new unsigned char[length];
            this->readBytes( buffer, length );
            return buffer;
        } else {
            return NULL;
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::getBodyLength() const
    throw( cms::MessageNotReadableException, cms::CMSException ) {

    try{
        initializeReading();
        return this->length;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::clearBody() throw( cms::CMSException ) {

    // Invoke base class's version.
    ActiveMQMessageTemplate<cms::BytesMessage>::clearBody();

    this->dataOut.reset( NULL );
    this->dataIn.reset( NULL );
    this->length = 0;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::onSend() {

    this->storeContent();
    ActiveMQMessageTemplate<cms::BytesMessage>::onSend();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::reset()
    throw ( cms::MessageFormatException, cms::CMSException ) {

    try{
        storeContent();
        this->bytesOut.reset( NULL );
        this->dataIn.reset( NULL );
        this->dataOut.reset( NULL );
        this->length = 0;
        this->setReadOnlyBody(true);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQBytesMessage::readBoolean() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readBoolean();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBoolean( bool value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeBoolean( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQBytesMessage::readByte() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readByte();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeByte( unsigned char value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeByte( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( std::vector<unsigned char>& value ) const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    return this->readBytes( &value[0], value.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const std::vector<unsigned char>& value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQBytesMessage::readBytes( unsigned char* buffer, std::size_t length ) const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{

        std::size_t n = 0;

        while( n < length ) {
            int count = this->dataIn->read( buffer, length, n, length - n );
            if( count < 0 ) {
                break;
            }
            n += count;
        }

        if( n == 0 && length > 0 ) {
            n = -1;
        }

        return n;

    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeBytes( const unsigned char* value, std::size_t offset, std::size_t length )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( value, length, offset, length );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQBytesMessage::readChar() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readChar();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeChar( char value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeChar( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQBytesMessage::readFloat() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readFloat();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeFloat( float value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeFloat( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQBytesMessage::readDouble() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readDouble();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeDouble( double value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeDouble( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQBytesMessage::readShort() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readShort();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeShort( short value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeShort( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQBytesMessage::readUnsignedShort() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readUnsignedShort();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUnsignedShort( unsigned short value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeUnsignedShort( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQBytesMessage::readInt() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readInt();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeInt( int value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeInt( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQBytesMessage::readLong() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readLong();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeLong( long long value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeLong( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readString() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readString();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeString( const std::string& value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeChars( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQBytesMessage::readUTF() const
    throw ( cms::MessageEOFException, cms::MessageNotReadableException, cms::CMSException ) {

    initializeReading();
    try{
        return this->dataIn->readUTF();
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( IOException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::writeUTF( const std::string& value )
    throw ( cms::MessageNotWriteableException, cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->writeUTF( value );
    } catch( EOFException& ex ) {
        throw CMSExceptionSupport::createMessageEOFException( ex );
    } catch( Exception& ex ) {
        throw CMSExceptionSupport::create( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::storeContent() {

    try {

        if( this->dataOut.get() != NULL ) {

            this->dataOut->close();

            this->setContent( this->bytesOut->toByteArrayRef() );

            this->dataOut.reset( NULL );
            this->bytesOut.reset( NULL );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::initializeReading() const {

    this->failIfWriteOnlyBody();
    try {
        if( this->dataIn.get() == NULL) {
            ByteArrayInputStream* is = new ByteArrayInputStream( this->getContent() );
            this->dataIn.reset( new DataInputStream( is, true ) );
            this->length = this->getContent().size();
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQBytesMessage::initializeWriting() {

    this->failIfReadOnlyBody();
    try{
        if( this->dataOut.get() == NULL ) {
            this->length = 0;
            this->bytesOut.reset( new ByteArrayOutputStream() );
            this->dataOut.reset( new DataOutputStream( this->bytesOut.get() ) );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
