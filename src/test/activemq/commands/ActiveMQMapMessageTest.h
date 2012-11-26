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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGETEST_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq{
namespace commands{

    class ActiveMQMapMessageTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ActiveMQMapMessageTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testBytesConversion );
        CPPUNIT_TEST( testGetBoolean );
        CPPUNIT_TEST( testGetByte );
        CPPUNIT_TEST( testGetShort );
        CPPUNIT_TEST( testGetChar );
        CPPUNIT_TEST( testGetInt );
        CPPUNIT_TEST( testGetLong );
        CPPUNIT_TEST( testGetFloat );
        CPPUNIT_TEST( testGetDouble );
        CPPUNIT_TEST( testGetString );
        CPPUNIT_TEST( testGetBytes );
        CPPUNIT_TEST( testGetMapNames );
        CPPUNIT_TEST( testItemExists );
        CPPUNIT_TEST( testClearBody );
        CPPUNIT_TEST( testReadOnlyBody );
        CPPUNIT_TEST( testWriteOnlyBody );
        CPPUNIT_TEST_SUITE_END();

    private:

        std::string name;

    public:

        ActiveMQMapMessageTest() : name("test-name") {}
        virtual ~ActiveMQMapMessageTest() {}

        void test();
        void testBytesConversion();
        void testGetBoolean();
        void testGetByte();
        void testGetShort();
        void testGetChar();
        void testGetInt();
        void testGetLong();
        void testGetFloat();
        void testGetDouble();
        void testGetString();
        void testGetBytes();
        void testGetMapNames();
        void testItemExists();
        void testClearBody();
        void testReadOnlyBody();
        void testWriteOnlyBody();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQMAPMESSAGETEST_H_*/
