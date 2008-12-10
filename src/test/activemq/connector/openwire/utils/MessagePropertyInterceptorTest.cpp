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

#include "MessagePropertyInterceptorTest.h"

#include <activemq/connector/openwire/utils/MessagePropertyInterceptor.h>
#include <activemq/connector/openwire/commands/Message.h>
#include <activemq/util/PrimitiveMap.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
void MessagePropertyInterceptorTest::test() {

    PrimitiveMap properties;
    Message message;

    MessagePropertyInterceptor interceptor( &message, &properties );

    CPPUNIT_ASSERT( message.getGroupID() == "" );
    CPPUNIT_ASSERT( message.getGroupSequence() == 0 );
    CPPUNIT_ASSERT( message.getRedeliveryCounter() == 0 );

    interceptor.setStringProperty( "JMSXGroupID", "TEST" );
    interceptor.setStringProperty( "JMSXGroupSeq", "15" );
    interceptor.setStringProperty( "JMSXDeliveryCount", "12" );

    CPPUNIT_ASSERT( message.getGroupID() == "TEST" );
    CPPUNIT_ASSERT( message.getGroupSequence() == 15 );
    CPPUNIT_ASSERT( message.getRedeliveryCounter() == 12 );
    CPPUNIT_ASSERT( interceptor.getStringProperty( "JMSXGroupID" ) == "TEST" );
    CPPUNIT_ASSERT( interceptor.getIntProperty( "JMSXGroupSeq" ) == 15 );
    CPPUNIT_ASSERT( interceptor.getIntProperty( "JMSXDeliveryCount" ) == 12 );

    interceptor.setStringProperty( "JMSXGroupSeq", "15" );
    interceptor.setStringProperty( "JMSXDeliveryCount", "12" );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an ActiveMQException",
        interceptor.setBooleanProperty( "JMSXGroupSeq", false ),
        ActiveMQException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an ActiveMQException",
        interceptor.setStringProperty( "JMSXGroupSeq", "FOO" ),
        ActiveMQException );

}
