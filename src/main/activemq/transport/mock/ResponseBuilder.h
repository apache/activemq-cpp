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

#ifndef _ACTIVEMQ_TRANSPORT_MOCK_RESPONSEBUILDER_H_
#define _ACTIVEMQ_TRANSPORT_MOCK_RESPONSEBUILDER_H_

#include <activemq/util/Config.h>

#include <activemq/commands/Command.h>
#include <activemq/commands/Response.h>

#include <decaf/lang/Pointer.h>
#include <decaf/util/LinkedList.h>

namespace activemq {
namespace transport {
namespace mock {

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * Interface for all Protocols to implement that defines the behavior
     * of the Broker in response to messages of that protocol.
     */
    class AMQCPP_API ResponseBuilder {
    public:

        virtual ~ResponseBuilder() {}

        /**
         * Given a Command, check if it requires a response and return the
         * appropriate Response that the Broker would send for this Command
         * @param command - The command to build a response for
         * @return A Response object pointer, or NULL if no response.
         */
        virtual Pointer<Response> buildResponse( const Pointer<Command>& command ) = 0;

        /**
         * When called the ResponseBuilder must construct all the
         * Responses or Asynchronous commands that would be sent to
         * this client by the Broker upon receipt of the passed command.
         * @param command - The Command being sent to the Broker.
         * @param queue - Queue of Command sent back from the broker.
         */
        virtual void buildIncomingCommands(
            const Pointer<Command>& command,
            decaf::util::LinkedList< Pointer<Command> >& queue ) = 0;

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_MOCK_RESPONSEBUILDER_H_ */
