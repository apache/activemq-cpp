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

#include "CharacterTest.h"

using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
CharacterTest::CharacterTest() {
}

////////////////////////////////////////////////////////////////////////////////
void CharacterTest::test() {

    Character character( 'b' );

    CPPUNIT_ASSERT( ( character < 'a' ) == false );
    CPPUNIT_ASSERT( character.compareTo( 'a' ) == 1 );
    CPPUNIT_ASSERT( character.compareTo( 'b' ) == 0 );
    CPPUNIT_ASSERT( character.compareTo( 'c' ) == -1 );

    CPPUNIT_ASSERT( Character::isDigit('a') == false );
    CPPUNIT_ASSERT( Character::isDigit('7') == true );
    CPPUNIT_ASSERT( Character::isLowerCase('a') == true );
    CPPUNIT_ASSERT( Character::isLowerCase('A') == false );
    CPPUNIT_ASSERT( Character::isUpperCase('a') == false );
    CPPUNIT_ASSERT( Character::isUpperCase('A') == true );
    CPPUNIT_ASSERT( Character::isLetter('a') == true );
    CPPUNIT_ASSERT( Character::isLetter('8') == false );
    CPPUNIT_ASSERT( Character::isLetterOrDigit('a') == true );
    CPPUNIT_ASSERT( Character::isLetterOrDigit('&') == false );
    CPPUNIT_ASSERT( Character::digit( '9', 10 ) == 9 );

}

