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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATIONTEST_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/openwire/commands/ActiveMQDestination.h>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQDestinationTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( ActiveMQDestinationTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testOptions );
        CPPUNIT_TEST_SUITE_END();

        class MyDestination : public ActiveMQDestination {
        public:

            /**
             * Returns the Type of Destination that this object represents
             * @returns int type qualifier.
             */
            virtual cms::Destination::DestinationType getDestinationType() const {
                return cms::Destination::TOPIC;
            }

            /**
             * Clone this object and return a new instance that the
             * caller now owns, this will be an exact copy of this one
             * @returns new copy of this object.
             */
            virtual MyDestination* cloneDataStructure() const {
                MyDestination* message = new MyDestination();
                message->copyDataStructure( this );
                return message;
            }

            /**
             * Copy the contents of the passed object into this objects
             * members, overwriting any existing data.
             * @return src - Source Object
             */
            virtual void copyDataStructure( const DataStructure* src ) {
                ActiveMQDestination::copyDataStructure( src );
            }
        };

        class MyTempDestination : public MyDestination {
        public:

            /**
             * Returns the Type of Destination that this object represents
             * @returns int type qualifier.
             */
            virtual cms::Destination::DestinationType getDestinationType() const {
                return cms::Destination::TEMPORARY_TOPIC;
            }

        };

    public:

        ActiveMQDestinationTest() {}
        virtual ~ActiveMQDestinationTest() {}

        virtual void test();
        virtual void testOptions();

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQDESTINATIONTEST_H_*/
