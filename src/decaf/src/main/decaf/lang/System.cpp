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
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/util/Date.h>
#include <decaf/util/StringTokenizer.h>
#include <apr.h>
#include <apr_errno.h>
#include <apr_env.h>

#ifdef APR_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <cstdlib>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::internal;
using namespace decaf::lang::exceptions;

#if !defined(_WIN32)
    extern char** environ;
#endif 

////////////////////////////////////////////////////////////////////////////////
AprPool System::aprPool;

////////////////////////////////////////////////////////////////////////////////
System::System() {
}

////////////////////////////////////////////////////////////////////////////////
void System::unsetenv( const std::string& name ) throw ( lang::Exception ) {

    apr_status_t result = APR_SUCCESS;

    // Clear the value, errors are thrown out as an exception
    result = apr_env_delete( name.c_str(), aprPool.getAprPool() );
    aprPool.cleanup();

    if( result != APR_SUCCESS ) {

        char buffer[256] = {0};

        throw NullPointerException(
            __FILE__, __LINE__,
            "System::getenv - ",
            apr_strerror( result, buffer, 255 ) );
    }
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

////////////////////////////////////////////////////////////////////////////////
void System::setenv( const std::string& name, const std::string& value )
    throw ( lang::Exception ) {

    apr_status_t result = APR_SUCCESS;

    // Write the value, errors are thrown out as an exception
    result = apr_env_set( name.c_str(), value.c_str(), aprPool.getAprPool() );
    aprPool.cleanup();

    if( result != APR_SUCCESS ) {

        char buffer[256] = {0};

        throw NullPointerException(
            __FILE__, __LINE__,
            "System::getenv - ",
            apr_strerror( result, buffer, 255 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
long long System::currentTimeMillis() {
    return Date::getCurrentTimeMilliseconds();
}

////////////////////////////////////////////////////////////////////////////////
Map<string, string> System::getenv() throw ( Exception ) {

    Map<string, string> values;
    StringTokenizer tokenizer( "" );
    string key = "";
    string value = "";
    int tokens = 0;
    char** env = getEnvArray();
    if( env == NULL ) {
        throw RuntimeException(
            __FILE__, __LINE__,
            "System::getenv - Failed to enumerate the environment." );
    }

    for( int i = 0; *(env + i); i++ ){
        tokenizer.reset( env[i], "=" );
        delete env[i];  // Clean them as we go

        tokens = tokenizer.countTokens();

        if( tokens == 1 ) {
            // special case, no value set, store empty string as value
            key = tokenizer.nextToken();
            value = string("");
        } else if( tokens > 2 ) {
            // special case: first equals delimits the key value, the rest are
            // part of the variable
            std::string envVal( environ[i] );
            int pos = envVal.find( "=" );
            key = envVal.substr( 0, pos );
            value = envVal.substr( pos + 1, string::npos );
        } else if( tokens == 0 ) {
            // Odd case, got a string with no equals sign.
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "System::getenv - Invalid env string. %s",
                environ[i] );
        } else {
            // Normal case.
            key = tokenizer.nextToken();
            value = tokenizer.nextToken();
        }

        // Store the env var
        values.setValue( key, value );
    }

    // cleanup
    delete [] env;

    return values;
}

#if defined(_WIN32)

#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
char** System::getEnvArray() {

    char** buffer = NULL;
    int count = 0;
    LPTSTR lpszVars;
    LPVOID lpvEnv;

    lpvEnv = GetEnvironmentStrings();
    if( NULL == lpvEnv ){
        return NULL;
    }

    lpszVars = (LPTSTR)lpvEnv;
    while( *lpszVars != NULL ) {
        count++;
        lpszVars += strlen(lpszVars)+1;
    }

    // allocate buffer first dimension
    buffer = new char*[count+1];
    buffer[count] = NULL;

    lpszVars = (LPTSTR)lpvEnv;
    int index = 0;
    while( *lpszVars != NULL ) {
        buffer[++index] = strdup( lpszVars );
        lpszVars += strlen(lpszVars)+1;
    }

    FreeEnvironmentStrings( (LPTCH)lpvEnv );
    return buffer;
}

#else


////////////////////////////////////////////////////////////////////////////////
char** System::getEnvArray() {

    char** buffer = NULL;
    int count = 0;

    for( int i = 0; *(environ + i); i++ ){
        count++;
    }

    // allocate buffer first dimension
    buffer = new char*[count+1];
    buffer[count] = NULL;

    for( int i = 0; *(environ + i); i++ ){
        buffer[i] = strdup( environ[i] );
    }

    return buffer;
}

#endif
