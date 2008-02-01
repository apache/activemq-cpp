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

#include "StompSessionManager.h"

#include <activemq/core/ActiveMQMessage.h>
#include <activemq/core/ActiveMQConstants.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <activemq/connector/stomp/StompSessionInfo.h>
#include <activemq/connector/stomp/StompConsumerInfo.h>
#include <activemq/connector/stomp/commands/SubscribeCommand.h>
#include <activemq/connector/stomp/commands/UnsubscribeCommand.h>
#include <activemq/connector/stomp/StompSelector.h>
#include <decaf/util/Properties.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <activemq/util/Config.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::exceptions;
using namespace activemq::transport;
using namespace activemq::connector;
using namespace activemq::connector::stomp;
using namespace activemq::connector::stomp::commands;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StompSessionManager::StompSessionManager( const std::string& connectionId,
                                          Connector* connector,
                                          Transport* transport ) {

    if( transport == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__,
            "StompSessionManager::StompSessionManager - Null param passed" );
    }

    this->transport = transport;
    this->connector = connector;
    this->connectionId = connectionId;
    this->messageListener = NULL;
}

////////////////////////////////////////////////////////////////////////////////
StompSessionManager::~StompSessionManager() {
    // NOTE - I am not cleaning out the ConsumerInfo objects in the
    // map because it is really the job of the consumer to remove itself
    // when it is destructed.  If it doesn't then we would have problems,
    // but it's deleted after this object then we would still have problems.
}

