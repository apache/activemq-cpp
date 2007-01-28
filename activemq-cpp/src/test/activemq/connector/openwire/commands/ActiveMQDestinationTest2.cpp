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

#include "ActiveMQDestinationTest2.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::commands::ActiveMQDestinationTest );

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestinationTest::test()
{
    MyDestination dest;

    dest.setPhysicalName( "test" );
    dest.setAdvisory( true );
    dest.setExclusive( true );
    dest.setOrdered( true );

    CPPUNIT_ASSERT( dest.getPhysicalName().find( "test" ) != string::npos );
    CPPUNIT_ASSERT( dest.isAdvisory() == true );
    CPPUNIT_ASSERT( dest.isExclusive() == true );
    CPPUNIT_ASSERT( dest.isAdvisory() == true );
    CPPUNIT_ASSERT( dest.isConsumerAdvisory() == false );

}
