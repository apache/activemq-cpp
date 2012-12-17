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

#include "ConnectionStateTrackerTest.h"

#include <activemq/state/ConnectionStateTracker.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/SessionState.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTrackerTest::test() {

    Pointer<ConnectionId> conn_id( new ConnectionId );
    conn_id->setValue( "CONNECTION" );
    Pointer<ConnectionInfo> conn_info( new ConnectionInfo );
    conn_info->setConnectionId( conn_id );

    Pointer<SessionId> session_id( new SessionId );
    session_id->setConnectionId( "CONNECTION" );
    session_id->setValue( 12345 );
    Pointer<SessionInfo> session_info( new SessionInfo );
    session_info->setSessionId( session_id );

    Pointer<ConsumerId> consumer_id( new ConsumerId );
    consumer_id->setConnectionId( "CONNECTION" );
    consumer_id->setSessionId( 12345 );
    consumer_id->setValue( 42 );
    Pointer<ConsumerInfo> consumer_info( new ConsumerInfo );
    consumer_info->setConsumerId( consumer_id );

    Pointer<ProducerId> producer_id( new ProducerId );
    producer_id->setConnectionId( "CONNECTION" );
    producer_id->setSessionId( 12345 );
    producer_id->setValue( 42 );
    Pointer<ProducerInfo> producer_info( new ProducerInfo );
    producer_info->setProducerId( producer_id );

    ConnectionStateTracker tracker;
    tracker.processConnectionInfo( conn_info.get() );
    tracker.processSessionInfo( session_info.get() );
    tracker.processConsumerInfo( consumer_info.get() );
    tracker.processProducerInfo( producer_info.get() );

    tracker.processRemoveProducer( producer_id.get() );
    tracker.processRemoveConsumer( consumer_id.get() );
    tracker.processRemoveSession( session_id.get() );
    tracker.processRemoveConnection( conn_id.get() );

}
