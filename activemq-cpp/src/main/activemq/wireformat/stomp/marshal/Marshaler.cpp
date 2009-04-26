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

#include <activemq/wireformat/stomp/marshal/Marshaler.h>

#include <activemq/commands/Command.h>
#include <activemq/wireformat/stomp/StompCommandConstants.h>
#include <activemq/wireformat/stomp/StompFrame.h>

#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ActiveMQBytesMessage.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/BrokerError.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace activemq::wireformat::stomp::marshal;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::marshal( const Pointer<StompFrame>& frame )
    throw ( decaf::io::IOException ) {

    try {

        StompCommandConstants::CommandId commandId =
            StompCommandConstants::toCommandId( frame->getCommand().c_str() );

        Pointer<Command> command;

        if( commandId == StompCommandConstants::CONNECTED ){
//            command = new ConnectedCommand( frame );
        }
        else if( commandId == StompCommandConstants::ERROR_CMD ){
//            command = new ErrorCommand( frame );
        }
        else if( commandId == StompCommandConstants::RECEIPT ){
//            command = new ReceiptCommand( frame );
        }
        else if( commandId == StompCommandConstants::MESSAGE ){

//            if( !frame->getProperties().hasProperty(
//                    CommandConstants::toString(
//                        CommandConstants::HEADER_CONTENTLENGTH ) ) ) {
//                command = new TextMessageCommand( frame );
//            } else {
//                command = new BytesMessageCommand( frame );
//            }
        }

        // We either got a command or a response, but if we got neither
        // then complain, something went wrong.
        if( command == NULL ) {
            throw decaf::io::IOException(
                __FILE__, __LINE__,
                "Marshaler::marshal - No Command Created from frame");
        }

        return command;
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshal( const Pointer<Command>& command )
    throw ( decaf::io::IOException ) {

    try{

        return Pointer<StompFrame>();
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}
