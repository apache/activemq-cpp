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
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Runtime.h>
#include <decaf/lang/Integer.h>
#include <iostream>

int main( int argc, char **argv ) {

    activemq::library::ActiveMQCPP::initializeLibrary();

    bool wasSuccessful = false;
    int iterations = 1;

    if( argc > 1 ) {
        try {
            iterations = decaf::lang::Integer::parseInt( argv[1] );
        } catch( decaf::lang::exceptions::NumberFormatException& ex ) {
            iterations = 1;
        }
    }

    for( int i = 0; i < iterations; ++i ) {

        std::ofstream outputFile( "activemq-test.xml"  );

        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );

        // Shows a message as each test starts
        CppUnit::BriefTestProgressListener listener;
        runner.eventManager().addListener( &listener );

        // Specify XML output and inform the test runner of this format.  The TestRunner
        // will delete the passed XmlOutputter for us.
        runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(), outputFile ) );

        wasSuccessful = runner.run( "", false );

        outputFile.close();
    }

    activemq::library::ActiveMQCPP::shutdownLibrary();

    return !wasSuccessful;
}

