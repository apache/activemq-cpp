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
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/cmsutil/MessageCreator.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::cmsutil;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class Receiver: Runnable {
    private:

        string name;
        CountDownLatch ready;
        bool isClosing;
        CmsTemplate* cmsTemplate;
        Thread* asyncReceiverThread;

    private:

        Receiver( const Receiver& );
        Receiver& operator= ( const Receiver& );

    public:

        Receiver( string name, CmsTemplate* cmsTemplate ) :
            name(name), ready(1), isClosing(false), cmsTemplate(cmsTemplate), asyncReceiverThread(NULL) {

            asyncReceiverThread = new Thread( this, "AsyncReceiver" );
            asyncReceiverThread->start();

            waitUntilReady();
        }

        virtual ~Receiver() {

            isClosing = true;

            //wait for the m_asyncReceiverThread to terminate
            if( asyncReceiverThread ) {
                asyncReceiverThread->join();
                delete asyncReceiverThread;
            }

            delete cmsTemplate;
        }

        virtual void run() {

            ready.countDown();
            while( !isClosing ) {
                receiveMessage();
            }
        }

    private:

        /**
         * to reveive message from a destination synchronously
         * Note: If parameter retryOnError is true, then this function will retry on errors
         *       as long as the receiver's timeout has not been reached. Otherwise, this function will not retry on errors.
         */
        void receiveMessage() {

            try {

                Message* cmsMessage = cmsTemplate->receive();
                if( cmsMessage ) {
                    printf( "%s received a message\n", name.c_str() );
                    delete cmsMessage;
                }

            } catch( cms::CMSException& ex ) {
                std::cout << "Caught CMSException: ";
                std::cout << ex.getMessage().c_str() << endl;
                Thread::sleep( 10000 );
            }
        }

        void waitUntilReady() {
            ready.await();
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc AMQCPP_UNUSED, char* argv[] AMQCPP_UNUSED ) {

    std::cout << "=====================================================\n";
    std::cout << "Starting the MessageListener application..." << std::endl;

    //initialize ActiveMQCPP library
    activemq::library::ActiveMQCPP::initializeLibrary();

    ActiveMQConnectionFactory* connectionFactory =
        new ActiveMQConnectionFactory( "failover://(tcp://127.0.0.1:61616)" );

    CmsTemplate* cmsTemplate = new CmsTemplate( connectionFactory );
    cmsTemplate->setDefaultDestinationName( "CMSTemplateExamples" );
    cmsTemplate->setPubSubDomain( true );
    cmsTemplate->setReceiveTimeout( 2000 );
    Receiver* receiver = new Receiver( "MessageListener", cmsTemplate );

    std::cout << "Click any char to terminate the application..." << std::endl;
    getchar();

    //shut down ActiveMQCPP library
    delete connectionFactory;
    delete receiver;

    activemq::library::ActiveMQCPP::shutdownLibrary();

}
