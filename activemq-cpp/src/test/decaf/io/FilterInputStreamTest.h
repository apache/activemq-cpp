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
        CPPUNIT_TEST( testReadBIIIExceptions );
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
        void testReadBIIIExceptions();

    };

}}

#endif /*_DECAF_IO_FILTERINPUTSTREAMTEST_H_*/
