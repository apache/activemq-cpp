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

#if APR_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <cstdlib>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::internal;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
System::System() {
}

////////////////////////////////////////////////////////////////////////////////
AprPool& System::getAprPool() {
    static AprPool aprPool;
    return aprPool;
}

////////////////////////////////////////////////////////////////////////////////
void System::unsetenv( const std::string& name ) throw ( lang::Exception ) {

    apr_status_t result = APR_SUCCESS;

    // Clear the value, errors are thrown out as an exception
    result = apr_env_delete( name.c_str(), getAprPool().getAprPool() );
    getAprPool().cleanup();

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
    result = apr_env_get( &value, name.c_str(), getAprPool().getAprPool() );

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
    getAprPool().cleanup();

    return value;
}

////////////////////////////////////////////////////////////////////////////////
void System::setenv( const std::string& name, const std::string& value )
    throw ( lang::Exception ) {

    apr_status_t result = APR_SUCCESS;

    // Write the value, errors are thrown out as an exception
    result = apr_env_set( name.c_str(), value.c_str(), getAprPool().getAprPool() );
    getAprPool().cleanup();

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
long long System::nanoTime() {
    return apr_time_now() * 1000;
}

////////////////////////////////////////////////////////////////////////////////
Map<string, string> System::getenv() throw ( Exception ) {

    Map<string, string> values;
    StringTokenizer tokenizer( "" );
    string key = "";
    string value = "";
    int tokens = 0;
    std::vector<std::string> env = getEnvArray();

    for( std::size_t i = 0; i < env.size(); i++ ){
        tokenizer.reset( env[i], "=" );

        tokens = tokenizer.countTokens();

        if( tokens == 1 ) {
            // special case, no value set, store empty string as value
            key = tokenizer.nextToken();
            value = string("");
        } else if( tokens > 2 ) {
            // special case: first equals delimits the key value, the rest are
            // part of the variable
            int pos = (int)env[i].find( "=" );
            key = env[i].substr( 0, pos );
            value = env[i].substr( pos + 1, string::npos );
        } else if( tokens == 0 ) {
            // Odd case, got a string with no equals sign.
            throw IllegalArgumentException(
                __FILE__, __LINE__,
                "System::getenv - Invalid env string. %s",
                env[i].c_str() );
        } else {
            // Normal case.
            key = tokenizer.nextToken();
            value = tokenizer.nextToken();
        }

        // Store the env var
        values.setValue( key, value );
    }

    return values;
}

#if defined(_WIN32)

#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> System::getEnvArray() {

    std::vector<std::string> buffer;
    int count = 0;
    LPTSTR lpszVars;
    LPVOID lpvEnv;

    lpvEnv = GetEnvironmentStrings();
    if( NULL == lpvEnv ){
        return buffer;
    }

    lpszVars = (LPTSTR)lpvEnv;
    while( *lpszVars != NULL ) {
        count++;
        lpszVars += strlen(lpszVars)+1;
    }

    lpszVars = (LPTSTR)lpvEnv;
    int index = 0;
    while( *lpszVars != NULL ) {
        buffer.push_back( lpszVars );
        lpszVars += strlen(lpszVars)+1;
    }

    FreeEnvironmentStrings( (LPTCH)lpvEnv );
    return buffer;
}

#else

#if defined(__APPLE__)
   #include <crt_externs.h>
   #define environ (*_NSGetEnviron())
#else
   extern char** environ;
#endif

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> System::getEnvArray() {

    std::vector<std::string> buffer;

    for( int i = 0; *(environ + i); i++ ){
        buffer.push_back( environ[i] );
    }

    return buffer;
}

#endif

