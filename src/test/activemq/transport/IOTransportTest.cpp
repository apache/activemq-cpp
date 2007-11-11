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

#include "IOTransportTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::transport::IOTransportTest );

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
// This will just test that we can start and stop the
// transport without any exceptions.
void IOTransportTest::testStartClose(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    transport.start();
    decaf::lang::Thread::sleep( 60 );
    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testRead(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    transport.start();

    decaf::lang::Thread::sleep( 10 );

    unsigned char buffer[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
    try{
        synchronized( &is ){
            is.setByteArray( buffer, 10 );
        }
    }catch( decaf::lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
    }

    decaf::lang::Thread::sleep( 100 );

    CPPUNIT_ASSERT( listener.str == "1234567890" );

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testWrite(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    transport.start();

    MyCommand cmd;
    cmd.c = '1';
    transport.oneway( &cmd );
    cmd.c = '2';
    transport.oneway( &cmd );
    cmd.c = '3';
    transport.oneway( &cmd );
    cmd.c = '4';
    transport.oneway( &cmd );
    cmd.c = '5';
    transport.oneway( &cmd );

    const unsigned char* bytes = os.toByteArray();
    std::size_t size = os.size();
    CPPUNIT_ASSERT( size >= 5 );
    CPPUNIT_ASSERT( bytes[0] == '1' );
    CPPUNIT_ASSERT( bytes[1] == '2' );
    CPPUNIT_ASSERT( bytes[2] == '3' );
    CPPUNIT_ASSERT( bytes[3] == '4' );
    CPPUNIT_ASSERT( bytes[4] == '5' );

    transport.close();
}

////////////////////////////////////////////////////////////////////////////////
void IOTransportTest::testException(){

    decaf::io::BlockingByteArrayInputStream is;
    decaf::io::ByteArrayOutputStream os;
    MyCommandListener listener;
    MyCommandReader reader;
    MyCommandWriter writer;
    MyExceptionListener exListener;
    IOTransport transport;
    transport.setCommandListener( &listener );
    transport.setCommandReader( &reader );
    reader.throwException = true;
    transport.setCommandWriter( &writer );
    transport.setInputStream( &is );
    transport.setOutputStream( &os );
    transport.setTransportExceptionListener( &exListener );

    unsigned char buffer[1] = { '1' };
    try{
        synchronized( &is ){
            is.setByteArray( buffer, 1);
        }
    }catch( decaf::lang::Exception& ex ){
        ex.setMark(__FILE__, __LINE__ );
    }

    transport.start();

    synchronized(&exListener.mutex)
    {
       if(exListener.transport != &transport)
       {
          exListener.mutex.wait(1000);
       }
    }

    CPPUNIT_ASSERT( exListener.transport == &transport );

    transport.close();
}
