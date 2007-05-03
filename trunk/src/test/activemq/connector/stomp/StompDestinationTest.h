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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPDESTINATIONTEST_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPDESTINATIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/connector/stomp/StompDestination.h>
#include <cms/Topic.h>

namespace activemq{
namespace connector{
namespace stomp{

    class StompDestinationTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( StompDestinationTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();

    public:

        class MyDestination : public StompDestination< cms::Topic >
        {
        public:

            MyDestination( const cms::Destination* dest ) :
                StompDestination< cms::Topic >( dest ) {}

            MyDestination( const std::string& name )
                : StompDestination< cms::Topic >( name, cms::Destination::TOPIC )
            {}

            virtual ~MyDestination() {}

            /**
             * Converts the Destination Name into a String
             * @return string name
             */
            virtual std::string toString(void) const {
                return getName();
            }

            /**
             * Converts the Destination to a String value representing the
             * Provider specific name fot this destination, which is not
             * necessarily equal to the User Supplied name of the Destination
             * @return Provider specific Name
             */
            virtual std::string toProviderString(void) const {
                return getName();
            }

            /**
             * Creates a new instance of this destination type that is a
             * copy of this one, and returns it.
             * @returns cloned copy of this object
             */
            virtual cms::Destination* clone(void) const {
                return new MyDestination( this );
            }

              /**
             * Gets the name of this topic.
             * @return The topic name.
             */
            virtual std::string getTopicName(void)
                const throw( cms::CMSException ) { return getName(); }

        protected:

            /**
             * Retrieves the proper Stomp Prefix for the specified type
             * of Destination
             * @return string prefix
             */
            virtual std::string getPrefix() const {
                return "topic/";
            }

        };

        StompDestinationTest() {}
        virtual ~StompDestinationTest() {}

        virtual void test();

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPDESTINATIONTEST_H_*/
