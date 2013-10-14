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

#ifndef _ACTIVEMQ_UTIL_SUSPENDABLE_H_
#define _ACTIVEMQ_UTIL_SUSPENDABLE_H_

#include <activemq/util/Config.h>

namespace activemq {
namespace util {

    /**
     * Interface for any resouce that support a suspend / resume style of operation.
     *
     * @since 3.9.0
     */
    class AMQCPP_API Suspendable {
    public:

        virtual ~Suspendable();

        /**
         * Suspend the given resource, all activity within the resource should halt and
         * enter a waiting state.  When a call to resume is made this resource should then
         * resume normal operation.
         *
         * @throws Exception if an error occurs while the resource is being suspended.
         */
        virtual void suspend() = 0;

        /**
         * Resumes normal operation of this resource after it has been suspended.
         *
         * @throws Exception if an error occurs while the resource is being resumed.
         */
        virtual void resume() = 0;

    };

}}

#endif /* _ACTIVEMQ_UTIL_SUSPENDABLE_H_ */
