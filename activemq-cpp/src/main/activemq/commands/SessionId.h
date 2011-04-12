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

#ifndef _ACTIVEMQ_COMMANDS_SESSIONID_H_
#define _ACTIVEMQ_COMMANDS_SESSIONID_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/BaseDataStructure.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    class ProducerId;
    class ConsumerId;

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for SessionId
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API SessionId : public BaseDataStructure, public decaf::lang::Comparable<SessionId> {
    protected:

        std::string connectionId;
        long long value;

    public:

        const static unsigned char ID_SESSIONID = 121;

        typedef decaf::lang::PointerComparator<SessionId> COMPARATOR;

    private:

        mutable Pointer<ConnectionId> parentId;

    public:

        SessionId();

        SessionId( const SessionId& other );

        SessionId( const ConnectionId* connectionId, long long sessionId );

        SessionId( const ProducerId* producerId );

        SessionId( const ConsumerId* consumerId );

        virtual ~SessionId();

        virtual unsigned char getDataStructureType() const;

        virtual SessionId* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        const Pointer<ConnectionId>& getParentId() const;

        virtual const std::string& getConnectionId() const;
        virtual std::string& getConnectionId();
        virtual void setConnectionId( const std::string& connectionId );

        virtual long long getValue() const;
        virtual void setValue( long long value );

        virtual int compareTo( const SessionId& value ) const;

        virtual bool equals( const SessionId& value ) const;

        virtual bool operator==( const SessionId& value ) const;

        virtual bool operator<( const SessionId& value ) const;

        SessionId& operator= ( const SessionId& other );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_SESSIONID_H_*/
