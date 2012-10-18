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

#ifndef _ACTIVEMQ_EXCEPTIONS_BROKEREXCEPTION_H_
#define _ACTIVEMQ_EXCEPTIONS_BROKEREXCEPTION_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/commands/BrokerError.h>

namespace activemq {
namespace exceptions {

    class AMQCPP_API BrokerException: public exceptions::ActiveMQException {
    public:

        BrokerException();

        BrokerException(const exceptions::ActiveMQException& ex);

        BrokerException(const BrokerException& ex);

        BrokerException(const char* file, const int lineNumber, const char* msg, ...);

        BrokerException(const char* file, const int lineNumber, const commands::BrokerError* error);

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         *
         * @returns new BrokerException instance that is a clone of this one.
         */
        virtual BrokerException* clone() const;

        virtual ~BrokerException() throw ();
    };

}}

#endif /*_ACTIVEMQ_EXCEPTIONS_BROKEREXCEPTION_H_*/
