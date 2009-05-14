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

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::ActiveMQMapMessage() :
    ActiveMQMessageTemplate<cms::MapMessage>() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::~ActiveMQMapMessage() {
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
    std::ostringstream stream;

    stream << "Begin Class = ActiveMQMapMessage" << std::endl;
    stream << ActiveMQMessageTemplate<cms::MapMessage>::toString();
    stream << "End Class = ActiveMQMapMessage" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::equals( const DataStructure* value ) const {
    return ActiveMQMessageTemplate<cms::MapMessage>::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::beforeMarshal( WireFormat* wireFormat )
    throw ( decaf::io::IOException ) {

    try{

        // Let the base class do its thing.
        ActiveMQMessageTemplate<cms::MapMessage>::beforeMarshal( wireFormat );

        if( map.get() != NULL && !map->isEmpty() ) {
            // Marshal as Content.
            PrimitiveTypesMarshaller::marshal( map.get(), getContent() );
        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveMap& ActiveMQMapMessage::getMap() throw ( NullPointerException ) {

    try{

        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
const PrimitiveMap& ActiveMQMapMessage::getMap() const
    throw ( decaf::lang::exceptions::NullPointerException ) {

    try{

        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::checkMapIsUnmarshalled() const
    throw ( decaf::lang::exceptions::NullPointerException ) {

    try {

        if( map.get() == NULL ) {

            map.reset( new PrimitiveMap() );

            if( getContent().size() != 0 ){
                PrimitiveTypesMarshaller::unmarshal( map.get(), getContent() );
            }

            if( map.get() == NULL ) {
                throw NullPointerException(
                    __FILE__, __LINE__,
                    "ActiveMQMapMessage::getMap() - All attempts to create a "
                    "map have failed." );
            }
        }
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> ActiveMQMapMessage::getMapNames() const {

    try{
        return getMap().keySet();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::itemExists( const std::string& name ) const {

    try{
        return getMap().containsKey( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::getBoolean( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getBool( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBoolean( const std::string& name, bool value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setBool( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getByte( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getByte( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setByte( const std::string& name,
                                  unsigned char value )
                                      throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setByte( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> ActiveMQMapMessage::getBytes( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getByteArray( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBytes( const std::string& name,
                                   const std::vector<unsigned char>& value )
                                       throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setByteArray( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQMapMessage::getChar( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getChar( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setChar( const std::string& name, char value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setChar( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQMapMessage::getDouble( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getDouble( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setDouble( const std::string& name,
                                    double value )
                                        throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setDouble( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQMapMessage::getFloat( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getFloat( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setFloat( const std::string& name, float value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setFloat( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMapMessage::getInt( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getInt( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setInt( const std::string& name, int value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setInt( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQMapMessage::getLong( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getLong( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setLong( const std::string& name, long long value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setLong( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQMapMessage::getShort( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getShort( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setShort( const std::string& name, short value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setShort( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMapMessage::getString( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getString( name );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setString( const std::string& name,
                                    const std::string& value )
                                        throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setString( name, value );
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}
