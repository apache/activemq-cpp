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

#include "HexTableTest.h"

#include <activemq/connector/openwire/utils/HexTable.h>

using namespace activemq;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;

////////////////////////////////////////////////////////////////////////////////
void HexTableTest::test(){
    
    HexTable table;
    
    CPPUNIT_ASSERT( table[0] == "00" );
    CPPUNIT_ASSERT( table[32] == "20" );
    CPPUNIT_ASSERT( table[55] == "37" );
    CPPUNIT_ASSERT( table[96] == "60" );
    CPPUNIT_ASSERT( table[156] == "9c" );
    CPPUNIT_ASSERT( table[232] == "e8" );
    CPPUNIT_ASSERT( table[255] == "ff" );

    CPPUNIT_ASSERT( table.size() == 256 );
    
    try{
        std::string test = table[277];
        CPPUNIT_ASSERT( false );
    }
    catch(...) {}
}
