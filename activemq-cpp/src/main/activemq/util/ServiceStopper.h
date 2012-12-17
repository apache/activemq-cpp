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

#ifndef _ACTIVEMQ_UTIL_SERVICESTOPPER_H_
#define _ACTIVEMQ_UTIL_SERVICESTOPPER_H_

#include <activemq/util/Config.h>

#include <decaf/lang/Exception.h>

namespace activemq {
namespace util {

    class Service;

    class AMQCPP_API ServiceStopper {
    private:

        decaf::lang::Exception firstException;
        bool hasException;

    private:

        ServiceStopper(const ServiceStopper&);
        ServiceStopper& operator= (const ServiceStopper&);

    public:

        ServiceStopper();
        virtual ~ServiceStopper();

        void stop(Service* service);

        void throwFirstException();

        virtual void onException(Service* service, decaf::lang::Exception& ex);

    };

}}

#endif /* _ACTIVEMQ_UTIL_SERVICESTOPPER_H_ */
