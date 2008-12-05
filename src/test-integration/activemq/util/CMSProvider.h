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

#ifndef _ACTIVEMQ_UTIL_CMSPROVIDER_H_
#define _ACTIVEMQ_UTIL_CMSPROVIDER_H_

#include <string>
#include <memory>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/Session.h>
#include <cms/Destination.h>

#include <decaf/io/Closeable.h>

namespace activemq {
namespace util {

    class CMSProvider : decaf::io::Closeable {
    private:

        std::string brokerURL;
        cms::Session::AcknowledgeMode ackMode;
        std::string username;
        std::string password;
        std::string clientId;

        std::string destinationName;
        bool topic;
        bool durable;
        std::string subscription;

        std::auto_ptr<cms::ConnectionFactory> connectionFactory;
        std::auto_ptr<cms::Connection> connection;
        std::auto_ptr<cms::Session> session;
        std::auto_ptr<cms::MessageConsumer> consumer;
        std::auto_ptr<cms::MessageProducer> producer;
        std::auto_ptr<cms::MessageProducer> noDestProducer;
        std::auto_ptr<cms::Destination> destination;
        std::auto_ptr<cms::Destination> tempDestination;

    public:

        CMSProvider( const std::string& brokerURL,
                     cms::Session::AcknowledgeMode ackMode = cms::Session::AUTO_ACKNOWLEDGE );

        virtual ~CMSProvider();

        virtual void close() throw( decaf::lang::Exception );

        std::string getBrokerURL() const {
            return this->brokerURL;
        }

        void setBrokerURL( const std::string& brokerURL ) {
            this->brokerURL = brokerURL;
        }

        void setDestinationName( const std::string name ) {
            this->destinationName = name;
        }

        std::string getDestinationName() const {
            return this->destinationName;
        }

        void setSubscription( const std::string name ) {
            this->subscription = name;
        }

        std::string getSubscription() const {
            return this->subscription;
        }

        void setTopic( bool value ) {
            this->topic = value;
        }

        bool isTopic() const {
            return this->topic;
        }

        void setDurable( bool value ) {
            this->durable = value;
        }

        bool isDurable() const {
            return this->durable;
        }

        void setAckMode( cms::Session::AcknowledgeMode ackMode ) {
            this->ackMode = ackMode;
        }

        cms::Session::AcknowledgeMode getAckMode() const {
            return this->ackMode;
        }

    public:

        /**
         * Initializes a CMSProvider with the Login data for the session that
         * this provider is managing.  Once called a new Connection to the broker
         * is made and will remain open until a reconnect is requested or until
         * the CMSProvider instance is closed.
         */
        virtual void initialize( const std::string& username = "",
                                 const std::string& password = "",
                                 const std::string& clientId = "" );

        /**
         * Forces a reconnect of the Connection and then of the Session and its
         * associated resources.
         */
        virtual void reconnect();

        /**
         * Forces a Recreation of a Session and any of its Resources.
         */
        virtual void reconnectSession();

        /**
         * Unsubscribes a durable consumer if one has been created and the chosen
         * wireformat supports it.  The consumer is closed as a result any calls to
         * it after calling this method will result in an error.
         */
        virtual void unsubscribe();

        /**
         * Returns the ConnectionFactory object that this Provider has allocated.
         */
        virtual cms::ConnectionFactory* getConnectionFactory();

        /**
         * Returns the Connection object that this Provider has allocated.
         */
        virtual cms::Connection* getConnection();

        /**
         * Returns the Session object that this Provider has allocated.
         */
        virtual cms::Session* getSession();

        /**
         * Returns the MessageConsumer object that this Provider has allocated.
         */
        virtual cms::MessageConsumer* getConsumer();

        /**
         * Returns the MessageProducer object that this Provider has allocated.
         */
        virtual cms::MessageProducer* getProducer();

        /**
         * Returns the MessageProducer object that this Provider has allocated that has
         * no assigned Destination, message sent must be assigned one at send time.
         */
        virtual cms::MessageProducer* getNoDestProducer();

        /**
         * Returns the Destination object that this Provider has allocated.
         */
        virtual cms::Destination* getDestination();

        /**
         * Returns the Temporary Destination object that this Provider has allocated.
         */
        virtual cms::Destination* getTempDestination();

        /**
         * Destroys a Destination at the Broker side, freeing the resources associated with it.
         */
        virtual void destroyDestination( const cms::Destination* destination );

    };

}}

#endif /*_ACTIVEMQ_UTIL_CMSPROVIDER_H_*/
