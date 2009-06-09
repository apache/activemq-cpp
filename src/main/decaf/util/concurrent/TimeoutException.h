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
#ifndef _DECAF_UTIL_CONCURRENT_TIMEOUTEXCEPTION_H_
#define _DECAF_UTIL_CONCURRENT_TIMEOUTEXCEPTION_H_

#include <decaf/lang/Exception.h>

namespace decaf{
namespace util{
namespace concurrent{

    /*
     * Exception thrown when a blocking operation times out. Blocking operations for which
     * a timeout is specified need a means to indicate that the timeout has occurred. For
     * many such operations it is possible to return a value that indicates timeout; when
     * that is not possible or desirable then TimeoutException should be declared and thrown.
     */
    class DECAF_API TimeoutException : public decaf::lang::Exception {
    public:

        /**
         * Default Constructor
         */
        TimeoutException() throw() : decaf::lang::Exception() {}

        /**
         * Conversion Constructor from some other Exception
         * @param ex An exception that should become this type of Exception
         */
        TimeoutException( const decaf::lang::Exception& ex ) throw()
        : decaf::lang::Exception() {
            *(Exception*)this = ex;
        }

        /**
         * Copy Constructor
         *
         * @param ex
         *      The exception to copy from.
         */
        TimeoutException( const TimeoutException& ex ) throw()
        : decaf::lang::Exception() {
            *(Exception*)this = ex;
        }

        /**
         * Constructor
         * @param cause
         *      Pointer to the exception that caused this one to
         *      be thrown, the object is cloned caller retains ownership.
         */
        TimeoutException( const std::exception* cause )
            throw() : decaf::lang::Exception( cause ) {}

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file The file name where exception occurs
         * @param lineNumber The line number where the exception occurred.
         * @param msg The string message to report
         * @param ... list of primitives that are formatted into the message
         */
        TimeoutException( const char* file, const int lineNumber,
                          const char* msg, ... ) throw()
            : decaf::lang::Exception()
        {

            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file The file name where exception occurs
         * @param lineNumber The line number where the exception occurred.
         * @param cause The exception that was the cause for this one to be thrown.
         * @param msg The string message to report
         * @param ... list of primitives that are formatted into the message
         */
        TimeoutException( const char* file, const int lineNumber,
                          const std::exception* cause,
                          const char* msg, ... ) throw()
            : decaf::lang::Exception( cause )
        {
            va_list vargs ;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         *
         * @return a new TimeoutException that is a copy of this one.
         */
        virtual TimeoutException* clone() const{
            return new TimeoutException( *this );
        }

        virtual ~TimeoutException() throw() {}

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_TIMEOUTEXCEPTION_H_*/
