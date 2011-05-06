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
#include <cppunit/TestResult.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <iostream>

int main( int argc AMQCPP_UNUSED, char **argv AMQCPP_UNUSED ) {

    activemq::library::ActiveMQCPP::initializeLibrary();
    bool wasSuccessful = false;

    try {
        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry =
            CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );

        std::cout << "=====================================================\n";
        std::cout << "Starting the Benchmarks:" << std::endl;
        std::cout << "-----------------------------------------------------\n";

        wasSuccessful = runner.run( "", false );

        std::cout << "-----------------------------------------------------\n";
        std::cout << "Finished with the Benchmarks." << std::endl;
        std::cout << "=====================================================\n";

    } catch(...) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "- AN ERROR HAS OCCURED:                -" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }

    activemq::library::ActiveMQCPP::shutdownLibrary();

    return !wasSuccessful;
}
