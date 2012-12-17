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

#include <iostream>
#include <sstream>
#include "cppunit/Test.h"
#include "cppunit/Exception.h"

#include <util/teamcity/TeamCityProgressListener.h>

using namespace CPPUNIT_NS;
using namespace std;
using namespace test;
using namespace test::util;
using namespace test::util::teamcity;

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::startTest( Test* test ) {
    writeOpen( "testStarted" );
    writeProperty( "name", test->getName() );
    writeClose();
}

////////////////////////////////////////////////////////////////////////////////
static string sourceLine2string( const SourceLine& sline ) {

    std::stringstream ss;
    ss << sline.fileName() << ":" << sline.lineNumber();
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::addFailure( const TestFailure& failure ) {

    const Exception *e = failure.thrownException();

    string details = e->message().details();

    if( e->sourceLine().isValid() ) {
        details.append( " at " );
        details.append( sourceLine2string( e->sourceLine() ) );
        details.append( "\n" );
    }

    writeOpen( "testFailed" );
    writeProperty( "name", failure.failedTest()->getName() );
    writeProperty( "message", e->message().shortDescription() );
    writeProperty( "details", details );
    writeClose();
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::endTest( Test* test ) {
    writeOpen( "testFinished" );
    writeProperty("name", test->getName() );
    writeClose();
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::startSuite( Test* test ) {
    writeOpen( "testSuiteStarted" );
    writeProperty( "name", test->getName() );
    writeClose();
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::endSuite( Test* test ) {
    writeOpen( "testSuiteFinished" );
    writeProperty( "name", test->getName() );
    writeClose();
}

////////////////////////////////////////////////////////////////////////////////
std::string TeamCityProgressListener::escape( const std::string& value ) const {

    std::string result;

    for( std::size_t i = 0; i < value.length(); i++ ) {

        char c = value[i];

        switch(c) {
            case '\n':
                result.append( "|n" );
                break;
            case '\r':
                result.append( "|r" );
                break;
            case '\'':
                result.append( "|'" );
                break;
            case '|':
                result.append( "||" );
                break;
            case ']':
                result.append( "|]" );
                break;
            default:
                result.append( &c, 1 );
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::writeOpen( const std::string& name ) {
    std::cout << std::endl << "##teamcity[" << name;
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::writeClose() {
    std::cout << "]" << std::endl;
    std::cout.flush();
}

////////////////////////////////////////////////////////////////////////////////
void TeamCityProgressListener::writeProperty( const std::string& name,
                                              const std::string& value ) {

    std::cout << " " << name << "='" << escape( value ) << "'";
}
