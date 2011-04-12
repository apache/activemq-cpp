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

#ifndef _ACTIVEMQ_COMMANDS_CONSUMERCONTROL_H_
#define _ACTIVEMQ_COMMANDS_CONSUMERCONTROL_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for ConsumerControl
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API ConsumerControl : public BaseCommand {
    protected:

        Pointer<ActiveMQDestination> destination;
        bool close;
        Pointer<ConsumerId> consumerId;
        int prefetch;
        bool flush;
        bool start;
        bool stop;

    public:

        const static unsigned char ID_CONSUMERCONTROL = 17;

    private:

        ConsumerControl( const ConsumerControl& );
        ConsumerControl& operator= ( const ConsumerControl& );

    public:

        ConsumerControl();

        virtual ~ConsumerControl();

        virtual unsigned char getDataStructureType() const;

        virtual ConsumerControl* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination( const Pointer<ActiveMQDestination>& destination );

        virtual bool isClose() const;
        virtual void setClose( bool close );

        virtual const Pointer<ConsumerId>& getConsumerId() const;
        virtual Pointer<ConsumerId>& getConsumerId();
        virtual void setConsumerId( const Pointer<ConsumerId>& consumerId );

        virtual int getPrefetch() const;
        virtual void setPrefetch( int prefetch );

        virtual bool isFlush() const;
        virtual void setFlush( bool flush );

        virtual bool isStart() const;
        virtual void setStart( bool start );

        virtual bool isStop() const;
        virtual void setStop( bool stop );

        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_CONSUMERCONTROL_H_*/
