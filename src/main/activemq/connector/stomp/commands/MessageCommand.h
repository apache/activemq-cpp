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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMAND_H_

#include <cms/Message.h>
#include <activemq/connector/stomp/commands/StompMessage.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Message command which represents a ActiveMQMessage with no body
     * can be sent or recieved.
     */
    class AMQCPP_API MessageCommand : public StompMessage< cms::Message > {
    public:

        MessageCommand() :
            StompMessage< cms::Message >() {
                initialize( getFrame() );
        }

        MessageCommand( StompFrame* frame ) :
            StompMessage< cms::Message >( frame ) {
                validate( getFrame() );
        }

        virtual ~MessageCommand() {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return dynamic_cast<StompCommand*>( this->clone() );
        }

        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::Message* clone() const {

            MessageCommand* command =
                new MessageCommand( getFrame().clone() );

            command->setAckHandler( this->getAckHandler() );
            command->setReadOnlyBody( this->isReadOnlyBody() );
            command->setReadOnlyProperties( this->isReadOnlyProperties() );

            return command;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_MESSAGECOMMAND_H_*/
