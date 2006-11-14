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

#include <activemq/connector/openwire/marshal/v2/XATransactionIdMarshaller.h>

#include <activemq/connector/openwire/commands/XATransactionId.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal::v2;

///////////////////////////////////////////////////////////////////////////////
DataStructure* XATransactionIdMarshaller::createObject() const {
    return new XATransactionId();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char XATransactionIdMarshaller::getDataStructureType() const {
    return XATransactionId::ID_XATRANSACTIONID;
}

///////////////////////////////////////////////////////////////////////////////
void XATransactionIdMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) throw( io::IOException ){
   TransactionIdMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

    XATransactionId* info =
        dynamic_cast<XATransactionId*>( dataStructure );
    info->setFormatId( dataIn->readInt() );
    info->setGlobalTransactionId( tightUnmarshalByteArray( dataIn, bs ) );
    info->setBranchQualifier( tightUnmarshalByteArray( dataIn, bs ) );
}

///////////////////////////////////////////////////////////////////////////////
int XATransactionIdMarshaller::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) throw( io::IOException ){

    XATransactionId* info =
        dynamic_cast<XATransactionId*>( dataStructure );

    int rc = TransactionIdMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
    bs->writeBoolean( info->getGlobalTransactionId().size() != 0 );
    rc += info->getGlobalTransactionId().size() == 0 ? 0 : info->getGlobalTransactionId().size() + 4;
    bs->writeBoolean( info->getBranchQualifier().size() != 0 );
    rc += info->getBranchQualifier().size() == 0 ? 0 : info->getBranchQualifier().size() + 4;

    return rc + 4;
}

///////////////////////////////////////////////////////////////////////////////
void XATransactionIdMarshaller::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) throw( io::IOException ){

    TransactionIdMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

    XATransactionId* info =
        dynamic_cast<XATransactionId*>( dataStructure );
    dataOut->write( info->getFormatId() );
    if( bs->readBoolean() ) {
        dataOut->write( info->getGlobalTransactionId().size() );
        dataOut->write( (const unsigned char*)&(info->getGlobalTransactionId()[0]), info->getGlobalTransactionId().size() );
    }
    if( bs->readBoolean() ) {
        dataOut->write( info->getBranchQualifier().size() );
        dataOut->write( (const unsigned char*)&(info->getBranchQualifier()[0]), info->getBranchQualifier().size() );
    }
}

///////////////////////////////////////////////////////////////////////////////
void XATransactionIdMarshaller::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) throw( io::IOException ){
    TransactionIdMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
    XATransactionId* info = 
        dynamic_cast<XATransactionId*>( dataStructure );
    info->setFormatId( dataIn->readInt() );
    info->setGlobalTransactionId( looseUnmarshalByteArray( dataIn ) );
    info->setBranchQualifier( looseUnmarshalByteArray( dataIn ) );
}

///////////////////////////////////////////////////////////////////////////////
void XATransactionIdMarshaller::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) throw( io::IOException ){
    XATransactionId* info =
        dynamic_cast<XATransactionId*>( dataStructure );
    TransactionIdMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

    dataOut->write( info->getFormatId() );
    dataOut->write( info->getGlobalTransactionId().size() != 0 );
    if( info->getGlobalTransactionId().size() != 0 ) {
        dataOut->write( info->getGlobalTransactionId().size() );
        dataOut->write( (const unsigned char*)&(info->getGlobalTransactionId()[0]), info->getGlobalTransactionId().size() );
    }
    dataOut->write( info->getBranchQualifier().size() != 0 );
    if( info->getBranchQualifier().size() != 0 ) {
        dataOut->write( info->getBranchQualifier().size() );
        dataOut->write( (const unsigned char*)&(info->getBranchQualifier()[0]), info->getBranchQualifier().size() );
    }
}

