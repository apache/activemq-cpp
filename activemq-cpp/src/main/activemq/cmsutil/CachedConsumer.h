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

#ifndef _ACTIVEMQ_CMSUTIL_CACHEDCONSUMER_H_
#define _ACTIVEMQ_CMSUTIL_CACHEDCONSUMER_H_

#include <cms/MessageConsumer.h>
#include <activemq/util/Config.h>

namespace activemq {
namespace cmsutil {

    /**
     * A cached message consumer contained within a pooled session.
     */
    class AMQCPP_API CachedConsumer: public cms::MessageConsumer {
    private:

        cms::MessageConsumer* consumer;

    private:

        CachedConsumer(const CachedConsumer&);
        CachedConsumer& operator=(const CachedConsumer&);

    public:

        CachedConsumer(cms::MessageConsumer* consumer);

        virtual ~CachedConsumer();

        /**
         * Does nothing - the real producer resource will be closed
         * by the lifecycle manager.
         */
        virtual void close() {
            // Do nothing.
        }

        virtual void start() {
            consumer->start();
        }

        virtual void stop() {
            consumer->stop();
        }

        virtual cms::Message* receive() {
            return consumer->receive();
        }

        virtual cms::Message* receive(int millisecs) {
            return consumer->receive(millisecs);
        }

        virtual cms::Message* receiveNoWait() {
            return consumer->receiveNoWait();
        }

        virtual void setMessageListener(cms::MessageListener* listener) {
            consumer->setMessageListener(listener);
        }

        virtual cms::MessageListener* getMessageListener() const {
            return consumer->getMessageListener();
        }

        virtual std::string getMessageSelector() const {
            return consumer->getMessageSelector();
        }

        virtual void setMessageAvailableListener(cms::MessageAvailableListener* listener) {
            consumer->setMessageAvailableListener(listener);
        }

        virtual cms::MessageAvailableListener* getMessageAvailableListener() const {
            return consumer->getMessageAvailableListener();
        }

        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            consumer->setMessageTransformer(transformer);
        }

        virtual cms::MessageTransformer* getMessageTransformer() const {
            return consumer->getMessageTransformer();
        }

    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_CACHEDCONSUMER_H_*/
