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

#ifndef _DECAF_IO_OUTPUTSTREAMWRITERTEST_H_
#define _DECAF_IO_OUTPUTSTREAMWRITERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/io/ByteArrayOutputStream.h>
#include <decaf/io/OutputStreamWriter.h>
#include <decaf/io/InputStreamReader.h>

namespace decaf {
namespace io {

    class OutputStreamWriterTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( OutputStreamWriterTest );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testFlush );
        CPPUNIT_TEST( testWriteCharArrayIntIntInt );
        CPPUNIT_TEST( testWriteChar );
        CPPUNIT_TEST( testWriteStringIntInt );
        CPPUNIT_TEST( testOutputStreamWriterOutputStream );
        CPPUNIT_TEST( testWriteString );
        CPPUNIT_TEST_SUITE_END();

    private:

        OutputStreamWriter* writer1;

        ByteArrayOutputStream* buffer1;

        InputStreamReader* reader;

        static const int BUFFER_SIZE;
        static const std::string TEST_STRING;

    private:

        OutputStreamWriterTest(const OutputStreamWriterTest&);
        OutputStreamWriterTest& operator= (const OutputStreamWriterTest&);

    public:

        OutputStreamWriterTest();

        virtual ~OutputStreamWriterTest();

        virtual void setUp();
        virtual void tearDown();

        void testClose();
        void testFlush();
        void testWriteCharArrayIntIntInt();
        void testWriteChar();
        void testWriteStringIntInt();
        void testOutputStreamWriterOutputStream();
        void testWriteString();

    private:

        void openInputStream();

    };

}}

#endif /* _DECAF_IO_OUTPUTSTREAMWRITERTEST_H_ */
