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

#ifndef _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNELTEST_H_
#define _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNELTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace core {

    class SimplePriorityMessageDispatchChannelTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( SimplePriorityMessageDispatchChannelTest );
        CPPUNIT_TEST( testCtor );
        CPPUNIT_TEST( testStart );
        CPPUNIT_TEST( testStop );
        CPPUNIT_TEST( testClose );
        CPPUNIT_TEST( testEnqueue );
        CPPUNIT_TEST( testEnqueueFront );
        CPPUNIT_TEST( testPeek );
        CPPUNIT_TEST( testDequeueNoWait );
        CPPUNIT_TEST( testDequeue );
        CPPUNIT_TEST( testRemoveAll );
        CPPUNIT_TEST_SUITE_END();

    public:

        SimplePriorityMessageDispatchChannelTest() {}
        virtual ~SimplePriorityMessageDispatchChannelTest() {}

        void testCtor();
        void testStart();
        void testStop();
        void testClose();
        void testEnqueue();
        void testEnqueueFront();
        void testPeek();
        void testDequeueNoWait();
        void testDequeue();
        void testRemoveAll();

    };

}}

#endif /* _ACTIVEMQ_CORE_SIMPLEPRIORITYMESSAGEDISPATCHCHANNELTEST_H_ */
