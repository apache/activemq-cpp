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
#include <activemq/commands/ActiveMQMapMessage.h>
#include <activemq/wireformat/openwire/marshal/PrimitiveTypesMarshaller.h>
#include <activemq/util/CMSExceptionSupport.h>

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/BufferedInputStream.h>
#include <decaf/io/DataInputStream.h>
#include <decaf/io/DataOutputStream.h>
#include <decaf/util/zip/DeflaterOutputStream.h>
#include <decaf/util/zip/InflaterInputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::ActiveMQMapMessage() :
    ActiveMQMessageTemplate<cms::MapMessage>(), map() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::~ActiveMQMapMessage() throw() {
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getDataStructureType() const {
    return ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage* ActiveMQMapMessage::cloneDataStructure() const {
    ActiveMQMapMessage* message = new ActiveMQMapMessage();
    message->copyDataStructure( this );
    return message;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::copyDataStructure( const DataStructure* src ) {
    ActiveMQMessageTemplate<cms::MapMessage>::copyDataStructure( src );

    const ActiveMQMapMessage* srcMap =
        dynamic_cast< const ActiveMQMapMessage* >( src );

    if( srcMap != NULL && srcMap->map.get() != NULL ) {
        this->map.reset( new util::PrimitiveMap( *srcMap->map ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMapMessage::toString() const{
    return ActiveMQMessageTemplate<cms::MapMessage>::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::MapMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::clearBody() throw( cms::CMSException ) {

    ActiveMQMessageTemplate<cms::MapMessage>::clearBody();
    this->getMap().clear();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::beforeMarshal( WireFormat* wireFormat ) {

    try{

        // Let the base class do its thing.
        ActiveMQMessageTemplate<cms::MapMessage>::beforeMarshal( wireFormat );

        if( map.get() != NULL && !map->isEmpty() ) {

            ByteArrayOutputStream* bytesOut = new ByteArrayOutputStream();

            OutputStream* os = bytesOut;

            if( this->connection != NULL && this->connection->isUseCompression() ) {
                this->compressed = true;

                Deflater* deflator = new Deflater( this->connection->getCompressionLevel() );

                os = new DeflaterOutputStream( os, deflator, true, true );
            }

            DataOutputStream dataOut( os, true );
            PrimitiveTypesMarshaller::marshalMap( map.get(), dataOut );
            dataOut.close();

            std::pair<unsigned char*, int> array = bytesOut->toByteArray();
            this->setContent( std::vector<unsigned char>( array.first, array.first + array.second ) );
            delete [] array.first;

        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveMap& ActiveMQMapMessage::getMap() {

    try{

        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
const PrimitiveMap& ActiveMQMapMessage::getMap() const {

    try{

        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::checkMapIsUnmarshalled() const {

    try {

        if( map.get() == NULL && !getContent().empty() ) {

            InputStream* is = new ByteArrayInputStream( getContent() );

            if( isCompressed() ) {
                is = new InflaterInputStream( is, true );
                is = new BufferedInputStream( is, true );
            }

            DataInputStream dataIn( is, true );

            map.reset( PrimitiveTypesMarshaller::unmarshalMap( dataIn ) );

            if( map.get() == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__,
                    "ActiveMQMapMessage::getMap() - All attempts to create a "
                    "map have failed." );
            }
        } else if( map.get() == NULL ) {
            map.reset( new PrimitiveMap() );
        }
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::isEmpty() const {

    try{
        return getMap().isEmpty();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> ActiveMQMapMessage::getMapNames() const {

    try{
        return getMap().keySet().toArray();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::itemExists(const std::string& name) const {

    try {
        return getMap().containsKey( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
cms::Message::ValueType ActiveMQMapMessage::getValueType(const std::string& key) const {

    try {
        util::PrimitiveValueNode::PrimitiveType type = this->getMap().getValueType(key);

        switch(type) {
            case util::PrimitiveValueNode::NULL_TYPE:
                return cms::Message::NULL_TYPE;
            case util::PrimitiveValueNode::BOOLEAN_TYPE:
                return cms::Message::BOOLEAN_TYPE;
            case util::PrimitiveValueNode::BYTE_TYPE:
                return cms::Message::BYTE_TYPE;
            case util::PrimitiveValueNode::CHAR_TYPE:
                return cms::Message::CHAR_TYPE;
            case util::PrimitiveValueNode::SHORT_TYPE:
                return cms::Message::SHORT_TYPE;
            case util::PrimitiveValueNode::INTEGER_TYPE:
                return cms::Message::INTEGER_TYPE;
            case util::PrimitiveValueNode::LONG_TYPE:
                return cms::Message::LONG_TYPE;
            case util::PrimitiveValueNode::DOUBLE_TYPE:
                return cms::Message::DOUBLE_TYPE;
            case util::PrimitiveValueNode::FLOAT_TYPE:
                return cms::Message::FLOAT_TYPE;
            case util::PrimitiveValueNode::STRING_TYPE:
            case util::PrimitiveValueNode::BIG_STRING_TYPE:
                return cms::Message::STRING_TYPE;
            case util::PrimitiveValueNode::BYTE_ARRAY_TYPE:
                return cms::Message::BYTE_ARRAY_TYPE;
            default:
                break;
        }

        return cms::Message::UNKNOWN_TYPE;
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::getBoolean( const std::string& name ) const {

    try{
        return getMap().getBool( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBoolean( const std::string& name, bool value ) {

    failIfReadOnlyBody();
    try{
        getMap().setBool( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getByte( const std::string& name ) const {

    try{
        return getMap().getByte( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setByte( const std::string& name, unsigned char value ) {

    failIfReadOnlyBody();
    try{
        getMap().setByte( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> ActiveMQMapMessage::getBytes( const std::string& name ) const {

    try{
        return getMap().getByteArray( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBytes( const std::string& name,
                                   const std::vector<unsigned char>& value ) {

    failIfReadOnlyBody();
    try{
        getMap().setByteArray( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQMapMessage::getChar( const std::string& name ) const {

    try{
        return getMap().getChar( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setChar( const std::string& name, char value ) {

    failIfReadOnlyBody();
    try{
        getMap().setChar( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQMapMessage::getDouble( const std::string& name ) const {

    try{
        return getMap().getDouble( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setDouble( const std::string& name, double value ) {

    failIfReadOnlyBody();
    try{
        getMap().setDouble( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQMapMessage::getFloat( const std::string& name ) const {

    try{
        return getMap().getFloat( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setFloat( const std::string& name, float value ) {

    failIfReadOnlyBody();
    try{
        getMap().setFloat( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMapMessage::getInt( const std::string& name ) const {

    try{
        return getMap().getInt( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setInt( const std::string& name, int value ) {

    failIfReadOnlyBody();
    try{
        getMap().setInt( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQMapMessage::getLong( const std::string& name ) const {

    try{
        return getMap().getLong( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setLong( const std::string& name, long long value ) {

    failIfReadOnlyBody();
    try{
        getMap().setLong( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQMapMessage::getShort( const std::string& name ) const {

    try{
        return getMap().getShort( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setShort( const std::string& name, short value ) {

    failIfReadOnlyBody();
    try{
        getMap().setShort( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMapMessage::getString( const std::string& name ) const {

    try{
        return getMap().getString( name );
    } catch( UnsupportedOperationException& ex ) {
        throw CMSExceptionSupport::createMessageFormatException( ex );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setString( const std::string& name, const std::string& value ) {

    failIfReadOnlyBody();
    try{
        getMap().setString( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