////////////////////////////////////////////////////////////////////////////////
connector::SessionInfo* StompSessionManager::createSession(
    cms::Session::AcknowledgeMode ackMode )
        throw ( activemq::exceptions::ActiveMQException ) {

    try{

        SessionInfo* session = new StompSessionInfo( connector );

        // Init data
        session->setAckMode( ackMode );
        session->setConnectionId( connectionId );
        session->setSessionId( this->nextSessionId.getNextSequenceId() );

        return session;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}

////////////////////////////////////////////////////////////////////////////////
void StompSessionManager::removeSession(
    connector::SessionInfo* session AMQCPP_UNUSED)
        throw ( activemq::exceptions::ActiveMQException ) {
    // NO-op
}

////////////////////////////////////////////////////////////////////////////////
connector::ConsumerInfo* StompSessionManager::createConsumer(
    const cms::Destination* destination,
    SessionInfo* session,
    const std::string& selector,
    bool noLocal )
        throw( StompConnectorException ) {

    try{
        // Delegate to the createDurableConsumer method, just pass the
        // appropriate params so that a regular consumer is created on
        // the broker side.
        return createDurableConsumer(
            destination, session, "", selector, noLocal );
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
connector::ConsumerInfo* StompSessionManager::createDurableConsumer(
    const cms::Destination* destination,
    SessionInfo* session,
    const std::string& name,
    const std::string& selector,
    bool noLocal )
        throw ( StompConnectorException ) {

    try {

        // Initialize a new Consumer info Message
        StompConsumerInfo* consumer = new StompConsumerInfo( connector );

        consumer->setConsumerId( this->nextConsumerId.getNextSequenceId() );
        consumer->setDestination( destination );
        consumer->setMessageSelector( selector );
        consumer->setSessionInfo( session );
        consumer->setNoLocal( noLocal );
        consumer->setName( name );

        return consumer;
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void StompSessionManager::startConsumer(
    connector::ConsumerInfo* consumer) throw ( StompConnectorException ) {

    try {

        StompConsumerInfo* stompConsumer = dynamic_cast<StompConsumerInfo*>(consumer);
        if( stompConsumer == NULL ) {
            throw StompConnectorException(
                __FILE__, __LINE__,
                "StompSessionManager::startConsumer - "
                "ConsumerInfo is not of type created by this connector" );
        }

        synchronized( &mutex ) {

            const cms::Destination* destination = stompConsumer->getDestination();

            // Find the right mapping to consumers
            ConsumerMap& consumerMap =
                destinationMap[ destination->toProviderString() ];

            // We only need to send a sub request if there are no active
            // consumers on this destination.
            if( consumerMap.empty() ) {

                // Send the request to the Broker
                SubscribeCommand cmd;

                if( stompConsumer->getSessionInfo()->getAckMode() == cms::Session::CLIENT_ACKNOWLEDGE ) {
                    cmd.setAckMode( CommandConstants::ACK_CLIENT );
                }
                cmd.setDestination( destination->toProviderString() );

                if( stompConsumer->getNoLocal() ) {
                    cmd.setNoLocal( stompConsumer->getNoLocal() );
                }

                if( stompConsumer->getName() != "" ) {
                    cmd.setSubscriptionName( stompConsumer->getName() );
                }

                // Grab any options from the destination and set them
                // for this subscription.
                setSubscribeOptions( destination, cmd );

                // The Selector is set on the first subscribe on this dest,
                // and if another consumer is created on this destination
                // that specifies a selector it will be ignored.  While
                // this is not ideal, is the only way to handle the fact
                // that activemq stomp doesn't support multiple sessions.
                if( stompConsumer->getMessageSelector() != "" ) {
                    cmd.setMessageSelector( stompConsumer->getMessageSelector() );
                }

                // Fire the message
                try{
                    transport->oneway( &cmd );
                } catch( CommandIOException& ex ){
                    transport->close();
                    throw StompConnectorException( __FILE__, __LINE__,
                        ex.what() );
                }
            }

            // Store this consumer for later message dispatching.
            consumerMap.insert(
                make_pair( stompConsumer->getConsumerId(), stompConsumer ) );
        }
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void StompSessionManager::removeConsumer(
    connector::ConsumerInfo* consumer )
        throw( StompConnectorException ) {

    try{

        synchronized( &mutex ) {

            DestinationMap::iterator itr =
                destinationMap.find( consumer->getDestination()->toProviderString() );

            if( itr == destinationMap.end() ) {
                // Already removed from the map
                return;
            }

            ConsumerMap& consumers = itr->second;

            // Remove from the map.
            consumers.erase( consumer->getConsumerId() );

            // If there are no more on this destination then we unsubscribe
            if( consumers.empty() ) {

                UnsubscribeCommand cmd;

                cmd.setDestination(
                    consumer->getDestination()->toProviderString() );

                // Send the message
                try{
                    transport->oneway( &cmd );
                } catch( CommandIOException& ex ){
                    transport->close();
                    throw StompConnectorException( __FILE__, __LINE__,
                        ex.what() );
                }
            }
        }
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void StompSessionManager::onStompCommand( commands::StompCommand* command )
    throw ( StompConnectorException ) {

    try{

        cms::Message* message = dynamic_cast<cms::Message*>( command );

        if( message == NULL ) {
            throw StompConnectorException(
                __FILE__, __LINE__,
                "StompSessionManager::onStompCommand - Invalid Command" );
        }

        if( messageListener == NULL ) {
            throw StompConnectorException(
                __FILE__, __LINE__,
                "StompSessionManager::onStompCommand - "
                "No Message Listener Registered." );
        }

        synchronized( &mutex ) {

            DestinationMap::iterator itr =
                destinationMap.find( message->getCMSDestination()->toProviderString() );

            if( itr == destinationMap.end() ) {
                throw StompConnectorException(
                    __FILE__, __LINE__,
                    "StompSessionManager::onStompCommand - "
                    "Received a Message that doesn't have a listener" );
            }

            // If we only have 1 consumer, we don't need to clone the original
            // message.
            if( itr->second.size() == 1 ) {

                ConsumerInfo* consumerInfo = itr->second.begin()->second;

                if( StompSelector::isSelected(
                        consumerInfo->getMessageSelector(),
                        message ) ) {

                    ActiveMQMessage* msg =
                        dynamic_cast<ActiveMQMessage*>( message );
                    messageListener->onConsumerMessage( consumerInfo, msg );
                }

                return;
            }

            // We have more than one consumer of this message - we have to
            // clone the message for each consumer so they don't destroy each other's
            // message.
            ConsumerMap::iterator c_itr = itr->second.begin();

            for( ; c_itr != itr->second.end(); ++c_itr ) {

                ConsumerInfo* consumerInfo = c_itr->second;

                if( StompSelector::isSelected(
                        consumerInfo->getMessageSelector(),
                        message ) ) {

                    ActiveMQMessage* msg =
                        dynamic_cast<ActiveMQMessage*>( message->clone() );
                    messageListener->onConsumerMessage( consumerInfo, msg );
                }
            }

            // We got here which means that we sent copies, so remove
            // the original.
            delete command;
        }
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}

////////////////////////////////////////////////////////////////////////////////
void StompSessionManager::setSubscribeOptions( const cms::Destination* dest,
                                               SubscribeCommand& command )
    throw ( StompConnectorException ) {

    try{

        // Get the properties of this destination
        const cms::CMSProperties& destProperties = dest->getCMSProperties();

        if( destProperties.isEmpty() ) {
            // Nothing to do, so save some work and quit now.
            return;
        }

        std::string noLocalStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_NOLOCAL );

        if( destProperties.hasProperty( noLocalStr ) ) {
            command.setNoLocal(
                Boolean::parseBoolean(
                    destProperties.getProperty( noLocalStr ) ) );
        }

        std::string selectorStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_SELECTOR );

        if( destProperties.hasProperty( selectorStr ) ) {
            command.setMessageSelector(
                destProperties.getProperty( selectorStr ) );
        }

        std::string priorityStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_PRIORITY );

        if( destProperties.hasProperty( priorityStr ) ) {
            command.setPriority(
                Integer::parseInt(
                    destProperties.getProperty( priorityStr ) ) );
        }

        std::string dispatchAsyncStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_DISPATCHASYNC );

        if( destProperties.hasProperty( dispatchAsyncStr ) ) {
            command.setDispatchAsync(
                Boolean::parseBoolean(
                    destProperties.getProperty( dispatchAsyncStr ) ) );
        }

        std::string exclusiveStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_EXCLUSIVE );

        if( destProperties.hasProperty( exclusiveStr ) ) {
            command.setExclusive(
                Boolean::parseBoolean(
                    destProperties.getProperty( exclusiveStr ) ) );
        }

        std::string maxPendingMsgLimitStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CUNSUMER_MAXPENDINGMSGLIMIT );

        if( destProperties.hasProperty( maxPendingMsgLimitStr ) ) {
            command.setMaxPendingMsgLimit(
                Integer::parseInt(
                    destProperties.getProperty( maxPendingMsgLimitStr ) ) );
        }

        std::string prefetchSizeStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_PREFECTCHSIZE );

        if( destProperties.hasProperty( prefetchSizeStr ) ) {
            command.setPrefetchSize(
                Integer::parseInt(
                    destProperties.getProperty( prefetchSizeStr ) ) );
        }

        std::string retroactiveStr =
            ActiveMQConstants::toString(
                ActiveMQConstants::CONSUMER_RETROACTIVE );

        if( destProperties.hasProperty( retroactiveStr ) ) {
            command.setRetroactive(
                Boolean::parseBoolean(
                    destProperties.getProperty( retroactiveStr ) ) );
        }
    }
    AMQ_CATCH_RETHROW( StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, StompConnectorException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, StompConnectorException )
    AMQ_CATCHALL_THROW( StompConnectorException )
}
