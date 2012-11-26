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

#include "XATransactionIdTest.h"

#include <memory>
#include <activemq/commands/XATransactionId.h>

#include <cms/Xid.h>
#include <cms/XAException.h>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class DummyXid : public cms::Xid {
    private:

        std::vector<unsigned char> branchQualifier;
        std::vector<unsigned char> globalTransactionId;

    public:

        DummyXid() : branchQualifier(), globalTransactionId() {

            for(int i = 0; i < Xid::MAXBQUALSIZE; ++i ) {
                this->branchQualifier.push_back( (unsigned char)i );
            }

            for(int i = 0; i < Xid::MAXGTRIDSIZE; ++i ) {
                this->globalTransactionId.push_back( (unsigned char)i );
            }
        }

        virtual ~DummyXid() {
        }

        virtual Xid* clone() const {
            return new DummyXid();
        }

        virtual bool equals( const Xid* other ) const {

            if( (void*)this == other ) {
                return true;
            }

            if( other == NULL ) {
                return false;
            }

            if( this->getFormatId() != other->getFormatId() ) {
                return false;
            }

            std::vector<unsigned char> otherBQual( Xid::MAXBQUALSIZE );
            std::vector<unsigned char> otherGblTx( Xid::MAXGTRIDSIZE );

            other->getBranchQualifier( &otherBQual[0], Xid::MAXBQUALSIZE );
            other->getGlobalTransactionId( &otherGblTx[0], Xid::MAXGTRIDSIZE );

            if( this->branchQualifier != otherBQual ) {
                return false;
            }

            if( this->globalTransactionId != otherGblTx ) {
                return false;
            }

            return true;
        }

        virtual int getBranchQualifier( unsigned char* buffer, int size ) const {

            if( size < 0 ) {
                throw XAException("Specified Buffer Size was negative.");
            }

            if( buffer == 0 ) {
                throw XAException("The Buffer provided was null.");
            }

            if( size < Xid::MAXBQUALSIZE ) {
                return -1;
            }

            for( int i = 0; i < Xid::MAXBQUALSIZE; ++i ) {
                buffer[i] = this->branchQualifier[i];
            }

            return Xid::MAXBQUALSIZE;
        }

        virtual int getFormatId() const {
            return 42;
        }

        virtual int getGlobalTransactionId( unsigned char* buffer, int size ) const {
            if( size < 0 ) {
                throw XAException("Specified Buffer Size was negative.");
            }

            if( buffer == 0 ) {
                throw XAException("The Buffer provided was null.");
            }

            if( size < Xid::MAXGTRIDSIZE ) {
                return -1;
            }

            for( int i = 0; i < Xid::MAXGTRIDSIZE; ++i ) {
                buffer[i] = this->globalTransactionId[i];
            }

            return Xid::MAXGTRIDSIZE;
        }

    };
}

////////////////////////////////////////////////////////////////////////////////
XATransactionIdTest::XATransactionIdTest() {
}

