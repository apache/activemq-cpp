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

#include "ShortTest.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
ShortTest::ShortTest() {
}

////////////////////////////////////////////////////////////////////////////////
void ShortTest::test() {

    Short short1( 65 );

    CPPUNIT_ASSERT( short1.byteValue() == 65 );
    CPPUNIT_ASSERT( short1.shortValue() == 65 );
    CPPUNIT_ASSERT( short1.intValue() == 65 );
    CPPUNIT_ASSERT( short1.longValue() == 65 );
    CPPUNIT_ASSERT( short1.floatValue() == 65.0f );
    CPPUNIT_ASSERT( short1.doubleValue() == 65.0 );

    CPPUNIT_ASSERT( short1.toString() == "65" );
    CPPUNIT_ASSERT( short1.toString( 99 ) == "99" );

    CPPUNIT_ASSERT( Short::reverseBytes( (short)0xFF00 ) == (short)0x00FF );
    CPPUNIT_ASSERT( Short::reverseBytes( (short)0x0F00 ) == (short)0x000F );
    CPPUNIT_ASSERT( Short::reverseBytes( (short)0xDE00 ) == (short)0x00DE );
    CPPUNIT_ASSERT( Short::reverseBytes( (short)0x00AB ) == (short)0xAB00 );

    Short short2( 255 );

    // Comparison functions
    CPPUNIT_ASSERT( short2.compareTo( 256 ) == -1 );
    CPPUNIT_ASSERT( short2.compareTo( 255 ) == 0 );
    CPPUNIT_ASSERT( short2.compareTo( 254 ) == 1 );
    CPPUNIT_ASSERT( short2.equals( Short( 255 ) ) == true );
    CPPUNIT_ASSERT( short2.compareTo( Short( 255 ) ) == 0 );
    CPPUNIT_ASSERT( short2 == Short( 255 ) );

    // decode
    CPPUNIT_ASSERT( short2 == Short::decode( "255" ) );
    CPPUNIT_ASSERT( short2 == Short::decode( "0xFF" ) );
    CPPUNIT_ASSERT( short2 == Short::decode( "255" ) );
    CPPUNIT_ASSERT( Short::decode( "-255" ) == -255 );

    // parseInt
    CPPUNIT_ASSERT( Short::parseShort( "255") == 255 );
    CPPUNIT_ASSERT( Short::parseShort( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( Short::parseShort( "255", 11 ) != 255 );
    CPPUNIT_ASSERT( Short::parseShort( "FF", 16 ) == 255 );

    // valueOf
    CPPUNIT_ASSERT( Short::valueOf( 255 ) == 255 );
    CPPUNIT_ASSERT( Short::valueOf( "255" ) == 255 );
    CPPUNIT_ASSERT( Short::valueOf( "255", 10 ) == 255 );
    CPPUNIT_ASSERT( (Short::valueOf( "255", 11 )).shortValue() != 255 );
    CPPUNIT_ASSERT( Short::valueOf( "FF", 16 ) == 255 );

    CPPUNIT_ASSERT( Short::toString( 255 ) == "255" );

}

