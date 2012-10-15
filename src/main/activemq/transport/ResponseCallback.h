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

#ifndef _ACTIVEMQ_TRANSPORT_RESPONSECALLBACK_H_
#define _ACTIVEMQ_TRANSPORT_RESPONSECALLBACK_H_

#include <activemq/util/Config.h>
#include <activemq/commands/Response.h>

#include <decaf/lang/Pointer.h>

namespace activemq {
namespace transport {

    /**
     * Allows an async send to complete at a later time via a Response event.
     */
    class AMQCPP_API ResponseCallback {
    private:

        ResponseCallback(const ResponseCallback&);
        ResponseCallback& operator= (const ResponseCallback&);

    public:

        ResponseCallback();
        virtual ~ResponseCallback();

    public:

        /**
         * When an Asynchronous operations completes this event is fired.
         *
         * The provided FutureResponse can either contain the result of the operation
         * or an exception indicating that the operation failed.
         *
         * @param response
         *      The result of the asynchronous operation that registered this call-back.
         */
        virtual void onComplete(decaf::lang::Pointer<commands::Response> response) = 0;

    };

}}

#endif /* _ACTIVEMQ_TRANSPORT_RESPONSECALLBACK_H_ */
