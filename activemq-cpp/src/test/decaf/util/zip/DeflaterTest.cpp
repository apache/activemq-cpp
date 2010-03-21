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

#include "DeflaterTest.h"

#include <decaf/util/zip/Deflater.h>
#include <decaf/util/zip/Inflater.h>
#include <decaf/util/zip/Adler32.h>
#include <decaf/util/zip/CRC32.h>

#include <decaf/lang/Integer.h>

#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <vector>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
DeflaterTest::DeflaterTest() {
}

////////////////////////////////////////////////////////////////////////////////
DeflaterTest::~DeflaterTest() {
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testDeflateVector() {

    unsigned char byteArray[5] = { 1, 3, 4, 7, 8 };

    std::vector<unsigned char> outPutBuf( 50 );
    std::vector<unsigned char> outPutInf( 50 );

    int x = 0;

    Deflater defl;
    defl.setInput( byteArray, 5, 0, 5 );
    defl.finish();

    while( !defl.finished() ) {
        x += defl.deflate( outPutBuf );
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Deflater at end of stream, should return 0",
                                  0, (int)defl.deflate( outPutBuf ) );

    long long totalOut = defl.getBytesWritten();
    long long totalIn = defl.getBytesRead();

    CPPUNIT_ASSERT_EQUAL( (long long)x, totalOut );
    CPPUNIT_ASSERT_EQUAL( 5LL, totalIn );

    defl.end();

    Inflater infl;
    try {
        infl.setInput( outPutBuf );
        while( !infl.finished() ) {
            infl.inflate( outPutInf );
        }
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    CPPUNIT_ASSERT_EQUAL( totalIn, infl.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( totalOut, infl.getBytesRead() );

    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteArray[i], outPutInf[i] );
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Final decompressed data contained more bytes than original",
                                  (unsigned char) 0, outPutInf[5] );

    infl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testDeflateArray() {

    static const int BUFFER_SIZE = 50;
    static const int INPUT_SIZE = 5;

    unsigned char outPutBuf[BUFFER_SIZE];
    memset( outPutBuf, 0, BUFFER_SIZE );
    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };
    unsigned char outPutInf[BUFFER_SIZE];
    memset( outPutInf, 0, BUFFER_SIZE );

    int offSet = 1;
    int length = BUFFER_SIZE - 1;
    int x = 0;

    Deflater defl;

    defl.setInput( byteArray, 5, 0, 5 );
    defl.finish();

    while( !defl.finished() ) {
        x += defl.deflate( outPutBuf, BUFFER_SIZE, offSet, length );
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Deflater at end of stream, should return 0",
                                  0, (int)defl.deflate( outPutBuf, BUFFER_SIZE, offSet, length ) );

    long long totalOut = defl.getBytesWritten();
    long long totalIn = defl.getBytesRead();
    CPPUNIT_ASSERT_EQUAL( x, (int)totalOut );
    CPPUNIT_ASSERT_EQUAL( INPUT_SIZE, (int)totalIn );
    defl.end();

    Inflater infl;
    try {
        infl.setInput( outPutBuf, BUFFER_SIZE, offSet, length );
        while( !infl.finished() ) {
            infl.inflate( outPutInf, BUFFER_SIZE, 0, BUFFER_SIZE );
        }
    } catch( DataFormatException& e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    CPPUNIT_ASSERT_EQUAL( totalIn, infl.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( totalOut, infl.getBytesRead() );
    for( int i = 0; i < INPUT_SIZE; i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteArray[i], outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Final decompressed data contained more bytes than original",
                                  0, (int) outPutInf[BUFFER_SIZE-1] );
    infl.end();

    // Set of tests testing the boundaries of the offSet/length
    Deflater deflater;
    static const int SIZE = 100;
    unsigned char outPutBuf2[SIZE];
    deflater.setInput( byteArray, 5, 0, 5 );

    for( int i = 0; i < 2; i++ ) {

        if( i == 0 ) {
            offSet = SIZE + 1;
            length = SIZE;
        } else {
            offSet = 0;
            length = SIZE + 1;
        }

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown an IndexOutOfBoundsException",
            deflater.deflate( outPutBuf2, SIZE, offSet, length ),
            IndexOutOfBoundsException );
    }

    defl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testEnd() {

    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };
    std::vector<unsigned char> outPutBuf(100);

    Deflater defl;

    defl.setInput( byteArray, 5, 0, 5 );
    defl.finish();

    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }

    defl.end();

    helperEndTest( defl, "end" );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testFinish() {

    // This test already here, its the same as test_deflate()
    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };
    std::vector<unsigned char> outPutBuf(100);
    std::vector<unsigned char> outPutInf(100);

    int x = 0;
    Deflater defl;
    defl.setInput( byteArray, 5, 0, 5 );
    defl.finish();

    while( !defl.finished() ) {
        x += defl.deflate( outPutBuf );
    }

    long long totalOut = defl.getBytesWritten();
    long long totalIn = defl.getBytesRead();
    CPPUNIT_ASSERT_EQUAL( (long long)x, totalOut);
    CPPUNIT_ASSERT_EQUAL( 5LL, totalIn );
    defl.end();

    Inflater infl;
    infl.setInput( outPutBuf );
    while( !infl.finished() ) {
        infl.inflate( outPutInf );
    }
    CPPUNIT_ASSERT_EQUAL( totalIn, infl.getBytesWritten() );
    CPPUNIT_ASSERT_EQUAL( totalOut, infl.getBytesRead() );
    for( int i = 0; i < 5; i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteArray[i], outPutInf[i] );
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Final decompressed data contained more bytes than original",
                                  0, (int)outPutInf[5] );
    infl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testFinished() {

    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };
    std::vector<unsigned char> outPutBuf( 100 );
    Deflater defl;

    CPPUNIT_ASSERT_MESSAGE( "Test 1: Deflater should not be finished.", !defl.finished() );
    defl.setInput( byteArray, 5, 0, 5 );
    CPPUNIT_ASSERT_MESSAGE( "Test 2: Deflater should not be finished.", !defl.finished() );
    defl.finish();
    CPPUNIT_ASSERT_MESSAGE( "Test 3: Deflater should not be finished.", !defl.finished() );
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    CPPUNIT_ASSERT_MESSAGE( "Test 4: Deflater should be finished.", defl.finished() );
    defl.end();
    CPPUNIT_ASSERT_MESSAGE( "Test 5: Deflater should be finished.", defl.finished() );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testGetAdler() {

    unsigned char byteArray[] = { 'a', 'b', 'c', 1, 2, 3 };
    std::vector<unsigned char> outPutBuf( 100 );
    Deflater defl;

    // getting the checkSum value using the Adler
    defl.setInput( byteArray, 5, 0, 5 );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    long long checkSumD = defl.getAdler();
    defl.end();

    // getting the checkSum value through the Adler32 class
    Adler32 adl;
    adl.update( byteArray, 5, 0, 5 );
    long long checkSumR = adl.getValue();

    CPPUNIT_ASSERT_EQUAL_MESSAGE( std::string() +
                                  "The checksum value returned by getAdler() is not the same " +
                                  "as the checksum returned by creating the adler32 instance",
                                  checkSumD, checkSumR );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testNeedsInput() {

    Deflater defl;
    CPPUNIT_ASSERT_MESSAGE( "needsInput give the wrong boolean value as a result of no input buffer",
                            defl.needsInput() );
    unsigned char byteArray[] = { 1, 2, 3 };
    defl.setInput( byteArray, 3, 0, 3 );
    CPPUNIT_ASSERT_MESSAGE( "needsInput give wrong boolean value as a result of a full input buffer",
                            !defl.needsInput() );
    std::vector<unsigned char> outPutBuf( 100 );
    while( !defl.needsInput() ) {
        defl.deflate( outPutBuf );
    }
    std::vector<unsigned char> emptyByteArray( 0 );
    defl.setInput( emptyByteArray );
    CPPUNIT_ASSERT_MESSAGE( "needsInput give wrong boolean value as a result of an empty input buffer",
                            defl.needsInput() );
    defl.setInput( byteArray, 3, 0, 3 );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    defl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testReset() {

    std::vector<unsigned char> outPutBuf( 100 );
    std::vector<unsigned char> outPutInf( 100 );

    unsigned char curArray[5];
    unsigned char byteArray[] = { 1, 3, 4, 7, 8 };
    unsigned char byteArray2[] = { 8, 7, 4, 3, 1 };

    int x = 0;
    int orgValue = 0;
    Deflater defl;

    for( int i = 0; i < 3; i++ ) {

        if( i == 0 ) {
            memcpy( curArray, byteArray, 5 );
        } else if( i == 1 ) {
            memcpy( curArray, byteArray2, 5 );
        } else {
            defl.reset();
        }

        defl.setInput( curArray, 5, 0, 5 );
        defl.finish();
        while( !defl.finished() ) {
            x += defl.deflate( outPutBuf );
        }

        if( i == 0 ) {
            CPPUNIT_ASSERT_EQUAL( x, (int)defl.getBytesWritten() );
        } else if( i == 1 ) {
            CPPUNIT_ASSERT_EQUAL( x, orgValue );
        } else {
            CPPUNIT_ASSERT_EQUAL( x, orgValue * 2 );
        }

        if( i == 0 ) {
            orgValue = x;
        }

        try {
            Inflater infl;
            infl.setInput( outPutBuf );
            while( !infl.finished() ) {
                infl.inflate( outPutInf );
            }
            infl.end();
        } catch( DataFormatException e ) {
            CPPUNIT_FAIL( std::string( "Test " ) + Integer::toString( i ) +
                          ": Invalid input to be decompressed" );
        }

        if( i == 1 ) {
            memcpy( curArray, byteArray, 5 );
        }

        for( int j = 0; j < 5; j++ ) {
            CPPUNIT_ASSERT_EQUAL( curArray[j], outPutInf[j] );
        }

        CPPUNIT_ASSERT_EQUAL( 0, (int)outPutInf[5] );
    }
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetDictionaryVector() {

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
    long long deflAdler = defl.getAdler();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "No dictionary set, no data deflated, getAdler should return 1",
                                  1LL, deflAdler );
    defl.setDictionary( dictionary, 0, DICT_SIZE );
    deflAdler = defl.getAdler();

    // getting the checkSum value through the Adler32 class
    Adler32 adl;
    adl.update( dictionary, 0, DICT_SIZE );
    long long realAdler = adl.getValue();
    CPPUNIT_ASSERT_EQUAL( deflAdler, realAdler );

    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    deflAdler = defl.getAdler();
    Adler32 adl2;
    adl2.update( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    realAdler = adl2.getValue();
    // Deflate is finished and there were bytes deflated that did not occur
    // in the dictionaryArray, therefore a new dictionary was automatically
    // set.
    CPPUNIT_ASSERT_EQUAL( realAdler, deflAdler );
    defl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetDictionaryBIII() {

    static const int DICT_SIZE = 9;
    static const int ARRAY_SIZE = 23;

    // This test is very close to getAdler()
    unsigned char dictionaryArray[] = { 'e', 'r', 't', 'a', 'b', 2, 3, 'o', 't' };
    unsigned char byteArray[] = { 4, 5, 3, 2, 'a', 'b', 6, 7, 8, 9, 0, 's',
                                  '3', 'w', 'r', 't', 'u', 'i', 'o', 4, 5, 6, 7 };
    std::vector<unsigned char> outPutBuf( 100 );

    int offSet = 4;
    int length = 5;

    Deflater defl;
    long long deflAdler = defl.getAdler();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "No dictionary set, no data deflated, getAdler should return 1",
                                  1LL, deflAdler );
    defl.setDictionary( dictionaryArray, DICT_SIZE, offSet, length );
    deflAdler = defl.getAdler();

    // getting the checkSum value through the Adler32 class
    Adler32 adl;
    adl.update( dictionaryArray, DICT_SIZE, offSet, length );
    long long realAdler = adl.getValue();
    CPPUNIT_ASSERT_EQUAL( deflAdler, realAdler );

    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    while( !defl.needsInput() ) {
        defl.deflate( outPutBuf );
    }
    deflAdler = defl.getAdler();
    Adler32 adl2;
    adl2.update( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    realAdler = adl2.getValue();
    // Deflate is finished and there were bytes deflated that did not occur
    // in the dictionaryArray, therefore a new dictionary was automatically
    // set.
    CPPUNIT_ASSERT_EQUAL( realAdler, deflAdler );
    defl.end();

    // boundary check
    Deflater defl2;
    for( int i = 0; i < 2; i++ ) {

        if( i == 0 ) {
            offSet = 0;
            length = DICT_SIZE + 1;
        } else {
            offSet = DICT_SIZE + 1;
            length = 1;
        }

        try {
            defl2.setDictionary( dictionaryArray, DICT_SIZE, offSet, length );
            CPPUNIT_FAIL( std::string( "Test " ) + Integer::toString( i ) +
                          ": boundary check for setDictionary CPPUNIT_FAILed for offset " +
                          Integer::toString( offSet ) + " and length " +
                          Integer::toString( length ) );
        } catch( IndexOutOfBoundsException& e ) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetInputVector() {

    std::vector<unsigned char> byteVector( 3 );
    std::vector<unsigned char> outPutBuf( 100 );
    std::vector<unsigned char> outPutInf( 100 );

    byteVector[0] = 1;
    byteVector[1] = 2;
    byteVector[2] = 3;

    Deflater defl;
    defl.setInput( byteVector );
    CPPUNIT_ASSERT_MESSAGE( "the array buffer in setInput() is empty",
                    !defl.needsInput());
    // The second setInput() should be ignored since needsInput() return
    // false
    defl.setInput( byteVector );
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    defl.end();

    Inflater infl;
    try {
        infl.setInput( outPutBuf );
        while( !infl.finished() ) {
            infl.inflate( outPutInf );
        }
    } catch( DataFormatException e ) {
        CPPUNIT_FAIL("Invalid input to be decompressed");
    }

    for( int i = 0; i < (int)byteVector.size(); i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteVector[i], outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL( (long long)byteVector.size(), infl.getBytesWritten() );
    infl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetInputBIII() {

    static const int SIZE = 5;

    unsigned char byteArray[] = { 1, 2, 3, 4, 5 };
    std::vector<unsigned char> outPutBuf( 50 );
    std::vector<unsigned char> outPutInf( 50 );

    int offSet = 1;
    int length = 3;

    Deflater defl;
    defl.setInput( byteArray, SIZE, offSet, length);

    CPPUNIT_ASSERT_MESSAGE( "the array buffer in setInput() is empty", !defl.needsInput() );

    // The second setInput() should be ignored since needsInput() return
    // false
    defl.setInput( byteArray, SIZE, offSet, length );
    defl.finish();

    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    defl.end();

    Inflater infl;
    infl.setInput( outPutBuf );
    while( !infl.finished() ) {
        infl.inflate( outPutInf );
    }

    for( int i = 0; i < length; i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteArray[i + offSet], outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL( length, (int)infl.getBytesWritten() );
    infl.end();

    // boundary check
    Deflater defl2;
    for( int i = 0; i < 2; i++ ) {

        if( i == 0 ) {
            offSet = 0;
            length = SIZE + 1;
        } else {
            offSet = SIZE + 1;
            length = 1;
        }

        try {
            defl2.setInput( byteArray, SIZE, offSet, length );
            CPPUNIT_FAIL( std::string( "Test " ) + Integer::toString( i ) +
                          ": boundary check for setInput CPPUNIT_FAILed for offset " +
                          Integer::toString( offSet ) + " and length " +
                          Integer::toString( length ) );
        } catch( IndexOutOfBoundsException& e ) {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetLevel() {

    std::vector<unsigned char> byteArray( 100 );
    for( int ix = 0; ix < 100; ++ix ) {
        byteArray[ix] = (unsigned char)( ix + 1 );
    }

    std::vector<unsigned char> outPutBuf( 500 );

    long long totalOut;
    for( int i = 0; i < 10; i++ ) {

        {
            Deflater defl;
            defl.setLevel( i );
            outPutBuf.assign( outPutBuf.size(), 0 );
            defl.setInput( byteArray );
            while( !defl.needsInput() ) {
                defl.deflate( outPutBuf );
            }
            defl.finish();
            while( !defl.finished() ) {
                defl.deflate( outPutBuf );
            }
            totalOut = defl.getBytesWritten();
            defl.end();
        }
        {
            outPutBuf.assign( outPutBuf.size(), 0 );
            Deflater defl( i );
            defl.setInput( byteArray );
            while( !defl.needsInput() ) {
                defl.deflate( outPutBuf );
            }
            defl.finish();
            while( !defl.finished() ) {
                defl.deflate( outPutBuf );
            }
            CPPUNIT_ASSERT_EQUAL( totalOut, defl.getBytesWritten() );
            defl.end();
        }
    }

    Deflater boundDefl;

    // testing boundaries
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number < 0.",
        boundDefl.setLevel( -2 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number > 9.",
        boundDefl.setLevel( 10 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testSetStrategy() {

    std::vector<unsigned char> byteArray( 100 );
    for( int ix = 0; ix < 100; ++ix ) {
        byteArray[ix] = (unsigned char)( ix + 1 );
    }

    for( int i = 0; i < 3; i++ ) {

        std::vector<unsigned char> outPutBuf( 500 );
        Deflater mdefl;

        if( i == 0 ) {
            mdefl.setStrategy( Deflater::DEFAULT_STRATEGY );
        } else if( i == 1 ) {
            mdefl.setStrategy( Deflater::HUFFMAN_ONLY );
        } else {
            mdefl.setStrategy( Deflater::FILTERED );
        }

        mdefl.setInput( byteArray );
        while( !mdefl.needsInput() ) {
            mdefl.deflate( outPutBuf );
        }
        mdefl.finish();
        while( !mdefl.finished() ) {
            mdefl.deflate( outPutBuf );
        }

        if( i == 0 ) {
            CPPUNIT_ASSERT_MESSAGE( "getBytesWritten() for the default strategy did not produce data",
                                    0LL != mdefl.getBytesWritten() );
        } else if( i == 1 ) {
            CPPUNIT_ASSERT_MESSAGE( "getBytesWritten() for the Huffman strategy did not produce data",
                                    0LL != mdefl.getBytesWritten() );
        } else {
            CPPUNIT_ASSERT_MESSAGE( "getBytesWritten for the Filtered strategy did not produce data",
                                    0LL != mdefl.getBytesWritten() );
        }
        mdefl.end();
    }

    // Attempting to setStrategy to an invalid value
    Deflater boundDefl;

    // testing boundaries
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting strategy to an invalid value.",
        boundDefl.setStrategy( 424 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testConstructor() {

    std::vector<unsigned char> byteArray( 100 );
    for( int ix = 0; ix < 100; ++ix ) {
        byteArray[ix] = (unsigned char)( ix + 1 );
    }

    Deflater defl;
    std::vector<unsigned char> outPutBuf( 500 );
    defl.setInput( byteArray, 0, 100 );
    while( !defl.needsInput() ) {
        defl.deflate( outPutBuf );
    }
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    long long totalOut = defl.getBytesWritten();
    defl.end();

    // creating a Deflater using the DEFAULT_COMPRESSION as the int
    Deflater mdefl( 6 );
    outPutBuf.assign( outPutBuf.size(), 0 );
    mdefl.setInput( byteArray );
    while( !mdefl.needsInput() ) {
        mdefl.deflate( outPutBuf );
    }
    mdefl.finish();
    while( !mdefl.finished() ) {
        mdefl.deflate( outPutBuf );
    }
    CPPUNIT_ASSERT_EQUAL( totalOut, mdefl.getBytesWritten() );
    mdefl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testConstructorIB() {

    static const int ARRAY_SIZE = 15;

    unsigned char byteArray[] = { 4, 5, 3, 2, 'a', 'b', 6, 7, 8, 9, 0, 's', '3', 'w', 'r' };

    Deflater defl;
    std::vector<unsigned char> outPutBuf( 500 );
    defl.setLevel( 2 );
    defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
    while( !defl.needsInput() ) {
        defl.deflate( outPutBuf );
    }
    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }
    long long totalOut = defl.getBytesWritten();
    defl.end();

    {
        outPutBuf.assign( outPutBuf.size(), 0 );
        Deflater defl( 2, false );
        defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
        while( !defl.needsInput() ) {
            defl.deflate( outPutBuf );
        }
        defl.finish();
        while( !defl.finished() ) {
            defl.deflate( outPutBuf );
        }
        CPPUNIT_ASSERT_EQUAL( totalOut, defl.getBytesWritten() );
        defl.end();
    }

    {
        outPutBuf.assign( outPutBuf.size(), 0 );
        Deflater defl( 2, true );
        defl.setInput( byteArray, ARRAY_SIZE, 0, ARRAY_SIZE );
        while( !defl.needsInput() ) {
            defl.deflate( outPutBuf );
        }
        defl.finish();
        while( !defl.finished() ) {
            defl.deflate( outPutBuf );
        }
        CPPUNIT_ASSERT_MESSAGE( "getBytesWritten() should not be equal comparing two Deflaters with different header options.",
                                defl.getBytesWritten() != totalOut );
        defl.end();
    }

    std::vector<unsigned char> outPutInf( 500 );
    Inflater infl( true );
    while( !infl.finished() ) {
        if( infl.needsInput() ) {
            infl.setInput( outPutBuf );
        }
        infl.inflate( outPutInf );
    }
    for( int i = 0; i < ARRAY_SIZE; i++ ) {
        CPPUNIT_ASSERT_EQUAL( byteArray[i], outPutInf[i] );
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "final decompressed data contained more bytes than original - constructorIZ",
                                  0, (int)outPutInf[ARRAY_SIZE] );
    infl.end();

    {
        Inflater infl( false );
        outPutBuf.assign( outPutBuf.size(), 0 );
        int r = 0;
        try {
            while( !infl.finished() ) {
                if( infl.needsInput() ) {
                    infl.setInput( outPutBuf );
                }
                infl.inflate( outPutInf );
            }
        } catch( DataFormatException e ) {
            r = 1;
        }
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "header option did not correspond", 1, (int)r );
    }

    Deflater boundDefl;

    // testing boundaries
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number < 0.",
        boundDefl.setLevel( -2 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number > 9.",
        boundDefl.setLevel( 10 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testConstructorI() {

    std::vector<unsigned char> byteArray( 100 );
    for( int ix = 0; ix < 100; ++ix ) {
        byteArray[ix] = (unsigned char)( ix + 1 );
    }

    std::vector<unsigned char> outPutBuf( 500 );
    Deflater defl( 3 );

    defl.setInput( byteArray );
    while( !defl.needsInput() ) {
        defl.deflate( outPutBuf );
    }

    defl.finish();
    while( !defl.finished() ) {
        defl.deflate( outPutBuf );
    }

    long long totalOut = defl.getBytesWritten();
    defl.end();

    // test to see if the compression ratio is the same as setting the level
    // on a deflater
    outPutBuf.assign( outPutBuf.size(), 0 );
    Deflater defl2;
    defl2.setLevel( 3 );
    defl2.setInput( byteArray );
    while( !defl2.needsInput() ) {
        defl2.deflate( outPutBuf );
    }

    defl2.finish();
    while( !defl2.finished() ) {
        defl2.deflate( outPutBuf );
    }
    CPPUNIT_ASSERT_EQUAL( totalOut, defl2.getBytesWritten() );
    defl2.end();

    Deflater boundDefl;

    // testing boundaries
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number < 0.",
        boundDefl.setLevel( -2 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IllegalArgumentException not thrown when setting level to a number > 9.",
        boundDefl.setLevel( 10 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::helperEndTest( Deflater& defl, const std::string& desc ) {

    // Help tests for test_end() and test_reset().
    unsigned char byteArray[] = { 5, 2, 3, 7, 8 };

    // Methods where we expect IllegalStateException or NullPointerException
    // to be thrown
    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.getBytesWritten() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.getBytesWritten(),
        IllegalStateException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.getAdler() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.getAdler(),
        IllegalStateException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.getBytesRead() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.getBytesRead(),
        IllegalStateException );

    unsigned char dict[] = {'a', 'b', 'c'};
    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.setDictionary() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.setDictionary( dict, 3, 0, 3 ),
        IllegalStateException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.deflate() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.deflate( byteArray, 5, 0, 5 ),
        IllegalStateException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.setInput() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.setInput( byteArray, 5, 0, 5 ),
        IllegalStateException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        std::string() + "defl.reset() can still be used after " + desc
                      + " is called in test_" + desc,
        defl.reset(),
        IllegalStateException );

    // Methods that should be allowed to be called after end() is called
    defl.needsInput();
    defl.setStrategy( 1 );
    defl.setLevel( 1 );
    defl.end();
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testInitialState() {

    Deflater inf;
    CPPUNIT_ASSERT_EQUAL( false, inf.finished() );
    CPPUNIT_ASSERT_EQUAL( 0LL, inf.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( 0LL, inf.getBytesWritten() );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testGetBytesRead() {

    Deflater def;
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesWritten() );

    // Encode a String into bytes
    std::string inputString = "blahblahblah??";

    // Compress the bytes
    std::vector<unsigned char> output( 100 );

    def.setInput( (unsigned char*)inputString.c_str(), (int)inputString.size(), 0, (int)inputString.size() );
    def.finish();

    long long compressedDataLength = (long long)def.deflate( output );

    CPPUNIT_ASSERT_EQUAL( 14LL, def.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( compressedDataLength, def.getBytesWritten() );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testGetBytesWritten() {

    Deflater def;
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( 0LL, def.getBytesWritten() );

    // Encode a String into bytes
    std::string inputString = "blahblahblah??";

    // Compress the bytes
    std::vector<unsigned char> output( 100 );

    def.setInput( (unsigned char*)inputString.c_str(), (int)inputString.size(), 0, (int)inputString.size() );
    def.finish();

    long long compressedDataLength = (long long)def.deflate( output );

    CPPUNIT_ASSERT_EQUAL( 14LL, def.getBytesRead() );
    CPPUNIT_ASSERT_EQUAL( compressedDataLength, def.getBytesWritten() );
}

////////////////////////////////////////////////////////////////////////////////
void DeflaterTest::testDeflateBeforeSetInput() {

    Deflater deflater;
    deflater.finish();

    std::vector<unsigned char> buffer( 1024 );

    CPPUNIT_ASSERT_EQUAL( 8, (int)deflater.deflate( buffer ) );

    unsigned char expectedBytes[] = { 120, -100, 3, 0, 0, 0, 0, 1 };

    for( int i = 0; i < 8; i++ ) {
        CPPUNIT_ASSERT_EQUAL( expectedBytes[i], buffer[i] );
    }
}
