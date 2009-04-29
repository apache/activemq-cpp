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

#include <activemq/wireformat/stomp/marshal/Marshaler.h>

#include <activemq/commands/Command.h>
#include <activemq/wireformat/stomp/StompCommandConstants.h>
#include <activemq/wireformat/stomp/StompFrame.h>

#include <activemq/core/ActiveMQConstants.h>
#include <activemq/commands/ActiveMQMessage.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ActiveMQBytesMessage.h>
#include <activemq/commands/Response.h>
#include <activemq/commands/BrokerError.h>
#include <activemq/commands/MessageAck.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/ExceptionResponse.h>
#include <activemq/commands/ShutdownInfo.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/commands/TransactionInfo.h>
#include <activemq/commands/LocalTransactionId.h>
#include <activemq/commands/ProducerInfo.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/RemoveSubscriptionInfo.h>

#include <decaf/lang/exceptions/ClassCastException.h>
#include <decaf/lang/Boolean.h>
#include <decaf/lang/Integer.h>
#include <decaf/io/IOException.h>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::stomp;
using namespace activemq::wireformat::stomp::marshal;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::marshal( const Pointer<StompFrame>& frame )
    throw ( decaf::io::IOException ) {

    try {

        const std::string commandId = frame->getCommand();

        if( commandId == StompCommandConstants::CONNECTED ){
            return this->unmarshalConnected( frame );
        } else if( commandId == StompCommandConstants::ERROR_CMD ){
            return this->unmarshalError( frame );
        } else if( commandId == StompCommandConstants::RECEIPT ){
            return this->unmarshalReceipt( frame );
        } else if( commandId == StompCommandConstants::MESSAGE ){
            return this->unmarshalMessage( frame );
        }

        // We didn't seem to know what it was we got, so throw an exception.
        throw decaf::io::IOException(
            __FILE__, __LINE__,
            "Marshaler::marshal - No Command Created from frame" );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshal( const Pointer<Command>& command )
    throw ( decaf::io::IOException ) {

    try{

        if( command->isMessage() ) {
            return this->marshalMessage( command );
        } else if( command->isRemoveInfo() ) {
            return this->marshalRemoveInfo( command );
        } else if( command->isShutdownInfo() ) {
            return this->marshalShutdownInfo( command );
        } else if( command->isMessageAck() ) {
            return this->marshalAck( command );
        } else if( command->isConnectionInfo() ) {
            return this->marshalConnectionInfo( command );
        } else if( command->isTransactionInfo() ) {
            return this->marshalTransactionInfo( command );
        } else if( command->isConsumerInfo() ) {
            return this->marshalConsumerInfo( command );
        } else if( command->isRemoveSubscriptionInfo() ) {
            return this->marshalRemoveSubscriptionInfo( command );
        } else if( command->isResponseRequired() ) {

            // Send a fake Unsub command and ask for a receipt.
            Pointer<StompFrame> frame( new StompFrame() );
            frame->setCommand( StompCommandConstants::UNSUBSCRIBE );
            frame->setProperty( StompCommandConstants::HEADER_ID, "-1" );
            frame->setProperty( StompCommandConstants::HEADER_RECEIPT_REQUIRED,
                                Integer::toString( command->getCommandId() ) );
        }

        // Ignoring this command.
        return Pointer<StompFrame>();
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( ActiveMQException, decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::unmarshalMessage( const Pointer<StompFrame>& frame ) {

    Pointer<Message> message;

    // Convert from the Frame to standard message properties.
    helper.convertProperties( frame, message );

    // Check for Content length, that tells us if its a Text or Bytes Message
    if( frame->removeProperty( StompCommandConstants::HEADER_CONTENTLENGTH ) != "" ) {
        message.reset( new ActiveMQBytesMessage() );
        message->setContent( frame->getBody() );
    } else {
        Pointer<ActiveMQTextMessage> txtMessage( new ActiveMQTextMessage() );

        if( frame->getBodyLength() > 0 ) {
            std::string text( (char*)( &(frame->getBody()[0]) ), frame->getBodyLength() );
            txtMessage->setText( text );
        }

        message = txtMessage.dynamicCast<Message>();
    }

    return Pointer<Command>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::unmarshalReceipt( const Pointer<StompFrame>& frame ){

    Pointer<Response> response( new Response() );
    if( frame->hasProperty( StompCommandConstants::HEADER_RECEIPTID ) ) {
        response->setCorrelationId( Integer::parseInt(
            frame->getProperty( StompCommandConstants::HEADER_RECEIPTID ) ) );
    } else {
        throw IOException(
            __FILE__, __LINE__, "Error, Connected Command has no Response ID." );
    }

    return response;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::unmarshalConnected( const Pointer<StompFrame>& frame ) {

    Pointer<Response> response( new Response() );
    if( frame->hasProperty( StompCommandConstants::HEADER_RESPONSEID ) ) {
        response->setCorrelationId( Integer::parseInt(
            frame->getProperty( StompCommandConstants::HEADER_RESPONSEID ) ) );
    } else {
        throw IOException(
            __FILE__, __LINE__, "Error, Connected Command has no Response ID." );
    }

    return response;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<Command> Marshaler::unmarshalError( const Pointer<StompFrame>& frame ) {

    Pointer<Command> result;

    Pointer<BrokerError> error( new BrokerError() );
    error->setMessage(
        frame->removeProperty( StompCommandConstants::HEADER_MESSAGE ) );

    if( frame->hasProperty( StompCommandConstants::HEADER_RECEIPTID ) ) {
        Pointer<ExceptionResponse> errorResponse( new ExceptionResponse() );
        errorResponse->setException( error );
        errorResponse->setCorrelationId( Integer::parseInt(
            frame->removeProperty( StompCommandConstants::HEADER_RECEIPTID ) ) );
        return errorResponse;
    } else {
        return error;
    }
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalMessage( const Pointer<Command>& command ) {

    Pointer<Message> message = command.dynamicCast<Message>();

    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::SEND );

    // Convert the standard headers to the Stomp Format.
    helper.convertProperties( message, frame );

    // Convert the Content
    try{
        Pointer<ActiveMQTextMessage> txtMessage = message.dynamicCast<ActiveMQTextMessage>();
        std::string text = txtMessage->getText();
        frame->setBody( (unsigned char*)text.c_str(), text.length() );
        return frame;
    } catch( ClassCastException& ex ) {}

    try{
        Pointer<ActiveMQBytesMessage> bytesMessage = message.dynamicCast<ActiveMQBytesMessage>();
        frame->setBody( bytesMessage->getBodyBytes(), bytesMessage->getBodyLength() );
        return frame;
    } catch( ClassCastException& ex ) {}

    throw UnsupportedOperationException(
        __FILE__, __LINE__,
        "Stomp Marshaler can't marshal message of type: %s",
        typeid( message.get() ).name() );
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalAck( const Pointer<Command>& command ) {

    Pointer<MessageAck> ack = command.dynamicCast<MessageAck>();

    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::ACK );
    frame->setProperty( StompCommandConstants::HEADER_MESSAGEID,
                        helper.convertMessageId( ack->getLastMessageId() ) );

    if( ack->getTransactionId() != NULL ) {
        frame->setProperty( StompCommandConstants::HEADER_TRANSACTIONID,
                            helper.convertTransactionId( ack->getTransactionId() ) );
    }

    return frame;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalConnectionInfo( const Pointer<Command>& command ) {

    Pointer<ConnectionInfo> info = command.dynamicCast<ConnectionInfo>();

    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::CONNECT );
    frame->setProperty( StompCommandConstants::HEADER_CLIENT_ID, info->getClientId() );
    frame->setProperty( StompCommandConstants::HEADER_LOGIN, info->getUserName() );
    frame->setProperty( StompCommandConstants::HEADER_PASSWORD, info->getPassword() );
    frame->setProperty( StompCommandConstants::HEADER_REQUESTID,
                        Integer::toString( info->getCommandId() ) );

    return frame;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalTransactionInfo( const Pointer<Command>& command ) {

    Pointer<TransactionInfo> info = command.dynamicCast<TransactionInfo>();
    Pointer<LocalTransactionId> id = info->getTransactionId().dynamicCast<LocalTransactionId>();

    Pointer<StompFrame> frame( new StompFrame() );

    if( info->getType() == ActiveMQConstants::TRANSACTION_STATE_BEGIN ) {
        frame->setCommand( StompCommandConstants::BEGIN );
    } else if( info->getType() == ActiveMQConstants::TRANSACTION_STATE_ROLLBACK ) {
        frame->setCommand( StompCommandConstants::ABORT );
    } else if( info->getType() == ActiveMQConstants::TRANSACTION_STATE_COMMITONEPHASE ) {
        frame->setCommand( StompCommandConstants::COMMIT );
    }

    frame->setProperty( StompCommandConstants::HEADER_TRANSACTIONID,
                        helper.convertTransactionId( info->getTransactionId() ) );

    return frame;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalShutdownInfo( const Pointer<Command>& command AMQCPP_UNUSED ) {

    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::CONNECT );
    return frame;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalRemoveInfo( const Pointer<Command>& command ) {

    Pointer<RemoveInfo> info = command.dynamicCast<RemoveInfo>();
    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::UNSUBSCRIBE );

    try{
        Pointer<ConsumerId> id = info->getObjectId().dynamicCast<ConsumerId>();
        frame->setProperty( StompCommandConstants::HEADER_ID, helper.convertConsumerId( id ) );
        return frame;
    } catch( ClassCastException& ex ) {}

    return Pointer<StompFrame>();
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalConsumerInfo( const Pointer<Command>& command ) {

    Pointer<ConsumerInfo> info = command.dynamicCast<ConsumerInfo>();

    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::SUBSCRIBE );

    frame->setProperty( StompCommandConstants::HEADER_DESTINATION,
                        helper.convertDestination( info->getDestination() ) );
    frame->setProperty( StompCommandConstants::HEADER_ID,
                        helper.convertConsumerId( info->getConsumerId() ) );

    frame->setProperty( StompCommandConstants::HEADER_SUBSCRIPTIONNAME,
                        info->getSubscriptionName() );
    frame->setProperty( StompCommandConstants::HEADER_SELECTOR,
                        info->getSelector() );
    frame->setProperty( StompCommandConstants::HEADER_ACK, "client" );

    if( info->isNoLocal() ) {
        frame->setProperty( StompCommandConstants::HEADER_NOLOCAL, "true" );
    }

    frame->setProperty( StompCommandConstants::HEADER_DISPATCH_ASYNC,
                        Boolean::toString( info->isDispatchAsync() ) );

    if( info->isExclusive() ) {
        frame->setProperty( StompCommandConstants::HEADER_EXCLUSIVE, "true" );
    }

    frame->setProperty( StompCommandConstants::HEADER_MAXPENDINGMSGLIMIT,
                        Integer::toString( info->getMaximumPendingMessageLimit() ) );
    frame->setProperty( StompCommandConstants::HEADER_PREFETCHSIZE,
                        Integer::toString( info->getPrefetchSize() ) );
    frame->setProperty( StompCommandConstants::HEADER_CONSUMERPRIORITY,
                        Integer::toString( info->getPriority() ) );

    if( info->isRetroactive() ) {
        frame->setProperty( StompCommandConstants::HEADER_RETROACTIVE, "true" );
    }

    return frame;
}

////////////////////////////////////////////////////////////////////////////////
Pointer<StompFrame> Marshaler::marshalRemoveSubscriptionInfo( const Pointer<Command>& command ) {

    Pointer<RemoveSubscriptionInfo> info = command.dynamicCast<RemoveSubscriptionInfo>();
    Pointer<StompFrame> frame( new StompFrame() );
    frame->setCommand( StompCommandConstants::UNSUBSCRIBE );

    frame->setProperty( StompCommandConstants::HEADER_ID, info->getClientId() );
    frame->setProperty( StompCommandConstants::HEADER_SUBSCRIPTIONNAME,
                        info->getSubcriptionName() );

    return frame;
}
