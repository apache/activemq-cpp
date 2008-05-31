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

#include "PrimitiveList.h"

#include <stdio.h>
#include <string.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList() {
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList( const decaf::util::List<PrimitiveValueNode>& src )
  : List<PrimitiveValueNode>( src ){
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList( const PrimitiveList& src )
  : List<PrimitiveValueNode>( src ){
}

////////////////////////////////////////////////////////////////////////////////
std::string PrimitiveList::toString() const {

    ostringstream stream;

    stream << "Begin Class PrimitiveList:" << std::endl;

    for( std::size_t i = 0; i < this->size(); ++i ) {
        stream << "list[" << i << "] = "
               << this->get( i ).toString() << std::endl;
    }

    stream << "End Class PrimitiveList:" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool PrimitiveList::getBool( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getBool();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setBool( std::size_t index, bool value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setBool( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char PrimitiveList::getByte( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getByte();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setByte( std::size_t index, unsigned char value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setByte( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
char PrimitiveList::getChar( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getChar();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setChar( std::size_t index, char value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setChar( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
short PrimitiveList::getShort( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getShort();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setShort( std::size_t index, short value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setShort( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
int PrimitiveList::getInt( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getInt();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setInt( std::size_t index, int value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setInt( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
long long PrimitiveList::getLong( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getLong();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setLong( std::size_t index, long long value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setLong( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
double PrimitiveList::getDouble( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getDouble();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setDouble( std::size_t index, double value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setDouble( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
float PrimitiveList::getFloat( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getFloat();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setFloat( std::size_t index, float value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setFloat( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
string PrimitiveList::getString( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ){

    PrimitiveValueNode node = this->get( index );
    return node.getString();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setString( std::size_t index, const string& value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setString( value );

    this->set( index, node );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> PrimitiveList::getByteArray( std::size_t index ) const
    throw( IndexOutOfBoundsException, NoSuchElementException ) {

    PrimitiveValueNode node = this->get( index );
    return node.getByteArray();
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setByteArray( std::size_t index,
                                  const std::vector<unsigned char>& value )
    throw( IndexOutOfBoundsException ){

    PrimitiveValueNode node;
    node.setByteArray( value );

    this->set( index, node );
}
