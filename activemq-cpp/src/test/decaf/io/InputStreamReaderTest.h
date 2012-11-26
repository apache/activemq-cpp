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

#ifndef _DECAF_IO_INPUTSTREAMREADERTEST_H_
#define _DECAF_IO_INPUTSTREAMREADERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/io/ByteArrayInputStream.h>
#include <decaf/io/InputStreamReader.h>

namespace decaf {
namespace io {

    class InputStreamReaderTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( InputStreamReaderTest );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testConstructorInputStream );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testReady );
        CPPUNIT_TEST_SUITE_END();

    private:

        ByteArrayInputStream* buffer1;

        InputStreamReader* reader1;

        static const std::string TEST_STRING;

    private:

        InputStreamReaderTest(const InputStreamReaderTest&);
        InputStreamReaderTest& operator= (const InputStreamReaderTest&);

    public:

        InputStreamReaderTest();
        virtual ~InputStreamReaderTest();

        virtual void setUp();
        virtual void tearDown();

        void testClose();
        void testConstructorInputStream();
        void testRead();
        void testReady();

    };

}}

#endif /* _DECAF_IO_INPUTSTREAMREADERTEST_H_ */
