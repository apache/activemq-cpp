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
#include <activemq/exceptions/NullPointerException.h>
#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/util/Date.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::ActiveMQProducer( connector::ProducerInfo* producerInfo,
                                    ActiveMQSession* session )
{
    if( session == NULL || producerInfo == NULL )
    {
        throw NullPointerException(
            __FILE__, __LINE__,
            "ActiveMQProducer::ActiveMQProducer - Init with NULL Session" );
    }

    // Init Producer Data
    this->session      = session;
    this->producerInfo = producerInfo;
    this->closed       = false;

    // Default the Delivery options
    this->defaultDeliveryMode = cms::DeliveryMode::PERSISTENT;
    this->disableMsgId        = false;
    this->disableTimestamps   = false;
    this->defaultPriority     = 4;
    this->defaultTimeToLive   = 0;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQProducer::~ActiveMQProducer(void)
{
    try
    {
        close();
    }
    AMQ_CATCH_NOTHROW( ActiveMQException )
    AMQ_CATCHALL_NOTHROW( )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::close()
    throw ( cms::CMSException )
{
    try
    {
        if( !closed ) {
            // Dispose of the ProducerInfo
            session->onDestroySessionResource( this );

            closed = true;
        }
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( cms::Message* message )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - This Producer is closed" );
        }

        send( producerInfo->getDestination(), message );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( cms::Message* message, int deliveryMode,
    int priority,
    long long timeToLive )
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - This Producer is closed" );
        }

        send( producerInfo->getDestination(), message, deliveryMode,
            priority, timeToLive );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( const cms::Destination* destination,
                             cms::Message* message) throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - This Producer is closed" );
        }

        send( destination, message, defaultDeliveryMode,
            defaultPriority, defaultTimeToLive );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQProducer::send( const cms::Destination* destination,
                             cms::Message* message, int deliveryMode,
                             int priority, long long timeToLive)
    throw ( cms::CMSException )
{
    try
    {
        if( closed )
        {
            throw InvalidStateException(
                __FILE__, __LINE__,
                "ActiveMQProducer::send - This Producer is closed" );
        }

        // configure the message
        message->setCMSDestination( destination );
        message->setCMSDeliveryMode( deliveryMode );
        message->setCMSPriority( priority );

        long long expiration = 0LL;

        if( !disableTimestamps ) {
            long long timeStamp = Date::getCurrentTimeMilliseconds();
            message->setCMSTimeStamp(timeStamp);
            if( timeToLive > 0LL ) {
                expiration = timeToLive + timeStamp;
            }
        }

        message->setCMSExpiration(expiration);

        session->send( message, this );
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

