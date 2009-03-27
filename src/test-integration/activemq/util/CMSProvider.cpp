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

#include "CMSProvider.h"

#include <cms/ConnectionFactory.h>

#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/util/IntegrationCommon.h>

#include <decaf/util/UUID.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CMSProvider::CMSProvider( const std::string& brokerURL,
                          cms::Session::AcknowledgeMode ackMode )
: brokerURL( brokerURL ), ackMode( ackMode ) {

    this->topic = true;
    this->destinationName = UUID::randomUUID().toString();
    this->durable = false;
    this->subscription = UUID::randomUUID().toString();

    this->initialize();
}

////////////////////////////////////////////////////////////////////////////////
CMSProvider::~CMSProvider() {
    try{
        close();
    }
    DECAF_CATCH_NOTHROW( decaf::lang::Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::close() throw( decaf::lang::Exception ) {

    if( this->consumer.get() != NULL ) {
        this->consumer->close();
    }
    if( this->producer.get() != NULL ) {
        this->producer->close();
    }
    if( this->noDestProducer.get() != NULL ) {
        this->noDestProducer->close();
    }

    if( this->destination.get() != NULL && !isDurable() ) {
        this->destroyDestination( this->destination.get() );
    }

    this->destination.reset( NULL );
    this->tempDestination.reset( NULL );

    if( this->session.get() != NULL ) {
        this->session->close();
    }

    if( this->connection.get() != NULL ) {
        this->connection->close();
    }

    this->consumer.reset( NULL );
    this->producer.reset( NULL );
    this->noDestProducer.reset( NULL );
    this->session.reset( NULL );
    this->connection.reset( NULL );
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::initialize( const std::string& username,
                              const std::string& password,
                              const std::string& clientId ) {

    try {

        this->username = username;
        this->password = password;
        this->clientId = clientId;

        if( this->clientId == "" ) {
            this->clientId = UUID::randomUUID().toString();
        }

        this->connectionFactory.reset(
            cms::ConnectionFactory::createCMSConnectionFactory( this->brokerURL ) );

        // Force a connect
        reconnect();

        // Force a new session to be created.
        reconnectSession();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::reconnect() {

    try{

        // Close everything first
        this->close();

        // Now create the connection
        this->connection.reset( getConnectionFactory()->createConnection(
            username, password, this->clientId ) );
        this->connection->start();

        if( this->session.get() != NULL ) {
            reconnectSession();
        }
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::reconnectSession() {

    try{

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->consumer.get() != NULL ) {
            this->consumer->close();
        }
        if( this->producer.get() != NULL ) {
            this->producer->close();
        }
        if( this->noDestProducer.get() != NULL ) {
            this->noDestProducer->close();
        }

        // Free any previously held resources.
        this->destination.reset( NULL );
        this->tempDestination.reset( NULL );
        this->consumer.reset( NULL );
        this->producer.reset( NULL );
        this->noDestProducer.reset( NULL );

        // Create a new session, if there was one here before it will be
        // destroyed.
        this->session.reset( this->connection->createSession( this->ackMode ) );
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::unsubscribe() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->consumer.get() && this->durable && this->topic ) {

            if( this->brokerURL.find_first_of( "stomp", 0 ) == string::npos ) {
                this->consumer->close();
                this->session->unsubscribe( this->subscription );
            }
        }
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::ConnectionFactory* CMSProvider::getConnectionFactory() {
    try {

        if( this->connectionFactory.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        return this->connectionFactory.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CMSProvider::getConnection() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        return this->connection.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Session* CMSProvider::getSession() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        return this->session.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* CMSProvider::getProducer() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->producer.get() == NULL ) {
            this->producer.reset(
                this->getSession()->createProducer( this->getDestination() ) );
        }

        return this->producer.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageProducer* CMSProvider::getNoDestProducer() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->noDestProducer.get() == NULL ) {
            this->noDestProducer.reset(
                this->getSession()->createProducer( NULL ) );
        }

        return this->noDestProducer.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* CMSProvider::getConsumer() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->consumer.get() == NULL ) {
            if( this->durable && this->topic ) {
               this->consumer.reset(
                   this->getSession()->createDurableConsumer(
                      dynamic_cast<cms::Topic*>( this->getDestination() ),
                      this->subscription, "" ) );
            } else {
                this->consumer.reset(
                    this->getSession()->createConsumer( this->getDestination() ) );
            }
        }

        return this->consumer.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CMSProvider::getDestination() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->destination.get() == NULL ) {
            if( this->topic == true ) {
                this->destination.reset(
                    this->getSession()->createTopic( this->getDestinationName() ) );
            } else {
                this->destination.reset(
                    this->getSession()->createQueue( this->getDestinationName() ) );
            }
        }

        return this->destination.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* CMSProvider::getTempDestination() {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        if( this->tempDestination.get() == NULL ) {
            if( this->topic == true ) {
                this->tempDestination.reset(
                    this->getSession()->createTemporaryTopic() );
            } else {
                this->tempDestination.reset(
                    this->getSession()->createTemporaryQueue() );
            }
        }

        return this->tempDestination.get();
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void CMSProvider::destroyDestination( const cms::Destination* destination ) {

    try {

        if( this->connection.get() == NULL ) {
            throw decaf::lang::exceptions::IllegalStateException(
                __FILE__, __LINE__,
                "CMSProvider has not been Initialized or is closed." );
        }

        ActiveMQConnection* amqConnection =
            dynamic_cast<ActiveMQConnection*>( this->connection.get() );

        try{
            amqConnection->destroyDestination( destination );
        } catch( decaf::lang::Exception& ex ) {
            if( IntegrationCommon::debug ) {
                ex.printStackTrace();
            }
        } catch( ... ) {
        }
    }
    AMQ_CATCH_RETHROW( activemq::exceptions::ActiveMQException )
    AMQ_CATCHALL_THROW( activemq::exceptions::ActiveMQException )
}
