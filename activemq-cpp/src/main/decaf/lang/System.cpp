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
#include <decaf/util/StlMap.h>
#include <decaf/util/concurrent/TimeUnit.h>
#include <decaf/util/Properties.h>
#include <apr.h>
#include <apr_errno.h>
#include <apr_env.h>
#include <apr_time.h>

#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#if defined(_HP_NAMESPACE_STD)
#include <sys/mpctl.h>
#endif

#include <cstdlib>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace lang {

    class SystemData {
    public:

        StlMap<string, string> cachedEnvValues;
        AprPool aprPool;
        Properties systemProperties;

    public:

        SystemData() : cachedEnvValues(), aprPool(), systemProperties() {}

        ~SystemData() {}
    };

}}

////////////////////////////////////////////////////////////////////////////////
SystemData* System::sys = NULL;

////////////////////////////////////////////////////////////////////////////////
System::System() {
}

////////////////////////////////////////////////////////////////////////////////
void System::initSystem( int argc DECAF_UNUSED, char **argv DECAF_UNUSED ) {

    // TODO - Parse out properties specified at the Command Line level.

    // Create the System Data class.
    System::sys = new SystemData();
}

////////////////////////////////////////////////////////////////////////////////
void System::shutdownSystem() {

    // Destroy the System Data class.
    delete System::sys;
}

////////////////////////////////////////////////////////////////////////////////
AprPool& System::getAprPool() {
    return System::sys->aprPool;
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const char* src, std::size_t srcPos,
                        char* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length );
    } else {
        ::memmove( dest + destPos, src + srcPos, length );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const unsigned char* src, std::size_t srcPos,
                        unsigned char* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length );
    } else {
        ::memmove( dest + destPos, src + srcPos, length );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const short* src, std::size_t srcPos,
                        short* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length * sizeof( short ) );
    } else {
        ::memmove( dest + destPos, src + srcPos, length * sizeof( short ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const int* src, std::size_t srcPos,
                        int* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length * sizeof( int ) );
    } else {
        ::memmove( dest + destPos, src + srcPos, length * sizeof( int ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const long long* src, std::size_t srcPos,
                        long long* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length * sizeof( long long ) );
    } else {
        ::memmove( dest + destPos, src + srcPos, length * sizeof( long long ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const float* src, std::size_t srcPos,
                        float* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length * sizeof( float ) );
    } else {
        ::memmove( dest + destPos, src + srcPos, length * sizeof( float ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::arraycopy( const double* src, std::size_t srcPos,
                        double* dest, std::size_t destPos, std::size_t length ) {

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    if( src == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "Given Source Pointer was null." );
    }

    // Now we try and copy, could still segfault.
    if( src != dest ) {
        ::memcpy( dest + destPos, src + srcPos, length * sizeof( double ) );
    } else {
        ::memmove( dest + destPos, src + srcPos, length * sizeof( double ) );
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::unsetenv( const std::string& name ) {

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
std::string System::getenv( const std::string& name ) {

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
void System::setenv( const std::string& name, const std::string& value ) {

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

#ifdef _WIN32

    /* Number of micro-seconds between the beginning of the Windows epoch
     * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
     */
    static const unsigned long long DELTA_EPOCH_IN_USEC = 116444736000000000ULL;

    unsigned long long time = 0;
    ::GetSystemTimeAsFileTime( (FILETIME*)&time );
    return ( time - DELTA_EPOCH_IN_USEC ) / 10000;

#else

    struct timeval tv;
    gettimeofday( &tv, NULL );
    return ( ( (long long)tv.tv_sec * 1000000 ) + tv.tv_usec ) / 1000;

#endif
}

////////////////////////////////////////////////////////////////////////////////
long long System::nanoTime() {

#ifdef _WIN32

    LARGE_INTEGER freq, i, multiplier;
    long long result;

    if( !::QueryPerformanceFrequency( &freq ) ) {
        return ::GetTickCount();
    }

    multiplier.QuadPart = freq.QuadPart / 1000000;

    ::QueryPerformanceCounter( &i );
    result = i.QuadPart / multiplier.QuadPart;

    return result * 1000;

#else

    struct timeval tv;
    gettimeofday( &tv, NULL );
    return ( ( (long long)tv.tv_sec * 1000000 ) + tv.tv_usec ) * 1000;

#endif
}

////////////////////////////////////////////////////////////////////////////////
const Map<string, string>& System::getenv() {

    System::sys->cachedEnvValues.clear();

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
        System::sys->cachedEnvValues.put( key, value );
    }

    return System::sys->cachedEnvValues;
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

////////////////////////////////////////////////////////////////////////////////
int System::availableProcessors() {

    int numCpus = 1;

#if defined(_WIN32)

    SYSTEM_INFO sysInfo;
    ::GetSystemInfo( &sysInfo );
    numCpus = sysInfo.dwNumberOfProcessors;

#elif defined(__APPLE__)

    // derived from examples in the sysctl(3) man page from FreeBSD
    int mib[2];
    std::size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    len = sizeof(numCpus);
    sysctl(mib, 2, &numCpus, &len, NULL, 0);

#elif defined(_HP_NAMESPACE_STD)
    numCpus = (int)mpctl(MPC_GETNUMSPUS, NULL, NULL);
#else

    // returns number of online(_SC_NPROCESSORS_ONLN) processors, number configured(_SC_NPROCESSORS_CONF)
    // may be more than online
    numCpus = (int)sysconf( _SC_NPROCESSORS_ONLN );

#endif

    if( numCpus < 1 ) {
        numCpus = 1;
    }

    return numCpus;
}

////////////////////////////////////////////////////////////////////////////////
decaf::util::Properties& System::getProperties() {
    return System::sys->systemProperties;
}

////////////////////////////////////////////////////////////////////////////////
std::string System::getProperty( const std::string& key ) {

    if( key.empty() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Cannot pass an empty key to getProperty." );
    }

    return System::sys->systemProperties.getProperty( key, "" );
}

////////////////////////////////////////////////////////////////////////////////
std::string System::getProperty( const std::string& key, const std::string& defaultValue ) {

    if( key.empty() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Cannot pass an empty key to getProperty." );
    }

    return System::sys->systemProperties.getProperty( key, defaultValue );
}

////////////////////////////////////////////////////////////////////////////////
std::string System::setProperty( const std::string& key, const std::string& value ) {

    if( key.empty() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Cannot pass an empty key to setProperty." );
    }

    if( value == "" ) {
        return System::clearProperty( key );
    }

    return System::sys->systemProperties.setProperty( key, value );
}

////////////////////////////////////////////////////////////////////////////////
std::string System::clearProperty( const std::string& key ) {

    if( key.empty() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Cannot pass an empty key to clearProperty." );
    }

    std::string oldValue = System::sys->systemProperties.getProperty( key, "" );
    System::sys->systemProperties.remove( key );

    return oldValue;
}
