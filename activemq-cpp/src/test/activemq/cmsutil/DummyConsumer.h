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

#ifndef _ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_
#define _ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_

#include <cms/MessageConsumer.h>
#include <activemq/cmsutil/MessageContext.h>

namespace activemq {
namespace cmsutil {

    class DummyConsumer: public cms::MessageConsumer {
    private:

        std::string selector;
        cms::MessageListener* listener;
        cms::MessageAvailableListener* messageAvailableListener;
        cms::MessageTransformer* transformer;
        MessageContext* messageContext;
        const cms::Destination* dest;
        bool noLocal;

    private:

        DummyConsumer(const DummyConsumer&);
        DummyConsumer& operator= (const DummyConsumer&);

    public:

        DummyConsumer(MessageContext* messageContext, const cms::Destination* dest, const std::string& selector, bool noLocal) :
            selector(selector), listener(NULL), messageAvailableListener(NULL), transformer(NULL),
            messageContext(messageContext), dest(dest), noLocal(noLocal) {
        }

        virtual ~DummyConsumer() {}

        virtual void close() {}

        virtual void start() {}

        virtual void stop() {}

        virtual cms::Message* receive() {
            return messageContext->receive(dest, selector, noLocal, 0);
        }

        virtual cms::Message* receive(int millisecs) {
            return messageContext->receive(dest, selector, noLocal, millisecs);
        }

        virtual cms::Message* receiveNoWait() {
            return messageContext->receive(dest, selector, noLocal, -1);
        }

        virtual void setMessageListener(cms::MessageListener* listener) {
            this->listener = listener;
        }

        virtual cms::MessageListener* getMessageListener() const {
            return listener;
        }

        virtual void setMessageAvailableListener(cms::MessageAvailableListener* listener) {
            messageAvailableListener = listener;
        }

        virtual cms::MessageAvailableListener* getMessageAvailableListener() const {
            return messageAvailableListener;
        }

        virtual std::string getMessageSelector() const {
            return selector;
        }

        virtual cms::MessageTransformer* getMessageTransformer() const {
            return transformer;
        }

        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            this->transformer = transformer;
        }

    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_DUMMYCONSUMER_H_*/
