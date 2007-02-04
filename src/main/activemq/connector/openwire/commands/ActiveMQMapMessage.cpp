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
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::ActiveMQMapMessage() :
    ActiveMQMessageBase<cms::MapMessage>()
{
    this->map = NULL;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQMapMessage::~ActiveMQMapMessage()
{
    delete map;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getDataStructureType() const
{
    return ActiveMQMapMessage::ID_ACTIVEMQMAPMESSAGE; 
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::beforeMarshal( OpenWireFormat* wireFormat AMQCPP_UNUSED ) {

    try{ 
  
        if( map != NULL && !map->isEmpty() ) {        
            // Marshal as Content.
            PrimitiveMapMarshaller::marshal( map, getContent() );        
        } else {
            clearBody();
        }
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
util::PrimitiveMap& ActiveMQMapMessage::getMap() throw ( NullPointerException ) {

    try{ 
        
        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( exceptions::NullPointerException )
    AMQ_CATCHALL_THROW( exceptions::NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
const util::PrimitiveMap& ActiveMQMapMessage::getMap() const 
    throw ( exceptions::NullPointerException ) {

    try{

        this->checkMapIsUnmarshalled();
        return *map;
    }
    AMQ_CATCH_RETHROW( exceptions::NullPointerException )
    AMQ_CATCHALL_THROW( exceptions::NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::checkMapIsUnmarshalled() const 
    throw ( exceptions::NullPointerException ) {

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
    AMQ_CATCH_RETHROW( exceptions::NullPointerException )
    AMQ_CATCHALL_THROW( exceptions::NullPointerException )
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> ActiveMQMapMessage::getMapNames(void) const {

    try{
        return getMap().getKeys();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::itemExists( const std::string& name ) const {

    try{
        return getMap().contains( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQMapMessage::getBoolean( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getBool( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBoolean( const std::string& name, bool value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setBool( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQMapMessage::getByte( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getByte( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setByte( const std::string& name,
                                  unsigned char value ) 
                                      throw( cms::CMSException ) {

    try{
        getMap().setByte( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char* ActiveMQMapMessage::getBytes( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return NULL; // TODO getMap().getByteArray( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setBytes( const std::string& name,
                                   const std::vector<unsigned char>& value ) 
                                       throw( cms::CMSException ) {

    try{
        getMap().setByteArray( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
char ActiveMQMapMessage::getChar( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getChar( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setChar( const std::string& name, char value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setChar( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
double ActiveMQMapMessage::getDouble( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getDouble( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setDouble( const std::string& name,
                                    double value ) 
                                        throw( cms::CMSException ) {

    try{
        getMap().setDouble( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
float ActiveMQMapMessage::getFloat( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getFloat( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setFloat( const std::string& name, float value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setFloat( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
int ActiveMQMapMessage::getInt( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getInt( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setInt( const std::string& name, int value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setInt( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
long long ActiveMQMapMessage::getLong( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getLong( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setLong( const std::string& name, long long value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setLong( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
short ActiveMQMapMessage::getShort( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getShort( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setShort( const std::string& name, short value ) 
    throw( cms::CMSException ) {

    try{
        getMap().setShort( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQMapMessage::getString( const std::string& name ) const 
    throw( cms::CMSException ) {

    try{
        return getMap().getString( name );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQMapMessage::setString( const std::string& name, 
                                    const std::string& value ) 
                                        throw( cms::CMSException ) {

    try{
        getMap().setString( name, value );
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}
