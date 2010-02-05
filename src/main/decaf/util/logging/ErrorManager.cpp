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

#include "ErrorManager.h"

#include <iostream>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
const int ErrorManager::GENERIC_FAILURE = 0;
const int ErrorManager::WRITE_FAILURE = 1;
const int ErrorManager::FLUSH_FAILURE = 2;
const int ErrorManager::CLOSE_FAILURE = 3;
const int ErrorManager::OPEN_FAILURE = 4;
const int ErrorManager::FORMAT_FAILURE = 5;

////////////////////////////////////////////////////////////////////////////////
namespace {

    std::string FAILURE_STRINGS[] = {
       "GENERIC_FAILURE",
       "WRITE_FAILURE",
       "FLUSH_FAILURE",
       "CLOSE_FAILURE",
       "OPEN_FAILURE",
       "FORMAT_FAILURE"
    };
}

////////////////////////////////////////////////////////////////////////////////
ErrorManager::ErrorManager() : wasCalled( false ) {
}

////////////////////////////////////////////////////////////////////////////////
ErrorManager::~ErrorManager() {
}

////////////////////////////////////////////////////////////////////////////////
void ErrorManager::error( const std::string& message, decaf::lang::Exception* ex, int code ) {

    if( !wasCalled.compareAndSet( false, true ) ) {
        return;
    }

    std::cerr << "ErrorManager: " << FAILURE_STRINGS[code] << std::endl;

    if( message != "" ) {
        std::cerr << "ErrorManager: message = " << message << std::endl;
    }

    if( ex != NULL ) {
        std::cerr << "ErrorManager: Exception Message = " << ex->getMessage() << std::endl;
    }
}
