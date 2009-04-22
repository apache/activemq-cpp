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

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/Outputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <util/teamcity/TeamCityProgressListener.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Runtime.h>
#include <decaf/lang/Integer.h>
#include <iostream>
#include <memory>

int main( int argc, char **argv ) {

    bool wasSuccessful = false;
    int iterations = 1;
    std::ofstream outputFile;
    bool useXMLOutputter = false;
    std::auto_ptr<CppUnit::TestListener> listener( new CppUnit::BriefTestProgressListener );

    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i ) {
            const std::string arg( argv[i] );
            if( arg == "-runs" ) {
                if( ( i + 1 ) >= argc ) {
                    std::cout << "-runs requires a value for the iteration count" << std::endl;
                    return -1;
                }
                try {
                    iterations = decaf::lang::Integer::parseInt( argv[++i] );
                } catch( decaf::lang::exceptions::NumberFormatException& ex ) {
                    std::cout << "Invalid iteration count specified on command line: "
                              << argv[i] << std::endl;
                    return -1;
                }
            } else if( arg == "-teamcity" ) {
                listener.reset( new test::util::teamcity::TeamCityProgressListener() );
            } else if( arg == "-quiet" ) {
                listener.reset( NULL );
            } else if( arg == "-xml" ) {
                if( ( i + 1 ) >= argc ) {
                    std::cout << "-xml requires a filename to be specified" << std::endl;
                    return -1;
                }

                std::ofstream outputFile( argv[++i] );
                useXMLOutputter = true;
            }
        }
    }

    for( int i = 0; i < iterations; ++i ) {

        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );

        // Shows a message as each test starts
        if( listener.get() != NULL ) {
            runner.eventManager().addListener( listener.get() );
        }

        // Specify XML output and inform the test runner of this format.  The TestRunner
        // will delete the passed XmlOutputter for us.
        if( useXMLOutputter ) {
            runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(), outputFile ) );
        }

        wasSuccessful = runner.run( "", false );

        if( useXMLOutputter ) {
            outputFile.close();
        }
    }

    return !wasSuccessful;
}
