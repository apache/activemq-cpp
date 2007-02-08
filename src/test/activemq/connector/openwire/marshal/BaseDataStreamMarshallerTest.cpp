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

#include "BaseDataStreamMarshallerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::connector::openwire::marshal::BaseDataStreamMarshallerTest );

#include <activemq/util/PrimitiveMap.h>
#include <activemq/connector/openwire/marshal/BaseDataStreamMarshaller.h>
#include <activemq/connector/openwire/OpenWireFormatFactory.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/io/ByteArrayOutputStream.h>
#include <activemq/io/DataOutputStream.h>
#include <activemq/io/ByteArrayInputStream.h>
#include <activemq/io/DataInputStream.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::io;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
void BaseDataStreamMarshallerTest::testLooseMarshal()
{   
    SimpleProperties props;
    OpenWireFormat openWireFormat(props);
    SimpleDataStructureMarshaller dsm;

    // Marshal the dataStructure to a byte array.
    ByteArrayOutputStream baos;
    DataOutputStream looseOut( &baos );
    looseOut.writeByte( dataStructure.getDataStructureType() );    
    dsm.looseMarshal( &openWireFormat, &dataStructure, &looseOut );
    
    // Now read it back in and make sure it's all right.
    SimpleDataStructure ds;
    ByteArrayInputStream bais( baos.getByteArray(), baos.getByteArraySize() );
    DataInputStream looseIn( &bais );
    
    unsigned char dataType = looseIn.readByte();
    CPPUNIT_ASSERT( dataType == dataStructure.getDataStructureType() );

    dsm.looseUnmarshal( &openWireFormat, &ds, &looseIn );
    
    CPPUNIT_ASSERT( ds.boolValue == dataStructure.boolValue );
    CPPUNIT_ASSERT( ds.charValue == dataStructure.charValue );
    CPPUNIT_ASSERT( ds.shortValue == dataStructure.shortValue );
    CPPUNIT_ASSERT( ds.intValue == dataStructure.intValue );
    CPPUNIT_ASSERT( ds.floatValue == dataStructure.floatValue );
    CPPUNIT_ASSERT( ds.doubleValue == dataStructure.doubleValue );
    CPPUNIT_ASSERT( ds.stringValue == dataStructure.stringValue );
}
