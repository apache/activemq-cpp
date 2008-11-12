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
#ifndef _ACTIVEMQ_CMSUTIL_DYNAMICDESTINATIONRESOLVER_H_
#define _ACTIVEMQ_CMSUTIL_DYNAMICDESTINATIONRESOLVER_H_

#include <activemq/cmsutil/DestinationResolver.h>
#include <cms/Session.h>
#include <decaf/util/Map.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    /**
     * Resolves a CMS destination name to a <code>Destination</code>.
     */
    class AMQCPP_API DynamicDestinationResolver : public DestinationResolver {
    private:

        /**
         * Manages maps of names to topics and queues for a single session.
         */
        class SessionResolver {
        private:

            ResourceLifecycleManager* resourceLifecycleManager;
            cms::Session* session;
            decaf::util::Map<std::string, cms::Topic*> topicMap;
            decaf::util::Map<std::string, cms::Queue*> queueMap;

        public:

            SessionResolver(cms::Session* session,
                ResourceLifecycleManager* resourceLifecycleManager ) {

                this->session = session;
                this->resourceLifecycleManager = resourceLifecycleManager;
            }

            virtual ~SessionResolver() {}

            cms::Topic* getTopic(const std::string& topicName )
                throw ( cms::CMSException );

            cms::Queue* getQueue(const std::string& queueName )
                throw ( cms::CMSException );

        };

        /**
         * Maps a given session to the resolver for that session.
         */
        decaf::util::Map< cms::Session*, SessionResolver*> sessionResolverMap;

        /**
         * Manages lifecycle of any allocated topics/queues.
         */
        ResourceLifecycleManager* resourceLifecycleManager;

    public:

        virtual ~DynamicDestinationResolver();

        virtual void init( ResourceLifecycleManager* mgr ) {

            // since we're changing the lifecycle manager, clear out references
            // to old resources.
            destroy();

            this->resourceLifecycleManager = mgr;
        }

        virtual void destroy();

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
         * @return the resolved destination
         * @throws cms::CMSException if resolution failed.
         */
        virtual cms::Destination* resolveDestinationName(
            cms::Session* session,
            const std::string& destName,
            bool pubSubDomain )
                throw ( cms::CMSException );

    };

}}

#endif /* _ACTIVEMQ_CMSUTIL_DYNAMICDESTINATIONRESOLVER_H_ */
