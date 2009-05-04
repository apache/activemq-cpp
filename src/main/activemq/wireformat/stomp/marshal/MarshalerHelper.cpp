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

#include "MarshalerHelper.h"

#include <activemq/wireformat/stomp/StompCommandConstants.h>
#include <activemq/commands/LocalTransactionId.h>

#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Long.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace activemq::wireformat::stomp::marshal;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
void MarshalerHelper::convertProperties( const Pointer<StompFrame>& frame,
                                         const Pointer<Message>& message ) {

    const std::string destination =
        frame->removeProperty( StompCommandConstants::HEADER_DESTINATION );
    message->setDestination( convertDestination( destination ) );

    const std::string messageId =
        frame->removeProperty( StompCommandConstants::HEADER_MESSAGEID );
    message->setMessageId( convertMessageId( messageId ) );

    // the standard JMS headers
    message->setCorrelationId( StompCommandConstants::HEADER_CORRELATIONID );

    if( frame->hasProperty( StompCommandConstants::HEADER_EXPIRES ) ) {
        message->setExpiration( Integer::parseInt(
            frame->removeProperty( StompCommandConstants::HEADER_EXPIRES ) ) );
    }

    if( frame->hasProperty( StompCommandConstants::HEADER_JMSPRIORITY ) ) {
        message->setPriority( Integer::parseInt(
            frame->removeProperty( StompCommandConstants::HEADER_JMSPRIORITY ) ) );
    }

    if( frame->hasProperty( StompCommandConstants::HEADER_TYPE ) ) {
        message->setType( frame->removeProperty( StompCommandConstants::HEADER_TYPE ) );
    }

    if( frame->hasProperty( StompCommandConstants::HEADER_REPLYTO ) ) {
        message->setReplyTo( convertDestination(
            frame->removeProperty( StompCommandConstants::HEADER_REPLYTO ) ) );
    }

    if( frame->hasProperty( StompCommandConstants::HEADER_PERSISTENT ) ) {
        message->setPersistent( Boolean::parseBoolean(
            frame->removeProperty( StompCommandConstants::HEADER_PERSISTENT ) ) );
    }

    if( frame->hasProperty( StompCommandConstants::HEADER_TRANSACTIONID ) ) {
        std::string transactionId =
            frame->removeProperty( StompCommandConstants::HEADER_TRANSACTIONID );
        message->setTransactionId( convertTransactionId( transactionId ) );
    }

    // Copy the general headers over to the Message.
    std::vector< std::pair<std::string, std::string> > properties = frame->getProperties().toArray();
    std::vector< std::pair<std::string, std::string> >::const_iterator iter = properties.begin();

    for( ; iter != properties.end(); ++iter ) {
        message->getMessageProperties().setString( iter->first, iter->second );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MarshalerHelper::convertProperties( const Pointer<Message>& message,
                                         const Pointer<StompFrame>& frame ) {

    frame->setProperty( StompCommandConstants::HEADER_DESTINATION,
                        convertDestination( message->getDestination() ) );
    frame->setProperty( StompCommandConstants::HEADER_MESSAGEID,
                        convertMessageId( message->getMessageId() ) );

    if( message->getCorrelationId() != "" ) {
        frame->setProperty( StompCommandConstants::HEADER_CORRELATIONID,
                            message->getCorrelationId() );
    }

    frame->setProperty( StompCommandConstants::HEADER_EXPIRES,
                        Long::toString( message->getExpiration() ) );
    frame->setProperty( StompCommandConstants::HEADER_PERSISTENT,
                        Boolean::toString( message->isPersistent() ) );

    if( message->getRedeliveryCounter() != 0 ) {
        frame->setProperty( StompCommandConstants::HEADER_REDELIVERED, "true" );
    }

    frame->setProperty( StompCommandConstants::HEADER_JMSPRIORITY,
                        Integer::toString( message->getPriority() ) );

    if( message->getReplyTo() != NULL ) {
        frame->setProperty( StompCommandConstants::HEADER_REPLYTO,
                            convertDestination( message->getReplyTo() ) );
    }

    frame->setProperty( StompCommandConstants::HEADER_TIMESTAMP,
                        Long::toString( message->getTimestamp() ) );

    if( message->getType() != "" ) {
        frame->setProperty( StompCommandConstants::HEADER_TYPE, message->getType() );
    }

    if( message->getTransactionId() != NULL ) {
        frame->setProperty( StompCommandConstants::HEADER_TRANSACTIONID,
                            convertTransactionId( message->getTransactionId() ) );
    }

    std::vector<std::string> keys = message->getMessageProperties().keySet();
    std::vector<std::string>::const_iterator iter = keys.begin();

    for( ; iter != keys.end(); ++iter ) {
        frame->setProperty( *iter, message->getMessageProperties().getString( *iter ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshalerHelper::convertDestination( const Pointer<ActiveMQDestination>& destination ) {

    if( destination == NULL ) {
        return "";
    } else {

        switch( destination->getDestinationType() ) {

            case cms::Destination::TOPIC:
                return std::string( "/topic/" ) + destination->getPhysicalName();
            case cms::Destination::TEMPORARY_TOPIC:

                if( destination->getPhysicalName().find( "/remote-temp-topic/" ) == 0 ) {
                    return destination->getPhysicalName();
                } else {
                    return std::string( "/temp-topic/" ) + destination->getPhysicalName();
                }

            case cms::Destination::TEMPORARY_QUEUE:

                if( destination->getPhysicalName().find( "/remote-temp-queue/" ) == 0 ) {
                    return destination->getPhysicalName();
                } else {
                    return std::string( "/temp-queue/" ) + destination->getPhysicalName();
                }

            default:
                return std::string( "/queue/" ) + destination->getPhysicalName();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ActiveMQDestination> MarshalerHelper::convertDestination( const std::string& destination ) {

    if( destination == "" ) {
        return Pointer<ActiveMQDestination>();
    }

    int type = 0;
    std::string dest = "";

    if( destination.find( "/queue/" ) == 0 ) {
        dest = destination.substr( 7 );
        type = cms::Destination::QUEUE;
    } else if( destination.find( "/topic/" ) == 0 ) {
        dest = destination.substr( 7 );
        type = cms::Destination::TOPIC;
    } else if( destination.find( "/temp-topic/" ) == 0 ) {
        dest = destination.substr( 12 );
        type = cms::Destination::TEMPORARY_TOPIC;
    } else if( destination.find( "/temp-queue/" ) == 0 ) {
        dest = destination.substr( 12 );
        type = cms::Destination::TEMPORARY_QUEUE;
    } else if( destination.find( "/remote-temp-topic/" ) == 0 ) {
        type = cms::Destination::TEMPORARY_TOPIC;
    } else if( destination.find( "/remote-temp-queue/" ) == 0 ) {
        type = cms::Destination::TEMPORARY_QUEUE;
    }

    return ActiveMQDestination::createDestination( type, dest );
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshalerHelper::convertMessageId( const Pointer<MessageId>& messageId ) {

    std::string result = convertProducerId( messageId->getProducerId() ) + ":" +
                         Long::toString( messageId->getProducerSequenceId() );

    return result;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<MessageId> MarshalerHelper::convertMessageId( const std::string& messageId ) {

    if( messageId == "" ) {
        return Pointer<MessageId>();
    }

    Pointer<MessageId> id( new MessageId() );
    StringTokenizer tokenizer( messageId, ":" );

    id->setProducerId( convertProducerId( tokenizer.nextToken() ) );

    while( tokenizer.hasMoreTokens() ){
        std::string text = tokenizer.nextToken();
        id->setProducerSequenceId( Long::parseLong( text ) );
    }

    return id;
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshalerHelper::convertConsumerId( const Pointer<ConsumerId>& consumerId ) {

    return consumerId->getConnectionId() + ":" +
           Long::toString( consumerId->getSessionId() ) + ":" +
           Long::toString( consumerId->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ConsumerId> MarshalerHelper::convertConsumerId( const std::string& consumerId ) {

    if( consumerId == "" ) {
        return Pointer<ConsumerId>();
    }

    Pointer<ConsumerId> id( new ConsumerId() );
    StringTokenizer tokenizer( consumerId, ":" );

    id->setConnectionId( tokenizer.nextToken() );

    while( tokenizer.hasMoreTokens() ){
        string text = tokenizer.nextToken();

        if( tokenizer.hasMoreTokens() ) {
            id->setSessionId( Long::parseLong( text ) );
        } else {
            id->setValue( Long::parseLong( text ) );
        }
    }

    return id;
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshalerHelper::convertProducerId( const Pointer<ProducerId>& producerId ) {

    return producerId->getConnectionId() + ":" +
           Long::toString( producerId->getSessionId() ) + ":" +
           Long::toString( producerId->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<ProducerId> MarshalerHelper::convertProducerId( const std::string& producerId ) {

    if( producerId == "" ) {
        return Pointer<ProducerId>();
    }

    Pointer<ProducerId> id( new ProducerId() );
    StringTokenizer tokenizer( producerId, ":" );

    id->setConnectionId( tokenizer.nextToken() );

    while( tokenizer.hasMoreTokens() ){
        string text = tokenizer.nextToken();

        if( tokenizer.hasMoreTokens() ) {
            id->setSessionId( Long::parseLong( text ) );
        } else {
            id->setValue( Long::parseLong( text ) );
        }
    }

    return id;
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshalerHelper::convertTransactionId( const Pointer<TransactionId>& transactionId ) {

    Pointer<LocalTransactionId> id = transactionId.dynamicCast<LocalTransactionId>();
    std::string result = id->getConnectionId()->getValue() + ":" + Long::toString( id->getValue() );
    return result;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<TransactionId> MarshalerHelper::convertTransactionId( const std::string& transactionId ) {

    if( transactionId == "" ) {
        return Pointer<TransactionId>();
    }

    Pointer<LocalTransactionId> id( new LocalTransactionId() );
    StringTokenizer tokenizer( transactionId, ":" );

    Pointer<ConnectionId> connectionId( new ConnectionId() );
    connectionId->setValue( tokenizer.nextToken() );
    id->setConnectionId( connectionId );

    while( tokenizer.hasMoreTokens() ){
        string text = tokenizer.nextToken();
        id->setValue( Long::parseLong( text ) );
    }

    return id;
}
