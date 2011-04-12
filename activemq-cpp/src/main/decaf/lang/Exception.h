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
#ifndef _DECAF_LANG_EXCEPTION_EXCEPTION_H_
#define _DECAF_LANG_EXCEPTION_EXCEPTION_H_

#ifdef _WIN32
#pragma warning( disable: 4251 )
#endif

#include <decaf/lang/Throwable.h>
#include <decaf/lang/exceptions/ExceptionDefines.h>
#include <decaf/util/Config.h>
#include <stdarg.h>
#include <sstream>

namespace decaf{
namespace lang{

    /*
     * Base class for all exceptions.
     */
    class DECAF_API Exception : public Throwable {
    protected:

        /**
         * The cause of this exception.
         */
        std::string message;

        /**
         * The Exception that caused this one to be thrown.
         */
        std::exception* cause;

        /**
         * The stack trace.
         */
        std::vector< std::pair< std::string, int> > stackTrace;

    public:

        /**
         * Default Constructor
         */
        Exception() throw();

        /**
         * Copy Constructor
         *
         * @param ex
         *      The <code>Exception</code> instance to copy.
         */
        Exception( const Exception& ex ) throw();

        /**
         * Constructor
         *
         * @param cause
         *      Pointer to the exception that caused this one to
         *      be thrown, the object is cloned caller retains ownership.
         */
        Exception( const std::exception* cause ) throw();

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file The file name where exception occurs
         * @param lineNumber The line number where the exception occurred.
         * @param msg The message to report
         * @param ... list of primitives that are formatted into the message
         */
        Exception( const char* file, const int lineNumber,
                   const char* msg, ... ) throw();

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
        Exception( const char* file, const int lineNumber,
                   const std::exception* cause,
                   const char* msg, ... ) throw();

        virtual ~Exception() throw();

        /**
         * Gets the message for this exception.
         * @return Text formatted error message
         */
        virtual std::string getMessage() const{
            return message;
        }

        /**
         * Gets the exception that caused this one to be thrown, this allows
         * for chaining of exceptions in the case of a method that throws only
         * a particular exception but wishes to allow for the real causal
         * exception to be passed only in case the caller knows about that
         * type of exception and wishes to respond to it.
         * @returns a const pointer reference to the causal exception, if there
         * was no cause associated with this exception then NULL is returned.
         */
        virtual const std::exception* getCause() const {
            return this->cause;
        }

        /**
         * Initializes the contained cause exception with the one given.  A copy
         * is made to avoid ownership issues.
         * @param cause The exception that was the cause of this one.
         */
        virtual void initCause( const std::exception* cause );

        /**
         * Implement method from std::exception
         * @return the const char* of <code>getMessage()</code>.
         */
        virtual const char* what() const throw (){
            return message.c_str();
        }

        /**
         * Sets the cause for this exception.
         * @param msg the format string for the msg.
         * @param ... params to format into the string
         */
        virtual void setMessage( const char* msg, ... );

        /**
         * Adds a file/line number to the stack trace.
         * @param file The name of the file calling this method (use __FILE__).
         * @param lineNumber The line number in the calling file (use __LINE__).
         */
        virtual void setMark( const char* file, const int lineNumber );

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         * @return Copy of this Exception object
         */
        virtual Exception* clone() const;

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.  The first
         * item in the returned vector is the first point where the mark
         * was set (e.g. where the exception was created).
         * @return the stack trace.
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const;

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const;

        /**
         * Prints the stack trace to the given output stream.
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const;

        /**
         * Gets the stack trace as one contiguous string.
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const;

        /**
         * Assignment operator.
         * @param ex const reference to another Exception
         */
        Exception& operator =( const Exception& ex );

    protected:

        virtual void setStackTrace(
            const std::vector< std::pair< std::string, int> >& trace );

        virtual void buildMessage( const char* format, va_list& vargs );

   };

}}

#endif /*_DECAF_LANG_EXCEPTION_EXCEPTION_H_*/
