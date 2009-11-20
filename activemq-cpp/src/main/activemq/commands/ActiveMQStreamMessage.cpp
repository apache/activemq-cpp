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
#include <activemq/wireformat/openwire/utils/OpenwireStringSupport.h>
#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <cms/MessageEOFException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>

#include <algorithm>

#include <decaf/lang/Math.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Byte.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>
#include <decaf/io/ByteArrayInputStream.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::ActiveMQStreamMessage() :
    ActiveMQMessageTemplate< cms::StreamMessage >() {

    this->clearBody();
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQStreamMessage::~ActiveMQStreamMessage() {
    this->reset();
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

    // Protect against invalid self assignment.
    if( this == src ) {
        return;
    }

    const ActiveMQStreamMessage* srcPtr = dynamic_cast<const ActiveMQStreamMessage*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQStreamMessage::copyDataStructure - src is NULL or invalid" );
    }

    ActiveMQStreamMessage* nonConstSrc = const_cast<ActiveMQStreamMessage*>( srcPtr );
    nonConstSrc->storeContent();

    ActiveMQMessageTemplate<cms::StreamMessage>::copyDataStructure( src );
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
void ActiveMQStreamMessage::clearBody() throw( cms::CMSException ) {

    // Invoke base class's version.
    ActiveMQMessageTemplate<cms::StreamMessage>::clearBody();

    this->dataIn.reset( NULL );
    this->dataOut.reset( NULL );
    this->bytesOut.reset( NULL );
    this->remainingBytes = -1;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::onSend() {

    this->storeContent();
    ActiveMQMessageTemplate<cms::StreamMessage>::onSend();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::reset() throw ( cms::CMSException ) {

    try{
        storeContent();
        this->bytesOut.reset(NULL);
        this->dataIn.reset(NULL);
        this->dataOut.reset(NULL);
        this->remainingBytes = -1;
        this->setReadOnlyBody(true);
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQStreamMessage::readBoolean() const throw ( cms::MessageEOFException,
                                                        cms::MessageFormatException,
                                                        cms::MessageNotReadableException,
                                                        cms::CMSException ) {

    try{

        initializeReading();

        this->dataIn->mark(10);

        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "Reached the end of the Stream", NULL );
        }
        if( type == PrimitiveValueNode::BOOLEAN_TYPE ) {
            return this->dataIn->readBoolean();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Boolean::valueOf( this->dataIn->readUTF() ).booleanValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to boolean." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( "not a boolean type", NULL );
        }

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBoolean( bool value ) throw ( cms::MessageNotWriteableException,
                                                               cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::BOOLEAN_TYPE );
        this->dataOut->writeBoolean( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQStreamMessage::readByte() const throw ( cms::MessageEOFException,
                                                              cms::MessageFormatException,
                                                              cms::MessageNotReadableException,
                                                              cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark(10);
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return this->dataIn->readByte();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Byte::valueOf( this->dataIn->readUTF() ).byteValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to byte." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a byte type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& e ) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::createMessageFormatException( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeByte( unsigned char value ) throw ( cms::MessageNotWriteableException,
                                                                     cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::BYTE_TYPE );
        this->dataOut->writeByte( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( std::vector<unsigned char>& value ) const
    throw ( cms::MessageEOFException,
            cms::MessageFormatException,
            cms::MessageNotReadableException,
            cms::CMSException ) {

    if( value.size() == 0 ) {
        return 0;
    }

    return this->readBytes( &value[0], value.size() );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const std::vector<unsigned char>& value )
    throw ( cms::MessageNotWriteableException,
            cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::BYTE_ARRAY_TYPE );
        this->dataOut->writeInt( (int)value.size() );
        this->dataOut->write( &value[0], 0, value.size() );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::size_t ActiveMQStreamMessage::readBytes( unsigned char* buffer, std::size_t length ) const
    throw ( cms::MessageEOFException,
            cms::MessageFormatException,
            cms::MessageNotReadableException,
            cms::CMSException ) {

    initializeReading();
    try {

        if( buffer == NULL ) {
            throw NullPointerException( __FILE__, __LINE__, "Passed buffer was NULL" );
        }

        if( this->remainingBytes == -1 ) {

            this->dataIn->mark( (int)length + 1 );
            int type = this->dataIn->read();

            if( type == -1 ) {
                throw MessageEOFException( "reached end of data", NULL );
            }

            if( type != PrimitiveValueNode::BYTE_ARRAY_TYPE ) {
                throw MessageFormatException( "Not a byte array", NULL );
            }

            this->remainingBytes = this->dataIn->readInt();

        } else if( this->remainingBytes == 0 ) {
            remainingBytes = -1;
            return -1;
        }

        if( length <= (size_t)this->remainingBytes ) {
            // small buffer
            this->remainingBytes -= (int)length;
            this->dataIn->readFully( buffer, 0, length );
            return length;
        } else {
            // big buffer
            int rc = this->dataIn->read( buffer, 0, this->remainingBytes );
            this->remainingBytes = 0;
            return rc;
        }

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeBytes( const unsigned char* value,
                                        std::size_t offset,
                                        std::size_t length ) throw ( cms::MessageNotWriteableException,
                                                                     cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::BYTE_ARRAY_TYPE );
        this->dataOut->writeInt( (int)length );
        this->dataOut->write( value, offset, length );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQStreamMessage::readChar() const throw ( cms::MessageEOFException,
                                                     cms::MessageFormatException,
                                                     cms::MessageNotReadableException,
                                                     cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark( 17 );
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::CHAR_TYPE ) {
            return this->dataIn->readChar();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to char." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a char type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& ioe ) {
            throw CMSExceptionSupport::create( ioe );
        }

        throw CMSExceptionSupport::create( ex );;

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeChar( char value ) throw ( cms::MessageNotWriteableException,
                                                            cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::CHAR_TYPE );
        this->dataOut->writeChar( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQStreamMessage::readFloat() const throw ( cms::MessageEOFException,
                                                       cms::MessageFormatException,
                                                       cms::MessageNotReadableException,
                                                       cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark( 33 );
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::FLOAT_TYPE ) {
            return this->dataIn->readFloat();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Float::valueOf( this->dataIn->readUTF() ).floatValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE )  {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to float." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a float type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& ioe ) {
            throw CMSExceptionSupport::create( ioe );
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeFloat( float value ) throw ( cms::MessageNotWriteableException,
                                                              cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::FLOAT_TYPE );
        this->dataOut->writeFloat( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQStreamMessage::readDouble() const throw ( cms::MessageEOFException,
                                                         cms::MessageFormatException,
                                                         cms::MessageNotReadableException,
                                                         cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark( 33 );
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::DOUBLE_TYPE ) {
            return this->dataIn->readDouble();
        }
        if( type == PrimitiveValueNode::FLOAT_TYPE ) {
            return this->dataIn->readFloat();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Double::valueOf( this->dataIn->readUTF() ).doubleValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE )  {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to double." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a double type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& ioe ) {
            throw CMSExceptionSupport::create( ioe );
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeDouble( double value ) throw ( cms::MessageNotWriteableException,
                                                                cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::DOUBLE_TYPE );
        this->dataOut->writeDouble( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQStreamMessage::readShort() const throw ( cms::MessageEOFException,
                                                       cms::MessageFormatException,
                                                       cms::MessageNotReadableException,
                                                       cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark(17);
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::SHORT_TYPE ) {
            return this->dataIn->readShort();
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return this->dataIn->readByte();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Short::valueOf( this->dataIn->readUTF() ).shortValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to short." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a short type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& e ) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeShort( short value ) throw ( cms::MessageNotWriteableException,
                                                              cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::SHORT_TYPE );
        this->dataOut->writeShort( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned short ActiveMQStreamMessage::readUnsignedShort() const throw ( cms::MessageEOFException,
                                                                        cms::MessageFormatException,
                                                                        cms::MessageNotReadableException,
                                                                        cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark(17);
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::SHORT_TYPE ) {
            return this->dataIn->readUnsignedShort();
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return this->dataIn->readByte();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Short::valueOf( this->dataIn->readUTF() ).shortValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to short." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a short type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& e ) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeUnsignedShort( unsigned short value )
    throw ( cms::MessageNotWriteableException,
            cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::SHORT_TYPE );
        this->dataOut->writeUnsignedShort( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQStreamMessage::readInt() const throw ( cms::MessageEOFException,
                                                   cms::MessageFormatException,
                                                   cms::MessageNotReadableException,
                                                   cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark(33);
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::INTEGER_TYPE ) {
            return this->dataIn->readInt();
        }
        if( type == PrimitiveValueNode::SHORT_TYPE ) {
            return this->dataIn->readShort();
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return this->dataIn->readByte();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Integer::valueOf( this->dataIn->readUTF() ).intValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to int." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a int type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& e ) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeInt( int value ) throw ( cms::MessageNotWriteableException,
                                                          cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::INTEGER_TYPE );
        this->dataOut->writeInt( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQStreamMessage::readLong() const throw ( cms::MessageEOFException,
                                                          cms::MessageFormatException,
                                                          cms::MessageNotReadableException,
                                                          cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark(65);
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::LONG_TYPE ) {
            return this->dataIn->readLong();
        }
        if( type == PrimitiveValueNode::INTEGER_TYPE ) {
            return this->dataIn->readInt();
        }
        if( type == PrimitiveValueNode::SHORT_TYPE ) {
            return this->dataIn->readShort();
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return this->dataIn->readByte();
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return Long::valueOf( this->dataIn->readUTF() ).longValue();
        }

        if( type == PrimitiveValueNode::NULL_TYPE ) {
            this->dataIn->reset();
            throw NullPointerException( __FILE__, __LINE__, "Cannot convert NULL value to long." );
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a long type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& e ) {
            throw CMSExceptionSupport::create(e);
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeLong( long long value ) throw ( cms::MessageNotWriteableException,
                                                                 cms::CMSException ) {

    initializeWriting();
    try{
        this->dataOut->write( PrimitiveValueNode::LONG_TYPE );
        this->dataOut->writeLong( value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQStreamMessage::readString() const throw ( cms::MessageEOFException,
                                                              cms::MessageFormatException,
                                                              cms::MessageNotReadableException,
                                                              cms::CMSException ) {

    initializeReading();
    try {

        this->dataIn->mark( 65 );
        int type = this->dataIn->read();

        if( type == -1 ) {
            throw MessageEOFException( "reached end of data", NULL );
        }
        if( type == PrimitiveValueNode::NULL_TYPE ) {
            return "";
        }
        if( type == PrimitiveValueNode::BIG_STRING_TYPE ) {
            return OpenwireStringSupport::readString( *dataIn );
        }
        if( type == PrimitiveValueNode::STRING_TYPE ) {
            return this->dataIn->readUTF();
        }
        if( type == PrimitiveValueNode::LONG_TYPE ) {
            return Long( this->dataIn->readLong() ).toString();
        }
        if( type == PrimitiveValueNode::INTEGER_TYPE ) {
            return Integer( this->dataIn->readInt() ).toString();
        }
        if( type == PrimitiveValueNode::SHORT_TYPE ) {
            return Short( this->dataIn->readShort() ).toString();
        }
        if( type == PrimitiveValueNode::BYTE_TYPE ) {
            return Byte( this->dataIn->readByte() ).toString();
        }
        if( type == PrimitiveValueNode::FLOAT_TYPE ) {
            return Float( this->dataIn->readFloat() ).toString();
        }
        if( type == PrimitiveValueNode::DOUBLE_TYPE ) {
            return Double( this->dataIn->readDouble() ).toString();
        }
        if( type == PrimitiveValueNode::BOOLEAN_TYPE ) {
            return ( this->dataIn->readBoolean() ? Boolean::_TRUE : Boolean::_FALSE ).toString();
        }

        if( type == PrimitiveValueNode::CHAR_TYPE ) {
            return Character( this->dataIn->readChar() ).toString();
        } else {
            this->dataIn->reset();
            throw MessageFormatException( " not a String type", NULL );
        }

    } catch( NumberFormatException& ex ) {

        try {
            this->dataIn->reset();
        } catch( IOException& ioe ) {
            throw CMSExceptionSupport::create( ioe );
        }

        throw CMSExceptionSupport::create( ex );

    } catch( EOFException& e ) {
        throw CMSExceptionSupport::createMessageEOFException( e );
    } catch( IOException& e ) {
        throw CMSExceptionSupport::createMessageFormatException( e );
    } catch( Exception& e ) {
        throw CMSExceptionSupport::create( e );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::writeString( const std::string& value ) throw ( cms::MessageNotWriteableException,
                                                                            cms::CMSException ) {

    initializeWriting();
    try{
        if( value.size() <= Short::MAX_VALUE / 4 ) {
            this->dataOut->write( PrimitiveValueNode::STRING_TYPE );
            this->dataOut->writeUTF( value );
        } else {
            this->dataOut->write( PrimitiveValueNode::BIG_STRING_TYPE );
            OpenwireStringSupport::writeString( *(this->dataOut), &value );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::storeContent() {

    if( this->dataOut.get() != NULL ) {

        this->dataOut->close();
        this->setContent( this->bytesOut->toByteArrayRef() );
        this->dataOut.reset(NULL);
        this->bytesOut.reset(NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::initializeReading() const {

    this->failIfWriteOnlyBody();
    try {
        if( this->dataIn.get() == NULL) {
            ByteArrayInputStream* is = new ByteArrayInputStream( this->getContent() );
            this->dataIn.reset( new DataInputStream( is, true ) );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQStreamMessage::initializeWriting() {

    this->failIfReadOnlyBody();
    try{
        if( this->dataOut.get() == NULL ) {
            this->bytesOut.reset( new ByteArrayOutputStream() );
            this->dataOut.reset( new DataOutputStream( this->bytesOut.get() ) );
        }
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
