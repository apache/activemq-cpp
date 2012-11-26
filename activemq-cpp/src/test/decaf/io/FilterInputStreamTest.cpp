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

#include "FilterInputStreamTest.h"
#include <decaf/io/ByteArrayInputStream.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyInputStream : public InputStream{
    private:
        std::string data;
        int pos;
        bool throwOnRead;
        bool closed;

    public:

        MyInputStream( const std::string& data ) : data(), pos(), throwOnRead(), closed() {
            this->data = data;
            this->pos = 0;
            this->throwOnRead = false;
            this->closed = false;
        }
        virtual ~MyInputStream(){}

        void setThrowOnRead( bool value ) {
            this->throwOnRead = value;
        }

        bool isThrowOnRead() const {
            return this->throwOnRead;
        }

        bool isClosed() const {
            return this->closed;
        }

        virtual int available() const throw (IOException){
            if( isClosed() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Stream already closed." );
            }
            int len = (int)data.length();
            return len - pos;
        }

        virtual void close() {
            this->closed = true;
        }
        virtual long long skip( long long num ) {
            return ( pos += (int)std::min( num, (long long)available() ) );
        }

    protected:

        virtual int doReadByte() {
            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            if( pos >= (int)data.length() ){
                return -1;
            }

            return data.c_str()[pos++];
        }

        virtual int doReadArrayBounded( unsigned char* buffer, int size,
                                        int offset, int length ) {

            int numToRead = std::min( length, available() );

            if( buffer == NULL ) {
                throw NullPointerException( __FILE__, __LINE__, "Buffer was Null." );
            }

            if( offset > size || offset + length > size ) {
                throw IndexOutOfBoundsException(
                    __FILE__, __LINE__, "Offset + Length greater than the given buffer size." );
            }

            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            // Simulate EOF
            if( numToRead == 0 ) {
                return -1;
            }

            const char* str = data.c_str();
            for( int ix=0; ix<numToRead; ++ix ){
                buffer[ix+offset] = str[pos+ix];
            }

            pos += numToRead;

            return (int)numToRead;
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testAvailable() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                            is.available() == (int)testStr.length() );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testClose() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    try {
        is.close();
    } catch( IOException& e ) {
        CPPUNIT_FAIL("Exception attempting to close stream");
    }

    try {
        is.read();
    } catch( IOException& e ) {
        return;
    }

    CPPUNIT_FAIL("Able to read from closed stream");
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    char c = (char)is.read();
    CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                            c == testStr.at(0) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead2() {

    std::string testStr = "TEST12345678910ABCDEFGHIJKLMNOPQRSTU";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[30];
    is.read( buf, 30, 0, 30 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to read correct data",
        string( (const char*)buf, 30 ) == testStr.substr(0, 30) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead3() {

    std::string testStr;
    for( int i = 0; i < 4000; ++i ) {
        testStr += (char)i;
    }
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[100];
    is.skip(3000);
    is.read( buf, 100, 0, 100 );

    std::string bufferString( (const char*)buf, (const char*)(buf + 100) );
    std::string testSubString = testStr.substr( 3000, 100 );

    CPPUNIT_ASSERT_MESSAGE( "Failed to read correct data", bufferString == testSubString );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testSkip() {

    std::string testStr;
    for( int i = 0; i < 4000; ++i ) {
        testStr += (char)i;
    }
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[100];
    is.skip( 1000 );
    is.read( buf, 100, 0, 100 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to skip to correct position",
            string( (const char*)buf, 100 ) == testStr.substr( 1000, 100 ) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testReadBIIIExceptions() {

    std::string testStr;
    for( int i = 0; i < 1000; ++i ) {
        testStr += (char)i;
    }
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    CPPUNIT_ASSERT_THROW_MESSAGE(
         "should throw NullPointerException",
         is.read( NULL, 1000, 0, 1001 ),
         NullPointerException );

    unsigned char buf[1000];

    CPPUNIT_ASSERT_THROW_MESSAGE(
         "should throw IndexOutOfBoundsException",
         is.read( buf, 1000, 0, 1001 ),
         IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
         "should throw IndexOutOfBoundsException",
         is.read( buf, 1000, 1001, 0 ),
         IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
         "should throw IndexOutOfBoundsException",
         is.read( buf, 1000, 500, 501 ),
         IndexOutOfBoundsException );

    {
        MyInputStream myStream( testStr );
        FilterInputStream is( &myStream );

        unsigned char buf[1000];

        is.close();
        CPPUNIT_ASSERT_THROW_MESSAGE(
             "should throw IOException",
             is.read( buf, 1000, 0, 100 ),
             IOException );
    }
}
