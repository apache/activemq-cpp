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

#ifndef _ACTIVEMQ_COMMANDS_CONSUMERID_H_
#define _ACTIVEMQ_COMMANDS_CONSUMERID_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/BaseDataStructure.h>
#include <activemq/commands/SessionId.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for ConsumerId
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API ConsumerId : public BaseDataStructure, public decaf::lang::Comparable<ConsumerId> {
    private:

        mutable Pointer<SessionId> parentId;

    protected:

        std::string connectionId;
        long long sessionId;
        long long value;

    public:

        const static unsigned char ID_CONSUMERID = 122;

        typedef decaf::lang::PointerComparator<ConsumerId> COMPARATOR;

    public:

        ConsumerId();

        ConsumerId( const ConsumerId& other );

        ConsumerId( const SessionId& sessionId, long long consumerIdd );

        virtual ~ConsumerId();

        virtual unsigned char getDataStructureType() const;

        virtual ConsumerId* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        const Pointer<SessionId>& getParentId() const;

        virtual const std::string& getConnectionId() const;
        virtual std::string& getConnectionId();
        virtual void setConnectionId( const std::string& connectionId );

        virtual long long getSessionId() const;
        virtual void setSessionId( long long sessionId );

        virtual long long getValue() const;
        virtual void setValue( long long value );

        virtual int compareTo( const ConsumerId& value ) const;

        virtual bool equals( const ConsumerId& value ) const;

        virtual bool operator==( const ConsumerId& value ) const;

        virtual bool operator<( const ConsumerId& value ) const;

        ConsumerId& operator= ( const ConsumerId& other );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_CONSUMERID_H_*/
