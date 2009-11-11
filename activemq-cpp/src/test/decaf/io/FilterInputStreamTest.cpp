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

        virtual int read() throw (IOException){
            if( this->isThrowOnRead() ) {
                throw IOException(
                    __FILE__, __LINE__,
                    "MyInputStream::read - Throw on Read on." );
            }

            if( pos >= data.length() ){
                return -1;
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

        virtual void close() throw(IOException){
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
            return false;
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
void FilterInputStreamTest::testAvailable() {

    std::string testStr = "TEST12345678910";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect number of available bytes",
                            is.available() == testStr.length() );
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

    char c = is.read();
    CPPUNIT_ASSERT_MESSAGE( "read returned incorrect char",
                            c == testStr.at(0) );
}

////////////////////////////////////////////////////////////////////////////////
void FilterInputStreamTest::testRead2() {

    std::string testStr = "TEST12345678910ABCDEFGHIJKLMNOPQRSTU";
    MyInputStream myStream( testStr );
    FilterInputStream is( &myStream );

    unsigned char buf[30];
    is.read( buf, 0, 30 );
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
    is.read( buf, 0, 100 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to read correct data",
        string( (const char*)buf, 100 ) == testStr.substr( 3000, 100 ) );
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
    is.read( buf, 0, 100 );
    CPPUNIT_ASSERT_MESSAGE( "Failed to skip to correct position",
            string( (const char*)buf, 100 ) == testStr.substr( 1000, 100 ) );
}
