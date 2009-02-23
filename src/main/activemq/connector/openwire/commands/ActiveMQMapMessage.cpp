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
#include <activemq/connector/openwire/commands/ActiveMQMapMessage.h>
#include <activemq/connector/openwire/marshal/PrimitiveMapMarshaller.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::ActiveMQMapMessage() :
    ActiveMQMessageBase<cms::MapMessage>() {
    this->map = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::~ActiveMQMapMessage() {
    delete map;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getDataStructureType() const {
    return ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::beforeMarshal( OpenWireFormat* wireFormat )
    throw ( decaf::io::IOException ) {

    try{

        // Let the base class do its thing.
        ActiveMQMessageBase<cms::MapMessage>::beforeMarshal( wireFormat );

        if( map != NULL && !map->isEmpty() ) {
            // Marshal as Content.
            PrimitiveMapMarshaller::marshal( map, getContent() );
        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
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

        if( map == NULL ) {

            if( getContent().size() == 0 ){
                map = new PrimitiveMap;
            } else {
                map = PrimitiveMapMarshaller::unmarshal( getContent() );
            }

            if( map == NULL ) {
                throw NullPointerException(
                    __FILE__,
                    __LINE__,
                    "ActiveMQMapMessage::getMap() - All attempts to create a "
                    "map have fialed." );
            }
        }
    }
    AMQ_CATCH_RETHROW( NullPointerException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, NullPointerException )
    AMQ_CATCHALL_THROW( NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> ActiveMQMapMessage::getMapNames(void) const {

    try{
        return getMap().getKeys();
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::itemExists( const std::string& name ) const {

    try{
        return getMap().containsKey( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::getBoolean( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getBool( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBoolean( const std::string& name, bool value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setBool( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getByte( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getByte( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setByte( const std::string& name,
                                  unsigned char value )
                                      throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setByte( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> ActiveMQMapMessage::getBytes( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getByteArray( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBytes( const std::string& name,
                                   const std::vector<unsigned char>& value )
                                       throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setByteArray( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQMapMessage::getChar( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getChar( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setChar( const std::string& name, char value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setChar( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQMapMessage::getDouble( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getDouble( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setDouble( const std::string& name,
                                    double value )
                                        throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setDouble( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQMapMessage::getFloat( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getFloat( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setFloat( const std::string& name, float value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setFloat( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMapMessage::getInt( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getInt( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setInt( const std::string& name, int value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setInt( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQMapMessage::getLong( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getLong( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setLong( const std::string& name, long long value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setLong( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQMapMessage::getShort( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getShort( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setShort( const std::string& name, short value )
    throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setShort( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMapMessage::getString( const std::string& name ) const
    throw( cms::CMSException ) {

    try{
        return getMap().getString( name );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setString( const std::string& name,
                                    const std::string& value )
                                        throw( cms::CMSException ) {

    try{
        checkReadOnlyBody();
        getMap().setString( name, value );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}
