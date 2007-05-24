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

#include "ExpirationTest.h"
#include <integration/IntegrationCommon.h>

#include <sstream>

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/util/Properties.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/util/Guid.h>
#include <activemq/util/Properties.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <activemq/network/SocketFactory.h>
#include <activemq/transport/TransportFactory.h>
#include <activemq/network/Socket.h>
#include <activemq/exceptions/NullPointerException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/util/StringTokenizer.h>
#include <activemq/util/Boolean.h>

#include <cms/Connection.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>
#include <cms/Session.h>

using namespace activemq::connector::stomp;
using namespace activemq::transport;
using namespace activemq::util;
using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::network;
using namespace activemq::transport;
using namespace activemq::concurrent;

using namespace std;
using namespace integration;
using namespace integration::connector::stomp;

ExpirationTest::Producer::Producer( string topic, int numMessages, long long timeToLive ){
    connection = NULL;
    session = NULL;
    destination = NULL;
    producer = NULL;
    this->numMessages = numMessages;
    this->timeToLive = timeToLive;
    this->disableTimeStamps = false;
    this->topic = topic;
}

ExpirationTest::Producer::~Producer(){
    cleanup();
}

bool ExpirationTest::Producer::getDisableTimeStamps() const {
    return disableTimeStamps;
}

void ExpirationTest::Producer::setDisableTimeStamps( bool value ) {
    this->disableTimeStamps = value;
}

void ExpirationTest::Producer::run() {
    try {
        // Create a ConnectionFactory
        ActiveMQConnectionFactory* connectionFactory =
            new ActiveMQConnectionFactory( IntegrationCommon::getInstance().getStompURL() );

        // Create a Connection
        connection = connectionFactory->createConnection();
        delete connectionFactory;
        connection->start();

        string sss=connection->getClientID();
        cout << sss << endl;

        session = connection->createSession( Session::AUTO_ACKNOWLEDGE);
        destination = session->createTopic( topic );

        producer = session->createProducer( destination );
        producer->setDeliveryMode( DeliveryMode::PERSISTENT );
        producer->setDisableMessageTimeStamp( disableTimeStamps );

        //unsigned long ttt=getcurt();
        producer->setTimeToLive( 1);

        // Create the Thread Id String
        string threadIdStr = Integer::toString( Thread::getId() );

        // Create a messages
        string text = (string)"Hello world! from thread " + threadIdStr;

            for( int ix=0; ix<numMessages; ++ix ){
                TextMessage* message = session->createTextMessage( text );
                producer->send( message );
                delete message;
           }

       }catch ( CMSException& e ) {
           e.printStackTrace();
       }
   }

void ExpirationTest::Producer::cleanup(){

        // Destroy resources.
    try{
        if( destination != NULL ) delete destination;
    }catch ( CMSException& e ) {}
    destination = NULL;

    try{
        if( producer != NULL ) delete producer;
    }catch ( CMSException& e ) {}
    producer = NULL;

    // Close open resources.
    try{
        if( session != NULL ) session->close();
        if( connection != NULL ) connection->close();
    }catch ( CMSException& e ) {}

    try{
        if( session != NULL ) delete session;
    }catch ( CMSException& e ) {}
    session = NULL;

    try{
        if( connection != NULL ) delete connection;
    }catch ( CMSException& e ) {}
    connection = NULL;
}

ExpirationTest::Consumer::Consumer( string topic, long waitMillis ){
    connection = NULL;
    session = NULL;
    destination = NULL;
    consumer = NULL;
    this->waitMillis = waitMillis;
    numReceived = 0;
    this->topic = topic;
}

ExpirationTest::Consumer::~Consumer(){
    cleanup();
}

int ExpirationTest::Consumer::getNumReceived() const{
    return numReceived;
}

void ExpirationTest::Consumer::run() {

    try {

        string user,passwd,sID;
        user="default";
        passwd="";
        sID="lsgID";

        // Create a Connection
        connection = ActiveMQConnectionFactory::createConnection(
            IntegrationCommon::getInstance().getStompURL(), user, passwd, sID );

        connection->start();

        // Create a Session
        session = connection->createSession( Session::AUTO_ACKNOWLEDGE);

        // Create the destination (Topic or Queue)
        string t = topic + "?consumer.retroactive=true";

        destination = session->createTopic( t );

        consumer = session->createConsumer( destination );

        consumer->setMessageListener( this );

        // Sleep while asynchronous messages come in.
        Thread::sleep( waitMillis );

    } catch (CMSException& e) {
        e.printStackTrace();
    }
}

void ExpirationTest::Consumer::onMessage( const Message* message ){

    try
    {
        const TextMessage* textMessage =
            dynamic_cast< const TextMessage* >( message );
        string text = textMessage->getText();
        numReceived++;
    } catch (CMSException& e) {
        e.printStackTrace();
    }
}

void ExpirationTest::Consumer::cleanup(){

    // Destroy resources.
    try{
        if( destination != NULL ) delete destination;
    }catch (CMSException& e) {}
    destination = NULL;

    try{
        if( consumer != NULL ) delete consumer;
    }catch (CMSException& e) {}
    consumer = NULL;

    // Close open resources.
    try{
        if( session != NULL ) session->close();
        if( connection != NULL ) connection->close();
    }catch (CMSException& e) {}

    try{
        if( session != NULL ) delete session;
    }catch (CMSException& e) {}
    session = NULL;

    try{
        if( connection != NULL ) delete connection;
    }catch (CMSException& e) {}
    connection = NULL;
}

void ExpirationTest::testExpired()
{
    string topic = Guid().createGUID();
    Producer producer( topic, 1, 1 );
    Thread producerThread( &producer );
    producerThread.start();
    producerThread.join();

    Thread::sleep( 100 );

    Consumer consumer( topic, 2000 );
    Thread consumerThread( &consumer );
    consumerThread.start();
    consumerThread.join();

    Thread::sleep( 100 );

    CPPUNIT_ASSERT_EQUAL( 0, consumer.getNumReceived() );
}

void ExpirationTest::testNotExpired()
{
    string topic = Guid().createGUID();
    Producer producer( topic, 2, 2000 );
    producer.setDisableTimeStamps( true );
    Thread producerThread( &producer );
    producerThread.start();
    producerThread.join();

    Consumer consumer( topic, 3000 );
    Thread consumerThread( &consumer );
    consumerThread.start();
    consumerThread.join();

    Thread::sleep( 50 );

    CPPUNIT_ASSERT_EQUAL( 2, consumer.getNumReceived() );
}

