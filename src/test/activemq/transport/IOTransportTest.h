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

#ifndef ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_
#define ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace transport{

    class IOTransportTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( IOTransportTest );
        CPPUNIT_TEST( testStartClose );
        CPPUNIT_TEST( testStressTransportStartClose );
        CPPUNIT_TEST( testRead );
        CPPUNIT_TEST( testWrite );
        CPPUNIT_TEST( testException );
        CPPUNIT_TEST( testNarrow );
        CPPUNIT_TEST_SUITE_END();

    public:

        virtual ~IOTransportTest(){}

        void testException();
        void testWrite();
        void testRead();
        void testStartClose();
        void testStressTransportStartClose();
        void testNarrow();

    };

}}

#endif /*ACTIVEMQ_COMMANDS_IOTRANSPORTTEST_H_*/
