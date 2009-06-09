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

#include "SocketFactoryTest.h"

#include <decaf/util/Properties.h>
#include <decaf/net/SocketFactory.h>
#include <decaf/net/TcpSocket.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
void SocketFactoryTest::test()
{
    try
    {
        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 500 );

        util::Properties properties;

        std::ostringstream ostream;

        ostream << "127.0.0.1:" << port;

        properties.setProperty("soLinger", "false");

        Socket* client = SocketFactory::createSocket(
            ostream.str(), properties );

        synchronized(&serverThread.mutex)
        {
            if(serverThread.getNumClients() != 1)
            {
                serverThread.mutex.wait(10000);
            }
        }

        CPPUNIT_ASSERT( client->isConnected() );

        CPPUNIT_ASSERT( serverThread.getNumClients() == 1 );

        client->close();

        synchronized(&serverThread.mutex)
        {
            if(serverThread.getNumClients() != 0)
            {
                serverThread.mutex.wait(10000);
            }
        }

        CPPUNIT_ASSERT( serverThread.getNumClients() == 0 );

        serverThread.stop();
        serverThread.join();

        delete client;
    }
    catch(lang::Exception ex)
    {
        std::cout << "SocketFactoryTest::test - Caught Exception." << std::endl;
        ex.printStackTrace();
        CPPUNIT_ASSERT( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
void SocketFactoryTest::testNoDelay()
{
    try
    {
        MyServerThread serverThread;
        serverThread.start();

        Thread::sleep( 40 );

        util::Properties properties;

        std::ostringstream ostream;

        ostream << "127.0.0.1:" << port;

        properties.setProperty( "soLinger", "false" );
        properties.setProperty( "tcpNoDelay", "true" );

        Socket* client = SocketFactory::createSocket(
            ostream.str(), properties );

        TcpSocket* tcpSock = dynamic_cast<TcpSocket*>( client );
        CPPUNIT_ASSERT( tcpSock != NULL );
        CPPUNIT_ASSERT( tcpSock->getTcpNoDelay() == true );

        synchronized(&serverThread.mutex)
        {
            if(serverThread.getNumClients() != 1)
            {
                serverThread.mutex.wait(1000);
            }
        }

        CPPUNIT_ASSERT( client->isConnected() );

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

        delete client;
    }
    catch(lang::Exception ex)
    {
        CPPUNIT_ASSERT( false );
    }
}
