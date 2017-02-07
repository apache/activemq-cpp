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

#include "OpenWireFormatTest.h"

#include <decaf/util/Properties.h>
#include <activemq/wireformat/openwire/OpenWireFormatFactory.h>
#include <activemq/wireformat/openwire/OpenWireFormat.h>

#include <activemq/core/ActiveMQConnectionMetaData.h>

using namespace std;
using namespace activemq;
using namespace activemq::util;
using namespace activemq::core;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::util;
using namespace activemq::exceptions;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;

////////////////////////////////////////////////////////////////////////////////
void OpenWireFormatTest::testProviderInfoInWireFormat() {
    ActiveMQConnectionMetaData meta;

    OpenWireFormatFactory factory;
    Properties properties;

    Pointer<OpenWireFormat> myWireFormat =
            factory.createWireFormat(properties).dynamicCast<OpenWireFormat>();

    CPPUNIT_ASSERT_EQUAL(meta.getCMSProviderName(),
            myWireFormat->getPreferedWireFormatInfo()->getProperties().getString("ProviderName"));
    CPPUNIT_ASSERT_EQUAL(meta.getProviderVersion(),
            myWireFormat->getPreferedWireFormatInfo()->getProperties().getString("ProviderVersion"));
    CPPUNIT_ASSERT(!myWireFormat->getPreferedWireFormatInfo()->getProperties().getString("PlatformDetails").empty());
}
