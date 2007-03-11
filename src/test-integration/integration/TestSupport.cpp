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

#include "TestSupport.h"

#include <cppunit/extensions/HelperMacros.h>

#include <integration/IntegrationCommon.h>

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/util/Guid.h>

#include <cms/Connection.h>
#include <cms/Session.h>

#include <sstream>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::exceptions;
using namespace activemq::concurrent;

using namespace integration;

TestSupport::TestSupport( const string& brokerUrl, cms::Session::AcknowledgeMode ackMode  )
: 
    connectionFactory( NULL ),
    connection( NULL ),
    session( NULL )
{
    this->ackMode = ackMode;
    this->brokerUrl = brokerUrl;
}

TestSupport::~TestSupport()
{
    close();
}

void TestSupport::close() {
    try
    {
        if( session != NULL ) {
            session->close();
            delete session;
        }
        
        if( connection != NULL ) {
            connection->close();
            delete connection;
        }

        if( connectionFactory != NULL ) {
            delete connectionFactory;
        }
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
    
    session = NULL;
    connection = NULL;
    connectionFactory = NULL;
}

void TestSupport::initialize(){
    try
    {
        numReceived = 0;
    
        // Now create the connection
        connection = createDetachedConnection( 
            "", "", Guid().createGUIDString() );
    
        // Set ourself as a recipient of Exceptions        
        connection->setExceptionListener( this );
        connection->start();
        
        // Create a Session
        session = connection->createSession( ackMode );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

cms::Connection* TestSupport::createDetachedConnection(
    const std::string& username,
    const std::string& password,
    const std::string& clientId ) {

    try
    {
    
        if( connectionFactory == NULL ) {
            // Create a Factory
            connectionFactory = new ActiveMQConnectionFactory( brokerUrl );
        }

        // Now create the connection
        cms::Connection* connection = connectionFactory->createConnection(
            username, password, clientId );

        return connection;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

void TestSupport::doSleep(void) 
{
    Thread::sleep( IntegrationCommon::defaultDelay );
}

unsigned int TestSupport::produceTextMessages( 
    cms::MessageProducer& producer,
    unsigned int count )
{
    try
    {
        // Send some text messages.
        ostringstream stream;
        string text = "this is a test text message: id = ";
        
        cms::TextMessage* textMsg = 
            session->createTextMessage();

        unsigned int realCount = 0;
                     
        for( unsigned int ix=0; ix<count; ++ix ){
            stream << text << ix << ends;
            textMsg->setText( stream.str().c_str() );        
            stream.str("");  
            producer.send( textMsg );
            doSleep();
            ++realCount;
        }

        delete textMsg;

        return realCount;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )    
}

unsigned int TestSupport::produceBytesMessages( 
    cms::MessageProducer& producer,
    unsigned int count )
{
    try
    {
        unsigned char buf[10];
        memset( buf, 0, 10 );
        buf[0] = 0;
        buf[1] = 1;
        buf[2] = 2;
        buf[3] = 3;
        buf[4] = 0;
        buf[5] = 4;
        buf[6] = 5;
        buf[7] = 6;

        cms::BytesMessage* bytesMsg = 
            session->createBytesMessage();
        bytesMsg->setBodyBytes( buf, 10 );

        unsigned int realCount = 0;
        for( unsigned int ix=0; ix<count; ++ix ){                
            producer.send( bytesMsg ); 
            doSleep();
            ++realCount;
        }

        delete bytesMsg;

        return realCount;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )    
}

void TestSupport::waitForMessages( unsigned int count )
{
    try
    {
        synchronized( &mutex )
        {
            int stopAtZero = count + 5;
            
            while( numReceived < count )
            {
                mutex.wait( 500 );
                
                if( --stopAtZero == 0 )
                {
                    break;
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )    
}

void TestSupport::onException( const cms::CMSException& error )
{
    CPPUNIT_ASSERT_MESSAGE( error.getStackTraceString(), false );
}

void TestSupport::onMessage( const cms::Message* message )
{
    if( session->getAcknowledgeMode() == cms::Session::CLIENT_ACKNOWLEDGE ) {
        try {
            message->acknowledge();
        } catch( CMSException& ex ) {
            CPPUNIT_ASSERT_MESSAGE(ex.getStackTraceString(), false );
        }
    }
    
    // Got a text message.
    const cms::TextMessage* txtMsg = 
        dynamic_cast<const cms::TextMessage*>(message);
        
    if( txtMsg != NULL )
    {
        std::string text = txtMsg->getText();

        if( IntegrationCommon::debug ) {
            printf("received text msg: %s\n", txtMsg->getText().c_str() );
        }

        numReceived++;

        // Signal that we got one
        synchronized( &mutex )
        {
            mutex.notifyAll();
        }
        
        return;
    }
    
    // Got a bytes msg.
    const cms::BytesMessage* bytesMsg = 
        dynamic_cast<const cms::BytesMessage*>(message);

    if( bytesMsg != NULL )
    {
        const unsigned char* bytes = bytesMsg->getBodyBytes();
        
        string transcode( (const char*)bytes, bytesMsg->getBodyLength() );

        if( IntegrationCommon::debug ) {
            printf("received bytes msg: %s\n", transcode.c_str() );
        }

        numReceived++;
        
        // Signal that we got one
        synchronized( &mutex )
        {
            mutex.notifyAll();
        }
        
        return;
    }
    
}
