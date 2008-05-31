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

#include "StompDestinationTest.h"

using namespace std;
using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::stomp;

///////////////////////////////////////////////////////////////////////////////
void StompDestinationTest::test()
{
    MyDestination dest( "test" );

    CPPUNIT_ASSERT( dest.getTopicName() == "test" );

    MyDestination dest1( "test1?value1=1&value2=2" );

    CPPUNIT_ASSERT( dest1.getTopicName() == "test1" );
    CPPUNIT_ASSERT( dest1.getCMSProperties().hasProperty( "value1" ) == true );
    CPPUNIT_ASSERT( dest1.getCMSProperties().hasProperty( "value2" ) == true );
    CPPUNIT_ASSERT( dest1.getCMSProperties().hasProperty( "value3" ) != true );

    std::string value1 = dest1.getCMSProperties().getProperty( "value1" );
    std::string value2 = dest1.getCMSProperties().getProperty( "value2" );

    CPPUNIT_ASSERT( value1 == "1" );
    CPPUNIT_ASSERT( value2 == "2" );

    MyDestination* dest2 =
        dynamic_cast< MyDestination* >( dest1.clone() );

    CPPUNIT_ASSERT( dest2 != NULL );

    CPPUNIT_ASSERT( dest2->getTopicName() == "test1" );
    CPPUNIT_ASSERT( dest2->getCMSProperties().hasProperty( "value1" ) == true );
    CPPUNIT_ASSERT( dest2->getCMSProperties().hasProperty( "value2" ) == true );
    CPPUNIT_ASSERT( dest2->getCMSProperties().hasProperty( "value3" ) != true );

    value1 = dest2->getCMSProperties().getProperty( "value1" );
    value2 = dest2->getCMSProperties().getProperty( "value2" );

    CPPUNIT_ASSERT( value1 == "1" );
    CPPUNIT_ASSERT( value2 == "2" );

    delete dest2;

    MyDestination dest3("dummy");
    dest3.copy( dest1 );

    CPPUNIT_ASSERT( dest3.getTopicName() == "test1" );
    CPPUNIT_ASSERT( dest3.getCMSProperties().hasProperty( "value1" ) == true );
    CPPUNIT_ASSERT( dest3.getCMSProperties().hasProperty( "value2" ) == true );
    CPPUNIT_ASSERT( dest3.getCMSProperties().hasProperty( "value3" ) != true );

    value1 = dest3.getCMSProperties().getProperty( "value1" );
    value2 = dest3.getCMSProperties().getProperty( "value2" );

    CPPUNIT_ASSERT( value1 == "1" );
    CPPUNIT_ASSERT( value2 == "2" );

}
