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

#ifndef _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_
#define _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_

#include <activemq/util/Config.h>

#include <cms/CMSException.h>
#include <cms/MessageEOFException.h>
#include <cms/MessageFormatException.h>

#include <decaf/lang/Exception.h>

#include <string>

namespace activemq {
namespace util {

    class AMQCPP_API CMSExceptionSupport {
    private:

        CMSExceptionSupport();

    public:

        virtual ~CMSExceptionSupport();

        static cms::CMSException create( const std::string& msg, const decaf::lang::Exception& cause );

        static cms::CMSException create( const decaf::lang::Exception& cause );

        static cms::MessageEOFException createMessageEOFException( const decaf::lang::Exception& cause );

        static cms::MessageFormatException createMessageFormatException( const decaf::lang::Exception& cause );

    };

}}

#endif /* _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_ */
