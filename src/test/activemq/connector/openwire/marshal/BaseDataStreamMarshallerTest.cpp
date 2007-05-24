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
#include <activemq/connector/openwire/utils/BooleanStream.h>
#include <activemq/util/Properties.h>
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
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal;

////////////////////////////////////////////////////////////////////////////////
void BaseDataStreamMarshallerTest::testLooseMarshal()
{   
    SimpleDataStructureMarshaller* simpleMarshaller = new SimpleDataStructureMarshaller();
    ComplexDataStructureMarshaller* complexMarshaller = new ComplexDataStructureMarshaller();
    Properties props;    
    OpenWireFormat openWireFormat(props);
    openWireFormat.addMarshaller( simpleMarshaller );
    openWireFormat.addMarshaller( complexMarshaller );

    // Marshal the dataStructure to a byte array.
    ByteArrayOutputStream baos;
    DataOutputStream looseOut( &baos );
    looseOut.writeByte( dataStructure->getDataStructureType() );    
    complexMarshaller->looseMarshal( &openWireFormat, dataStructure, &looseOut );
    
    // Now read it back in and make sure it's all right.
    ByteArrayInputStream bais( baos.getByteArray(), baos.getByteArraySize() );
    DataInputStream looseIn( &bais );
    
    unsigned char dataType = looseIn.readByte();
    CPPUNIT_ASSERT( dataType == dataStructure->getDataStructureType() );

    ComplexDataStructure ds;
    complexMarshaller->looseUnmarshal( &openWireFormat, &ds, &looseIn );
    
    CPPUNIT_ASSERT( ds.boolValue == dataStructure->boolValue );
    CPPUNIT_ASSERT( ds.cachedChild != NULL );
    CPPUNIT_ASSERT( ds.cachedChild->boolValue == dataStructure->cachedChild->boolValue );
    CPPUNIT_ASSERT( ds.cachedChild->charValue == dataStructure->cachedChild->charValue );
    CPPUNIT_ASSERT( ds.cachedChild->shortValue == dataStructure->cachedChild->shortValue );
    CPPUNIT_ASSERT( ds.cachedChild->intValue == dataStructure->cachedChild->intValue );
    CPPUNIT_ASSERT( ds.cachedChild->floatValue == dataStructure->cachedChild->floatValue );
    CPPUNIT_ASSERT( ds.cachedChild->doubleValue == dataStructure->cachedChild->doubleValue );
    CPPUNIT_ASSERT( ds.cachedChild->stringValue == dataStructure->cachedChild->stringValue );
}

////////////////////////////////////////////////////////////////////////////////
void BaseDataStreamMarshallerTest::testTightMarshal()
{   
    SimpleDataStructureMarshaller* simpleMarshaller = new SimpleDataStructureMarshaller();
    ComplexDataStructureMarshaller* complexMarshaller = new ComplexDataStructureMarshaller();
    Properties props;    
    OpenWireFormat openWireFormat(props);
    openWireFormat.addMarshaller( simpleMarshaller );
    openWireFormat.addMarshaller( complexMarshaller );

    // Marshal the dataStructure to a byte array.
    ByteArrayOutputStream baos;
    DataOutputStream dataOut( &baos );
    
    // Phase 1 - count the size
    int size = 1;
    BooleanStream bs;
    size += complexMarshaller->tightMarshal1( &openWireFormat, dataStructure, &bs );
    size += bs.marshalledSize();
    
    // Phase 2 - marshal
    dataOut.writeByte( dataStructure->getDataStructureType() );
    bs.marshal( &dataOut );
    complexMarshaller->tightMarshal2( &openWireFormat, dataStructure, &dataOut, &bs );
    
    
    // Now read it back in and make sure it's all right.
    ByteArrayInputStream bais( baos.getByteArray(), baos.getByteArraySize() );
    DataInputStream dataIn( &bais );
    
    unsigned char dataType = dataIn.readByte();
    CPPUNIT_ASSERT( dataType == dataStructure->getDataStructureType() );

    ComplexDataStructure ds;
    bs.clear();
    bs.unmarshal( &dataIn );
    complexMarshaller->tightUnmarshal( &openWireFormat, &ds, &dataIn, &bs );
    
    CPPUNIT_ASSERT( ds.boolValue == dataStructure->boolValue );
    CPPUNIT_ASSERT( ds.cachedChild != NULL );
    CPPUNIT_ASSERT( ds.cachedChild->boolValue == dataStructure->cachedChild->boolValue );
    CPPUNIT_ASSERT( ds.cachedChild->charValue == dataStructure->cachedChild->charValue );
    CPPUNIT_ASSERT( ds.cachedChild->shortValue == dataStructure->cachedChild->shortValue );
    CPPUNIT_ASSERT( ds.cachedChild->intValue == dataStructure->cachedChild->intValue );
    CPPUNIT_ASSERT( ds.cachedChild->floatValue == dataStructure->cachedChild->floatValue );
    CPPUNIT_ASSERT( ds.cachedChild->doubleValue == dataStructure->cachedChild->doubleValue );
    printf("string1[%s,%d] string2[%s,%d]\n",ds.cachedChild->stringValue.c_str(), (int)ds.cachedChild->stringValue.size(), dataStructure->cachedChild->stringValue.c_str(), (int)dataStructure->cachedChild->stringValue.size() );
    CPPUNIT_ASSERT( ds.cachedChild->stringValue == dataStructure->cachedChild->stringValue );
}


