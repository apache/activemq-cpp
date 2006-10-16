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
    
    const char values[] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 
          'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
          'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    const int count = 36;
    
    for( int i = 0; i < count; ++i ) {
        string entry( 1, values[i] );
        
        for( int j = 0; j < count; ++j ) {
            entry += values[j];
        }
        
        table.push_back( entry );
    }
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
