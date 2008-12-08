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

#ifndef _EXAMPLE_TOPICS_CHAT_CHAT_H_
#define _EXAMPLE_TOPICS_CHAT_CHAT_H_

#include <memory>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageListener.h>
#include <cms/Destination.h>
#include <cms/ExceptionListener.h>
#include <cms/CMSException.h>

namespace example{
namespace topics{
namespace chat{

    /**
     * Simple Chat Client that uses a Topic
     */
    class Chat : public cms::MessageListener,
                 public cms::ExceptionListener {
    private:

        std::auto_ptr<cms::Connection> connection;
        std::auto_ptr<cms::Session> session;
        std::auto_ptr<cms::MessageConsumer> consumer;
        std::auto_ptr<cms::MessageProducer> producer;
        std::auto_ptr<cms::Destination> topic;

        std::string brokerURI;
        std::string username;
        std::string password;
        std::string topicName;

    public:

        static const char* DEFAULT_BROKER_URI;
        static const char* DEFAULT_TOPIC_NAME;

    public:

        Chat();
        virtual ~Chat();

        virtual void onMessage( const cms::Message* message );
        virtual void onException( const cms::CMSException& ex );

        /**
         * Sets the BrokerURI
         */
        void setBrokerURI( const std::string& uri ) {
            this->brokerURI = uri;
        }

        /**
         * Gets the BrokerURI
         */
        std::string getBrokerURI() const {
            return this->brokerURI;
        }

        /**
         * Sets the Topic Name
         */
        void setTopicName( const std::string& topicName ) {
            this->topicName = topicName;
        }

        /**
         * Gets the Topic Name
         */
        std::string getTopicName() const {
            return this->topicName;
        }

        /**
         * Sets the User Name
         */
        void setUsername( const std::string& username ) {
            this->username = username;
        }

        /**
         * Gets the User Name
         */
        std::string getUsername() const {
            return this->username;
        }

        /**
         * Sets the Password
         */
        void setPassword( const std::string& password ) {
            this->password = password;
        }

        /**
         * Gets the Password
         */
        std::string getPassword() const {
            return this->password;
        }

        /**
         * Run the Chat Application.
         */
        void run();

    };

}}}

#endif // _EXAMPLE_TOPICS_CHAT_CHAT_H_
