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

#include <decaf/lang/Integer.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
HexTable::HexTable(){

    const char values[] =
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
          'c', 'd', 'e', 'f' };
    const int count = 16;

    for( int i = 0; i < count; ++i ) {
        for( int j = 0; j < count; ++j ) {
            string entry( 1, values[i] );
            entry += values[j];
            table.push_back( entry );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
const std::string& HexTable::operator[]( std::size_t index )
    throw ( IndexOutOfBoundsException ) {

    try{

        if( size() < index ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                ( string( "HexTable::operator[] - Index passed is out of Bounds" ) +
                  Integer::toString( (int)index ) ).c_str() );
        }

        return table[index];
    }
    AMQ_CATCH_RETHROW( IndexOutOfBoundsException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    AMQ_CATCHALL_THROW( IndexOutOfBoundsException )
}

////////////////////////////////////////////////////////////////////////////////
const std::string& HexTable::operator[]( std::size_t index ) const
    throw ( IndexOutOfBoundsException ) {

    try{

        if( size() < index ) {
            throw IndexOutOfBoundsException(
                __FILE__, __LINE__,
                ( string( "HexTable::operator[] - Index passed is out of Bounds" ) +
                  Integer::toString( (int)index ) ).c_str() );
        }

        return table[index];
    }
    AMQ_CATCH_RETHROW( IndexOutOfBoundsException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, IndexOutOfBoundsException )
    AMQ_CATCHALL_THROW( IndexOutOfBoundsException )
}
