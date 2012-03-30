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

#include "OpenSSLSocketException.h"

#ifdef HAVE_OPENSSL
#include <openssl/err.h>
#endif

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::net;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException() {
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const Exception& ex )
: SocketException() {
    *(Exception*)this = ex;
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const OpenSSLSocketException& ex )
: SocketException() {
    *(Exception*)this = ex;
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const char* file, const int lineNumber,
                                                const std::exception* cause,
                                                const char* msg, ... )
: SocketException( cause ) {

    va_list vargs;
    va_start( vargs, msg );
    buildMessage( msg, vargs );

    // Set the first mark for this exception.
    setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const std::exception* cause )
: SocketException( cause ) {

}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const char* file, const int lineNumber,
                                                const char* msg, ... )
: SocketException() {

    va_list vargs ;
    va_start( vargs, msg );
    buildMessage( msg, vargs );

    // Set the first mark for this exception.
    setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::OpenSSLSocketException( const char* file, const int lineNumber )
: SocketException() {

    // Get from the OpenSSL error Stack.
    this->setMessage( this->getErrorString().c_str() );

    // Set the first mark for this exception.
    setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketException::~OpenSSLSocketException() throw() {
}

////////////////////////////////////////////////////////////////////////////////
std::string OpenSSLSocketException::getErrorString() const {

    std::string returnValue = "Error occurred while accessing an OpenSSL library method:";

#ifdef HAVE_OPENSSL
    for( unsigned long e = ERR_get_error(); e; e = ERR_get_error() ) {
        char msg[256];
        ERR_error_string_n(e, msg, sizeof msg);
        returnValue += "\n";
        returnValue += msg;
    }
#endif

    return returnValue;
}
