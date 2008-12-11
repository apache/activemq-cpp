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

#include "Chat.h"

#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <apr.h>
#include <apr_strings.h>

#include <cms/CMSException.h>
#include <cms/ConnectionFactory.h>

using namespace std;
using namespace cms;
using namespace example;
using namespace example::topics;
using namespace example::topics::chat;

////////////////////////////////////////////////////////////////////////////////
const char* Chat::DEFAULT_BROKER_URI = "tcp://127.0.0.1:61616";
const char* Chat::DEFAULT_TOPIC_NAME = "cms.sample.chat";

////////////////////////////////////////////////////////////////////////////////
Chat::Chat() {

    this->setBrokerURI( DEFAULT_BROKER_URI );
    this->setTopicName( DEFAULT_TOPIC_NAME );
}

////////////////////////////////////////////////////////////////////////////////
Chat::~Chat() {

    try {

        if( this->connection.get() != NULL ) {
            this->connection->close();
        }

    } catch( CMSException& ex ) {
        ex.printStackTrace();
    }
}

////////////////////////////////////////////////////////////////////////////////
void Chat::onException( const cms::CMSException& ex ) {
    ex.printStackTrace();
}

////////////////////////////////////////////////////////////////////////////////
void Chat::run() {

    auto_ptr<cms::ConnectionFactory> connectionFactory;

    // Create a connection.
    try {

        connectionFactory.reset(
            cms::ConnectionFactory::createCMSConnectionFactory( this->getBrokerURI() ) );

        this->connection.reset(
            connectionFactory->createConnection( this->getUsername(), this->getPassword() ) );

        this->connection->setExceptionListener( this );

    } catch( cms::CMSException& ex ) {
        std::cout << "==================================================" << std::endl;
        std::cout << "= Failed to Connect To Broker, Is it Running?    =" << std::endl;
        std::cout << "==================================================" << std::endl;
        return;
    }

    try{

        this->session.reset( this->connection->createSession( cms::Session::AUTO_ACKNOWLEDGE ) );
        this->topic.reset( this->session->createTopic( this->getTopicName() ) );
        this->producer.reset( this->session->createProducer( this->topic.get() ) );
        this->producer->setDeliveryMode( cms::DeliveryMode::NON_PERSISTENT );
        this->consumer.reset( this->session->createConsumer( this->topic.get() ) );
        this->consumer->setMessageListener( this );

        // Now that setup is complete, start the Connection
        this->connection->start();

    } catch( cms::CMSException& ex ) {
        onException( ex );
        return;
    }

    try {

        std::cout << "\n";
        std::cout << "Enter text messages to clients that subscribe to the ";
        std::cout << this->getTopicName() << " topic." << "\n";
        std::cout << "Press Enter to publish each message." << "\n";

        while( true ) {

            char s[120];
#ifdef AIX
            if( fgets( s, 120, stdin ) == NULL ) {
                break;
            } else if ( feof( stdin ) || ( strlen(s) == 0 ) || ( s[0] == L'\n' ) ) {
                break;
            }
#else

            std::cin.getline( s, 120, '\n' );

            // If there was an error reading input, or
            // the line was empty, exit the program.
            if( std::cin.fail() || (strlen(s) == 0) ) {
                break;
            }

#endif
            else if( strlen(s) > 0 ) {

                int cch = this->username.length() + strlen(s) + strlen(": ") + 1;
                char *text = new char[cch];

                if( text != NULL ) {

                    sprintf( text, "%s: %s", this->username.c_str(), s );

                    try {

                        auto_ptr<TextMessage> message(
                            this->session->createTextMessage( text ) );
                        this->producer->send( message.get() );

                    } catch( cms::CMSException& ex ) {
                        delete [] text;
                        onException( ex );
                        exit(1);
                    }

                    delete [] text;
                }
            }
        }

    } catch( cms::CMSException& ex ) {
        onException( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Chat::onMessage( const cms::Message* message ) {

    // Cast the message as a TextMessage if possible.
    try {

        const cms::TextMessage* textMessage =
            dynamic_cast<const cms::TextMessage*>( message );

        if( textMessage != NULL ) {
            std::cout << textMessage->getText() << std::endl;
        } else {
            std::cerr << "Unsupported message type received: "
                      << typeid( *message ).name()
                      << std::endl;
        }

    } catch( cms::CMSException& ex ) {
        onException( ex );
    }
}

////////////////////////////////////////////////////////////////////////////////
static void printUsage() {

    std::cout << "usage: Chat (options) ..." << "\n" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "  -b name:port Specify name:port of broker." << "\n";
    std::cout << "               Default broker: " << Chat::DEFAULT_BROKER_URI << "\n";
    std::cout << "  -u name      Specify a user name. (Required)" << "\n";
    std::cout << "               Default user name: <none> \n";
    std::cout << "  -p password  Specify password for user." << "\n";
    std::cout << "               Default password: <none> \n";
    std::cout << "  -h           This help screen." << "\n";
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

    Chat chat;

    // Is there anything to do?
    if( argc < 2 ) {
        printUsage();
        exit(1);
    }

    // Check parameters
    for( int i = 1; i < argc; i++ ) {

        char *arg = argv[i];

        if( apr_strnatcmp( arg,"-b" ) == 0 ) {

            if( i == argc - 1 || strncmp( argv[i+1], "-", 1 ) == 0 ) {
                std::cout << "error: missing broker name:port" << "\n";
                exit(1);
            }

            chat.setBrokerURI( argv[++i] );
            continue;
        }

        if( apr_strnatcmp( arg, "-u" ) == 0 ) {
            if( i == argc - 1 || strncmp( argv[i+1], "-", 1 ) == 0 ) {
                std::cout << "error: missing user name" << "\n";
                exit(1);
            }

            chat.setUsername( argv[++i] );
            continue;
        }

        if( apr_strnatcmp( arg, "-p" ) == 0 ) {
            if( i == argc - 1 || strncmp( argv[i+1], "-", 1 ) == 0 ) {
                std::cout << "error: missing password" << "\n";
                exit(1);
            }

            chat.setPassword( argv[++i] );
            continue;
        }

        if( apr_strnatcmp( arg, "-h" ) == 0 ) {
            printUsage();
            exit(1);
        }

        // Invalid argument
        std::cout << "error: unexpected argument: " << arg << "\n";
        printUsage();
        exit(1);
    }

    // Check values read in.
    if( chat.getUsername() == "" ) {
        std::cout << "error: user name must be supplied" << "\n";
        printUsage();
        exit(1);
    }

    // Start the CMS client for the "chat".
    chat.run();

    return 0;
}
