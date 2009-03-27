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

#include "ProducerStateTest.h"

#include <activemq/state/ProducerState.h>
#include <activemq/commands/ProducerInfo.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ProducerStateTest::test() {
    Pointer<ProducerId> id( new ProducerId );
    id->setConnectionId( "CONNECTION" );
    id->setSessionId( 42 );
    id->setValue( 4096 );

    Pointer<ProducerInfo> info( new ProducerInfo() );
    info->setProducerId( id );
    ProducerState state( info );

    CPPUNIT_ASSERT( state.toString() != "NULL" );
    CPPUNIT_ASSERT( info == state.getInfo() );
}

