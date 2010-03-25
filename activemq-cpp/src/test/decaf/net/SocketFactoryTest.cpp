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

#include <memory>

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

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket( "127.0.0.1", port ) );

        client->setSoLinger( 0 );

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
    }
    catch(lang::Exception ex)
    {
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

        std::auto_ptr<SocketFactory> factory( SocketFactory::getDefault() );
        std::auto_ptr<Socket> client( factory->createSocket( "127.0.0.1", port ) );

        client->setSoLinger( 0 );
        client->setTcpNoDelay( true );

        CPPUNIT_ASSERT( client->getTcpNoDelay() == true );

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
    }
    catch(lang::Exception ex)
    {
        CPPUNIT_ASSERT( false );
    }
}
