/**
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

#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/connector/openwire/commands/MessageId.h>
#include <activemq/connector/openwire/commands/ProducerId.h>
#include <activemq/connector/openwire/commands/TransactionId.h>
#include <activemq/connector/openwire/commands/LocalTransactionId.h>
#include <activemq/connector/openwire/commands/XATransactionId.h>
#include <activemq/connector/openwire/utils/HexTable.h>
#include <activemq/util/Long.h>
#include <activemq/util/Integer.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
BaseDataStreamMarshaller::BaseDataStreamMarshaller() {
}

////////////////////////////////////////////////////////////////////////////////
BaseDataStreamMarshaller::~BaseDataStreamMarshaller() {
}

////////////////////////////////////////////////////////////////////////////////
std::string BaseDataStreamMarshaller::toString( commands::MessageId* id )
{
    if( id == NULL ) return "";
    
    return toString( id->getProducerId() ) + ":" + 
           Long::toString( id->getProducerSequenceId() );
}

////////////////////////////////////////////////////////////////////////////////
std::string BaseDataStreamMarshaller::toString( commands::ProducerId* id )
{
    return id->getConnectionId() + ":" + 
           Long::toString( id->getSessionId() ) + ":" + 
           Long::toString( id->getValue() );
}

////////////////////////////////////////////////////////////////////////////////
std::string BaseDataStreamMarshaller::toString( commands::TransactionId* txnId )
{
    LocalTransactionId* ltxnId = dynamic_cast<LocalTransactionId*>( txnId );
    XATransactionId* xaTxnId = dynamic_cast<XATransactionId*>( txnId );
        
    if( ltxnId != NULL )
    {
        return Long::toString( ltxnId->getValue() );
    }
    else if( xaTxnId != NULL )
    {
        return string("XID:") + Integer::toString( xaTxnId->getFormatId() ) + ":" + 
               toHexFromBytes( xaTxnId->getGlobalTransactionId() ) + ":" + 
               toHexFromBytes( xaTxnId->getBranchQualifier() );        
    }

    return "";
}

////////////////////////////////////////////////////////////////////////////////
std::string BaseDataStreamMarshaller::toHexFromBytes( 
    const std::vector<unsigned char>& data )
{
    std::string buffer = "";
    
    for( int i = 0; i < data.size(); i++ )
    {
        buffer.append( hexTable[ data[i] ] );
    }
    
    return buffer;
}
