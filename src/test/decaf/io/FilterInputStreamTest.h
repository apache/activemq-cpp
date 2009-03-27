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

#ifndef _DECAF_IO_FILTERINPUTSTREAMTEST_H_
#define _DECAF_IO_FILTERINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Exception.h>
#include <decaf/io/FilterInputStream.h>

namespace decaf{
namespace io{

    class FilterInputStreamTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( FilterInputStreamTest );
        CPPUNIT_TEST( testAvailable );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testRead2 );
        CPPUNIT_TEST( testRead3 );
        CPPUNIT_TEST( testSkip );
        CPPUNIT_TEST_SUITE_END();

    public:

        FilterInputStreamTest() {}
        virtual ~FilterInputStreamTest() {}

        void testAvailable();
        void testClose();
        void testRead();
        void testRead2();
        void testRead3();
        void testSkip();

    public:

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

            virtual void close() throw(lang::Exception){
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

            virtual void lock() throw(lang::Exception){
            }
            virtual void unlock() throw(lang::Exception){
            }
            virtual void wait() throw(lang::Exception){
            }
            virtual void wait(unsigned long millisecs DECAF_UNUSED) throw(lang::Exception){
            }
            virtual void notify() throw(lang::Exception){
            }
            virtual void notifyAll() throw(lang::Exception){
            }
        };

    };

}}

#endif /*_DECAF_IO_FILTERINPUTSTREAMTEST_H_*/
