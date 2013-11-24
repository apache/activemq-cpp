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

#ifndef _DECAF_LANG_STRINGTEST_H_
#define _DECAF_LANG_STRINGTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace lang {

    class StringTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( StringTest );
        CPPUNIT_TEST( testDefaultConstructor );
        CPPUNIT_TEST( testConstructorCString );
        CPPUNIT_TEST( testConstructorCStringWithSize );
        CPPUNIT_TEST( testConstructorCStringOffsetAndLength );
        CPPUNIT_TEST( testConstructorCStringSizeOffsetAndLength );
        CPPUNIT_TEST( testConstructorStdString );
        CPPUNIT_TEST( testConstructorString );
        CPPUNIT_TEST( testConstructorCharFill );
        CPPUNIT_TEST( testAssignmentString );
        CPPUNIT_TEST( testAssignmentStdString );
        CPPUNIT_TEST( testAssignmentCString );
        CPPUNIT_TEST( testCompact );
        CPPUNIT_TEST( testHashCode );
        CPPUNIT_TEST( testIsEmpty );
        CPPUNIT_TEST( testSubstring1 );
        CPPUNIT_TEST( testSubstring2 );
        CPPUNIT_TEST( testSubstringExceptions );
        CPPUNIT_TEST( testTrim );
        CPPUNIT_TEST( testToString );
        CPPUNIT_TEST( testToCharArray );
        CPPUNIT_TEST( testCStr );
        CPPUNIT_TEST( testRegionMatches );
        CPPUNIT_TEST( testRegionMatchesCaseSensitive );
        CPPUNIT_TEST( testStartsWith );
        CPPUNIT_TEST( testStartsWithI );
        CPPUNIT_TEST( testEndsWith );
        CPPUNIT_TEST( testEquals );
        CPPUNIT_TEST( testEqualsCString );
        CPPUNIT_TEST( testEqualsStdString );
        CPPUNIT_TEST( testEqualsIgnoreCase );
        CPPUNIT_TEST( testEqualsIgnoreCaseCString );
        CPPUNIT_TEST( testEqualsIgnoreCaseStdString );
        CPPUNIT_TEST( testIndexOfChar );
        CPPUNIT_TEST( testIndexOfChar2 );
        CPPUNIT_TEST( testIndexOfString );
        CPPUNIT_TEST( testIndexOfString2 );
        CPPUNIT_TEST( testIndexOfStdString );
        CPPUNIT_TEST( testIndexOfStdString2 );
        CPPUNIT_TEST( testIndexOfCString );
        CPPUNIT_TEST( testIndexOfCString2 );
        CPPUNIT_TEST( testLastIndexOfChar );
        CPPUNIT_TEST( testLastIndexOfChar2 );
        CPPUNIT_TEST( testLastIndexOfString );
        CPPUNIT_TEST( testLastIndexOfString2 );
        CPPUNIT_TEST( testLastIndexOfStdString );
        CPPUNIT_TEST( testLastIndexOfStdString2 );
        CPPUNIT_TEST( testLastIndexOfCString );
        CPPUNIT_TEST( testLastIndexOfCString2 );
        CPPUNIT_TEST( testToLowerCase );
        CPPUNIT_TEST( testToUpperCase );
        CPPUNIT_TEST( testReplaceCharChar );
        CPPUNIT_TEST( testContainsString );
        CPPUNIT_TEST( testContainsStdString );
        CPPUNIT_TEST( testContainsCString );
        CPPUNIT_TEST( testConcatString );
        CPPUNIT_TEST( testConcatStdString );
        CPPUNIT_TEST( testConcatCString );
        CPPUNIT_TEST( testCompareToString );
        CPPUNIT_TEST( testCompareToStdString );
        CPPUNIT_TEST( testCompareToCString );
        CPPUNIT_TEST( testCompareToIgnoreCaseString );
        CPPUNIT_TEST( testCompareToIgnoreCaseStdString );
        CPPUNIT_TEST( testCompareToIgnoreCaseCString );
        CPPUNIT_TEST( testIsNullOrEmpty );
        CPPUNIT_TEST( testOperatorEqualsString );
        CPPUNIT_TEST( testOperatorEqualsStdString );
        CPPUNIT_TEST( testOperatorEqualsCString );
        CPPUNIT_TEST( testOperatorLessString );
        CPPUNIT_TEST( testOperatorLessStdString );
        CPPUNIT_TEST( testOperatorLessCString );
        CPPUNIT_TEST( testOperatorGreaterString );
        CPPUNIT_TEST( testOperatorGreaterStdString );
        CPPUNIT_TEST( testOperatorGreaterCString );
        CPPUNIT_TEST( testOperatorNotEqualsString );
        CPPUNIT_TEST( testOperatorNotEqualsStdString );
        CPPUNIT_TEST( testOperatorNotEqualsCString );
        CPPUNIT_TEST( testOperatorPlusString );
        CPPUNIT_TEST( testOperatorPlusStdString );
        CPPUNIT_TEST( testOperatorPlusCString );
        CPPUNIT_TEST( testFindFirstOf );
        CPPUNIT_TEST( testFindFirstOf2 );
        CPPUNIT_TEST( testFindFirstNotOf );
        CPPUNIT_TEST( testFindFirstNotOf2 );
        CPPUNIT_TEST( testGetChars );
        CPPUNIT_TEST_SUITE_END();

    public:

        StringTest();
        virtual ~StringTest();

        void testDefaultConstructor();
        void testConstructorCString();
        void testConstructorCStringWithSize();
        void testConstructorCStringOffsetAndLength();
        void testConstructorCStringSizeOffsetAndLength();
        void testConstructorStdString();
        void testConstructorString();
        void testConstructorCharFill();
        void testAssignmentString();
        void testAssignmentStdString();
        void testAssignmentCString();
        void testCompact();
        void testHashCode();
        void testIsEmpty();
        void testSubstring1();
        void testSubstring2();
        void testSubstringExceptions();
        void testTrim();
        void testToString();
        void testToCharArray();
        void testCStr();
        void testRegionMatches();
        void testRegionMatchesCaseSensitive();
        void testStartsWith();
        void testStartsWithI();
        void testEndsWith();
        void testEquals();
        void testEqualsCString();
        void testEqualsStdString();
        void testEqualsIgnoreCase();
        void testEqualsIgnoreCaseCString();
        void testEqualsIgnoreCaseStdString();
        void testIndexOfChar();
        void testIndexOfChar2();
        void testIndexOfString();
        void testIndexOfString2();
        void testFindFirstOf();
        void testFindFirstOf2();
        void testFindFirstNotOf();
        void testFindFirstNotOf2();
        void testIndexOfStdString();
        void testIndexOfStdString2();
        void testIndexOfCString();
        void testIndexOfCString2();
        void testLastIndexOfChar();
        void testLastIndexOfChar2();
        void testLastIndexOfString();
        void testLastIndexOfString2();
        void testLastIndexOfStdString();
        void testLastIndexOfStdString2();
        void testLastIndexOfCString();
        void testLastIndexOfCString2();
        void testToLowerCase();
        void testToUpperCase();
        void testReplaceCharChar();
        void testContainsString();
        void testContainsStdString();
        void testContainsCString();
        void testConcatString();
        void testConcatStdString();
        void testConcatCString();
        void testCompareToString();
        void testCompareToStdString();
        void testCompareToCString();
        void testCompareToIgnoreCaseString();
        void testCompareToIgnoreCaseStdString();
        void testCompareToIgnoreCaseCString();
        void testIsNullOrEmpty();
        void testOperatorEqualsString();
        void testOperatorEqualsStdString();
        void testOperatorEqualsCString();
        void testOperatorNotEqualsString();
        void testOperatorNotEqualsStdString();
        void testOperatorNotEqualsCString();
        void testOperatorLessString();
        void testOperatorLessStdString();
        void testOperatorLessCString();
        void testOperatorGreaterString();
        void testOperatorGreaterStdString();
        void testOperatorGreaterCString();
        void testOperatorPlusString();
        void testOperatorPlusStdString();
        void testOperatorPlusCString();
        void testGetChars();

    };

}}

#endif /* _DECAF_LANG_STRINGTEST_H_ */
