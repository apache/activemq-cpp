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

#include "ConnectionStateTest.h"

#include <activemq/state/ConnectionState.h>
#include <activemq/state/SessionState.h>
#include <activemq/commands/SessionInfo.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTest::test() {

    // Create a Session
    Pointer<SessionId> sid(new SessionId);
    sid->setConnectionId("CONNECTION");
    sid->setValue(42);
    Pointer<SessionInfo> sinfo(new SessionInfo);
    sinfo->setSessionId(sid);

    Pointer<ConnectionId> connectionId(new ConnectionId());
    connectionId->setValue("CONNECTION");
    Pointer<ConnectionInfo> info(new ConnectionInfo);
    info->setConnectionId(connectionId);

    ConnectionState state(info);

    state.addSession(sinfo);
    CPPUNIT_ASSERT( state.getSessionStates().size() == 2 );
    state.removeSession(sinfo->getSessionId());
    CPPUNIT_ASSERT( state.getSessionStates().size() == 1 );

    state.addSession(sinfo);
    state.addSession(sinfo);
    CPPUNIT_ASSERT( state.getSessionStates().size() == 2 );
}
