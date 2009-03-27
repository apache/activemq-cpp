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

#include "BrokerInfoTest.h"

#include <activemq/commands/BrokerInfo.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
void BrokerInfoTest::test()
{
    BrokerInfo myCommand;

    CPPUNIT_ASSERT( myCommand.getDataStructureType() == BrokerInfo::ID_BROKERINFO );

    myCommand.setBrokerName( "BrokerName" );
    myCommand.setBrokerURL( "http://www.example.com" );
    myCommand.setCommandId( 37 );
    myCommand.setMasterBroker( true );

    BrokerInfo* copy =
        dynamic_cast<BrokerInfo*>( myCommand.cloneDataStructure() );

    CPPUNIT_ASSERT( copy != NULL );
    CPPUNIT_ASSERT( copy->getBrokerName() == myCommand.getBrokerName() );
    CPPUNIT_ASSERT( copy->getBrokerURL() == myCommand.getBrokerURL() );
    CPPUNIT_ASSERT( copy->getCommandId() == myCommand.getCommandId() );
    CPPUNIT_ASSERT( copy->isMasterBroker() == myCommand.isMasterBroker() );

    delete copy;
}
