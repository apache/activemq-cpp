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

#include "System.h"

#include <decaf/lang/exceptions/NullPointerException.h>
#include <apr_errno.h>
#include <apr_env.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
AprPool System::aprPool;

////////////////////////////////////////////////////////////////////////////////
System::System() {
}

////////////////////////////////////////////////////////////////////////////////
std::string System::getenv( const std::string& name ) throw ( Exception ) {

    char* value = NULL;
    apr_status_t result = APR_SUCCESS;

    // Read the value, errors are thrown out as an exception
    result = apr_env_get( &value, name.c_str(), aprPool.getAprPool() );

    if( result != APR_SUCCESS ) {

        char buffer[256] = {0};

        throw NullPointerException(
            __FILE__, __LINE__,
            "System::getenv - ",
            apr_strerror( result, buffer, 255 ) );
    }

    // Copy and cleanup
    if( value == NULL ) {
        return "";
    }

    std::string envVal( value );
    aprPool.cleanup();

    return value;
}
