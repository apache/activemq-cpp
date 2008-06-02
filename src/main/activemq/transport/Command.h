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

#ifndef _ACTIVEMQ_TRANSPORT_COMMAND_H_
#define _ACTIVEMQ_TRANSPORT_COMMAND_H_

#include <string>
#include <activemq/util/Config.h>

namespace activemq{
namespace transport{

    class AMQCPP_API Command{
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
         * Returns a Cloned copy of this command, the caller is responsible
         * for deallocating the returned object.
         * @returns new copy of this command.
         */
        virtual Command* cloneCommand() const = 0;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_COMMAND_H_*/
