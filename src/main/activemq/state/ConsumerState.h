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

#ifndef _ACTIVEMQ_STATE_CONSUMERSTATE_H_
#define _ACTIVEMQ_STATE_CONSUMERSTATE_H_

#include <activemq/util/Config.h>

#include <activemq/commands/ConsumerInfo.h>
#include <decaf/lang/Pointer.h>

#include <string>
#include <memory>

namespace activemq {
namespace state {

    using namespace decaf::lang;
    using namespace activemq::commands;

    class AMQCPP_API ConsumerState {
    private:

        Pointer<ConsumerInfo> info;

    public:

        ConsumerState( const Pointer<ConsumerInfo>& info );

        virtual ~ConsumerState();

        std::string toString() const;

        const Pointer<ConsumerInfo>& getInfo() const {
            return this->info;
        }

    };

}}

#endif /*_ACTIVEMQ_STATE_CONSUMERSTATE_H_*/
