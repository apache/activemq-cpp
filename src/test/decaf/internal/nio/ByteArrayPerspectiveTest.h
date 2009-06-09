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

#ifndef _DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVETEST_H_
#define _DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/internal/nio/ByteArrayPerspective.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace internal{
namespace nio{

    class ByteArrayPerspectiveTest  : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ByteArrayPerspectiveTest );
        CPPUNIT_TEST( testReferences );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testReadExceptions );
        CPPUNIT_TEST( testWrite );
        CPPUNIT_TEST( testWriteExceptions );
        CPPUNIT_TEST( testCtor1 );
        CPPUNIT_TEST( testCtor2 );
        CPPUNIT_TEST( testClear );
        CPPUNIT_TEST( testReszie );
        CPPUNIT_TEST( testOperators );
        CPPUNIT_TEST( testOperatorsExceptions );
        CPPUNIT_TEST_SUITE_END();

    public:

        ByteArrayPerspectiveTest() {}
        virtual ~ByteArrayPerspectiveTest() {}

        virtual void testReferences();
        virtual void testRead();
        virtual void testWrite();
        virtual void testCtor1();
        virtual void testCtor2();
        virtual void testClear();
        virtual void testReszie();
        virtual void testOperators();
        virtual void testReadExceptions();
        virtual void testWriteExceptions();
        virtual void testOperatorsExceptions();

    };

}}}

#endif /*_DECAF_INTERNAL_NIO_BYTEARRAYPERSPECTIVETEST_H_*/
