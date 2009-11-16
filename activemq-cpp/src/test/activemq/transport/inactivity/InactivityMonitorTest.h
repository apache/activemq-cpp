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

#ifndef _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITORTEST_H_
#define _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/util/Config.h>
#include <activemq/commands/WireFormatInfo.h>
#include <activemq/transport/mock/MockTransport.h>

#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {
namespace inactivity {

    class InactivityMonitorTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( InactivityMonitorTest );
        CPPUNIT_TEST( testCreate );
        CPPUNIT_TEST( testReadTimeout );
        CPPUNIT_TEST( testWriteMessageFail );
        CPPUNIT_TEST( testNonFailureSendCase );
        CPPUNIT_TEST_SUITE_END();

    private:

        decaf::lang::Pointer<mock::MockTransport> transport;

        Pointer<activemq::commands::WireFormatInfo> localWireFormatInfo;

    public:

        InactivityMonitorTest();
        virtual ~InactivityMonitorTest();

        virtual void setUp();
        virtual void tearDown();

        void testCreate();
        void testReadTimeout();
        void testWriteMessageFail();
        void testNonFailureSendCase();

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_INACTIVITY_INACTIVITYMONITORTEST_H_ */
