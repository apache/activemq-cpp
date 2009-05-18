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

#ifndef _DECAF_LANG_THROWABLE_H_
#define _DECAF_LANG_THROWABLE_H_

// Includes
#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include <decaf/util/Config.h>

namespace decaf{
namespace lang{

    /**
     * This class represents an error that has occurred.  All Exceptions in the Decaf
     * library should extend from this or from the Exception class in order to ensure
     * that all Decaf Exceptions are interchangeable with the std::exception class.
     *
     * Throwable can wrap another Throwable as the cause if the error being thrown.
     * The user can inspect the cause by calling <code>getCause</code>, the pointer
     * returned is the property of the Throwable instance and will be deleted when it
     * is deleted or goes out of scope.
     *
     * @since 1.0
     */
    class DECAF_API Throwable : public std::exception {
    public:

        Throwable() throw() : std::exception() {}

        virtual ~Throwable() throw() {}

        /**
         * Gets the cause of the error, if no message was provided to the instance
         * of this interface but a cause was then the value cause.getMessage is
         * then returned.
         * @return string errors message
         */
        virtual std::string getMessage() const = 0;

        /**
         * Gets the exception that caused this one to be thrown, this allows
         * for chaining of exceptions in the case of a method that throws only
         * a particular exception but wishes to allow for the real causal
         * exception to be passed only in case the caller knows about that
         * type of exception and wishes to respond to it.
         * @returns a const pointer reference to the causal exception, if there
         * was no cause associated with this exception then NULL is returned.
         */
        virtual const std::exception* getCause() const = 0;

        /**
         * Initializes the contained cause exception with the one given.  A copy
         * is made to avoid ownership issues.
         * @param cause The exception that was the cause of this one.
         */
        virtual void initCause( const std::exception* cause ) = 0;

        /**
         * Adds a file/line number to the stack trace.
         * @param file The name of the file calling this method (use __FILE__).
         * @param lineNumber The line number in the calling file (use __LINE__).
         */
        virtual void setMark( const char* file, const int lineNumber ) = 0;

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         * @return Copy of this Exception object
         */
        virtual Throwable* clone() const = 0;

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.
         * @return vector containing stack trace strings
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const = 0;

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const = 0;

        /**
         * Prints the stack trace to the given output stream.
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const = 0;

        /**
         * Gets the stack trace as one contiguous string.
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const = 0;

    };

}}

#endif /*_DECAF_LANG_THROWABLE_H_*/
