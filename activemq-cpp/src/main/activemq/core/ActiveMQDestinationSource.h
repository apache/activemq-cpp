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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONSOURCE_H_
#define _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONSOURCE_H_

#include <cms/DestinationSource.h>

#include <activemq/util/Config.h>

namespace activemq {
namespace core {

    class DestinationSourceImpl;
    class ActiveMQConnection;

    class AMQCPP_API ActiveMQDestinationSource : public cms::DestinationSource {
    private:

        ActiveMQDestinationSource(ActiveMQDestinationSource&);
        ActiveMQDestinationSource& operator= (const ActiveMQDestinationSource&);

    private:

        DestinationSourceImpl* impl;

    public:

        ActiveMQDestinationSource(ActiveMQConnection* connection);

        virtual ~ActiveMQDestinationSource();

    public:

        virtual void start();

        virtual void stop();

        virtual void setListener(cms::DestinationListener* listener);

        virtual cms::DestinationListener* getListener() const;

        virtual std::vector<cms::Queue*> getQueues() const;

        virtual std::vector<cms::Topic*> getTopics() const;

        virtual std::vector<cms::TemporaryQueue*> getTemporaryQueues() const;

        virtual std::vector<cms::TemporaryTopic*> getTemporaryTopics() const;

    };

}}

#endif /* _ACTIVEMQ_CORE_ACTIVEMQDESTINATIONSOURCE_H_ */
