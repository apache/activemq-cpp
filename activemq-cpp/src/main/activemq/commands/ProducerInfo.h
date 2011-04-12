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

#ifndef _ACTIVEMQ_COMMANDS_PRODUCERINFO_H_
#define _ACTIVEMQ_COMMANDS_PRODUCERINFO_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/BrokerId.h>
#include <activemq/commands/ProducerId.h>
#include <activemq/commands/RemoveInfo.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for ProducerInfo
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API ProducerInfo : public BaseCommand {
    protected:

        Pointer<ProducerId> producerId;
        Pointer<ActiveMQDestination> destination;
        std::vector< decaf::lang::Pointer<BrokerId> > brokerPath;
        bool dispatchAsync;
        int windowSize;

    public:

        const static unsigned char ID_PRODUCERINFO = 6;

    private:

        ProducerInfo( const ProducerInfo& );
        ProducerInfo& operator= ( const ProducerInfo& );

    public:

        ProducerInfo();

        virtual ~ProducerInfo();

        virtual unsigned char getDataStructureType() const;

        virtual ProducerInfo* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        Pointer<RemoveInfo> createRemoveCommand() const;

        virtual const Pointer<ProducerId>& getProducerId() const;
        virtual Pointer<ProducerId>& getProducerId();
        virtual void setProducerId( const Pointer<ProducerId>& producerId );

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination( const Pointer<ActiveMQDestination>& destination );

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath();
        virtual void setBrokerPath( const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath );

        virtual bool isDispatchAsync() const;
        virtual void setDispatchAsync( bool dispatchAsync );

        virtual int getWindowSize() const;
        virtual void setWindowSize( int windowSize );

        /**
         * @return an answer of true to the isProducerInfo() query.
         */
        virtual bool isProducerInfo() const {
            return true;
        }

        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_PRODUCERINFO_H_*/
