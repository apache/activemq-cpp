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
#include "Logger.h"

#include <decaf/util/logging/Handler.h>
#include <decaf/util/Config.h>
#include <algorithm>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
Logger::Logger( const std::string& name DECAF_UNUSED,
                Logger* parent DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
Logger::~Logger() {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::addHandler( Handler* handler ) throw ( IllegalArgumentException ) {

    if( handler == NULL )
    {
        IllegalArgumentException(
            __FILE__, __LINE__,
            "Logger::addHandler - HAndler cannot be null");
    }

    if( find( handlers.begin(), handlers.end(), handler) != handlers.end() )
    {
        handlers.push_back( handler );
    }
}

////////////////////////////////////////////////////////////////////////////////
void Logger::removeHandler( Handler* handler ) {

    list<Handler*>::iterator itr =
        find( handlers.begin(), handlers.end(), handler );

    if( itr != handlers.end() ) {

        delete *itr;
        handlers.erase(itr);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Logger::setFilter( Filter* filter DECAF_UNUSED ){
}

////////////////////////////////////////////////////////////////////////////////
bool Logger::isLoggable( Level level DECAF_UNUSED ) const{
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Logger::entry( const std::string& blockName DECAF_UNUSED,
                    const std::string& file DECAF_UNUSED,
                    const int line DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::exit(const std::string& blockName DECAF_UNUSED,
                  const std::string& file DECAF_UNUSED,
                  const int line DECAF_UNUSED) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::debug( const std::string& file DECAF_UNUSED,
                    const int line DECAF_UNUSED,
                    const std::string fnctionName DECAF_UNUSED,
                    const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::info( const std::string& file DECAF_UNUSED,
                   const int line DECAF_UNUSED,
                   const std::string fnctionName DECAF_UNUSED,
                   const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::error( const std::string& file DECAF_UNUSED,
                    const int line DECAF_UNUSED,
                    const std::string fnctionName DECAF_UNUSED,
                    const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::warn( const std::string& file DECAF_UNUSED,
                   const int line DECAF_UNUSED,
                   const std::string fnctionName DECAF_UNUSED,
                   const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::fatal( const std::string& file DECAF_UNUSED,
                    const int line DECAF_UNUSED,
                    const std::string fnctionName DECAF_UNUSED,
                    const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::log( Level level DECAF_UNUSED,
                  const std::string& message DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::log( Level level DECAF_UNUSED,
                  const std::string& file DECAF_UNUSED,
                  const int line DECAF_UNUSED,
                  const std::string& message DECAF_UNUSED,
                  lang::Exception& ex DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::log( Level level DECAF_UNUSED,
                  const std::string& file DECAF_UNUSED,
                  const int line DECAF_UNUSED,
                  const std::string& message DECAF_UNUSED, ... ) {
}

////////////////////////////////////////////////////////////////////////////////
void Logger::log( LogRecord& record DECAF_UNUSED ) {
}

////////////////////////////////////////////////////////////////////////////////
Logger* Logger::getLogger( const std::string& name DECAF_UNUSED ) {
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
Logger* Logger::getAnonymousLogger() {
    return NULL;
}
