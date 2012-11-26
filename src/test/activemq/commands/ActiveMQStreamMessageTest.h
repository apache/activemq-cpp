/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGETEST_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace commands {

    class ActiveMQStreamMessageTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ActiveMQStreamMessageTest );
        CPPUNIT_TEST( testSetAndGet );
        CPPUNIT_TEST( testReadBoolean );
        CPPUNIT_TEST( testReadByte );
        CPPUNIT_TEST( testReadShort );
        CPPUNIT_TEST( testReadChar );
        CPPUNIT_TEST( testReadInt );
        CPPUNIT_TEST( testReadLong );
        CPPUNIT_TEST( testReadFloat );
        CPPUNIT_TEST( testReadDouble );
        CPPUNIT_TEST( testReadString );
        CPPUNIT_TEST( testReadBigString );
        CPPUNIT_TEST( testReadBytes );
        CPPUNIT_TEST( testClearBody );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testReadOnlyBody );
        CPPUNIT_TEST( testWriteOnlyBody );
        CPPUNIT_TEST_SUITE_END();

    private:

        std::vector<unsigned char> buffer;

    public:

        ActiveMQStreamMessageTest() : buffer() {}
        virtual ~ActiveMQStreamMessageTest() {}

        void setUp();
        void tearDown();

        void testSetAndGet();
        void testReadBoolean();
        void testReadByte();
        void testReadShort();
        void testReadChar();
        void testReadInt();
        void testReadLong();
        void testReadFloat();
        void testReadDouble();
        void testReadString();
        void testReadBigString();
        void testReadBytes();
        void testClearBody();
        void testReset();
        void testReadOnlyBody();
        void testWriteOnlyBody();

    };

}}

#endif /* _ACTIVEMQ_COMMANDS_ACTIVEMQSTREAMMESSAGETEST_H_ */
