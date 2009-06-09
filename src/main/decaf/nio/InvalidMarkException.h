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

#ifndef _DECAF_NIO_INVALIDMARKEXCEPTION_H_
#define _DECAF_NIO_INVALIDMARKEXCEPTION_H_

#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

namespace decaf{
namespace nio{

    class DECAF_API InvalidMarkException : public lang::exceptions::IllegalStateException {
    public:

        /**
         * Default Constructor
         */
        InvalidMarkException() throw() {}

        /**
         * Conversion Constructor from some other Exception
         *
         * @param ex
         *      The Exception whose state data is to be copied into this Exception.
         */
        InvalidMarkException( const lang::Exception& ex ) throw()
        : lang::exceptions::IllegalStateException() {
            *(lang::Exception*)this = ex;
        }

        /**
         * Copy Constructor
         *
         * @param ex
         *      The Exception whose state data is to be copied into this Exception.
         */
        InvalidMarkException( const InvalidMarkException& ex ) throw()
        : lang::exceptions::IllegalStateException() {
            *(lang::Exception*)this = ex;
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file The file name where exception occurs
         * @param lineNumber The line number where the exception occurred.
         * @param cause The exception that was the cause for this one to be thrown.
         * @param msg The message to report
         * @param ... list of primitives that are formatted into the message
         */
        InvalidMarkException( const char* file, const int lineNumber,
                              const std::exception* cause,
                              const char* msg, ... )
            throw() : lang::exceptions::IllegalStateException( cause )
        {
            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Constructor
         * @param cause Pointer to the exception that caused this one to
         * be thrown, the object is cloned caller retains ownership.
         */
        InvalidMarkException( const std::exception* cause )
            throw() : lang::exceptions::IllegalStateException( cause ) {}

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file The file name where exception occurs
         * @param lineNumber The line number where the exception occurred.
         * @param msg The message to report
         * @param ... list of primitives that are formatted into the message
         */
        InvalidMarkException( const char* file, const int lineNumber,
                              const char* msg, ... ) throw ()
        : lang::exceptions::IllegalStateException() {

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
         * @return A new Exception instance that is a copy of this Exception.
         */
        virtual InvalidMarkException* clone() const {
            return new InvalidMarkException( *this );
        }

        virtual ~InvalidMarkException() throw() {}

    };

}}

#endif /*_DECAF_NIO_INVALIDMARKEXCEPTION_H_*/
