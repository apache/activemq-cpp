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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDITTEST_H_
#define _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDITTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq {
namespace core {

    class ActiveMQMessageAuditTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ActiveMQMessageAuditTest );
        CPPUNIT_TEST( testIsDuplicateString );
        CPPUNIT_TEST( testIsDuplicateMessageId );
        CPPUNIT_TEST( testIsInOrderString );
        CPPUNIT_TEST( testIsInOrderMessageId );
        CPPUNIT_TEST( testGetLastSeqId );
        CPPUNIT_TEST_SUITE_END();

    public:

        ActiveMQMessageAuditTest();
        virtual ~ActiveMQMessageAuditTest();

        void testIsDuplicateString();
        void testIsDuplicateMessageId();
        void testIsInOrderString();
        void testIsInOrderMessageId();
        void testGetLastSeqId();

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQMESSAGEAUDITTEST_H_ */
