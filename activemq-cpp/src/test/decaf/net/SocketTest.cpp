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

#include <decaf/net/Socket.h>
#include <decaf/net/SocketFactory.h>

#include <decaf/net/ServerSocket.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Thread.h>
#include <list>
#include <string.h>

using namespace std;
using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyServerThread : public lang::Thread{
    private:

        bool done;
        int numClients;
        std::string lastMessage;
        int port;

    public:

        util::concurrent::Mutex mutex;

    public:

        MyServerThread( int port ) : Thread(), done( false ), numClients( 0 ), lastMessage(), port( port ) {
        }

        virtual ~MyServerThread(){
            stop();
        }

        std::string getLastMessage(){
            return lastMessage;
        }

        int getNumClients(){
            return numClients;
        }

        virtual void stop(){
            done = true;
        }

        virtual void run(){
            try{
                unsigned char buf[1000];

                ServerSocket server;
                server.bind( "127.0.0.1", port );

                Socket* socket = server.accept();
                server.close();

                //socket->setSoTimeout( 10 );
                socket->setSoLinger( false, 0 );
                numClients++;

                synchronized(&mutex)
                {
                   mutex.notifyAll();
                }

                while( !done && socket != NULL ){

                    io::InputStream* stream = socket->getInputStream();

                    memset( buf, 0, 1000 );
                    try{

                        if( stream->read( buf, 1000, 0, 1000 ) == -1 ) {
                            done = true;
                            continue;
                        }

                        lastMessage = (char*)buf;

                        if( strcmp( (char*)buf, "reply" ) == 0 ){
                            io::OutputStream* output = socket->getOutputStream();
                            output->write( (unsigned char*)"hello", (int)strlen("hello"), 0, (int)strlen("hello") );

                              synchronized(&mutex) {
                                 mutex.notifyAll();
                              }
                        }

                    }catch( io::IOException& ex ){
                        done = true;
                    }
                }

                socket->close();
                delete socket;

                numClients--;

                synchronized(&mutex)
                {
                    mutex.notifyAll();
                }

            }catch( io::IOException& ex ){
                printf("%s\n", ex.getMessage().c_str() );
                CPPUNIT_ASSERT( false );
            }catch( ... ){
                CPPUNIT_ASSERT( false );
            }
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testConnect() {

    try{

        MyServerThread serverThread( port );
        serverThread.start();

        Thread::sleep( 40 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect( "127.0.0.1", port );
        client->setSoLinger( false, 0 );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        client->close();

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

        MyServerThread serverThread( port );
        serverThread.start();

        Thread::sleep( 10 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", port);
        client->setSoLinger( false, 0 );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client->getOutputStream();

        std::string msg = "don't reply";
        stream->write( (unsigned char*)msg.c_str(), (int)msg.length(), 0, (int)msg.length() );

        Thread::sleep( 10 );

        CPPUNIT_ASSERT( serverThread.getLastMessage() == msg );

        client->close();

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

        MyServerThread serverThread( port );
        serverThread.start();

        Thread::sleep( 10 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", port);
        client->setSoLinger( false, 0 );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client->getOutputStream();

        std::string msg = "reply";
        stream->write( (unsigned char*)msg.c_str(), (int)msg.length(), 0, (int)msg.length() );

        synchronized(&serverThread.mutex)
        {
           serverThread.mutex.wait(300);
        }

        unsigned char buf[500];
        memset( buf, 0, 500 );
        io::InputStream* istream = client->getInputStream();
        CPPUNIT_ASSERT( istream->available() != 0 );
        std::size_t numRead = istream->read( buf, 500, 0, 500 );
        CPPUNIT_ASSERT( numRead == 5 );
        CPPUNIT_ASSERT( strcmp( (char*)buf, "hello" ) == 0 );

        client->close();

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testRxFail() {

    try{

        MyServerThread serverThread( port );
        serverThread.start();

        Thread::sleep( 10 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", port);
        client->setSoLinger( false, 0 );

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

        client->close();

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

        MyServerThread serverThread( port );
        serverThread.start();

        Thread::sleep( 10 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", port);
        client->setSoLinger( false, 0 );
        client->setTcpNoDelay(true);

        CPPUNIT_ASSERT( client->getTcpNoDelay() == true );

        synchronized(&serverThread.mutex)
        {
           if(serverThread.getNumClients() != 1)
           {
              serverThread.mutex.wait(1000);
           }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        io::OutputStream* stream = client->getOutputStream();

        std::string msg = "reply";
        stream->write( (unsigned char*)msg.c_str(), (int)msg.length() );

        synchronized(&serverThread.mutex)
        {
           serverThread.mutex.wait(300);
        }

        unsigned char buf[500];
        memset( buf, 0, 500 );
        io::InputStream* istream = client->getInputStream();
        std::size_t numRead = istream->read( buf, 500, 0, 500 );
        CPPUNIT_ASSERT( numRead == 5 );
        CPPUNIT_ASSERT( strcmp( (char*)buf, "hello" ) == 0 );

        client->close();

        serverThread.stop();
        serverThread.join();

    }catch( io::IOException& ex ){
        printf( "%s\n", ex.getMessage().c_str() );
    }
}
