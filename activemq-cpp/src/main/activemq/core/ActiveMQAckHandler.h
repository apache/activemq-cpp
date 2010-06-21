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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQACKHANDLER_H_
#define _ACTIVEMQ_CORE_ACTIVEMQACKHANDLER_H_

#include <cms/CMSException.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace commands{
    class Message;
}
namespace core{

    /**
     * Interface class that is used to give CMS Messages an interface to
     * Ack themselves with.
     *
     * @since 2.0
     */
    class AMQCPP_API ActiveMQAckHandler {
    public:

        virtual ~ActiveMQAckHandler() {};

        /**
         * Method called to acknowledge the message once it has been received by a
         * MessageConsumer.
         *
         * @param message
         *      The Message to Acknowledge.
         *
         * @throw CMSException if an error occurs while acknowledging the given Message.
         */
        virtual void acknowledgeMessage( const commands::Message* message ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQACKHANDLER_H_*/
