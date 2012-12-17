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

#ifndef _ACTIVEMQ_UTIL_SERVICELISTENER_H_
#define _ACTIVEMQ_UTIL_SERVICELISTENER_H_

#include <activemq/util/Config.h>

namespace activemq {
namespace util {

    class Service;

    /**
     * Listener interface for observers of Service related events.
     *
     * @since 3.3.0
     */
    class AMQCPP_API ServiceListener {
    public:

        virtual ~ServiceListener();

        /**
         * indicates that the target service has completed its start operation.
         *
         * @param target
         *      The service that triggered this notification.
         */
        virtual void started(const Service* target) = 0;

        /**
         * indicates that the target service has completed its stop operation.
         *
         * @param target
         *      The service that triggered this notification.
         */
        virtual void stopped(const Service* target) = 0;

    };

}}

#endif /* _ACTIVEMQ_UTIL_SERVICELISTENER_H_ */
