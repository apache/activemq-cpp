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

#ifndef _ACTIVEMQ_TRANSPORT_RESPONSE_H_
#define _ACTIVEMQ_TRANSPORT_RESPONSE_H_

#include <activemq/util/Config.h>
#include <activemq/transport/Command.h>

namespace activemq{
namespace transport{

    /**
     * Defines the interface for Response Commands from the Broker, all
     * responses must implement this interface in order to work in the
     * transport layer.  Responses are mapped to the Command they are
     * linked to by a Correlation Id that is set by the Sedning Transport.
     */
    class AMQCPP_API Response : public Command{
    public:

        virtual ~Response() {}

        /**
         * Gets the Correlation Id that is associated with this message
         * @return the Correlation Id
         */
        virtual int getCorrelationId() const = 0;

        /**
         * Sets the Correlation Id if this Command
         * @param corrId
         */
        virtual void setCorrelationId( int corrId ) = 0;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_RESPONSE_H_*/
