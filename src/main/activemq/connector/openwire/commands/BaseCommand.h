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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASECOMMAND_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/Response.h>
#include <activemq/connector/openwire/commands/BaseDataStructure.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    template< typename T >
    class AMQCPP_API BaseCommand : public T,
                                   public BaseDataStructure {
    public:

        BaseCommand() {
            this->commandId = 0;
            this->responseRequired = false;
        }

        virtual ~BaseCommand() {}

        /**
         * Sets the Command Id of this Message
         * @param id Command Id
         */
        virtual void setCommandId( int id ) {
            this->commandId = id;
        }

        /**
         * Gets the Command Id of this Message
         * @return Command Id
         */
        virtual int getCommandId() const {
            return commandId;
        }

        /**
         * Set if this Message requires a Response
         * @param required true if response is required
         */
        virtual void setResponseRequired( const bool required ) {
            this->responseRequired = required;
        }

        /**
         * Is a Response required for this Command
         * @return true if a response is required.
         */
        virtual bool isResponseRequired() const {
            return responseRequired;
        }

        /**
         * Returns a Cloned copy of this command, the caller is responsible
         * for deallocating the returned object.
         * @returns new copy of this command.
         */
        virtual transport::Command* cloneCommand() const {
            return dynamic_cast<transport::Command*>( this->cloneDataStructure() );
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {

            const BaseCommand<T>* command =
                dynamic_cast< const BaseCommand<T>* >( src );

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

    private:

        bool responseRequired;
        int commandId;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASECOMMAND_H_*/
