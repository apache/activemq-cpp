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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_

#include <activemq/connector/openwire/commands/DataStructure.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class BaseDataStructure : public DataStructure
    {
    public:

        virtual ~BaseDataStructure() {}

        /**
         * Determine if this object is aware of marshalling and should have
         * its before and after marshalling methods called.  Defaults to false.
         * @returns true if aware of marshalling
         */
        virtual bool isMarshallAware() const {
            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_BASEDATASTRUCTURE_H_*/
