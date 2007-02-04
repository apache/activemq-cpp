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

#include <activemq/transport/Command.h>
#include <activemq/transport/Response.h>
#include <activemq/connector/openwire/commands/BaseDataStructure.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    template< typename T > 
    class BaseCommand : public T,
                        public BaseDataStructure
    {
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

    private:
    
        bool responseRequired;
        int commandId;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASECOMMAND_H_*/
