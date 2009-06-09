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
#include "Exception.h"
#include <decaf/util/logging/LoggerDefines.h>
#include <decaf/internal/AprPool.h>

#include <sstream>
#include <apr_strings.h>

using namespace std;
using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;
using namespace decaf::util::logging;

////////////////////////////////////////////////////////////////////////////////
Exception::Exception() throw() : Throwable(), cause( NULL ) {
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception( const Exception& ex ) throw() : Throwable(), cause( NULL ) {
    *this = ex;
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception( const std::exception* cause ) throw() : Throwable(), cause( NULL ) {
    this->initCause( cause );
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception( const char* file, const int lineNumber,
                      const char* msg, ... ) throw() : Throwable(), cause( NULL ) {
    va_list vargs;
    va_start( vargs, msg ) ;
    buildMessage( msg, vargs );
    va_end( vargs );

    // Set the first mark for this exception.
    setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
Exception::Exception( const char* file, const int lineNumber,
                      const std::exception* cause,
                      const char* msg, ... ) throw() : Throwable(), cause( NULL ) {
    va_list vargs;
    va_start( vargs, msg ) ;
    this->buildMessage( msg, vargs );
    va_end( vargs );

    // Store the cause
    this->initCause( cause );

    // Set the first mark for this exception.
    this->setMark( file, lineNumber );
}

////////////////////////////////////////////////////////////////////////////////
Exception::~Exception() throw(){
    delete this->cause;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMessage( const char* msg, ... ){
    va_list vargs;
    va_start( vargs, msg );
    buildMessage( msg, vargs );
    va_end( vargs );
}

////////////////////////////////////////////////////////////////////////////////
void Exception::buildMessage( const char* format, va_list& vargs ) {

    // Allocate buffer with a guess of it's size
    AprPool pool;

    // Allocate a buffer of the specified size.
    char* buffer = apr_pvsprintf( pool.getAprPool(), format, vargs );

    // Guessed size was enough. Assign the string.
    message.assign( buffer, strlen( buffer ) );
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMark( const char* file, const int lineNumber ) {

    // Add this mark to the end of the stack trace.
    stackTrace.push_back( std::make_pair( (std::string)file, (int)lineNumber ) );

    std::ostringstream stream;
    stream << "\tFILE: " << stackTrace[stackTrace.size()-1].first;
    stream << ", LINE: " << stackTrace[stackTrace.size()-1].second;

    //decaf::util::logger::SimpleLogger logger("com.yadda2");
    //logger.log( stream.str() );
}

////////////////////////////////////////////////////////////////////////////////
Exception* Exception::clone() const {
    return new Exception( *this );
}

////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, int> > Exception::getStackTrace() const {
    return stackTrace;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setStackTrace(
    const std::vector< std::pair< std::string, int> >& trace ) {

    this->stackTrace = trace;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::printStackTrace() const {
    printStackTrace( std::cerr );
}

////////////////////////////////////////////////////////////////////////////////
void Exception::printStackTrace( std::ostream& stream ) const {
    stream << getStackTraceString();
}

////////////////////////////////////////////////////////////////////////////////
std::string Exception::getStackTraceString() const {

    // Create the output stream.
    std::ostringstream stream;

    // Write the message and each stack entry.
    stream << message << std::endl;
    for( unsigned int ix=0; ix<stackTrace.size(); ++ix ){
        stream << "\tFILE: " << stackTrace[ix].first;
        stream << ", LINE: " << stackTrace[ix].second;
        stream << std::endl;
    }

    // Return the string from the output stream.
    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
Exception& Exception::operator =( const Exception& ex ){
    this->message = ex.message;
    this->stackTrace = ex.stackTrace;
    this->initCause( ex.cause );
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
void Exception::initCause( const std::exception* cause ) {

    if( cause == NULL || cause == this ) {
        return;
    }

    if( this->cause != NULL ) {
        delete this->cause;
    }

    const Exception* ptrCause = dynamic_cast<const Exception*>( cause );
    if( ptrCause == NULL ) {
        this->cause = new Exception( __FILE__, __LINE__, cause->what() );
    } else {
        this->cause = ptrCause->clone();
    }

    if( this->message == "" ) {
        this->message = cause->what();
    }
}
