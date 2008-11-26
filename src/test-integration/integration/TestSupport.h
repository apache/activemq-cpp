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

#ifndef _INTEGRATION_TESTSUPPORT_H_
#define _INTEGRATION_TESTSUPPORT_H_

#include <decaf/util/concurrent/Mutex.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageProducer.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>

#include <integration/IntegrationCommon.h>

namespace integration{

    class TestSupport : public cms::ExceptionListener, public cms::MessageListener
    {
    public:

      TestSupport();
      virtual ~TestSupport();

        virtual void initialize(
            const std::string& brokerUrl,
            cms::Session::AcknowledgeMode ackMode = cms::Session::AUTO_ACKNOWLEDGE );

        virtual void close();

        virtual decaf::util::concurrent::Mutex& getMutex() {
            return mutex;
        }

        virtual std::string getBrokerUrl() const {
            return brokerUrl;
        }

        virtual cms::Connection* getConnection() {
            return connection;
        }

        virtual cms::Session* getSession() {
            return session;
        }

        virtual unsigned int getNumReceived() const {
            return numReceived;
        }

        virtual void setNumReceived( unsigned int numReceived ) {
            this->numReceived = numReceived;
        }

        virtual void doSleep();

        virtual unsigned int produceTextMessages(
            cms::MessageProducer& producer,
            unsigned int count,
            cms::Destination* replyTo = NULL );
        virtual unsigned int produceBytesMessages(
            cms::MessageProducer& producer,
            unsigned int count,
            cms::Destination* replyTo = NULL );

        virtual void waitForMessages( unsigned int count );

        virtual void onException( const cms::CMSException& error );
        virtual void onMessage( const cms::Message* message );

        virtual cms::Connection* createDetachedConnection(
            const std::string& username,
            const std::string& password,
            const std::string& clientId );

        virtual void reconnect();
        virtual void reconnectSession();

    public:

        std::string brokerUrl;
        cms::Connection* connection;
        cms::Session* session;

        unsigned int numReceived;
        decaf::util::concurrent::Mutex mutex;
        cms::Session::AcknowledgeMode ackMode;

    };

}

#endif /*_INTEGRATION_TESTSUPPORT_H_*/
