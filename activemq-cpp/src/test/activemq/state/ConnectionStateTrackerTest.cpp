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

#include "ConnectionStateTrackerTest.h"

#include <activemq/transport/Transport.h>
#include <activemq/wireformat/WireFormat.h>
#include <activemq/state/ConnectionStateTracker.h>
#include <activemq/state/ConsumerState.h>
#include <activemq/state/SessionState.h>
#include <activemq/commands/ActiveMQTopic.h>
#include <activemq/commands/Message.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/SessionInfo.h>
#include <activemq/commands/Message.h>
#include <decaf/lang/Pointer.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/util/LinkedList.h>

using namespace std;
using namespace activemq;
using namespace activemq::state;
using namespace activemq::commands;
using namespace activemq::transport;
using namespace activemq::wireformat;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class TrackingTransport : public activemq::transport::Transport {
    public:

        LinkedList< Pointer<Command> > connections;
        LinkedList< Pointer<Command> > sessions;
        LinkedList< Pointer<Command> > producers;
        LinkedList< Pointer<Command> > consumers;
        LinkedList< Pointer<Command> > messages;
        LinkedList< Pointer<Command> > messagePulls;

    public:

        virtual ~TrackingTransport() {}

        virtual void start() {}

        virtual void stop() {}

        virtual void close() {}

        virtual void oneway(const Pointer<Command> command) {
            if (command->isConnectionInfo()) {
                connections.add(command);
            } else if (command->isSessionInfo()) {
                sessions.add(command);
            } else if (command->isProducerInfo()) {
                producers.add(command);
            } else if (command->isConsumerInfo()) {
                consumers.add(command);
            } else if (command->isMessage()) {
                messages.add(command);
            } else if (command->isMessagePull()) {
                messagePulls.add(command);
            }
        }

        virtual Pointer<FutureResponse> asyncRequest(const Pointer<Command> command,
                                                     const Pointer<ResponseCallback> responseCallback) {
            throw UnsupportedOperationException();
        }

        virtual Pointer<Response> request(const Pointer<Command> command) {
            throw UnsupportedOperationException();
        }

        virtual Pointer<Response> request(const Pointer<Command> command, unsigned int timeout) {
            throw UnsupportedOperationException();
        }

        virtual Pointer<wireformat::WireFormat> getWireFormat() const {
            return Pointer<wireformat::WireFormat>();
        }

        virtual void setWireFormat(const Pointer<wireformat::WireFormat> wireFormat) {
        }

        virtual void setTransportListener(TransportListener* listener) {
        }

        virtual TransportListener* getTransportListener() const {
            return NULL;
        }

        virtual Transport* narrow(const std::type_info& typeId) {
            return NULL;
        }

        virtual bool isFaultTolerant() const {
            return false;
        }

        virtual bool isConnected() const {
            return true;
        }

        virtual bool isClosed() const {
            return false;
        }

        virtual bool isReconnectSupported() const {
            return false;
        }

        virtual bool isUpdateURIsSupported() const {
            return false;
        }

        virtual std::string getRemoteAddress() const {
            return "";
        }

        virtual void reconnect(const decaf::net::URI& uri)  {
        }

        virtual void updateURIs(bool rebalance, const decaf::util::List<decaf::net::URI>& uris) {
        }

    };

    class ConnectionData {
    public:

        Pointer<ConnectionInfo> connection;
        Pointer<SessionInfo> session;
        Pointer<ConsumerInfo> consumer;
        Pointer<ProducerInfo> producer;

    };

    ConnectionData createConnectionState(ConnectionStateTracker& tracker) {

        ConnectionData conn;

        Pointer<ConnectionId> connectionId(new ConnectionId);
        connectionId->setValue("CONNECTION");
        conn.connection.reset(new ConnectionInfo);
        conn.connection->setConnectionId(connectionId);

        Pointer<SessionId> session_id(new SessionId);
        session_id->setConnectionId("CONNECTION");
        session_id->setValue(12345);
        conn.session.reset(new SessionInfo);
        conn.session->setSessionId(session_id);

        Pointer<ConsumerId> consumer_id(new ConsumerId);
        consumer_id->setConnectionId("CONNECTION");
        consumer_id->setSessionId(12345);
        consumer_id->setValue(42);
        conn.consumer.reset(new ConsumerInfo);
        conn.consumer->setConsumerId(consumer_id);

        Pointer<ProducerId> producer_id(new ProducerId);
        producer_id->setConnectionId("CONNECTION");
        producer_id->setSessionId(12345);
        producer_id->setValue(42);
        conn.producer.reset(new ProducerInfo);
        conn.producer->setProducerId(producer_id);

        tracker.processConnectionInfo(conn.connection.get());
        tracker.processSessionInfo(conn.session.get());
        tracker.processConsumerInfo(conn.consumer.get());
        tracker.processProducerInfo(conn.producer.get());

        return conn;
    }

    void clearConnectionState(ConnectionStateTracker& tracker, ConnectionData& conn) {
        tracker.processRemoveProducer(conn.producer->getProducerId().get());
        tracker.processRemoveConsumer(conn.consumer->getConsumerId().get());
        tracker.processRemoveSession(conn.session->getSessionId().get());
        tracker.processRemoveConnection(conn.connection->getConnectionId().get());
    }

}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTrackerTest::test() {

    ConnectionStateTracker tracker;
    ConnectionData conn = createConnectionState(tracker);
    clearConnectionState(tracker, conn);
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTrackerTest::testMessageCache() {

    Pointer<TrackingTransport> transport(new TrackingTransport);
    ConnectionStateTracker tracker;
    tracker.setTrackMessages(true);

    ConnectionData conn = createConnectionState(tracker);

    int messageSize;
    {
        decaf::lang::Pointer<commands::MessageId> id(new commands::MessageId());
        id->setProducerId(conn.producer->getProducerId());
        Pointer<Message> message(new Message);
        messageSize = message->getSize();
    }

    tracker.setMaxMessageCacheSize(messageSize * 3);

    int sequenceId = 1;

    for (int i = 0; i < 100; ++i) {
        decaf::lang::Pointer<commands::MessageId> id(new commands::MessageId());
        id->setProducerId(conn.producer->getProducerId());
        id->setProducerSequenceId(sequenceId++);
        Pointer<Message> message(new Message);
        message->setMessageId(id);

        tracker.processMessage(message.get());
        tracker.trackBack(message);
    }

    tracker.restore(transport);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should only be three messages", 4, transport->messages.size());
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionStateTrackerTest::testMessagePullCache() {

    Pointer<TrackingTransport> transport(new TrackingTransport);
    ConnectionStateTracker tracker;
    tracker.setTrackMessages(true);

    ConnectionData conn = createConnectionState(tracker);

    for (int i = 0; i < 100; ++i) {
        Pointer<commands::MessagePull> pull(new commands::MessagePull());
        Pointer<ActiveMQDestination> destination(new ActiveMQTopic("TEST" + Integer::toString(i)));
        pull->setConsumerId(conn.consumer->getConsumerId());
        pull->setDestination(destination);
        tracker.processMessagePull(pull.get());
        tracker.trackBack(pull);
    }

    tracker.restore(transport);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should only be three message pulls", 10, transport->messagePulls.size());
}
