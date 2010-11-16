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

#ifndef _ACTIVEMQ_COMMANDS_XATRANSACTIONID_H_
#define _ACTIVEMQ_COMMANDS_XATRANSACTIONID_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/TransactionId.h>
#include <activemq/util/Config.h>
#include <cms/Xid.h>
#include <decaf/lang/Comparable.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq{
namespace commands{

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for XATransactionId
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API XATransactionId : public TransactionId, public cms::Xid, public decaf::lang::Comparable<XATransactionId> {
    protected:

        int formatId;
        std::vector<unsigned char> globalTransactionId;
        std::vector<unsigned char> branchQualifier;

    public:

        const static unsigned char ID_XATRANSACTIONID = 112;

        typedef decaf::lang::PointerComparator<XATransactionId> COMPARATOR;

    public:

        XATransactionId();

        XATransactionId( const XATransactionId& other );

        XATransactionId( const cms::Xid* xid );

        virtual ~XATransactionId();

        virtual unsigned char getDataStructureType() const;

        virtual XATransactionId* cloneDataStructure() const;

        virtual void copyDataStructure( const DataStructure* src );

        virtual std::string toString() const;

        virtual bool equals( const DataStructure* value ) const;

        virtual bool isXATransactionId() const {
            return true;
        }

    public:  // Xid interface implementation.

        virtual Xid* clone() const;

        virtual bool equals( const Xid* other ) const;

        virtual int getBranchQualifier( unsigned char* buffer, int size ) const;

        virtual int getGlobalTransactionId( unsigned char* buffer, int size ) const;

        virtual int getFormatId() const;
        virtual void setFormatId( int formatId );

        virtual const std::vector<unsigned char>& getGlobalTransactionId() const;
        virtual std::vector<unsigned char>& getGlobalTransactionId();
        virtual void setGlobalTransactionId( const std::vector<unsigned char>& globalTransactionId );

        virtual const std::vector<unsigned char>& getBranchQualifier() const;
        virtual std::vector<unsigned char>& getBranchQualifier();
        virtual void setBranchQualifier( const std::vector<unsigned char>& branchQualifier );

        virtual int compareTo( const XATransactionId& value ) const;

        virtual bool equals( const XATransactionId& value ) const;

        virtual bool operator==( const XATransactionId& value ) const;

        virtual bool operator<( const XATransactionId& value ) const;

        XATransactionId& operator= ( const XATransactionId& other );

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_XATRANSACTIONID_H_*/
