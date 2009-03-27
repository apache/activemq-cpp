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
#include <stdio.h>
#include "ActiveMQException.h"
#include <decaf/util/logging/LoggerDefines.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace decaf::lang;
using namespace std;

// For supporting older versions of msvc (<=2003)
#if defined(_MSC_VER) && (_MSC_VER < 1400)
    #define vsnprintf _vsnprintf
#endif

////////////////////////////////////////////////////////////////////////////////
ActiveMQException::ActiveMQException() throw() : decaf::lang::Exception() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException::ActiveMQException( const ActiveMQException& ex ) throw()
:   cms::CMSException(),
    decaf::lang::Exception() {

  this->message = ex.getMessage();
  this->stackTrace = ex.getStackTrace();
  this->initCause( ex.cause );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException::ActiveMQException( const Exception& ex ) throw()
:   cms::CMSException(),
    decaf::lang::Exception() {

  this->message = ex.getMessage();
  this->stackTrace = ex.getStackTrace();
  this->initCause( &ex );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException::ActiveMQException( const char* file, const int lineNumber,
                   const char* msg, ... ) throw()
:   cms::CMSException(),
    decaf::lang::Exception() {

    va_list vargs;
    va_start( vargs, msg ) ;
    buildMessage( msg, vargs );

    // Set the first mark for this exception.
    Exception::setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException::~ActiveMQException() throw(){
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException* ActiveMQException::clone() const{
    return new ActiveMQException( *this );
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQException& ActiveMQException::operator =( const Exception& ex ){
    this->setMessage( ex.getMessage().c_str() );
    this->setStackTrace( ex.getStackTrace() );
    return *this;
}
