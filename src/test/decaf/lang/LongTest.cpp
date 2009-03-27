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

#include "LongTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void LongTest::test() {

    long long x = Long::parseLong("12");
    long long y = Long::parseLong("FF", 16);
    long long z = Long::parseLong("42");

    CPPUNIT_ASSERT( x == 12 );
    CPPUNIT_ASSERT( y == 255 );
    CPPUNIT_ASSERT( z == 42 );

    std::string x1 = Long::toString( x );
    std::string y1 = Long::toString( y );
    std::string z1 = Long::toString( z );

    CPPUNIT_ASSERT( x1 == "12" );
    CPPUNIT_ASSERT( y1 == "255" );
    CPPUNIT_ASSERT( z1 == "42" );
}

////////////////////////////////////////////////////////////////////////////////
void LongTest::test2() {

    Long llong( 255 );

    // Test cast functions
    CPPUNIT_ASSERT( llong.byteValue() == 255 );
    CPPUNIT_ASSERT( llong.shortValue() ==  255 );
    CPPUNIT_ASSERT( llong.intValue() == 255 );
    CPPUNIT_ASSERT( llong.longValue() == 255 );
    CPPUNIT_ASSERT( llong.floatValue() == 255.0f );
    CPPUNIT_ASSERT( llong.doubleValue() == 255.0 );

    // Comparison functions
    CPPUNIT_ASSERT( llong.compareTo( 256 ) == -1 );
    CPPUNIT_ASSERT( llong.compareTo( 255 ) == 0 );
    CPPUNIT_ASSERT( llong.compareTo( 254 ) == 1 );
    CPPUNIT_ASSERT( llong.equals( Long( 255 ) ) == true );
    CPPUNIT_ASSERT( llong.compareTo( Long( 255 ) ) == 0 );
    CPPUNIT_ASSERT( llong == Long( 255 ) );

    // decode
    CPPUNIT_ASSERT( llong == Long::decode( "255" ) );
    CPPUNIT_ASSERT( llong == Long::decode( "0xFF" ) );
    CPPUNIT_ASSERT( llong == Long::decode( "255" ) );
    CPPUNIT_ASSERT( Long::decode( "-255" ) == -255 );

    // reverseBytes
    CPPUNIT_ASSERT( (long long)0xFF00000000000000LL == Long::reverseBytes( 255 ) );

    // reverse
    CPPUNIT_ASSERT( Long::reverse( Long::reverse( 255 ) ) == 255 );

    // parseInt
    CPPUNIT_ASSERT( Long::parseLong( "255") == 255 );
    CPPUNIT_ASSERT( Long::parseLong( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( Long::parseLong( "255", 11 ) != 255 );
    CPPUNIT_ASSERT( Long::parseLong( "FF", 16 ) == 255 );

    // valueOf
    CPPUNIT_ASSERT( Long::valueOf( 255 ) == 255 );
    CPPUNIT_ASSERT( Long::valueOf( "255" ) == 255 );
    CPPUNIT_ASSERT( Long::valueOf( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( (Long::valueOf( "255", 11 )).intValue() != 255 );
    CPPUNIT_ASSERT( Long::valueOf( "FF", 16 ) == 255 );

    // bitCount
    CPPUNIT_ASSERT( Long::bitCount( 255 ) == 8 );
    CPPUNIT_ASSERT( Long::bitCount( 0xFFFFFFFF ) == 32 );

    //toXXXString
    CPPUNIT_ASSERT( Long::toString( 255 ) == "255" );
    CPPUNIT_ASSERT( Long::toString( 255, 16 ) == "ff" );
    CPPUNIT_ASSERT( Long::toHexString( 255 ) == "ff" );
    CPPUNIT_ASSERT( Long::toOctalString( 255 ) == "377" );
    CPPUNIT_ASSERT( Long::toBinaryString( 255 ) == "11111111" );
    CPPUNIT_ASSERT( Long::toString( 255255 ) == "255255" );

    // highestOneBit
    CPPUNIT_ASSERT( Long::highestOneBit( 255 ) == 128 );
    CPPUNIT_ASSERT( Long::highestOneBit( 0xFF000000 ) == (long long)0x80000000 );

    // lowestOneBit
    CPPUNIT_ASSERT( Long::lowestOneBit( 255 ) == 1 );
    CPPUNIT_ASSERT( Long::lowestOneBit( 0xFF000000 ) == (long long)0x01000000 );

}
