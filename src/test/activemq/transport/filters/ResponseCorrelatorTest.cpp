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

#include "ResponseCorrelatorTest.h"

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::transport::filters;

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testBasics(){

    try{

        MyListener listener;
        MyTransport transport;
        ResponseCorrelator correlator( &transport, false );
        correlator.setCommandListener( &listener );
        correlator.setTransportExceptionListener( &listener );
        CPPUNIT_ASSERT( transport.listener == &correlator );
        CPPUNIT_ASSERT( transport.exListener == &correlator );

        // Give the thread a little time to get up and running.
        synchronized(&transport.startedMutex)
        {
            // Start the transport.
            correlator.start();
            transport.startedMutex.wait();
        }

        // Send one request.
        MyCommand cmd;
        Response* resp = correlator.request( &cmd );
        CPPUNIT_ASSERT( resp != NULL );
        CPPUNIT_ASSERT( resp->getCorrelationId() == cmd.getCommandId() );

        // Wait to get the message back asynchronously.
        decaf::lang::Thread::sleep( 100 );

        // Since our transport relays our original command back at us as a
        // non-response message, check to make sure we received it and that
        // it is the original command.
        CPPUNIT_ASSERT( listener.commands.size() == 1 );
        CPPUNIT_ASSERT( listener.exCount == 0 );

        correlator.close();

        // Destroy the response.
        delete resp;
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testOneway(){

    try{

        MyListener listener;
        MyTransport transport;
        ResponseCorrelator correlator( &transport, false );
        correlator.setCommandListener( &listener );
        correlator.setTransportExceptionListener( &listener );
        CPPUNIT_ASSERT( transport.listener == &correlator );
        CPPUNIT_ASSERT( transport.exListener == &correlator );

        // Give the thread a little time to get up and running.
        synchronized(&transport.startedMutex)
        {
            // Start the transport.
            correlator.start();

            transport.startedMutex.wait();
        }

        // Send many oneway request (we'll get them back asynchronously).
        const unsigned int numCommands = 1000;
        MyCommand commands[numCommands];
        for( unsigned int ix=0; ix<numCommands; ix++ ){
            correlator.oneway( &commands[ix] );
        }

        // Give the thread a little time to get all the messages back.
        decaf::lang::Thread::sleep( 500 );

        // Make sure we got them all back.
        CPPUNIT_ASSERT( listener.commands.size() == numCommands );
        CPPUNIT_ASSERT( listener.exCount == 0 );

        correlator.close();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testTransportException(){

    try{

        MyListener listener;
        MyBrokenTransport transport;
        ResponseCorrelator correlator( &transport, false );
        correlator.setCommandListener( &listener );
        correlator.setTransportExceptionListener( &listener );
        CPPUNIT_ASSERT( transport.listener == &correlator );
        CPPUNIT_ASSERT( transport.exListener == &correlator );

        // Give the thread a little time to get up and running.
        synchronized(&transport.startedMutex)
        {
            // Start the transport.
            correlator.start();

            transport.startedMutex.wait();
        }

        // Send one request.
        MyCommand cmd;
        try{
            correlator.request( &cmd, 500 );
            CPPUNIT_ASSERT(false);
        }catch( CommandIOException& ex ){
            // Expected.
        }

        // Wait to make sure we get the asynchronous message back.
        decaf::lang::Thread::sleep( 200 );

        // Since our transport relays our original command back at us as a
        // non-response message, check to make sure we received it and that
        // it is the original command.
        CPPUNIT_ASSERT( listener.commands.size() == 0 );
        CPPUNIT_ASSERT( listener.exCount == 1 );

        correlator.close();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testMultiRequests(){

    try{

        MyListener listener;
        MyTransport transport;
        ResponseCorrelator correlator( &transport, false );
        correlator.setCommandListener( &listener );
        correlator.setTransportExceptionListener( &listener );
        CPPUNIT_ASSERT( transport.listener == &correlator );
        CPPUNIT_ASSERT( transport.exListener == &correlator );

        // Start the transport.
        correlator.start();

        // Make sure the start command got down to the thread.
        CPPUNIT_ASSERT( transport.thread != NULL );

        // Give the thread a little time to get up and running.
        synchronized(&transport.startedMutex)
        {
            transport.startedMutex.wait(500);
        }

        // Start all the requester threads.
        const unsigned int numRequests = 100;
        RequestThread requesters[numRequests];
        for( unsigned int ix=0; ix<numRequests; ++ix ){
            requesters[ix].setTransport( &correlator );
            requesters[ix].start();
        }

        // Make sure we got all the responses and that they were all
        // what we expected.
        for( unsigned int ix=0; ix<numRequests; ++ix ){
            requesters[ix].join();
            CPPUNIT_ASSERT( requesters[ix].resp != NULL );
            CPPUNIT_ASSERT( requesters[ix].cmd.getCommandId() == requesters[ix].resp->getCorrelationId() );
        }

        decaf::lang::Thread::sleep( 60 );
        synchronized( &listener.mutex )
        {
            unsigned int count = 0;

            while( listener.commands.size() != numRequests )
            {
                listener.mutex.wait( 75 );

                ++count;

                if( count == numRequests ) {
                    break;
                }
            }
        }

        // Since our transport relays our original command back at us as a
        // non-response message, check to make sure we received it and that
        // it is the original command.
        CPPUNIT_ASSERT( listener.commands.size() == numRequests );
        CPPUNIT_ASSERT( listener.exCount == 0 );

        correlator.close();
    }
    AMQ_CATCH_RETHROW( exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ResponseCorrelatorTest::testNarrow(){

    MyTransport transport;
    ResponseCorrelator correlator( &transport, false );

    Transport* narrowed = correlator.narrow( typeid( transport ) );
    CPPUNIT_ASSERT( narrowed == &transport );

    narrowed = correlator.narrow( typeid( std::string() ) );
    CPPUNIT_ASSERT( narrowed == NULL );

    narrowed = correlator.narrow( typeid( MyTransport ) );
    CPPUNIT_ASSERT( narrowed == &transport );

    narrowed = correlator.narrow( typeid( transport::filters::ResponseCorrelator ) );
    CPPUNIT_ASSERT( narrowed == &correlator );

    narrowed = correlator.narrow( typeid( correlator ) );
    CPPUNIT_ASSERT( narrowed == &correlator );

}
