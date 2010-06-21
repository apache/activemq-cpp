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

#ifndef ACTIVEMQ_CMSUTIL_SESSIONCALLBACK_H
#define ACTIVEMQ_CMSUTIL_SESSIONCALLBACK_H

#include <cms/Session.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    /**
     * Callback for executing any number of operations on a provided
     * CMS Session.
     */
    class AMQCPP_API SessionCallback {
    public:

        virtual ~SessionCallback(){}

        /**
         * Execute any number of operations against the supplied CMS
         * session.
         *
         * @param session
         *          the CMS <code>Session</code>
         *
         * @throws CMSException if thrown by CMS API methods
         */
        virtual void doInCms( cms::Session* session ) = 0;

    };

}}

#endif /*ACTIVEMQ_CMSUTIL_SESSIONCALLBACK_H*/
