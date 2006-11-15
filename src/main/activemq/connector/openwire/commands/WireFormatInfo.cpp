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

#include <activemq/connector/openwire/commands/WireFormatInfo.h>
#include <activemq/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::WireFormatInfo()
{
}

////////////////////////////////////////////////////////////////////////////////
WireFormatInfo::~WireFormatInfo()
{
}

////////////////////////////////////////////////////////////////////////////////
DataStructure* WireFormatInfo::cloneDataStructure() const {
    WireFormatInfo* wireFormatInfo = new WireFormatInfo();

    // Copy the data from the base class or classes
    wireFormatInfo->copyDataStructure( this );

    return wireFormatInfo;
}

////////////////////////////////////////////////////////////////////////////////
void WireFormatInfo::copyDataStructure( const DataStructure* src ) {

    // Copy the data of the base class or classes
    BaseDataStructure::copyDataStructure( src );

    const WireFormatInfo* srcPtr = dynamic_cast<const WireFormatInfo*>( src );

    if( srcPtr == NULL || src == NULL ) {
    
        throw exceptions::NullPointerException(
            __FILE__, __LINE__,
            "WireFormatInfo::copyDataStructure - src is NULL or invalid" );
    }
}

////////////////////////////////////////////////////////////////////////////////
unsigned char WireFormatInfo::getDataStructureType() const {
    return WireFormatInfo::ID_WIREFORMATINFO; 
}
