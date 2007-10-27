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

#ifndef _DECAF_IO_BUFFEREDOUTPUTSTREAMTEST_H_
#define _DECAF_IO_BUFFEREDOUTPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/io/BufferedOutputStream.h>
#include <decaf/util/Config.h>
#include <string.h>

namespace decaf{
namespace io{

    class BufferedOutputStreamTest : public CppUnit::TestFixture {

      CPPUNIT_TEST_SUITE( BufferedOutputStreamTest );
      CPPUNIT_TEST( testSmallerBuffer );
      CPPUNIT_TEST( testBiggerBuffer );
      CPPUNIT_TEST_SUITE_END();

    public:

        class MyOutputStream : public OutputStream{
        private:
            char buffer[100];
            std::size_t pos;
        public:

            MyOutputStream(){
                pos = 0;
                memset( buffer, 0, 100 );
            }
            virtual ~MyOutputStream(){}

            const char* getBuffer() const{ return buffer; }

            virtual void write( unsigned char c ) throw (IOException){
                if( pos >= 100 ){
                    throw IOException();
                }

                buffer[pos++] = c;
            }

            virtual void write( const unsigned char* buffer, std::size_t len ) throw (IOException){

                if( (pos + len) > 100 ){
                    throw IOException();
                }

                memcpy( this->buffer + pos, buffer, len );

                pos += len;
            }

            virtual void flush() throw (IOException){
            }

            virtual void close() throw(lang::Exception){
                // do nothing.
            }

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

    public:

        virtual ~BufferedOutputStreamTest(){}
        virtual void setUp(){}
        virtual void tearDown(){}

        void testSmallerBuffer();
        void testBiggerBuffer();

    };

}}

#endif /*_DECAF_IO_BUFFEREDOUTPUTSTREAMTEST_H_*/
