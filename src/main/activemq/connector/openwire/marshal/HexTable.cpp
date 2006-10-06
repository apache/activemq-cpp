/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HexTable.h"

#include <activemq/util/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
HexTable::HexTable(){
    
    const char values[] = { 'a', 'b', 'c' };
    table.push_back( "00" );
    table.push_back( "00" );
}

////////////////////////////////////////////////////////////////////////////////
const std::string& HexTable::operator[]( int index ) 
    throw ( exceptions::IndexOutOfBoundsException ) {
    if( size() < index ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__,
            ( string( "HexTable::operator[] - Index passed is out of Bounds" ) +
              Integer::toString( index ) ).c_str() );
    }
    
    return table[index];
}

////////////////////////////////////////////////////////////////////////////////
const std::string& HexTable::operator[]( int index ) const 
    throw ( exceptions::IndexOutOfBoundsException ) {
        
    if( size() < index ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__,
            ( string( "HexTable::operator[] - Index passed is out of Bounds" ) +
              Integer::toString( index ) ).c_str() );
    }
    
    return table[index];
}
