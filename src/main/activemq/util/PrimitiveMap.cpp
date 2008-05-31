/**
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

#include "PrimitiveMap.h"

#include <sstream>
#include <stdio.h>
#include <string.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
PrimitiveMap::PrimitiveMap(){
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveMap::PrimitiveMap( const decaf::util::Map<std::string, PrimitiveValueNode>& src )
  : decaf::util::Map<std::string, PrimitiveValueNode>( src ) {
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveMap::PrimitiveMap( const PrimitiveMap& src )
  : decaf::util::Map<std::string, PrimitiveValueNode>( src ) {
}

////////////////////////////////////////////////////////////////////////////////
std::string PrimitiveMap::toString() const {

    std::vector<std::string> keys = this->getKeys();
    ostringstream stream;

    stream << "Begin Class PrimitiveMap:" << std::endl;

    for( std::size_t i = 0; i < keys.size(); ++i ) {
        stream << "map[" << keys[i] << "] = "
               << this->getValue( keys[i] ).toString() << std::endl;
    }

    stream << "End Class PrimitiveMap:" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool PrimitiveMap::getBool( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getBool();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setBool( const string& key, bool value ){

    PrimitiveValueNode node;
    node.setBool( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char PrimitiveMap::getByte( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getByte();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setByte( const string& key, unsigned char value ){
    PrimitiveValueNode node;
    node.setByte( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
char PrimitiveMap::getChar( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getChar();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setChar( const string& key, char value ){
    PrimitiveValueNode node;
    node.setChar( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
short PrimitiveMap::getShort( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getShort();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setShort( const string& key, short value ){
    PrimitiveValueNode node;
    node.setShort( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
int PrimitiveMap::getInt( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getInt();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setInt( const string& key, int value ){
    PrimitiveValueNode node;
    node.setInt( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
long long PrimitiveMap::getLong( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getLong();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setLong( const string& key, long long value ){
    PrimitiveValueNode node;
    node.setLong( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
double PrimitiveMap::getDouble( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getDouble();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setDouble( const string& key, double value ){
    PrimitiveValueNode node;
    node.setDouble( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
float PrimitiveMap::getFloat( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getFloat();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setFloat( const string& key, float value ){
    PrimitiveValueNode node;
    node.setFloat( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
string PrimitiveMap::getString( const string& key ) const
    throw( NoSuchElementException ){

    PrimitiveValueNode node = this->getValue( key );
    return node.getString();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setString( const string& key, const string& value ){
    PrimitiveValueNode node;
    node.setString( value );

    this->setValue( key, node );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> PrimitiveMap::getByteArray( const std::string& key ) const
    throw( decaf::lang::exceptions::NoSuchElementException ) {

    PrimitiveValueNode node = this->getValue( key );
    return node.getByteArray();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveMap::setByteArray( const std::string& key,
                                 const std::vector<unsigned char>& value ) {

    PrimitiveValueNode node;
    node.setByteArray( value );

    this->setValue( key, node );
}
