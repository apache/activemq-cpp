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

#ifndef ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_
#define ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_

#include <cms/Connection.h>
#include <activemq/cmsutil/DummySession.h>

namespace activemq {
namespace cmsutil {

    class MessageContext;

    class DummyConnection: public cms::Connection {
    private:

        cms::ExceptionListener* listener;
        cms::MessageTransformer* transformer;
        std::string clientId;
        MessageContext* messageContext;

    private:

        DummyConnection(const DummyConnection&);
        DummyConnection& operator= (const DummyConnection&);

    public:

        DummyConnection(MessageContext* messageContext) :
            listener(), transformer(), clientId(), messageContext(messageContext) {}
        virtual ~DummyConnection() {}

        virtual const cms::ConnectionMetaData* getMetaData() const {
            return NULL;
        }

        virtual void close() {
        }

        virtual void start() {
        }

        virtual void stop() {
        }

        virtual cms::Session* createSession() {
            return new DummySession(messageContext);
        }

        virtual cms::Session* createSession(cms::Session::AcknowledgeMode ackMode) {
            DummySession* s = new DummySession(messageContext);
            s->setAcknowledgeMode(ackMode);
            return s;
        }

        virtual std::string getClientID() const {
            return clientId;
        }

        virtual void setClientID(const std::string& id) {
            this->clientId = id;
        }

        virtual cms::ExceptionListener* getExceptionListener() const {
            return listener;
        }

        virtual void setExceptionListener(cms::ExceptionListener* listener) {
            this->listener = listener;
        }

        virtual cms::MessageTransformer* getMessageTransformer() const {
            return transformer;
        }

        virtual void setMessageTransformer(cms::MessageTransformer* transformer) {
            this->transformer = transformer;
        }
    };

}}

#endif /*ACTIVEMQ_CMSUTIL_DUMMYCONNECTION_H_*/
