/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_COMMANDS_MESSAGEPULL_H_
#define _ACTIVEMQ_COMMANDS_MESSAGEPULL_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/MessageId.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for MessagePull
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API MessagePull : public BaseCommand {
    protected:

        Pointer<ConsumerId> consumerId;
        Pointer<ActiveMQDestination> destination;
        long long timeout;
        std::string correlationId;
        Pointer<MessageId> messageId;

    public:

        const static unsigned char ID_MESSAGEPULL = 20;

    private:

        MessagePull( const MessagePull& );
        MessagePull& operator= ( const MessagePull& );

    public:

        MessagePull();

        virtual ~MessagePull();

        virtual unsigned char getDataStructureType() const;

        virtual MessagePull* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual const Pointer<ConsumerId>& getConsumerId() const;
        virtual Pointer<ConsumerId>& getConsumerId();
        virtual void setConsumerId( const Pointer<ConsumerId>& consumerId );

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination( const Pointer<ActiveMQDestination>& destination );

        virtual long long getTimeout() const;
        virtual void setTimeout( long long timeout );

        virtual const std::string& getCorrelationId() const;
        virtual std::string& getCorrelationId();
        virtual void setCorrelationId( const std::string& correlationId );

        virtual const Pointer<MessageId>& getMessageId() const;
        virtual Pointer<MessageId>& getMessageId();
        virtual void setMessageId( const Pointer<MessageId>& messageId );

        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_MESSAGEPULL_H_*/
