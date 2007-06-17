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

#include "OpenWireResponseBuilder.h"

//#include <activemq/connector/stomp/commands/ConnectCommand.h>
//#include <activemq/connector/stomp/commands/ConnectedCommand.h>

using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::transport;

////////////////////////////////////////////////////////////////////////////////
Response* OpenWireResponseBuilder::buildResponse( const transport::Command* cmd ){

    // If this command requires a response we don't know what it is
    // so we throw an exception.
    if( cmd->isResponseRequired() ) {

        throw transport::CommandIOException( __FILE__, __LINE__,
            "OpenWireResponseBuilder - unrecognized command" );
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
Command* OpenWireResponseBuilder::buildIncomingCommand(
    const transport::Command* cmd ){

//    const commands::ConnectCommand* connectCommand =
//        dynamic_cast<const commands::ConnectCommand*>(cmd);
//
//    if( connectCommand != NULL ){
//        commands::ConnectedCommand* resp = new commands::ConnectedCommand();
//        resp->setCorrelationId( connectCommand->getCommandId() );
//
//        if( connectCommand->getClientId() == NULL )
//        {
//            resp->setSessionId( sessionId );
//        }
//        else
//        {
//            resp->setSessionId( connectCommand->getClientId() );
//        }
//
//        return resp;
//    }

    return NULL;
}
