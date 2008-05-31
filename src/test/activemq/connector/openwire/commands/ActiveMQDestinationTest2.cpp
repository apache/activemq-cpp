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

#include <decaf/util/UUID.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
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
    CPPUNIT_ASSERT( dest.isProducerAdvisory() == false );
    CPPUNIT_ASSERT( dest.isConnectionAdvisory() == false );

    MyDestination dest2;
    dest2.copyDataStructure( &dest );

    CPPUNIT_ASSERT( dest2.getPhysicalName().find( "test" ) != string::npos );
    CPPUNIT_ASSERT( dest2.isAdvisory() == true );
    CPPUNIT_ASSERT( dest2.isExclusive() == true );
    CPPUNIT_ASSERT( dest2.isAdvisory() == true );
    CPPUNIT_ASSERT( dest2.isConsumerAdvisory() == false );
    CPPUNIT_ASSERT( dest2.isProducerAdvisory() == false );

    MyDestination* dest3 = NULL;
    dest3 = dynamic_cast<MyDestination*>( dest.cloneDataStructure() );

    CPPUNIT_ASSERT( dest3 != NULL );
    CPPUNIT_ASSERT( dest3->getPhysicalName().find( "test" ) != string::npos );
    CPPUNIT_ASSERT( dest3->isAdvisory() == true );
    CPPUNIT_ASSERT( dest3->isExclusive() == true );
    CPPUNIT_ASSERT( dest3->isAdvisory() == true );
    CPPUNIT_ASSERT( dest3->isConsumerAdvisory() == false );
    CPPUNIT_ASSERT( dest3->isProducerAdvisory() == false );

    delete dest3;

    std::string clientId = UUID::randomUUID().toString();
    std::string result = dest.createTemporaryName( clientId );
    CPPUNIT_ASSERT( result.find( clientId ) != string::npos );
    dest.setPhysicalName( result );
    CPPUNIT_ASSERT( clientId != dest.getClientId( &dest ) );
    MyTempDestination tmpDest;
    tmpDest.setPhysicalName( result );
    CPPUNIT_ASSERT( clientId == ActiveMQDestination::getClientId( &tmpDest ) );

}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQDestinationTest::testOptions(){

    MyDestination dest;

    dest.setPhysicalName( "test?option1=test1&option2=test2" );

    const util::ActiveMQProperties& properties = dest.getOptions();

    CPPUNIT_ASSERT( properties.hasProperty( "option1" ) == true );
    CPPUNIT_ASSERT( properties.hasProperty( "option2" ) == true );

    CPPUNIT_ASSERT( std::string( properties.getProperty( "option1" ) ) == "test1" );
    CPPUNIT_ASSERT( std::string( properties.getProperty( "option2" ) ) == "test2" );
}
