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

#ifndef _ACTIVEMQ_COMMANDS_BASECOMMAND_H_
#define _ACTIVEMQ_COMMANDS_BASECOMMAND_H_

#include <activemq/util/Config.h>
#include <activemq/commands/Command.h>

namespace activemq{
namespace commands{

    class AMQCPP_API BaseCommand : public Command {
    private:

        bool responseRequired;
        int commandId;

    public:

        BaseCommand() : Command(), responseRequired(false), commandId(0) {
        }

        virtual ~BaseCommand() {}

        virtual void setCommandId( int id ) {
            this->commandId = id;
        }

        virtual int getCommandId() const {
            return commandId;
        }

        virtual void setResponseRequired( const bool required ) {
            this->responseRequired = required;
        }

        virtual bool isResponseRequired() const {
            return responseRequired;
        }

        virtual void copyDataStructure( const DataStructure* src ) {

            const BaseCommand* command =
                dynamic_cast< const BaseCommand* >( src );

            this->setResponseRequired( command->isResponseRequired() );
            this->setCommandId( command->getCommandId() );
        }

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const {
            std::ostringstream stream;

            stream << "Begin Class = BaseCommand" << std::endl;
            stream << BaseDataStructure::toString();
            stream << "  Response Required = " << responseRequired << std::endl;
            stream << "  Command Id = " << commandId << std::endl;
            stream << "End Class = BaseCommand" << std::endl;

            return stream.str();
        }

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const {
            return BaseDataStructure::equals( value );
        }

        virtual bool isConnectionControl() const {
            return false;
        }
        virtual bool isConnectionInfo() const {
            return false;
        }
        virtual bool isConsumerInfo() const {
            return false;
        }
        virtual bool isBrokerInfo() const  {
            return false;
        }
        virtual bool isMessage() const  {
            return false;
        }
        virtual bool isMessageAck() const  {
            return false;
        }
        virtual bool isKeepAliveInfo() const {
            return false;
        }
        virtual bool isMessageDispatch() const  {
            return false;
        }
        virtual bool isMessageDispatchNotification() const  {
            return false;
        }
        virtual bool isProducerAck() const  {
            return false;
        }
        virtual bool isProducerInfo() const  {
            return false;
        }
        virtual bool isResponse() const {
            return false;
        }
        virtual bool isRemoveInfo() const {
            return false;
        }
        virtual bool isRemoveSubscriptionInfo() const {
            return false;
        }
        virtual bool isShutdownInfo() const  {
            return false;
        }
        virtual bool isTransactionInfo() const {
            return false;
        }
        virtual bool isWireFormatInfo() const  {
            return false;
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_BASECOMMAND_H_*/
