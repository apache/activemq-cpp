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

#include "BufferedInputStreamTest.h"
#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::util;

namespace decaf{
namespace io{

    class MyInputStream : public InputStream{
    private:
        std::string data;
        std::size_t pos;
        bool throwOnRead;
        bool closed;

    public:

        MyInputStream( const std::string& data ){
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

        virtual std::size_t available() const throw (IOException){
            if( isClosed() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Stream already closed." );
            }
            std::size_t len = data.length();
            return len - pos;
        }

        virtual unsigned char read() throw (IOException){
            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            if( pos >= data.length() ){
                throw IOException();
            }

            return data.c_str()[pos++];
        }

        virtual int read( unsigned char* buffer,
                          std::size_t offset,
                          std::size_t bufferSize )
            throw (IOException){

            std::size_t numToRead = std::min( bufferSize, available() );

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
            for( std::size_t ix=0; ix<numToRead; ++ix ){
                buffer[ix+offset] = str[pos+ix];
            }

            pos += numToRead;

            return (int)numToRead;
        }

        virtual void close() throw( decaf::io::IOException){
            this->closed = true;
        }

        virtual std::size_t skip( std::size_t num ) throw ( io::IOException, lang::exceptions::UnsupportedOperationException ) {
            return ( pos += std::min( num, available() ) );
        }

        virtual void mark( int readLimit DECAF_UNUSED ) {

        }

        virtual void reset() throw ( IOException ) {
            throw IOException(
                __FILE__, __LINE__,
                "BufferedInputStream::reset - mark no yet supported." );
        }

        virtual bool markSupported() const{ return false; }

        virtual void lock() throw( decaf::lang::exceptions::RuntimeException ) {
        }

        virtual bool tryLock() throw( decaf::lang::exceptions::RuntimeException ) {
        }

        virtual void unlock() throw( decaf::lang::exceptions::RuntimeException ) {
        }

        virtual void wait() throw( decaf::lang::exceptions::RuntimeException,
                                   decaf::lang::exceptions::IllegalMonitorStateException,
                                   decaf::lang::exceptions::InterruptedException ) {
        }

        virtual void wait( long long millisecs )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {
        }

        virtual void wait( long long millisecs, int nanos )
            throw( decaf::lang::exceptions::RuntimeException,
                   decaf::lang::exceptions::IllegalArgumentException,
                   decaf::lang::exceptions::IllegalMonitorStateException,
                   decaf::lang::exceptions::InterruptedException ) {
        }

        virtual void notify() throw( decaf::lang::exceptions::RuntimeException,
                                     decaf::lang::exceptions::IllegalMonitorStateException ) {
        }

        virtual void notifyAll() throw( decaf::lang::exceptions::RuntimeException,
                                        decaf::lang::exceptions::IllegalMonitorStateException ) {
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testConstructor() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );

    // Test for method BufferedInputStream(InputStream, int)
    bool exceptionFired = false;
    try {

        // Create buffer with exact size of data
        BufferedInputStream is( &myStream, testStr.length() );

        // Ensure buffer gets filled by evaluating one read
        is.read();

        // Read the remaining buffered characters, no IOException should
        // occur.
        is.skip( testStr.length() - 2 );
        is.read();
        try {
            // is.read should now throw an exception because it will have to
            // be filled.
            is.read();
        } catch (IOException& e) {
            exceptionFired = true;
        }

        CPPUNIT_ASSERT_MESSAGE( "Exception should have been triggered by read()", exceptionFired );

    } catch (IOException& e) {
        e.printStackTrace();
        CPPUNIT_ASSERT_MESSAGE("Exception during test_1_Constructor", false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testAvailable() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    // Create buffer with exact size of data
    BufferedInputStream is( &myStream, testStr.length() );

    // Test for method int BufferedInputStream.available()
    try {
        CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                                 is.available() == testStr.length() );
    } catch( IOException& e ) {
        CPPUNIT_ASSERT_MESSAGE("Exception during available test", false );
    }

    // Test that a closed stream throws an IOE for available()
    std::string testStr2 = "hello world";
    MyInputStream myStream2( testStr2 );
    BufferedInputStream bis( &myStream2, testStr2.length() );

    std::size_t available;

    try {
        available = bis.available();
        bis.close();
    } catch( IOException& ex ) {
        CPPUNIT_ASSERT(false);
        return; // never reached.
    }
    CPPUNIT_ASSERT( available != 0 );

    try {
        bis.available();
        CPPUNIT_ASSERT_MESSAGE("Expected test to throw IOE.", false );
    } catch (IOException& ex) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testClose() {

    try{
        std::string testStr = "TEST12345678910";
        MyInputStream myStream( testStr );

        // Test for method void java.io.BufferedInputStream.close()
        BufferedInputStream i1( &myStream );
        BufferedInputStream i2( &myStream );

        // Test a null stream
        MyInputStream* ptr = NULL;
        BufferedInputStream buf( ptr, (std::size_t)5 );
        buf.close();
    } catch(...) {
        CPPUNIT_FAIL("Close shouldn't throw an error here" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testRead() {

    try {
        // Test for method int BufferedInputStream.read()
        std::string testStr = "TEST12345678910";
        MyInputStream myStream( testStr );
        // Create buffer with exact size of data
        BufferedInputStream is( &myStream, testStr.length() );

        char c = is.read();
        CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                                c == testStr.at(0) );
    } catch( IOException& e ) {
        CPPUNIT_FAIL( "Exception during read test" );
    }

    unsigned char bytes[256];
    for( int i = 0; i < 256; i++ ) {
        bytes[i] = (unsigned char)i;
    }

    // New stream, owns the inner one.
    BufferedInputStream is(
        new ByteArrayInputStream( &bytes[0], (std::size_t)256 ), (std::size_t)12, true );

    try {
        CPPUNIT_ASSERT_MESSAGE( "Wrong initial byte", 0 == is.read() );
        // Fill the buffer
        unsigned char buf[14];
        is.read( &buf[0], 0, (std::size_t)14 );

        // Read greater than the buffer
        CPPUNIT_ASSERT_MESSAGE( "Wrong block read data",
                string( (const char*)&buf[0], 14 ) ==
                string( (const char*)&bytes[1], 14 ) );

        CPPUNIT_ASSERT_MESSAGE("Wrong bytes", 15 == is.read() ); // Check next byte

    } catch( IOException& e ) {
        CPPUNIT_FAIL("Exception during read test");
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testRead2() {

    std::string testStr;
    testStr.append( "A", 3000 );
    testStr.append( "B", 1000 );
    MyInputStream myStream( testStr );
    // Create buffer with exact size of data
    BufferedInputStream is( &myStream, testStr.length() );

    // Test for method int BufferedInputStream.read(byte [], int,
    // int)
    unsigned char buf1[100];
    try {
        is.skip( 3000 );
        is.read( buf1, 0, 100 );
        CPPUNIT_ASSERT_MESSAGE(
            "Failed to read correct data",
            string( (const char*)&buf1[0], 100 ) == testStr.substr( 3000, 100 ) );

    } catch (IOException& e) {
        CPPUNIT_FAIL("Exception during read test");
    }
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testReadException() {

    BufferedInputStream bis( NULL );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        bis.read( NULL, 0, (size_t)-1 ),
        IOException );

    bis.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "should throw IOException",
        bis.read( NULL, 0, 1 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testSmallerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, (std::size_t)1 );

    std::size_t available = bufStream.available();
    CPPUNIT_ASSERT( available == testStr.length() );

    unsigned char dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 1) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    std::size_t numRead = bufStream.read( dummyBuf, 0, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}

////////////////////////////////////////////////////////////////////////////////
void BufferedInputStreamTest::testBiggerBuffer(){

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    BufferedInputStream bufStream( &myStream, (std::size_t)10 );

    std::size_t available = bufStream.available();
    CPPUNIT_ASSERT( available == testStr.length() );

    unsigned char dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 1 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'E' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'S' );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );

    dummy = bufStream.read();
    CPPUNIT_ASSERT( dummy == 'T' );

    unsigned char dummyBuf[20];
    memset( dummyBuf, 0, 20 );
    std::size_t numRead = bufStream.read( dummyBuf, 0, 10 );
    CPPUNIT_ASSERT( numRead == 10 );
    CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );

    available = bufStream.available();
    CPPUNIT_ASSERT( available == 1 );
}
