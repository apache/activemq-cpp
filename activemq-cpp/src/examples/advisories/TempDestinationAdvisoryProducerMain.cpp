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

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <activemq/library/ActiveMQCPP.h>
#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/Destination.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

using namespace decaf;
using namespace decaf::lang;
using namespace cms;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
int main( int argc AMQCPP_UNUSED, char* argv[] AMQCPP_UNUSED ) {

    // We must always init the library first before using any methods in it.
    activemq::library::ActiveMQCPP::initializeLibrary();

    std::cout << "=====================================================\n";
    std::cout << "Starting the example:" << std::endl;
    std::cout << "-----------------------------------------------------\n";

    // Set the URI to point to the IPAddress of your broker.
    // add any optional params to the url to enable things like
    // tightMarshalling or tcp logging etc.  See the CMS web site for
    // a full list of configuration options.
    //
    //  http://activemq.apache.org/cms/
    //
    std::string brokerURI = "failover:(tcp://127.0.0.1:61616)";

    // Create the Connection
    auto_ptr<cms::ConnectionFactory> connectionFactory(
        cms::ConnectionFactory::createCMSConnectionFactory( brokerURI ) );

    auto_ptr<cms::Connection> connection;

    // Create a Connection
    try{
        connection.reset( connectionFactory->createConnection() );
    } catch( CMSException& e ) {
        e.printStackTrace();
        return 1;
    }

    // Create the Session
    std::auto_ptr<cms::Session> session( connection->createSession() );

    // Start the Connection now.
    connection->start();

    // Create a Temporary Topic and Queue.
    std::auto_ptr<cms::Destination> tempTopic( session->createTemporaryTopic() );
    std::auto_ptr<cms::Destination> tempQueue( session->createTemporaryQueue() );

    // Give the Broker some time
    Thread::sleep( 2000 );

    // Shutdown now
    connection->stop();

    std::cout << "-----------------------------------------------------\n";
    std::cout << "Finished with the example." << std::endl;
    std::cout << "=====================================================\n";

    // We must also always remember to shut down the Library when done.
    activemq::library::ActiveMQCPP::shutdownLibrary();

    return 0;
}
