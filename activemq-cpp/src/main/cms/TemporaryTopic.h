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

#ifndef _CMS_TEMPORARYTOPIC_H_
#define _CMS_TEMPORARYTOPIC_H_

#include <cms/Config.h>
#include <cms/Destination.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * Defines a Temporary Topic based Destination.
     *
     * A TemporaryTopic is a special type of Topic Destination that can only be consumed
     * from the Connection which created it.  TemporaryTopics are most commonly used as the
     * reply to address for Message's that implement the request response pattern.
     *
     * A TemporaryTopic is guaranteed to exist at the Provider only for the lifetime of the
     * Connection that created it.
     *
     * @since 1.0
     */
    class CMS_API TemporaryTopic : public Destination {
    public:

        virtual ~TemporaryTopic() throw();

        /**
         * Gets the name of this topic.
         *
         * @return The topic name.
         *
         * @throws CMSException - if an internal error occurs.
         */
        virtual std::string getTopicName() const = 0;

        /**
         * Destroy's the Temporary Destination at the Provider
         *
         * @throws CMSException
         */
        virtual void destroy() = 0;

   };

}

#endif /*_CMS_TEMPORARYTOPIC_H_*/
