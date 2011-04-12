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

#include "InflaterTest.h"

#include <decaf/util/zip/Deflater.h>
#include <decaf/util/zip/Inflater.h>
#include <decaf/util/zip/Adler32.h>
#include <decaf/util/zip/CRC32.h>

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
InflaterTest::InflaterTest() {
}

////////////////////////////////////////////////////////////////////////////////
InflaterTest::~InflaterTest() {
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::setUp() {
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::tearDown() {
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testEnd() {

    // test method of java.util.zip.inflater.end()
    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };

    int r = 0;
    Inflater inflate;
    inflate.setInput( byteArray, 5, 0, 5 );
    inflate.end();

    try {
        inflate.reset();
        inflate.setInput( byteArray, 5, 0, 5 );
    } catch( IllegalStateException& e ) {
        r = 1;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "inflate can still be used after end is called", 1, r );

    Inflater i;
    i.end();
    // check for exception
    i.end();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testFinished() {

    static const std::size_t SIZE = 10;
    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 'e', 'r', 't', 'y', '5' };

    std::vector<unsigned char> outPutBuf( 500 );
    std::vector<unsigned char> outPutInf( 500 );

    Deflater deflater;
    deflater.setInput( byteArray, SIZE, 0, SIZE );
    deflater.finish();
    while( !deflater.finished() ) {
        deflater.deflate( outPutBuf );
    }

    Inflater inflate;
    try {

        while( !( inflate.finished() ) ) {
            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }

            inflate.inflate( outPutInf );
        }
        CPPUNIT_ASSERT_MESSAGE(
            "the method finished() returned false when no more data needs to be decompressed",
            inflate.finished() );
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL( "Invalid input to be decompressed" );
    }

    for( std::size_t i = 0; i < SIZE; i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Final decompressed data does not equal the original data",
                                byteArray[i] == outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "final decompressed data contained more bytes than original - finished()",
        0, (int)outPutInf[SIZE]);
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testGetAdler() {

    static const int DICT_SIZE = 7;
    static const int ARRAY_SIZE = 15;

    unsigned char dictionary[] = { 'e', 'r', 't', 'a', 'b', 2, 3 };
    unsigned char byteArray[] = { 4, 5, 3, 2, 'a', 'b', 6, 7, 8, 9, 0, 's', '3', 'w', 'r' };
    std::vector<unsigned char> outPutBuf( 100 );
    std::vector<unsigned char> outPutInf( 100 );

    Deflater defl;
    defl.setDictionary( dictionary, DICT_SIZE, 0, DICT_SIZE );

    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    defl.end();

    // getting the checkSum value through the Adler32 class
    Adler32 adl;
    adl.update( dictionary, DICT_SIZE, 0, DICT_SIZE );
    long long checkSumR = adl.getValue();

    Inflater inflateDiction;
    inflateDiction.setInput( outPutBuf );
    inflateDiction.inflate( outPutInf );

    CPPUNIT_ASSERT_MESSAGE( "Inflater did not detect the need for a Dictionary",
                            inflateDiction.needsDictionary() );

    CPPUNIT_ASSERT_MESSAGE(
        "the checksum value returned by getAdler() is not the same as the checksum returned "
        "by creating the adler32 instance",
        checkSumR == inflateDiction.getAdler() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testGetRemaining() {

    unsigned char byteArray[] = { 1, 3, 5, 6, 7 };
    Inflater inflate;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "upon creating an instance of inflate, getRemaining returned a non zero value",
                                  0, (int)inflate.getRemaining());
    inflate.setInput( byteArray, 5, 0, 5 );
    CPPUNIT_ASSERT_MESSAGE(
        "getRemaining returned zero when there is input in the input buffer",
        inflate.getRemaining() != 0 );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testInflateVector() {

    static const std::size_t SIZE = 39;

    unsigned char byteArray[] = { 120, -38, 75, -54, 73, -52, 80, 40,
                                  46, 41, -54, -52, 75, 87, 72, -50,
                                  -49, 43, 73, -52, -52, 43, 86, 72,
                                  2, 10, 34, 99, -123, -60, -68, 20,
                                  -80, 32, 0, -101, -69, 17, 84 };

    std::string codedString = "blah string contains blahblahblahblah and blah";

    std::vector<unsigned char> outPutBuf( byteArray, byteArray + SIZE );
    std::vector<unsigned char> outPutInf( 500, 0 );

    Inflater inflate;
    try {
        while( !( inflate.finished() ) ) {
            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }
            inflate.inflate( outPutInf );
        }
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    for( std::size_t i = 0; i < codedString.length(); i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Final decompressed data does not equal the original data",
                                codedString[i] == outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "final decompressed data contained more bytes than original - inflateB",
                                  0, (int)outPutInf[codedString.length()] );
    // testing for an empty input array
    outPutBuf.clear();
    outPutBuf.resize( 500 );
    outPutInf.assign( outPutInf.size(), 0 );
    std::vector<unsigned char> emptyArray( 11, 0 );
    int x = 0;
    Deflater defEmpty( 3 );
    defEmpty.setInput( emptyArray );
    while( !( defEmpty.needsInput() ) ) {
        x += defEmpty.deflate( outPutBuf, x, (int)outPutBuf.size() - x );
    }
    defEmpty.finish();
    while( !( defEmpty.finished() ) ) {
        x += defEmpty.deflate( outPutBuf, x, (int)outPutBuf.size() - x );
    }
    CPPUNIT_ASSERT_MESSAGE( "the total number of unsigned char from deflate did not equal "
                            "getTotalOut - inflate(unsigned char)",
                            (long long)x == defEmpty.getBytesWritten() );
    CPPUNIT_ASSERT_MESSAGE(
                "the number of input unsigned char from the array did not correspond with getTotalIn - inflate(unsigned char)",
                (std::size_t)defEmpty.getBytesRead() == emptyArray.size() );
    Inflater infEmpty;
    try {
        while( !( infEmpty.finished() ) ) {
            if( infEmpty.needsInput() ) {
                infEmpty.setInput( outPutBuf );
            }
            infEmpty.inflate( outPutInf );
        }
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL( "Invalid input to be decompressed" );
    }

    for( std::size_t i = 0; i < 11; i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Final decompressed data does not equal the original data",
                                emptyArray[i] == outPutInf[i] );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "Final decompressed data does not equal zero",
                                      0, (int)outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Final decompressed data contains more element than original data",
                                  0, (int)outPutInf[11] );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testInflateB1() {

    static const std::size_t CODEDATA_SIZE = 39;

    unsigned char codedData[] = { 120, -38, 75, -54, 73, -52, 80, 40,
                                  46, 41, -54, -52, 75, 87, 72, -50,
                                  -49, 43, 73, -52, -52, 43, 86, 72,
                                  2, 10, 34, 99, -123, -60, -68, 20,
                                  -80, 32, 0, -101, -69, 17, 84 };
    std::string codedString = "blah string contains blahblahblahblah and blah";

    Inflater infl1;
    Inflater infl2;

    std::vector<unsigned char> result( 100 );
    std::size_t decLen = 0;

    infl1.setInput( codedData, CODEDATA_SIZE, 0, CODEDATA_SIZE );
    try {
        decLen = infl1.inflate( result );
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Unexpected DataFormatException");
    }

    infl1.end();
    CPPUNIT_ASSERT_EQUAL( codedString, std::string( result.begin(), result.begin() + decLen ) );
    codedData[5] = 0;

    infl2.setInput( codedData, CODEDATA_SIZE, 0, CODEDATA_SIZE );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected DataFormatException",
        decLen = infl2.inflate( result ),
        DataFormatException );

    infl2.end();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testInflateBII() {

    static const std::size_t SIZE = 39;

    unsigned char byteArray[] = { 120, -38, 75, -54, 73, -52, 80, 40,
                                  46, 41, -54, -52, 75, 87, 72, -50,
                                  -49, 43, 73, -52, -52, 43, 86, 72,
                                  2, 10, 34, 99, -123, -60, -68, 20,
                                  -80, 32, 0, -101, -69, 17, 84 };

    std::string codedString = "blah string contains blahblahblahblah and blah";

    std::vector<unsigned char> outPutBuf( byteArray, byteArray + SIZE );
    std::vector<unsigned char> outPutInf( 100 );
    int y = 0;
    Inflater inflate;
    try {
        while( !( inflate.finished() ) ) {
            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }
            y += inflate.inflate( outPutInf, y, (int)outPutInf.size() - y );
        }
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }
    for( std::size_t i = 0; i < codedString.length(); i++ ) {
        CPPUNIT_ASSERT_MESSAGE(
                        "Final decompressed data does not equal the original data",
                        codedString[i] == outPutInf[i]);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "final decompressed data contained more bytes than original - inflateB",
                                  0, (int)outPutInf[codedString.length()]);

    // test boundary checks
    inflate.reset();
    int r = 0;
    int offSet = 0;
    int lengthError = 101;
    try {
        if( inflate.needsInput() ) {
            inflate.setInput( outPutBuf );
        }
        inflate.inflate( outPutInf, offSet, lengthError );

    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    } catch( IndexOutOfBoundsException& e ) {
        r = 1;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "out of bounds error did not get caught", 1, r );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testInflateBII1() {

    static const std::size_t CODEDATA_SIZE = 39;
    unsigned char codedData[] = { 120, -38, 75, -54, 73, -52, 80, 40, 46, 41, -54, -52, 75,
                                  87, 72, -50, -49, 43, 73, -52, -52, 43, 86, 72, 2, 10,
                                  34, 99, -123, -60, -68, 20, -80, 32, 0, -101, -69, 17, 84 };

    std::string codedString = "blah string";

    Inflater infl1;
    Inflater infl2;

    std::vector<unsigned char> result( 100 );
    std::size_t decLen = 0;

    infl1.setInput( codedData, CODEDATA_SIZE, 0, CODEDATA_SIZE );
    try {
        decLen = infl1.inflate( result, 10, 11 );
        CPPUNIT_ASSERT( decLen != 0 );
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Unexpected DataFormatException");
    }

    std::string outputStr;
    for( std::size_t ix = 10; ix < decLen + 10; ++ix ) {
        outputStr += (char)result[ix];
    }

    infl1.end();
    CPPUNIT_ASSERT_EQUAL( codedString, outputStr );
    codedData[5] = 0;

    infl2.setInput( codedData, CODEDATA_SIZE, 0, CODEDATA_SIZE );
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Expected DataFormatException",
        decLen = infl2.inflate( result, 10, 11 ),
        DataFormatException );

    infl2.end();
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testConstructor() {

    Inflater inflate;
    CPPUNIT_ASSERT_MESSAGE( "failed to create the instance of inflater",
                            inflate.getBytesRead() == 0LL );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testConstructorZ() {

    static const std::size_t SIZE = 10;

    // note does not throw exception if deflater has a header, but inflater
    // doesn't or vice versa.
    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 'e', 'r', 't', 'y', '5' };
    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;

    deflater.setInput( byteArray, SIZE, 0, SIZE );
    deflater.finish();

    int read = 0;
    while( !deflater.finished() ) {
        read = deflater.deflate( outPutBuf, read, (int)outPutBuf.size() - read );
    }
    deflater.end();

    Inflater inflate( true );
    std::vector<unsigned char> outPutInf( 500 );

    int r = 0;
    try {

        while( !( inflate.finished() ) ) {

            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }

            inflate.inflate( outPutInf );
        }
        for( std::size_t i = 0; i < SIZE; i++ ) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "the output array from inflate should contain 0 because the header of inflate "
                "and deflate did not match, but this failed",
                0, (int)outPutBuf[i] );
        }
    } catch( DataFormatException& e ) {
        r = 1;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Error: exception should be thrown because of header inconsistency", 1, r );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testNeedsDictionary() {

    static const int DICT_SIZE = 7;
    static const int ARRAY_SIZE = 15;

    // This test is very close to getAdler()
    std::vector<unsigned char> dictionary( DICT_SIZE );
    dictionary[0] = 'e';
    dictionary[1] = 'r';
    dictionary[2] = 't';
    dictionary[3] = 'a';
    dictionary[4] = 'b';
    dictionary[5] = 2;
    dictionary[6] = 3;

    unsigned char byteArray[] = { 4, 5, 3, 2, 'a', 'b', 6, 7, 8, 9, 0, 's', '3', 'w', 'r' };
    std::vector<unsigned char> outPutBuf( 100 );

    Deflater defl;
    defl.setDictionary( dictionary, 0, DICT_SIZE );

    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }

    // note: this flag is set after inflate is called
    std::vector<unsigned char> outPutInf( 500 );

    // testing with dictionary set.
    Inflater inflateDiction;
    if( inflateDiction.needsInput() ) {
        inflateDiction.setInput( outPutBuf );
    }
    try {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "should return 0 because needs dictionary",
                                      0, (int)inflateDiction.inflate( outPutInf ) );
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Should not cause exception");
    }
    CPPUNIT_ASSERT_MESSAGE(
        "method needsDictionary returned false when dictionary was used in deflater",
        inflateDiction.needsDictionary() );

    // Recompress without a Dictionary
    outPutBuf.assign( outPutBuf.size(), 0 );
    outPutInf.assign( outPutInf.size(), 0 );

    defl.reset();
    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    defl.end();

    // testing without dictionary
    Inflater inflate;
    try {
        inflate.setInput( outPutBuf );
        inflate.inflate( outPutInf );
        CPPUNIT_ASSERT_MESSAGE(
            "method needsDictionary returned true when dictionary was not used in deflater",
            !inflate.needsDictionary() );
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL( "Input to inflate is invalid or corrupted - needsDictionary" );
    }

    Inflater inf;
    CPPUNIT_ASSERT( !inf.needsDictionary() );
    CPPUNIT_ASSERT_EQUAL( 0LL, inf.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( 0LL, inf.getBytesWritten() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testNeedsInput() {

    Inflater inflate;
    CPPUNIT_ASSERT_MESSAGE(
        "needsInput give the wrong bool value as a result of no input buffer",
        inflate.needsInput() );

    static const std::size_t SIZE = 12;

    unsigned char byteArray[] = { 2, 3, 4, 't', 'y', 'u', 'e', 'w', 7, 6, 5, 9 };
    inflate.setInput( byteArray, SIZE, 0, SIZE );
    CPPUNIT_ASSERT_MESSAGE(
        "methodNeedsInput returned true when the input buffer is full",
        !inflate.needsInput() );

    inflate.reset();
    std::vector<unsigned char> byteArrayEmpty( 0 );;
    inflate.setInput( byteArrayEmpty );
    inflate.needsInput();
    CPPUNIT_ASSERT_MESSAGE(
        "needsInput give wrong bool value as a result of an empty input buffer",
        inflate.needsInput() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testReset() {

    static const std::size_t SIZE = 10;

    // note does not throw exception if deflater has a header, but inflater
    // doesn't or vice versa.
    unsigned char byteArray[] = { 1, 3, 4, 7, 8, 'e', 'r', 't', 'y', '5' };
    std::vector<unsigned char> outPutBuf( 500 );

    Deflater deflater;

    deflater.setInput( byteArray, SIZE, 0, SIZE );
    deflater.finish();

    int read = 0;
    while( !deflater.finished() ) {
        read = deflater.deflate( outPutBuf, read, (int)outPutBuf.size() - read );
    }
    deflater.end();

    std::vector<unsigned char> outPutInf( 100 );
    int y = 0;
    Inflater inflate;
    try {

        while( !( inflate.finished() ) ) {
            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }
            y += inflate.inflate( outPutInf, y, (int)outPutInf.size() - y );
        }

    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    for( std::size_t i = 0; i < SIZE; i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Final decompressed data does not equal the original data",
                                byteArray[i] == outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "final decompressed data contained more bytes than original - reset",
                                  0, (int)outPutInf[SIZE] );

    // testing that resetting the inflater will also return the correct
    // decompressed data

    inflate.reset();
    try {

        while( !( inflate.finished() ) ) {
            if( inflate.needsInput() ) {
                inflate.setInput( outPutBuf );
            }
            inflate.inflate( outPutInf );
        }

    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    for( std::size_t i = 0; i < SIZE; i++ ) {
        CPPUNIT_ASSERT_MESSAGE( "Final decompressed data does not equal the original data",
                                byteArray[i] == outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "final decompressed data contained more bytes than original - reset",
                                  0, (int)outPutInf[SIZE]);
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testSetInputB() {

    static const std::size_t SIZE = 12;
    unsigned char byteArray[] = { 2, 3, 4, 't', 'y', 'u', 'e', 'w', 7, 6, 5, 9 };
    Inflater inflate;
    inflate.setInput( byteArray, SIZE, 0, SIZE );
    CPPUNIT_ASSERT_MESSAGE( "setInputB did not deliver any unsigned char to the input buffer",
                            inflate.getRemaining() != 0 );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testSetInputBIII() {

    static const std::size_t SIZE = 12;
    unsigned char byteArray[] = { 2, 3, 4, 't', 'y', 'u', 'e', 'w', 7, 6, 5, 9 };
    int offSet = 6;
    int length = 6;

    Inflater inflate;
    inflate.setInput( byteArray, SIZE, offSet, length );
    CPPUNIT_ASSERT_MESSAGE( "setInputBII did not deliver the right number of bytes to the input buffer",
                            inflate.getRemaining() == length );

    // boundary check
    inflate.reset();
    int r = 0;
    try {
        inflate.setInput( byteArray, SIZE, 100, 100 );
    } catch( IndexOutOfBoundsException& e ) {
        r = 1;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "boundary check is not present for setInput", 1, r );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testGetBytesRead() {

    Deflater def;
    Inflater inf;
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesRead() );

    // Encode a String into bytes
    std::string inputString = "blahblahblah??";
    std::vector<unsigned char> input( inputString.begin(), inputString.end() );

    // Compress the bytes
    std::vector<unsigned char> output( 100 );

    def.setInput( input );
    def.finish();
    def.deflate( output );
    inf.setInput( output );
    int compressedDataLength = inf.inflate( input );
    CPPUNIT_ASSERT_EQUAL( (long long)compressedDataLength, inf.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( 16LL, inf.getBytesRead() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testGetBytesWritten() {

    Deflater def;
    Inflater inf;
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesWritten() );

    // Encode a String into bytes
    std::string inputString = "blahblahblah??";
    std::vector<unsigned char> input( inputString.begin(), inputString.end() );

    // Compress the bytes
    std::vector<unsigned char> output( 100 );
    def.setInput( input );
    def.finish();
    def.deflate( output );
    inf.setInput( output );
    int compressedDataLength = inf.inflate( input );
    CPPUNIT_ASSERT_EQUAL( 16LL, inf.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( compressedDataLength, (int)inf.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( 14LL, inf.getBytesWritten() );
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testInflate() {

    Inflater inf;
    std::vector<unsigned char> empty(0);
    int res = inf.inflate( empty );

    CPPUNIT_ASSERT_EQUAL( 0, (int)res );

    // Regression for HARMONY-2508
    Inflater inflater;
    std::vector<unsigned char> b( 1024 );
    CPPUNIT_ASSERT_EQUAL( 0, (int)inflater.inflate( b ) );
    inflater.end();

    {
        Inflater inflater;
        unsigned char array[] = { -1 };
        inflater.setInput( array, 1, 0, 1 );
        try {
            inflater.inflate( b );

            // The RI detects malformed data on the malformed input { -1 }. Both
            // this implementation and the native zlib API return "need input"
            // on that data. This is an error if the stream is exhausted, but
            // not one that results in an exception in the Inflater API.
            CPPUNIT_ASSERT( inflater.needsInput() );
        } catch( DataFormatException& e ) {
            // expected
        }
    }

    {
        Inflater inflater2;
        unsigned char array[] = { -1, -1, -1 };
        inflater2.setInput( array, 3, 0, 3);
        try {
            inflater2.inflate( b );
        } catch( DataFormatException& e ) {
            // expected
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testSetDictionaryB() {

    int i = 0;
    std::string inputString = "blah string contains blahblahblahblah and blah";
    std::vector<unsigned char> input( inputString.begin(), inputString.end() );
    std::string dictionary1String = "blah";
    std::vector<unsigned char> dictionary1( dictionary1String.begin(), dictionary1String.end() );
    std::string dictionary2String = "1234";
    std::vector<unsigned char> dictionary2( dictionary2String.begin(), dictionary2String.end() );

    std::vector<unsigned char> outputNo( 100 );
    std::vector<unsigned char> output1( 100 );
    std::vector<unsigned char> output2( 100 );

    Deflater defDictNo( 9 );
    Deflater defDict1( 9 );
    Deflater defDict2( 9 );

    defDict1.setDictionary( dictionary1 );
    defDict2.setDictionary( dictionary2 );

    defDictNo.setInput( input );
    defDict1.setInput( input );
    defDict2.setInput( input );

    defDictNo.finish();
    defDict1.finish();
    defDict2.finish();

    int dataLenNo = defDictNo.deflate( outputNo );
    int dataLen1 = defDict1.deflate( output1 );
    int dataLen2 = defDict2.deflate( output2 );

    bool passNo1 = false;
    bool passNo2 = false;
    bool pass12 = false;

    for( i = 0; i < ( dataLenNo < dataLen1 ? dataLenNo : dataLen1 ); i++ ) {
        if( outputNo[i] != output1[i] ) {
            passNo1 = true;
            break;
        }
    }
    for( i = 0; i < ( dataLenNo < dataLen1 ? dataLenNo : dataLen2 ); i++ ) {
        if( outputNo[i] != output2[i] ) {
            passNo2 = true;
            break;
        }
    }
    for( i = 0; i < ( dataLen1 < dataLen2 ? dataLen1 : dataLen2 ); i++ ) {
        if( output1[i] != output2[i] ) {
            pass12 = true;
            break;
        }
    }

    CPPUNIT_ASSERT_MESSAGE( "Compressed data the same for stream with dictionary and without it.", passNo1 );
    CPPUNIT_ASSERT_MESSAGE( "Compressed data the same for stream with dictionary and without it.", passNo2 );
    CPPUNIT_ASSERT_MESSAGE( "Compressed data the same for stream with different dictionaries.", pass12 );

    Inflater inflNo;
    Inflater infl1;
    Inflater infl2;

    std::vector<unsigned char> result(100);
    int decLen;

    inflNo.setInput( outputNo, 0, dataLenNo );
    decLen = inflNo.inflate( result );

    CPPUNIT_ASSERT( !inflNo.needsDictionary() );
    inflNo.end();
    CPPUNIT_ASSERT_EQUAL( inputString, std::string( result.begin(), result.begin() + decLen ) );

    infl1.setInput( output1, 0, dataLen1 );
    decLen = infl1.inflate( result );

    CPPUNIT_ASSERT( infl1.needsDictionary() );
    infl1.setDictionary( dictionary1 );
    decLen = infl1.inflate( result );
    infl1.end();
    CPPUNIT_ASSERT_EQUAL( inputString, std::string( result.begin(), result.begin() + decLen ) );

    infl2.setInput( output2, 0, dataLen2 );
    decLen = infl2.inflate( result );

    CPPUNIT_ASSERT(infl2.needsDictionary());
    infl2.setDictionary( dictionary2 );
    decLen = infl2.inflate( result );
    infl2.end();
    CPPUNIT_ASSERT_EQUAL(inputString, std::string( result.begin(), result.begin() + decLen ) );

    {
        Inflater inflNo;
        Inflater infl1;
        inflNo.setInput( outputNo, 0, dataLenNo );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "IllegalArgumentException expected.",
            infl1.setDictionary( dictionary1 ),
            IllegalArgumentException );

        inflNo.end();

        infl1.setInput( output1, 0, dataLen1 );
        decLen = infl1.inflate( result );

        CPPUNIT_ASSERT( infl1.needsDictionary() );

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "IllegalArgumentException expected.",
            infl1.setDictionary( dictionary2 ),
            IllegalArgumentException );

        infl1.end();
    }
}

////////////////////////////////////////////////////////////////////////////////
void InflaterTest::testSetDictionaryBIII() {

    int i = 0;
    std::string inputString = "blah string contains blahblahblahblah and blah";
    std::vector<unsigned char> input( inputString.begin(), inputString.end() );
    std::string dictionary1String = "blah";
    std::vector<unsigned char> dictionary1( dictionary1String.begin(), dictionary1String.end() );
    std::string dictionary2String = "blahblahblah";
    std::vector<unsigned char> dictionary2( dictionary2String.begin(), dictionary2String.end() );

    std::vector<unsigned char> output1( 100 );
    std::vector<unsigned char> output2( 100 );
    std::vector<unsigned char> output3( 100 );

    Deflater defDict1( 9 );
    Deflater defDict2( 9 );
    Deflater defDict3( 9 );

    defDict1.setDictionary( dictionary1 );
    defDict2.setDictionary( dictionary2 );
    defDict3.setDictionary( dictionary2, 4, 4 );

    defDict1.setInput( input );
    defDict2.setInput( input );
    defDict3.setInput( input );

    defDict1.finish();
    defDict2.finish();
    defDict3.finish();

    int dataLen1 = defDict1.deflate( output1 );
    int dataLen2 = defDict2.deflate( output2 );
    int dataLen3 = defDict3.deflate( output3 );

    bool pass12 = false;
    bool pass23 = false;
    bool pass13 = true;

    for( i = 0; i < ( dataLen1 < dataLen2 ? dataLen1 : dataLen2 ); i++ ) {
        if( output1[i] != output2[i] ) {
            pass12 = true;
            break;
        }
    }
    for( i = 0; i < ( dataLen2 < dataLen3 ? dataLen2 : dataLen3 ); i++ ) {
        if( output2[i] != output3[i] ) {
            pass23 = true;
            break;
        }
    }
    for( i = 0; i < ( dataLen1 < dataLen3 ? dataLen1 : dataLen3 ); i++ ) {
        if( output1[i] != output3[i] ) {
            pass13 = false;
            break;
        }
    }

    CPPUNIT_ASSERT_MESSAGE( "Compressed data the same for stream with different dictionaries.", pass12 );
    CPPUNIT_ASSERT_MESSAGE( "Compressed data the same for stream with different dictionaries.", pass23 );
    CPPUNIT_ASSERT_MESSAGE( "Compressed data the differs for stream with the same dictionaries.", pass13 );

    Inflater infl1;
    Inflater infl2;
    Inflater infl3;

    std::vector<unsigned char> result(100);
    int decLen;

    infl1.setInput( output1, 0, dataLen1 );
    decLen = infl1.inflate( result );

    CPPUNIT_ASSERT( infl1.needsDictionary() );
    infl1.setDictionary( dictionary2, 4, 4 );
    decLen = infl1.inflate( result );
    infl1.end();
    CPPUNIT_ASSERT_EQUAL( inputString, std::string( result.begin(), result.begin() + decLen ) );

    infl2.setInput( output2, 0, dataLen2 );
    decLen = infl2.inflate( result );

    CPPUNIT_ASSERT(infl2.needsDictionary());
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException expected.",
        infl2.setDictionary( dictionary1 ),
        IllegalArgumentException );

    infl2.end();

    infl3.setInput( output3, 0, dataLen3 );
    decLen = infl3.inflate( result );

    CPPUNIT_ASSERT( infl3.needsDictionary() );
    infl3.setDictionary( dictionary1 );
    decLen = infl3.inflate( result );
    infl3.end();
    CPPUNIT_ASSERT_EQUAL( inputString, std::string( result.begin(), result.begin() + decLen ) );
}
