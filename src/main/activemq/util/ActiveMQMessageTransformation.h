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

#ifndef _ACTIVEMQ_UTIL_ACTIVEMQMESSAGETRANSFORMATION_H_
#define _ACTIVEMQ_UTIL_ACTIVEMQMESSAGETRANSFORMATION_H_

#include <cms/Message.h>
#include <cms/Destination.h>

#include <activemq/util/Config.h>

#include <string>

namespace activemq {
namespace core {
    class ActiveMQConnection;
}
namespace commands {
    class Message;
    class ActiveMQDestination;
}
namespace util {

    class AMQCPP_API ActiveMQMessageTransformation {
    private:

        ActiveMQMessageTransformation();

    public:

        virtual ~ActiveMQMessageTransformation();

        /**
         * Creates a fast shallow copy of the current ActiveMQDestination or creates a whole new
         * destination instance from an available CMS destination from another provider.
         *
         * This method will return true if the passed CMS Destination was cloned and a new Destination
         * object created or false if the input Destination was already an ActiveMQ destination.  The
         * should use the return value as a hint to determine if it needs to delete the amqDestinatio
         * object or not.
         *
         * @param destination
         *      Destination to be converted into ActiveMQ's implementation.
         * @param amqDestination
         *      Pointer to a pointer where the casted or cloned AMQ destination is stored.
         *
         * @return true if the amqDestination is a new instance and not just a cast of the input destination.
         *
         * @throws CMSException if an error occurs
         */
        static bool transformDestination(const cms::Destination* destination, const commands::ActiveMQDestination** amqDestination);

        /**
         * Creates a fast shallow copy of the current ActiveMQMessage or creates a whole new
         * message instance from an available CMS message from another provider.
         *
         * This method will return true if the passed CMS Message was cloned and a new ActiveMQMessage
         * object created or false if the input Message was already an ActiveMQMessage instance.  The
         * caller should use the return value as a hint to determine if it needs to delete the resulting
         * ActiveMQMessage object or not.
         *
         * @param message
         *      CMS Message to be converted into ActiveMQ's implementation.
         * @param amqMessage
         *      Pointer to a pointer where the casted or cloned AMQ message is stored.
         *
         * @return true if the amqMessage is a new instance and not just a cast of the input message.
         *
         * @throws CMSException if an error occurs
         */
        static bool transformMessage(cms::Message* message, core::ActiveMQConnection* connection, commands::Message** amqMessage);

        /**
         * Copies the standard CMS and user defined properties from the given message to the
         * specified message.
         *
         * @param fromMessage
         *      The message to take the properties from.
         * @param toMessage
         *      The message to add the properties to.
         *
         * @throws CMSException if an error occurs during the copy.
         */
        static void copyProperties(const cms::Message* fromMessage, cms::Message* toMessage);

    };

}}

#endif /* _ACTIVEMQ_UTIL_ACTIVEMQMESSAGETRANSFORMATION_H_ */
