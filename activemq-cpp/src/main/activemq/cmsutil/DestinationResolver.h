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
#ifndef _ACTIVEMQ_CMSUTIL_DESTINATIONRESOLVER_H_
#define _ACTIVEMQ_CMSUTIL_DESTINATIONRESOLVER_H_

#include <cms/Session.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    // Forward declarations.
    class ResourceLifecycleManager;

    /**
     * Resolves a CMS destination name to a <code>Destination</code>.
     */
    class AMQCPP_API DestinationResolver {
    public:

        virtual ~DestinationResolver() throw() {}

        /**
         * Initializes this destination resolver for use.  Ensures that any
         * previously allocated resources are first destroyed
         * (e.g. calls destroy()).
         *
         * @param mgr
         *      the resource lifecycle manager.
         */
        virtual void init( ResourceLifecycleManager* mgr ) = 0;

        /**
         * Destroys any allocated resources.
         */
        virtual void destroy() = 0;

        /**
         * Resolves the given name to a destination.  If
         * <code>pubSubDomain</code> is true, a topic will be returned,
         * otherwise a queue will be returned.
         *
         * @param session
         *      the session for which to retrieve resolve the
         *      destination.
         * @param destName
         *      the name to be resolved.
         * @param pubSubDomain
         *      If true, the name will be resolved to a Topic,
         *      otherwise a Queue.
         *
         * @return the resolved destination
         *
         * @throws cms::CMSException if resolution failed.
         */
        virtual cms::Destination* resolveDestinationName( cms::Session* session,
                                                          const std::string& destName,
                                                          bool pubSubDomain ) = 0;

    };

}}

#endif /* _ACTIVEMQ_CMSUTIL_DESTINATIONRESOLVER_H_ */
