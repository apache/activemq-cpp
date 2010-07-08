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

#ifndef _ACTIVEMQ_COMMANDS_CONNECTIONINFO_H_
#define _ACTIVEMQ_COMMANDS_CONNECTIONINFO_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/BrokerId.h>
#include <activemq/commands/ConnectionId.h>
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
     *  Command code for OpenWire format for ConnectionInfo
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API ConnectionInfo : public BaseCommand {
    protected:

        Pointer<ConnectionId> connectionId;
        std::string clientId;
        std::string password;
        std::string userName;
        std::vector< decaf::lang::Pointer<BrokerId> > brokerPath;
        bool brokerMasterConnector;
        bool manageable;
        bool clientMaster;
        bool faultTolerant;
        bool failoverReconnect;

    public:

        const static unsigned char ID_CONNECTIONINFO = 3;

    private:

        ConnectionInfo( const ConnectionInfo& );
        ConnectionInfo& operator= ( const ConnectionInfo& );

    public:

        ConnectionInfo();

        virtual ~ConnectionInfo();

        virtual unsigned char getDataStructureType() const;

        virtual ConnectionInfo* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        Pointer<RemoveInfo> createRemoveCommand() const;

        virtual const Pointer<ConnectionId>& getConnectionId() const;
        virtual Pointer<ConnectionId>& getConnectionId();
        virtual void setConnectionId( const Pointer<ConnectionId>& connectionId );

        virtual const std::string& getClientId() const;
        virtual std::string& getClientId();
        virtual void setClientId( const std::string& clientId );

        virtual const std::string& getPassword() const;
        virtual std::string& getPassword();
        virtual void setPassword( const std::string& password );

        virtual const std::string& getUserName() const;
        virtual std::string& getUserName();
        virtual void setUserName( const std::string& userName );

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath();
        virtual void setBrokerPath( const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath );

        virtual bool isBrokerMasterConnector() const;
        virtual void setBrokerMasterConnector( bool brokerMasterConnector );

        virtual bool isManageable() const;
        virtual void setManageable( bool manageable );

        virtual bool isClientMaster() const;
        virtual void setClientMaster( bool clientMaster );

        virtual bool isFaultTolerant() const;
        virtual void setFaultTolerant( bool faultTolerant );

        virtual bool isFailoverReconnect() const;
        virtual void setFailoverReconnect( bool failoverReconnect );

        /**
         * @return an answer of true to the isConnectionInfo() query.
         */
        virtual bool isConnectionInfo() const {
            return true;
        }

        virtual Pointer<Command> visit( activemq::state::CommandVisitor* visitor );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_CONNECTIONINFO_H_*/
