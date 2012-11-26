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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEST_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/core/ActiveMQAckHandler.h>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/ActiveMQTempTopic.h>
#include <activemq/commands/MessageId.h>

#include <decaf/lang/Pointer.h>

#include <vector>
#include <memory>

namespace activemq{
namespace commands{

    class ActiveMQMessageTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ActiveMQMessageTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testSetReadOnly );
        CPPUNIT_TEST( testSetToForeignJMSID );
        CPPUNIT_TEST( testEqualsObject );
        CPPUNIT_TEST( testShallowCopy );
        CPPUNIT_TEST( testCopy );
        CPPUNIT_TEST( testGetAndSetCMSMessageID );
        CPPUNIT_TEST( testGetAndSetCMSTimestamp );
        CPPUNIT_TEST( testGetAndSetCMSCorrelationID );
        CPPUNIT_TEST( testGetAndSetCMSDeliveryMode );
        CPPUNIT_TEST( testGetAndSetCMSRedelivered );
        CPPUNIT_TEST( testGetAndSetCMSType );
        CPPUNIT_TEST( testGetAndSetCMSExpiration );
        CPPUNIT_TEST( testGetAndSetCMSPriority );
        CPPUNIT_TEST( testClearProperties );
        CPPUNIT_TEST( testPropertyExists );
        CPPUNIT_TEST( testGetBooleanProperty );
        CPPUNIT_TEST( testGetByteProperty );
        CPPUNIT_TEST( testGetShortProperty );
        CPPUNIT_TEST( testGetIntProperty );
        CPPUNIT_TEST( testGetLongProperty );
        CPPUNIT_TEST( testGetFloatProperty );
        CPPUNIT_TEST( testGetDoubleProperty );
        CPPUNIT_TEST( testGetStringProperty );
        CPPUNIT_TEST( testGetPropertyNames );
        CPPUNIT_TEST( testSetEmptyPropertyName );
        CPPUNIT_TEST( testGetAndSetCMSXDeliveryCount );
        CPPUNIT_TEST( testClearBody );
        CPPUNIT_TEST( testBooleanPropertyConversion );
        CPPUNIT_TEST( testBytePropertyConversion );
        CPPUNIT_TEST( testShortPropertyConversion );
        CPPUNIT_TEST( testIntPropertyConversion );
        CPPUNIT_TEST( testLongPropertyConversion );
        CPPUNIT_TEST( testFloatPropertyConversion );
        CPPUNIT_TEST( testDoublePropertyConversion );
        CPPUNIT_TEST( testReadOnlyProperties );
        CPPUNIT_TEST( testIsExpired );
        CPPUNIT_TEST_SUITE_END();

    private:

        bool readOnlyMessage;
        decaf::lang::Pointer<commands::MessageId> cmsMessageId;
        std::string cmsCorrelationID;
        std::auto_ptr<commands::ActiveMQTopic> cmsDestination;
        std::auto_ptr<commands::ActiveMQTempTopic> cmsReplyTo;
        int cmsDeliveryMode;
        bool cmsRedelivered;
        std::string cmsType;
        long long cmsExpiration;
        int cmsPriority;
        long long cmsTimestamp;

        std::vector<long long> consumerIDs;

    public:

        ActiveMQMessageTest() : readOnlyMessage(), cmsMessageId(), cmsCorrelationID(),
                                cmsDestination(), cmsReplyTo(), cmsDeliveryMode(), cmsRedelivered(),
                                cmsType(), cmsExpiration(), cmsPriority(), cmsTimestamp(), consumerIDs() {
        }
        virtual ~ActiveMQMessageTest() {}

        virtual void setUp();
        virtual void tearDown();

        void test();
        void testSetReadOnly();
        void testSetToForeignJMSID();
        void testEqualsObject();
        void testShallowCopy();
        void testCopy();
        void testGetAndSetCMSMessageID();
        void testGetAndSetCMSTimestamp();
        void testGetAndSetCMSCorrelationID();
        void testGetAndSetCMSDeliveryMode();
        void testGetAndSetCMSRedelivered();
        void testGetAndSetCMSType();
        void testGetAndSetCMSExpiration();
        void testGetAndSetCMSPriority();
        void testClearProperties();
        void testPropertyExists();
        void testGetBooleanProperty();
        void testGetByteProperty();
        void testGetShortProperty();
        void testGetIntProperty();
        void testGetLongProperty();
        void testGetFloatProperty();
        void testGetDoubleProperty();
        void testGetStringProperty();
        void testGetPropertyNames();
        void testSetEmptyPropertyName();
        void testGetAndSetCMSXDeliveryCount();
        void testClearBody();
        void testBooleanPropertyConversion();
        void testBytePropertyConversion();
        void testShortPropertyConversion();
        void testIntPropertyConversion();
        void testLongPropertyConversion();
        void testFloatPropertyConversion();
        void testDoublePropertyConversion();
        void testStringPropertyConversion();
        void testReadOnlyProperties();
        void testIsExpired();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQMESSAGETEST_H_*/
