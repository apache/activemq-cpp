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

#include "SimpleRollbackTest.h"

#include <activemq/util/CMSListener.h>
#include <activemq/util/IntegrationCommon.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Thread.h>
#include <decaf/util/UUID.h>

#include <sstream>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
SimpleRollbackTest::SimpleRollbackTest() {
}

////////////////////////////////////////////////////////////////////////////////
SimpleRollbackTest::~SimpleRollbackTest() {
}

////////////////////////////////////////////////////////////////////////////////
void SimpleRollbackTest::testRollbacks() {

    try {

        // Create CMS Object for Comms
        cms::Session* session( cmsProvider->getSession() );

        CMSListener listener( session );

        cms::MessageConsumer* consumer = cmsProvider->getConsumer();
        consumer->setMessageListener( &listener );
        cms::MessageProducer* producer = cmsProvider->getProducer();
        producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );

        auto_ptr<cms::TextMessage> txtMessage( session->createTextMessage() );

        for( unsigned int i = 0; i < IntegrationCommon::defaultMsgCount; ++i ) {
            ostringstream lcStream;
            lcStream << "SimpleTest - Message #" << i << ends;
            txtMessage->setText( lcStream.str() );
            producer->send( txtMessage.get() );
        }

        session->commit();
        Thread::sleep( 50 );

        // Wait for the messages to get here
        listener.asyncWaitForMessages( IntegrationCommon::defaultMsgCount );
        unsigned int numReceived = listener.getNumReceived();
        CPPUNIT_ASSERT( numReceived == IntegrationCommon::defaultMsgCount );

        for( unsigned int i = 0; i < 5; ++i ) {
            ostringstream lcStream;
            lcStream << "SimpleTest - Message #" << i << ends;
            txtMessage->setText( lcStream.str() );
            producer->send( txtMessage.get() );
        }

        listener.reset();
        session->rollback();
        Thread::sleep( 50 );

        // TODO - Should we get anything here?
        // Wait for the messages to get here
        listener.asyncWaitForMessages( 5 );
        //CPPUNIT_ASSERT( listener.getNumReceived() == 0 );

        listener.reset();
        txtMessage->setText( "SimpleTest - Message after Rollback" );
        producer->send( txtMessage.get() );
        session->commit();

        // Wait for the messages to get here
        listener.asyncWaitForMessages( 1 );
        CPPUNIT_ASSERT( listener.getNumReceived() == 1 );

        listener.reset();
        txtMessage->setText( "SimpleTest - Message after Rollback" );
        producer->send( txtMessage.get() );
        session->commit();

        // Wait for the messages to get here
        listener.asyncWaitForMessages( 1 );
        CPPUNIT_ASSERT( listener.getNumReceived() == 1 );

    } catch( ... ) {
        CPPUNIT_ASSERT( false );
    }
}
