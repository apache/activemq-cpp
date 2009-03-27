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

#include "IntegerTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void IntegerTest::test(void)
{
    int x = Integer::parseInt("12");
    int y = Integer::parseInt("FF", 16);
    int z = Integer::parseInt("42");

    CPPUNIT_ASSERT( x == 12 );
    CPPUNIT_ASSERT( y == 255 );
    CPPUNIT_ASSERT( z == 42 );

    std::string x1 = Integer::toString( x );
    std::string y1 = Integer::toString( y );
    std::string z1 = Integer::toString( z );

    CPPUNIT_ASSERT( x1 == "12" );
    CPPUNIT_ASSERT( y1 == "255" );
    CPPUNIT_ASSERT( z1 == "42" );

}

////////////////////////////////////////////////////////////////////////////////
void IntegerTest::test2() {
    Integer integer( 255 );

    // Test cast functions
    CPPUNIT_ASSERT( integer.byteValue() == 255 );
    CPPUNIT_ASSERT( integer.shortValue() ==  255 );
    CPPUNIT_ASSERT( integer.intValue() == 255 );
    CPPUNIT_ASSERT( integer.longValue() == 255 );
    CPPUNIT_ASSERT( integer.floatValue() == 255.0f );
    CPPUNIT_ASSERT( integer.doubleValue() == 255.0 );

    // Comparison functions
    CPPUNIT_ASSERT( integer.compareTo( 256 ) == -1 );
    CPPUNIT_ASSERT( integer.compareTo( 255 ) == 0 );
    CPPUNIT_ASSERT( integer.compareTo( 254 ) == 1 );
    CPPUNIT_ASSERT( integer.equals( Integer( 255 ) ) == true );
    CPPUNIT_ASSERT( integer.compareTo( Integer( 255 ) ) == 0 );
    CPPUNIT_ASSERT( integer == Integer( 255 ) );

    // decode
    CPPUNIT_ASSERT( integer == Integer::decode( "255" ) );
    CPPUNIT_ASSERT( integer == Integer::decode( "0xFF" ) );
    CPPUNIT_ASSERT( integer == Integer::decode( "255" ) );
    CPPUNIT_ASSERT( Integer::decode( "-255" ) == -255 );

    // reverseBytes
    CPPUNIT_ASSERT( (int)0xFF000000 == Integer::reverseBytes( 255 ) );

    // reverse
    CPPUNIT_ASSERT( Integer::reverse( Integer::reverse( 255 ) ) == 255 );

    // parseInt
    CPPUNIT_ASSERT( Integer::parseInt( "255") == 255 );
    CPPUNIT_ASSERT( Integer::parseInt( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( Integer::parseInt( "255", 11 ) != 255 );
    CPPUNIT_ASSERT( Integer::parseInt( "FF", 16 ) == 255 );

    // valueOf
    CPPUNIT_ASSERT( Integer::valueOf( 255 ) == 255 );
    CPPUNIT_ASSERT( Integer::valueOf( "255" ) == 255 );
    CPPUNIT_ASSERT( Integer::valueOf( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( (Integer::valueOf( "255", 11 )).intValue() != 255 );
    CPPUNIT_ASSERT( Integer::valueOf( "FF", 16 ) == 255 );

    // bitCount
    CPPUNIT_ASSERT( Integer::bitCount( 255 ) == 8 );
    CPPUNIT_ASSERT( Integer::bitCount( 0xFFFFFFFF ) == 32 );

    //toXXXString
    CPPUNIT_ASSERT( Integer::toString( 255 ) == "255" );
    CPPUNIT_ASSERT( Integer::toString( 255, 16 ) == "ff" );
    CPPUNIT_ASSERT( Integer::toHexString( 255 ) == "ff" );
    CPPUNIT_ASSERT( Integer::toOctalString( 255 ) == "377" );
    CPPUNIT_ASSERT( Integer::toBinaryString( 255 ) == "11111111" );
    CPPUNIT_ASSERT( Integer::toString( 255255 ) == "255255" );

    // highestOneBit
    CPPUNIT_ASSERT( Integer::highestOneBit( 255 ) == 128 );
    CPPUNIT_ASSERT( Integer::highestOneBit( 0xFF000000 ) == (int)0x80000000 );

    // lowestOneBit
    CPPUNIT_ASSERT( Integer::lowestOneBit( 255 ) == 1 );
    CPPUNIT_ASSERT( Integer::lowestOneBit( 0xFF000000 ) == (int)0x01000000 );
}
