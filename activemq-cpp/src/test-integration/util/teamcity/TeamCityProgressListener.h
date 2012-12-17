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

#ifndef _TEST_UTIL_TEAMCITY_TEAMCITYPROGRESSLISTENER_H_
#define _TEST_UTIL_TEAMCITY_TEAMCITYPROGRESSLISTENER_H_

#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>

#include <string>

namespace test {
namespace util {
namespace teamcity {

    /**
     * CPPUnit Derived Test Listener that can output the test life cycle messages in
     * a manner that can be processed by the TeamCity Continuous integration tool.
     */
    class TeamCityProgressListener: public CPPUNIT_NS::TestListener {
    public:

        TeamCityProgressListener() {}
        ~TeamCityProgressListener() {}

        void startTest( CPPUNIT_NS::Test *test );
        void addFailure( const CPPUNIT_NS::TestFailure &failure );
        void endTest( CPPUNIT_NS::Test *test );
        void startSuite( CPPUNIT_NS::Test *test );
        void endSuite( CPPUNIT_NS::Test *test );

    private:

        // Prevents the use of the copy constructor.
        TeamCityProgressListener( const TeamCityProgressListener& );

        // Prevents the use of the copy operator.
        void operator =( const TeamCityProgressListener& );

    protected:

        /**
         * Escapes the control characters in a string.
         *
         * @param value - String to escape
         * @return the new escaped string.
         */
        virtual std::string escape( const std::string& value ) const;

        virtual void writeOpen( const std::string& name );
        virtual void writeProperty( const std::string& name, const std::string& value );
        virtual void writeClose();

    };

}}}

#endif /* _TEST_UTIL_TEAMCITY_TEAMCITYPROGRESSLISTENER_H_ */
