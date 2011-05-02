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

#include "Handler.h"

#include <decaf/util/logging/Level.h>
#include <decaf/util/logging/ErrorManager.h>
#include <decaf/util/logging/Filter.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
Handler::Handler() : formatter(NULL), filter(NULL), errorManager(new ErrorManager()),
                     level(Level::ALL), prefix("Handler") {
}

////////////////////////////////////////////////////////////////////////////////
Handler::~Handler() {
    delete errorManager;
}

////////////////////////////////////////////////////////////////////////////////
void Handler::setFormatter( Formatter* formatter ) {

    if( formatter == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__, "The Formatter cannot be set to NULL." );
    }

    this->formatter = formatter;
}

////////////////////////////////////////////////////////////////////////////////
void Handler::setErrorManager( ErrorManager* errorManager ) {

    if( errorManager == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__, "Error Manager cannot be set to NULL." );
    }

    this->errorManager = errorManager;
}

////////////////////////////////////////////////////////////////////////////////
bool Handler::isLoggable( const LogRecord& record ) const {

    if( this->level.intValue() == Level::OFF.intValue() ) {
        return false;
    } else if( record.getLevel().intValue() >= this->level.intValue()) {
        return NULL == this->filter || this->filter->isLoggable( record );
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Handler::reportError( const std::string& message, decaf::lang::Exception* ex, int code ) {

    if( this->errorManager != NULL ) {
        this->errorManager->error( message, ex, code );
    }
}
