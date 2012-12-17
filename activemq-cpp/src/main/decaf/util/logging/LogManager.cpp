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
#include "LogManager.h"

#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/logging/PropertiesChangeListener.h>
#include <decaf/util/logging/Logger.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/Config.h>
#include <decaf/io/InputStream.h>

#include <string>
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace util{
namespace logging{

    class LogManagerInternals {
    public:

        StlMap<string, Logger*> loggers;

        LogManagerInternals() : loggers() {}

    };

}}}

////////////////////////////////////////////////////////////////////////////////
namespace {

    LogManager* theManager = NULL;
}

////////////////////////////////////////////////////////////////////////////////
LogManager::LogManager() : listeners(), properties(), internal(new LogManagerInternals()) {
}

////////////////////////////////////////////////////////////////////////////////
LogManager::~LogManager() {
    delete internal;
    // TODO - Delete all the loggers.
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::setProperties( const Properties& properties ) {
    // Copy the properties
    this->properties = properties;

    // Update the configuration of the loggers.
    // TODO
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::addPropertyChangeListener(
    PropertyChangeListener* listener )
{
    if( find(listeners.begin(), listeners.end(), listener) == listeners.end() ) {
        listeners.push_back( listener );
    }
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::removePropertyChangeListener(
    PropertyChangeListener* listener )
{
    listeners.remove( listener );
}

////////////////////////////////////////////////////////////////////////////////
bool LogManager::addLogger( Logger* logger DECAF_UNUSED ) {

    return false;
}

////////////////////////////////////////////////////////////////////////////////
Logger* LogManager::getLogger( const std::string& name DECAF_UNUSED ) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
int LogManager::getLoggerNames( const std::vector<std::string>& names  DECAF_UNUSED ) {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::string LogManager::getProperty( const std::string& name ) {
    return properties.getProperty( name );
}

////////////////////////////////////////////////////////////////////////////////
LogManager& LogManager::getLogManager() {

    if( theManager == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__, "The Logging Subsystem is not initialized." );
    }

    return *theManager;
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::initialize() {

    // Initialize the global instance.
    theManager = new LogManager;
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::shutdown() {

    // Destroy the global LogManager
    delete theManager;
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::readConfiguration() {
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::readConfiguration( decaf::io::InputStream* stream DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void LogManager::reset() {

}
