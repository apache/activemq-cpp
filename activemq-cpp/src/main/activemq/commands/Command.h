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

#ifndef _ACTIVEMQ_COMMANDS_COMMAND_H_
#define _ACTIVEMQ_COMMANDS_COMMAND_H_

#include <string>
#include <activemq/util/Config.h>
#include <activemq/commands/BaseDataStructure.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Pointer.h>

namespace activemq{
namespace state{
    class CommandVisitor;
}
namespace commands{

    class AMQCPP_API Command : public BaseDataStructure {
    public:

        virtual ~Command() {}

        /**
         * Sets the Command Id of this Message
         * @param id Command Id
         */
        virtual void setCommandId( int id ) = 0;

        /**
         * Gets the Command Id of this Message
         * @return Command Id
         */
        virtual int getCommandId() const = 0;

        /**
         * Set if this Message requires a Response
         * @param required true if response is required
         */
        virtual void setResponseRequired( const bool required ) = 0;

        /**
         * Is a Response required for this Command
         * @return true if a response is required.
         */
        virtual bool isResponseRequired() const = 0;

        /**
         * Returns a provider-specific string that provides information
         * about the contents of the command.
         */
        virtual std::string toString() const = 0;

        /**
         * Allows a Visitor to visit this command and return a response to the
         * command based on the command type being visited.  The command will call
         * the proper processXXX method in the visitor.
         *
         * @return a Response to the visitor being called or NULL if no response.
         */
        virtual decaf::lang::Pointer<commands::Command> visit(
            activemq::state::CommandVisitor* visitor ) = 0;

        /*
         * This section contains a set of short-cut methods for determining if a
         * Command is of a certain type.  These are the most commonly used Commands
         * and we save several casts and some ugly code by just adding these here.
         */
        virtual bool isConnectionControl() const = 0;
        virtual bool isConnectionInfo() const = 0;
        virtual bool isConsumerInfo() const = 0;
        virtual bool isBrokerInfo() const = 0;
        virtual bool isKeepAliveInfo() const = 0;
        virtual bool isMessage() const = 0;
        virtual bool isMessageAck() const = 0;
        virtual bool isMessageDispatch() const = 0;
        virtual bool isMessageDispatchNotification() const = 0;
        virtual bool isProducerAck() const = 0;
        virtual bool isProducerInfo() const = 0;
        virtual bool isResponse() const = 0;
        virtual bool isRemoveInfo() const = 0;
        virtual bool isRemoveSubscriptionInfo() const = 0;
        virtual bool isShutdownInfo() const = 0;
        virtual bool isTransactionInfo() const = 0;
        virtual bool isWireFormatInfo() const = 0;

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_COMMAND_H_*/
