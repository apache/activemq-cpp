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

#ifndef _ACTIVEMQ_COMMANDS_CONNECTIONID_H_
#define _ACTIVEMQ_COMMANDS_CONNECTIONID_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/BaseDataStructure.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    class SessionId;
    class ProducerId;
    class ConsumerId;

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for ConnectionId
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API ConnectionId : public BaseDataStructure, public decaf::lang::Comparable<ConnectionId> {
    protected:

        std::string value;

    public:

        const static unsigned char ID_CONNECTIONID = 120;

        typedef decaf::lang::PointerComparator<ConnectionId> COMPARATOR;

    public:

        ConnectionId();

        ConnectionId( const ConnectionId& other );

        ConnectionId( const SessionId* sessionId );

        ConnectionId( const ProducerId* producerId );

        ConnectionId( const ConsumerId* consumerId );

        virtual ~ConnectionId();

        virtual unsigned char getDataStructureType() const;

        virtual ConnectionId* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual const std::string& getValue() const;
        virtual std::string& getValue();
        virtual void setValue( const std::string& value );

        virtual int compareTo( const ConnectionId& value ) const;

        virtual bool equals( const ConnectionId& value ) const;

        virtual bool operator==( const ConnectionId& value ) const;

        virtual bool operator<( const ConnectionId& value ) const;

        ConnectionId& operator= ( const ConnectionId& other );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_CONNECTIONID_H_*/