////////////////////////////////////////////////////////////////////////////////
XATransactionIdTest::~XATransactionIdTest() {
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testConstructor() {

    XATransactionId id;

    CPPUNIT_ASSERT_EQUAL( 0, id.getFormatId() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getBranchQualifier().size() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getGlobalTransactionId().size() );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testConstructor2() {

    DummyXid myXid;
    XATransactionId id( &myXid );

    CPPUNIT_ASSERT_EQUAL( 42, id.getFormatId() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXBQUALSIZE, (int)id.getBranchQualifier().size() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXGTRIDSIZE, (int)id.getGlobalTransactionId().size() );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testEquals() {

    DummyXid myXid;
    XATransactionId id( &myXid );

    CPPUNIT_ASSERT_EQUAL( 42, id.getFormatId() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXBQUALSIZE, (int)id.getBranchQualifier().size() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXGTRIDSIZE, (int)id.getGlobalTransactionId().size() );

    CPPUNIT_ASSERT( id.equals( &myXid ) );
    CPPUNIT_ASSERT( myXid.equals( &id ) );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testClone() {

    DummyXid myXid;
    XATransactionId id( &myXid );
    std::auto_ptr<cms::Xid> cloned( id.clone() );

    CPPUNIT_ASSERT_EQUAL( 42, id.getFormatId() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXBQUALSIZE, (int)id.getBranchQualifier().size() );
    CPPUNIT_ASSERT_EQUAL( Xid::MAXGTRIDSIZE, (int)id.getGlobalTransactionId().size() );

    CPPUNIT_ASSERT_EQUAL( 42, cloned->getFormatId() );

    CPPUNIT_ASSERT( myXid.equals( cloned.get() ) );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testGetFormatId() {

    XATransactionId id;

    CPPUNIT_ASSERT_EQUAL( 0, id.getFormatId() );

    id.setFormatId( 42 );

    CPPUNIT_ASSERT_EQUAL( 42, id.getFormatId() );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testGetBranchQualifier() {

    XATransactionId id;

    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getBranchQualifier().size() );

    std::vector<unsigned char> bqual;
    for( int i = 0; i < cms::Xid::MAXBQUALSIZE; ++i ) {
        bqual.push_back( (unsigned char)(i+1) );
    }
    id.setBranchQualifier( bqual );

    CPPUNIT_ASSERT( bqual == id.getBranchQualifier() );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testGetGlobalTransactionId() {

    XATransactionId id;

    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getGlobalTransactionId().size() );

    std::vector<unsigned char> gtx;
    for( int i = 0; i < cms::Xid::MAXGTRIDSIZE; ++i ) {
        gtx.push_back( (unsigned char)(i+1) );
    }
    id.setGlobalTransactionId( gtx );

    CPPUNIT_ASSERT( gtx == id.getGlobalTransactionId() );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testGetBranchQualifier1() {

    XATransactionId id;
    std::vector<unsigned char> buffer( Xid::MAXBQUALSIZE );

    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getBranchQualifier().size() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an XAException",
        id.getBranchQualifier( NULL, 1 ),
        XAException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an XAException",
        id.getBranchQualifier( &buffer[0], -1 ),
        XAException );

    std::vector<unsigned char> gtx;
    for( int i = 0; i < cms::Xid::MAXBQUALSIZE; ++i ) {
        gtx.push_back( (unsigned char)(i+1) );
    }
    id.setBranchQualifier( gtx );

    CPPUNIT_ASSERT( id.getBranchQualifier( &buffer[0], 1 ) == -1 );
    CPPUNIT_ASSERT( id.getBranchQualifier( &buffer[0], Xid::MAXBQUALSIZE ) == Xid::MAXBQUALSIZE );
    CPPUNIT_ASSERT( gtx == buffer );
}

////////////////////////////////////////////////////////////////////////////////
void XATransactionIdTest::testGetGlobalTransactionId1() {

    XATransactionId id;
    std::vector<unsigned char> buffer( Xid::MAXGTRIDSIZE );

    CPPUNIT_ASSERT_EQUAL( 0, (int)id.getGlobalTransactionId().size() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an XAException",
        id.getGlobalTransactionId( NULL, 1 ),
        XAException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an XAException",
        id.getGlobalTransactionId( &buffer[0], -1 ),
        XAException );

    std::vector<unsigned char> gtx;
    for( int i = 0; i < cms::Xid::MAXGTRIDSIZE; ++i ) {
        gtx.push_back( (unsigned char)(i+1) );
    }
    id.setGlobalTransactionId( gtx );

    CPPUNIT_ASSERT( id.getGlobalTransactionId( &buffer[0], 1 ) == -1 );
    CPPUNIT_ASSERT( id.getGlobalTransactionId( &buffer[0], Xid::MAXGTRIDSIZE ) == Xid::MAXGTRIDSIZE );
    CPPUNIT_ASSERT( gtx == buffer );
}
