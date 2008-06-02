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

#ifndef ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRERESPONSEBUILDER_H_
#define ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRERESPONSEBUILDER_H_

#include <activemq/util/Config.h>
#include <activemq/transport/MockTransport.h>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireResponseBuilder : public transport::MockTransport::ResponseBuilder{
    public:

        OpenWireResponseBuilder() {}
        virtual ~OpenWireResponseBuilder() {}

        virtual transport::Response* buildResponse( const transport::Command* command );

        virtual void buildIncomingCommands(
            const transport::Command* command,
            decaf::util::Queue<transport::Command*>& queue );

    };

}}}

#endif /*ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRERESPONSEBUILDER_H_*/
