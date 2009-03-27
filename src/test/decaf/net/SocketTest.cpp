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

#include "SocketTest.h"

#include <decaf/net/TcpSocket.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testConnect() {

    try{

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 40 );

        TcpSocket client;

        client.connect("127.0.0.1", port);
        client.setSoLinger( false );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        client.close();

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 0)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 0 );

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testTx() {

    try{

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 10 );

        TcpSocket client;

        client.connect("127.0.0.1", port);
        client.setSoLinger( false );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client.getOutputStream();

        std::string msg = "don't reply";
        stream->write( (unsigned char*)msg.c_str(), 0, msg.length() );

        Thread::sleep( 10 );

        CPPUNIT_ASSERT( serverThread.getLastMessage() == msg );

        client.close();

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 0)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 0 );

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testTrx() {

    try{

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 10 );

        TcpSocket client;

        client.connect("127.0.0.1", port);
        client.setSoLinger(false);

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client.getOutputStream();

        std::string msg = "reply";
        stream->write( (unsigned char*)msg.c_str(), 0, msg.length() );

        synchronized(&serverThread.mutex)
        {
           serverThread.mutex.wait(300);
        }

        unsigned char buf[500];
        memset( buf, 0, 500 );
        io::InputStream* istream = client.getInputStream();
        CPPUNIT_ASSERT( istream->available() != 0 );
        std::size_t numRead = istream->read( buf, 0, 500 );
        CPPUNIT_ASSERT( numRead == 5 );
        CPPUNIT_ASSERT( strcmp( (char*)buf, "hello" ) == 0 );

        client.close();

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testRxFail() {

    try{

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 10 );

        TcpSocket client;

        client.connect("127.0.0.1", port);
        client.setSoLinger( false );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        // Give it a chance to get to its read call
        Thread::sleep( 100 );

        client.close();

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 0)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 0 );

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testTrxNoDelay() {

    try{

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 10 );

        TcpSocket client;

        client.connect("127.0.0.1", port);
        client.setSoLinger(false);
        client.setTcpNoDelay(true);

        CPPUNIT_ASSERT( client.getTcpNoDelay() == true );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client.getOutputStream();

        std::string msg = "reply";
        stream->write( (unsigned char*)msg.c_str(), 0, msg.length() );

        synchronized(&serverThread.mutex)
        {
           serverThread.mutex.wait(300);
        }

        unsigned char buf[500];
        memset( buf, 0, 500 );
        io::InputStream* istream = client.getInputStream();
        std::size_t numRead = istream->read( buf, 0, 500 );
        CPPUNIT_ASSERT( numRead == 5 );
        CPPUNIT_ASSERT( strcmp( (char*)buf, "hello" ) == 0 );

        client.close();

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}
