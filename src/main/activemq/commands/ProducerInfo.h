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

#include <activemq/commands/BrokerId.h>
#include <decaf/lang/Pointer.h>
#include <activemq/util/Config.h>
#include <string>
#include <activemq/commands/BaseCommand.h>
#include <vector>
#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/ProducerId.h>

namespace activemq{
namespace commands{

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

        decaf::lang::Pointer<ProducerId> producerId;
        decaf::lang::Pointer<ActiveMQDestination> destination;
        std::vector< decaf::lang::Pointer<BrokerId> > brokerPath;
        bool dispatchAsync;
        int windowSize;

    public:

        const static unsigned char ID_PRODUCERINFO = 6;

    protected:

        ProducerInfo( const ProducerInfo& other );
        ProducerInfo& operator= ( const ProducerInfo& other );

    public:

        ProducerInfo();

        virtual ~ProducerInfo();

        /**
         * Get the unique identifier that this object and its own
         * Marshaler share.
         * @returns new DataStructure type copy.
         */
        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual ProducerInfo* cloneDataStructure() const;

        /**
         * Copy the contents of the passed object into this object's
         * members, overwriting any existing data.
         * @param src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        /**
         * Returns a string containing the information for this DataStructure
         * such as its type and value of its elements.
         * @return formatted string useful for debugging.
         */
        virtual std::string toString() const;

        /**
         * Compares the DataStructure passed in to this one, and returns if
         * they are equivalent.  Equivalent here means that they are of the
         * same type, and that each element of the objects are the same.
         * @returns true if DataStructure's are Equal.
         */
        virtual bool equals( const DataStructure* value ) const;

        virtual const decaf::lang::Pointer<ProducerId>& getProducerId() const;
        virtual decaf::lang::Pointer<ProducerId>& getProducerId();
        virtual void setProducerId( const decaf::lang::Pointer<ProducerId>& producerId );

        virtual const decaf::lang::Pointer<ActiveMQDestination>& getDestination() const;
        virtual decaf::lang::Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination( const decaf::lang::Pointer<ActiveMQDestination>& destination );

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath();
        virtual void setBrokerPath( const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath );

        virtual bool isDispatchAsync() const;
        virtual void setDispatchAsync( bool dispatchAsync );

        virtual int getWindowSize() const;
        virtual void setWindowSize( int windowSize );

        /**
         * Allows a Visitor to visit this command and return a response to the
         * command based on the command type being visited.  The command will call
         * the proper processXXX method in the visitor.
         * 
         * @return a Response to the visitor being called or NULL if no response.
         */
        virtual decaf::lang::Pointer<commands::Command> visit( activemq::state::CommandVisitor* visitor )
            throw( exceptions::ActiveMQException );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_PRODUCERINFO_H_*/
