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
#include "ActiveMQProducer.h"

#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConnection.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/InvalidStateException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/util/Date.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::ActiveMQProducer( commands::ProducerInfo* producerInfo,
                                    const cms::Destination* destination,
                                    ActiveMQSession* session ) {

    if( session == NULL || producerInfo == NULL ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQProducer::ActiveMQProducer - Init with NULL Session" );
    }

    // Init Producer Data
    this->session = session;
    this->producerInfo.reset( producerInfo );
    this->destination.reset( destination != NULL ? destination->clone() : NULL );
    this->closed = false;

    // Default the Delivery options
    this->defaultDeliveryMode = cms::DeliveryMode::PERSISTENT;
    this->disableTimestamps = false;
    this->disableMessageId = false;
    this->defaultPriority = 4;
    this->defaultTimeToLive = 0;

    // TODO - How to manage resources
    // Listen for our resource to close
    //this->producerInfo->addListener( this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::~ActiveMQProducer() {
    try {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::close() throw ( cms::CMSException ) {

    try{

        if( !closed ) {

            this->session->getConnection()->disposeOf( this->producerInfo->getProducerId() );
            closed = true;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( cms::Message* message )
    throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        if( this->destination.get() == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - "
                "Producer has no Destination, must call send( dest, msg )" );
        }

        this->send( this->destination.get(), message );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( cms::Message* message, int deliveryMode,
                             int priority, long long timeToLive )
                                throw ( cms::CMSException ) {
    try {

        this->checkClosed();

        if( this->destination.get() == NULL ) {
            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - "
                "Producer has no Destination, must call send( dest, msg )" );
        }

        this->send( this->destination.get(), message, deliveryMode,
                    priority, timeToLive );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( const cms::Destination* destination,
                             cms::Message* message ) throw ( cms::CMSException ) {

    try {

        this->checkClosed();

        this->send( destination, message, defaultDeliveryMode,
                    defaultPriority, defaultTimeToLive );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( const cms::Destination* destination,
                             cms::Message* message, int deliveryMode,
                             int priority, long long timeToLive )
    throw ( cms::CMSException ) {

    try {

        checkClosed();

        if( destination == NULL ) {

            throw ActiveMQException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - Attempting to send on NULL destination");
        }

        // configure the message
        message->setCMSDestination( destination );
        message->setCMSDeliveryMode( deliveryMode );
        message->setCMSPriority( priority );

        long long expiration = 0LL;

        if( !disableTimestamps ) {
            long long timeStamp = Date::getCurrentTimeMilliseconds();
            message->setCMSTimestamp( timeStamp );
            if( timeToLive > 0LL ) {
                expiration = timeToLive + timeStamp;
            }
        }

        message->setCMSExpiration( expiration );

        // Delegate send to the session so that it can choose how to
        // send the message.
        this->session->send( message, this, this->memoryUsage.get() );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

// TODO
////////////////////////////////////////////////////////////////////////////////
//void ActiveMQProducer::onConnectorResourceClosed(
//    const ConnectorResource* resource ) throw ( cms::CMSException ) {
//
//    try{
//
//        checkClosed();
//
//        if( resource != producerInfo ) {
//            throw ActiveMQException(
//                __FILE__, __LINE__,
//                "ActiveMQProducer::onConnectorResourceClosed - "
//                "Unknown object passed to this callback");
//        }
//
//        // If our producer isn't closed already, then lets close
//        this->close();
//    }
//    AMQ_CATCH_RETHROW( ActiveMQException )
//    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
//    AMQ_CATCHALL_THROW( ActiveMQException )
//}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::onProducerAck( const commands::ProducerAck& ack ) {

    try{

        if( this->memoryUsage.get() != NULL ) {
            this->memoryUsage->decreaseUsage( ack.getSize() );
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::checkClosed() throw( exceptions::ActiveMQException ) {
    if( closed ) {
        throw ActiveMQException(
            __FILE__, __LINE__,
            "ActiveMQProducer - Producer Already Closed" );
    }
}
