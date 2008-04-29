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

#ifndef _DECAF_NET_SOCKETTEST_H_
#define _DECAF_NET_SOCKETTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/net/ServerSocket.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Thread.h>
#include <list>
#include <string.h>

namespace decaf{
namespace net{

    class SocketTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( SocketTest );
        CPPUNIT_TEST( testConnect );
        CPPUNIT_TEST( testTx );
        CPPUNIT_TEST( testTrx );
        CPPUNIT_TEST( testTrxNoDelay );
        CPPUNIT_TEST( testRxFail );
        CPPUNIT_TEST_SUITE_END();

    public:

        static const int port = 23232;

        class MyServerThread : public lang::Thread{
        private:

            bool done;
            int numClients;
            std::string lastMessage;

        public:

            util::concurrent::Mutex mutex;

        public:

            MyServerThread(){
                done = false;
                numClients = 0;
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
                    socket->setSoLinger( false );
                    numClients++;

                    synchronized(&mutex)
                    {
                       mutex.notifyAll();
                    }

                    while( !done && socket != NULL ){

                        io::InputStream* stream = socket->getInputStream();

                        memset( buf, 0, 1000 );
                        try{

                            if( stream->read( buf, 0, 1000 ) == -1 ) {
                                done = true;
                                continue;
                            }

                            lastMessage = (char*)buf;

                            if( strcmp( (char*)buf, "reply" ) == 0 ){
                                io::OutputStream* output = socket->getOutputStream();
                                output->write( (unsigned char*)"hello", 0, strlen("hello" ) );

                                  synchronized(&mutex)
                                  {
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

    public:

        virtual ~SocketTest() {}

        void testConnect();
        void testTx();
        void testTrx();
        void testRxFail();
        void testTrxNoDelay();

    };

}}

#endif /*_DECAF_NET_SOCKETTEST_H_*/
