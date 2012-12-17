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

#include "PrimitiveValueConverterTest.h"

#include <activemq/util/PrimitiveValueConverter.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToBoolean() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( true );
    PrimitiveValueNode input2( false );
    PrimitiveValueNode input3( "true" );
    PrimitiveValueNode input4( "a23" );
    PrimitiveValueNode input5( 65536 );

    bool result1 = converter.convert<bool>( input1 );
    bool result2 = converter.convert<bool>( input2 );
    bool result3 = converter.convert<bool>( input3 );
    bool result4 = converter.convert<bool>( input4 );

    CPPUNIT_ASSERT( result1 == true );
    CPPUNIT_ASSERT( result2 == false );
    CPPUNIT_ASSERT( result3 == true );
    CPPUNIT_ASSERT( result4 == false );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<bool>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToChar() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (char)127 );
    PrimitiveValueNode input2( (char)255 );
    PrimitiveValueNode input3( 'a' );
    PrimitiveValueNode input4( "a23" );
    PrimitiveValueNode input5( 65536 );

    char result1 = converter.convert<char>( input1 );
    char result2 = converter.convert<char>( input2 );
    char result3 = converter.convert<char>( input3 );

    CPPUNIT_ASSERT( result1 == (char)127 );
    CPPUNIT_ASSERT( result2 == (char)255 );
    CPPUNIT_ASSERT( result3 == 'a' );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<char>( input4 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<char>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToByte() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (unsigned char)127 );
    PrimitiveValueNode input2( (unsigned char)255 );
    PrimitiveValueNode input3( "4" );
    PrimitiveValueNode input4( "4asd" );
    PrimitiveValueNode input5( 65536 );

    unsigned char result1 = converter.convert<unsigned char>( input1 );
    unsigned char result2 = converter.convert<unsigned char>( input2 );
    unsigned char result3 = converter.convert<unsigned char>( input3 );

    CPPUNIT_ASSERT( result1 == (unsigned char)127 );
    CPPUNIT_ASSERT( result2 == (unsigned char)255 );
    CPPUNIT_ASSERT( result3 == (unsigned char)4 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<unsigned char>( input4 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<unsigned char>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToShort() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (unsigned char)127 );
    PrimitiveValueNode input2( (short)65535 );
    PrimitiveValueNode input3( "4" );
    PrimitiveValueNode input4( 65537 );
    PrimitiveValueNode input5( "4asd" );

    short result1 = converter.convert<short>( input1 );
    short result2 = converter.convert<short>( input2 );
    short result3 = converter.convert<short>( input3 );

    CPPUNIT_ASSERT( result1 == 127 );
    CPPUNIT_ASSERT( result2 == (short)65535 );
    CPPUNIT_ASSERT( result3 == 4 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<short>( input4 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<short>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToInt() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (unsigned char)127 );
    PrimitiveValueNode input2( (short)65535 );
    PrimitiveValueNode input3( (int)-1 );
    PrimitiveValueNode input4( "4" );
    PrimitiveValueNode input5( 45LL );
    PrimitiveValueNode input6( "4asd" );

    int result1 = converter.convert<int>( input1 );
    int result2 = converter.convert<int>( input2 );
    int result3 = converter.convert<int>( input3 );
    int result4 = converter.convert<int>( input4 );

    CPPUNIT_ASSERT( result1 == 127 );
    CPPUNIT_ASSERT( result2 == (short)65535 );
    CPPUNIT_ASSERT( result3 == -1 );
    CPPUNIT_ASSERT( result4 == 4 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<int>( input5 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<int>( input6 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToLong() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (unsigned char)127 );
    PrimitiveValueNode input2( (short)65535 );
    PrimitiveValueNode input3( (int)-1 );
    PrimitiveValueNode input4( "4" );
    PrimitiveValueNode input5( 45LL );
    PrimitiveValueNode input6( "4asd" );
    PrimitiveValueNode input7( 12.6f );

    long long result1 = converter.convert<long long>( input1 );
    long long result2 = converter.convert<long long>( input2 );
    long long result3 = converter.convert<long long>( input3 );
    long long result4 = converter.convert<long long>( input4 );
    long long result5 = converter.convert<long long>( input5 );

    CPPUNIT_ASSERT( result1 == 127 );
    CPPUNIT_ASSERT( result2 == (short)65535 );
    CPPUNIT_ASSERT( result3 == -1 );
    CPPUNIT_ASSERT( result4 == 4 );
    CPPUNIT_ASSERT( result5 == 45LL );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<long long>( input6 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<long long>( input7 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToFloat() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( 12.1f );
    PrimitiveValueNode input2( 135.0f );
    PrimitiveValueNode input3( "4.0" );
    PrimitiveValueNode input4( "4asd" );
    PrimitiveValueNode input5( 65536 );

    float result1 = converter.convert<float>( input1 );
    float result2 = converter.convert<float>( input2 );
    float result3 = converter.convert<float>( input3 );

    CPPUNIT_ASSERT( result1 == 12.1f );
    CPPUNIT_ASSERT( result2 == 135.0f );
    CPPUNIT_ASSERT( result3 == 4.0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<float>( input4 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<float>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToDouble() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( 12.1 );
    PrimitiveValueNode input2( 135.0 );
    PrimitiveValueNode input3( "4.0" );
    PrimitiveValueNode input4( "4asd" );
    PrimitiveValueNode input5( 65536 );

    double result1 = converter.convert<double>( input1 );
    double result2 = converter.convert<double>( input2 );
    double result3 = converter.convert<double>( input3 );

    CPPUNIT_ASSERT( result1 == 12.1 );
    CPPUNIT_ASSERT( result2 == (double)135.0 );
    CPPUNIT_ASSERT( result3 == (double)4 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<double>( input4 ),
        UnsupportedOperationException );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<double>( input5 ),
        UnsupportedOperationException );
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueConverterTest::testConvertToString() {

    PrimitiveValueConverter converter;
    PrimitiveValueNode input1( (unsigned char)9 );
    PrimitiveValueNode input2( 45LL );
    PrimitiveValueNode input3( 135.0 );
    PrimitiveValueNode input4( 135.01f );
    PrimitiveValueNode input5( "4.0" );
    PrimitiveValueNode input6( "4asd" );
    PrimitiveValueNode input7( 65539 );

    std::string result1 = converter.convert<std::string>( input1 );
    std::string result2 = converter.convert<std::string>( input2 );
    std::string result3 = converter.convert<std::string>( input3 );
    std::string result4 = converter.convert<std::string>( input4 );
    std::string result5 = converter.convert<std::string>( input5 );
    std::string result6 = converter.convert<std::string>( input6 );
    std::string result7 = converter.convert<std::string>( input7 );

    CPPUNIT_ASSERT( result1 == "9" );
    CPPUNIT_ASSERT( result2 == "45" );
    CPPUNIT_ASSERT( result3 == "135" );
    CPPUNIT_ASSERT( result4 == "135.01" );
    CPPUNIT_ASSERT( result5 == "4.0" );
    CPPUNIT_ASSERT( result6 == "4asd" );
    CPPUNIT_ASSERT( result7 == "65539" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException",
        converter.convert<unsigned int>( 24567 ),
        UnsupportedOperationException );
}
