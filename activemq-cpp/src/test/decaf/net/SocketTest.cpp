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

#include <apr_signal.h>

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
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testConnectUnknownHost() {

    // TODO - Should throw an UnknownHostException
    Socket s;
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "IOException should have been thrown",
        s.connect( "unknown.host", 45 ),
        IOException );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testConnectPortOutOfRange() {

    Socket s;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArguementException",
        s.connect( "0.0.0.0", 70000 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should Throw an IllegalArguementException",
        s.connect( "0.0.0.0", 70000, 1000 ),
        IllegalArgumentException );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testConstructor() {

    // create the socket and then validate some basic state
    Socket s;
    CPPUNIT_ASSERT_MESSAGE("new socket should not be connected", !s.isConnected());
    CPPUNIT_ASSERT_MESSAGE("new socket should not be bound", !s.isBound());
    CPPUNIT_ASSERT_MESSAGE("new socket should not be closed", !s.isClosed());
    CPPUNIT_ASSERT_MESSAGE("new socket should not be in InputShutdown", !s.isInputShutdown());
    CPPUNIT_ASSERT_MESSAGE("new socket should not be in OutputShutdown", !s.isOutputShutdown());
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetReuseAddress() {

    Socket s;
    s.setReuseAddress( true );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Socket Reuse Address value not what was expected.", true, s.getReuseAddress() );
    s.setReuseAddress( false );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Socket Reuse Address value not what was expected.", false, s.getReuseAddress() );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testClose() {

    ServerSocket ss(0);
    Socket client( "localhost", ss.getLocalPort() );

    CPPUNIT_ASSERT_NO_THROW_MESSAGE( "Exception on setSoLinger unexpected", client.setSoLinger( false, 100 ) );

    client.close();

    CPPUNIT_ASSERT_THROW_MESSAGE(
         "Should have thrown an IOException",
         client.getOutputStream(),
         IOException );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetPort() {

    ServerSocket server(0);
    int serverPort = server.getLocalPort();
    Socket client( "localhost", serverPort );

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect port", serverPort, client.getPort() );

    client.close();
    server.close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetInputStream() {

    ServerSocket ss(0);
    Socket client( "localhost", ss.getLocalPort() );

    InputStream* is = client.getInputStream();

    CPPUNIT_ASSERT( is != NULL );

    is->close();
    client.close();
    ss.close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetKeepAlive() {

    try {

        ServerSocket ss(0);
        Socket client( "localhost", ss.getLocalPort() );

        client.setKeepAlive( true );

        CPPUNIT_ASSERT_MESSAGE( "getKeepAlive false when it should be true",
                                client.getKeepAlive() );

        client.setKeepAlive( false );

        CPPUNIT_ASSERT_MESSAGE( "getKeepAlive true when it should be False",
                                !client.getKeepAlive() );

    } catch (Exception e) {
        CPPUNIT_FAIL( "Error during test of Get SO_KEEPALIVE" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetLocalPort() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect port", 0 != client.getLocalPort() );

    client.close();
    server.close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetSoLinger() {

    try {

        ServerSocket ss(0);
        Socket client( "localhost", ss.getLocalPort() );

        client.setSoLinger( true, 100 );

        CPPUNIT_ASSERT_MESSAGE( "getSoLinger returned incorrect value",
                                100 == client.getSoLinger() );

        client.setSoLinger( false, 100 );

        CPPUNIT_ASSERT_MESSAGE( "getSoLinger returned incorrect value",
                                -1 == client.getSoLinger() );

    } catch (Exception e) {
        CPPUNIT_FAIL( "Error during test of Get SO_LINGER" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetSoTimeout() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    client.setSoTimeout( 100 );
    CPPUNIT_ASSERT_MESSAGE( "Returned incorrect timeout", 100 == client.getSoTimeout() );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetTcpNoDelay() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    client.setTcpNoDelay( true );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect TCP_NODELAY value, should be true",
                                  true, client.getTcpNoDelay() );

    client.setTcpNoDelay( false );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Returned incorrect TCP_NODELAY value, should be false",
                                  false, client.getTcpNoDelay() );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testIsConnected() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    std::auto_ptr<Socket> worker( server.accept() );

    CPPUNIT_ASSERT_MESSAGE( "Socket indicated  not connected when it should be",
                            client.isConnected() );

    client.close();
    worker->close();
    server.close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testIsClosed() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    std::auto_ptr<Socket> worker( server.accept() );

    // validate isClosed returns expected values
    CPPUNIT_ASSERT_MESSAGE( "Socket should indicate it is not closed(1):",
                            !client.isClosed() );
    client.close();
    CPPUNIT_ASSERT_MESSAGE( "Socket should indicate it is closed(1):",
                            client.isClosed() );

    // validate that isClosed works ok for sockets returned from
    // ServerSocket.accept()
    CPPUNIT_ASSERT_MESSAGE( "Accepted Socket should indicate it is not closed:",
                            !worker->isClosed() );
    worker->close();
    CPPUNIT_ASSERT_MESSAGE( "Accepted Socket should indicate it is closed:",
                            worker->isClosed() );

    // and finally for the server socket
    CPPUNIT_ASSERT_MESSAGE( "Server Socket should indicate it is not closed:",
                            !server.isClosed() );
    server.close();
    CPPUNIT_ASSERT_MESSAGE( "Server Socket should indicate it is closed:",
                            server.isClosed() );
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testIsInputShutdown() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    std::auto_ptr<Socket> worker( server.accept() );

    InputStream* theInput = client.getInputStream();
    OutputStream* theOutput = worker->getOutputStream();

    // make sure we get the right answer with newly connected socket
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated input shutdown when it should not have",
                            !client.isInputShutdown() );

    // shutdown the output
    client.shutdownInput();

    // make sure we get the right answer once it is shut down
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated input was NOT shutdown when it should have been",
                            client.isInputShutdown() );

    client.close();
    worker->close();
    server.close();

    // make sure we get the right answer for closed sockets
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated input was shutdown when socket was closed",
                            !worker->isInputShutdown() );

    theInput->close();
    theOutput->close();
}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testIsOutputShutdown() {

    ServerSocket server(0);
    Socket client( "localhost", server.getLocalPort() );

    std::auto_ptr<Socket> worker( server.accept() );

    InputStream* theInput = client.getInputStream();
    OutputStream* theOutput = worker->getOutputStream();

    // make sure we get the right answer with newly connected socket
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated output shutdown when it should not have",
                            !worker->isOutputShutdown() );

    // shutdown the output
    worker->shutdownOutput();

    // make sure we get the right answer once it is shut down
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated output was NOT shutdown when it should have been",
                            worker->isOutputShutdown() );

    client.close();
    worker->close();
    server.close();

    // make sure we get the right answer for closed sockets
    CPPUNIT_ASSERT_MESSAGE( "Socket indicated output was output shutdown when the socket was closed",
                            !client.isOutputShutdown() );

    theInput->close();
    theOutput->close();
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    class GetOutputStreamRunnable : public Runnable {
    private:

        ServerSocket* server;

    private:

        GetOutputStreamRunnable(const GetOutputStreamRunnable&);
        GetOutputStreamRunnable& operator= (const GetOutputStreamRunnable&);

    public:

        GetOutputStreamRunnable( ServerSocket* server ) : server( server ) {
        }

        virtual void run() {

            try {
                std::auto_ptr<Socket> worker( server->accept() );
                server->close();
                InputStream* in = worker->getInputStream();
                in->read();
                in->close();
                worker->close();
                worker.reset( NULL );
            } catch( IOException& e ) {
                e.printStackTrace();
            }
        }

    };

}

////////////////////////////////////////////////////////////////////////////////
void SocketTest::testGetOutputStream() {

    {
        // Simple fetch test
        ServerSocket server(0);
        Socket client( "127.0.0.1", server.getLocalPort() );
        OutputStream* os = client.getOutputStream();
        CPPUNIT_ASSERT_MESSAGE( "Failed to get stream", os != NULL );
        os->close();
        client.close();
        server.close();
    }

    // Simple read/write test over the IO streams
    ServerSocket server1(0);
    GetOutputStreamRunnable runnable( &server1 );
    Thread thread( &runnable );
    thread.start();

    Socket pingClient( "127.0.0.1", server1.getLocalPort() );

    // Busy wait until the client is connected.
    int c = 0;
    while( !pingClient.isConnected() ) {

        try {
            Thread::sleep( 200 );
        } catch( InterruptedException& e ) {
        }

        if( ++c > 5 ) {
            CPPUNIT_FAIL( "thread is not alive" );
        }
    }

    Thread::sleep( 200 );

    // Write some data to the server
    OutputStream* out = pingClient.getOutputStream();
    unsigned char buffer[250] = {0};
    out->write( buffer, 1 );

    // Wait for the server to finish
    Thread::yield();
    c = 0;
    while( thread.isAlive() ) {

        try {
            Thread::sleep( 200 );
        } catch( InterruptedException& e ) {
        }

        if( ++c > 5 ) {
            CPPUNIT_FAIL( "read call did not exit" );
        }
    }

    // Subsequent writes should throw an exception
    try {

        // The output buffer may remain valid until the close completes
        for( int i = 0; i < 400; i++ ) {
            out->write( buffer, 1 );
        }
        CPPUNIT_FAIL( "write to closed socket did not cause exception" );
    } catch( IOException& e ) {
    }

    out->close();
    pingClient.close();
    server1.close();

    {
        // Test for exception on get when socket has its output shutdown.
        ServerSocket ss2(0);
        Socket s( "127.0.0.1", ss2.getLocalPort() );
        std::auto_ptr<Socket> worker( ss2.accept() );
        s.shutdownOutput();

        CPPUNIT_ASSERT_THROW_MESSAGE(
            "Should have thrown an IOException",
            s.getOutputStream(),
            IOException );
    }
}

// TODO - Remove or replace old tests

////////////////////////////////////////////////////////////////////////////////
namespace {

    class MyServerThread : public lang::Thread{
    private:

        bool done;
        int numClients;
        std::string lastMessage;
        std::auto_ptr<ServerSocket> server;

    public:

        util::concurrent::Mutex mutex;

    public:

        MyServerThread() : Thread(), done( false ), numClients( 0 ), lastMessage(), server(), mutex() {
            server.reset( new ServerSocket(0) );
        }

        virtual ~MyServerThread(){
            stop();
        }

        int getLocalPort() {
            if( this->server.get() != NULL ) {
                return server->getLocalPort();
            }

            return 0;
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

                std::auto_ptr<Socket> socket( server->accept() );
                server->close();

                //socket->setSoTimeout( 10 );
                socket->setSoLinger( false, 0 );
                numClients++;

                synchronized(&mutex)
                {
                   mutex.notifyAll();
                }

                while( !done && socket.get() != NULL ){

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

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 100 );

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect( "127.0.0.1", serverThread.getLocalPort() );
        client->setSoLinger( false, 0 );

        synchronized( &serverThread.mutex ) {

            if( serverThread.getNumClients() != 1 ) {
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

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 100 );

        SocketFactory* factory = SocketFactory::getDefault();
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", serverThread.getLocalPort() );
        client->setSoLinger( false, 0 );
        client->setTcpNoDelay( true );

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

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 100 );

        SocketFactory* factory = SocketFactory::getDefault();
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect( "127.0.0.1", serverThread.getLocalPort() );
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

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 100 );

        SocketFactory* factory = SocketFactory::getDefault();
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", serverThread.getLocalPort() );
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

        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 10 );

        SocketFactory* factory = SocketFactory::getDefault();
        std::auto_ptr<Socket> client( factory->createSocket() );

        client->connect("127.0.0.1", serverThread.getLocalPort() );
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
