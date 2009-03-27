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

#include "URIEncoderDecoderTest.h"

#include <decaf/net/URISyntaxException.h>

using namespace std;
using namespace decaf::lang;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URIEncoderDecoderTest::URIEncoderDecoderTest() {}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoderTest::testValidate() {

    string str1 = "ABCDefghIjKlMNOpqrsTuVwXyZ:1234567890:&^";
    string str2 = "ABCDefghIjKlMNOpqrsTuVwXyZ1234567890";

    string legalSet1 = ":&^";

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "1. String should Validate:",
        URIEncoderDecoder::validate( str1, legalSet1 ) );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "2. String should Validate:",
        URIEncoderDecoder::validate( str2, "" ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "1. String should not Validate:",
        URIEncoderDecoder::validate( str1, "" ),
        URISyntaxException );

}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoderTest::testValidateSimple() {

    string str1 = "ABCDefghIjKlMNOpqrsTuVwXyZ:1234567890:&^";
    string str2 = "ABCDefghIjKlMNOpqrsTuVwXyZ1234567890";

    string legalSet1 = ":&^";

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "1. String should Validate:",
        URIEncoderDecoder::validate( str1, legalSet1 ) );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "2. String should Validate:",
        URIEncoderDecoder::validate( str2, "" ) );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "1. String should not Validate:",
        URIEncoderDecoder::validate( str1, "" ),
        URISyntaxException );
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoderTest::testQuoteIllegal() {

    std::string pound1 = "#";  // %23
    std::string pound2 = "##"; // %23%23
    std::string pound3 = "A#*"; // A%23*
    std::string legal = "A";   // A

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "1. Result not equal to: %23",
        URIEncoderDecoder::quoteIllegal( pound1, "" ) == "%23" );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "2. Result not equal to: %23%23",
        URIEncoderDecoder::quoteIllegal( pound2, "" ) == "%23%23" );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "3. Result not equal to: A%23*",
        URIEncoderDecoder::quoteIllegal( pound3, "*" ) == "A%23*" );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "4. Result not equal to: A",
        URIEncoderDecoder::quoteIllegal( legal, "" ) == "A" );
}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoderTest::testEncodeOthers() {

    string test1;
    string test2;

    test1 += (char)156;
    test2 += 'A';

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "1. Result not equal to: %9C",
        URIEncoderDecoder::encodeOthers( test1 ) == "%9C" );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "2. Result not equal to: A",
        URIEncoderDecoder::encodeOthers( test2 ) == "A" );

}

////////////////////////////////////////////////////////////////////////////////
void URIEncoderDecoderTest::testDecode() {

    string test = "A%20B%20C %24%25";

    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "1. Result not equal to: 'A B C $%",
        URIEncoderDecoder::decode( test ) == "A B C $%" );
}
