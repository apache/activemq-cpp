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
#ifndef _ACTIVEMQ_EXCEPTIONS_ACTIVEMQEXCEPTION_H_
#define _ACTIVEMQ_EXCEPTIONS_ACTIVEMQEXCEPTION_H_

#include <activemq/util/Config.h>
#include <cms/CMSException.h>
#include <decaf/lang/Exception.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <stdarg.h>
#include <sstream>

namespace activemq{
namespace exceptions{

    /*
     * Base class for all exceptions.
     */
    class AMQCPP_API ActiveMQException : public cms::CMSException,
                                         public decaf::lang::Exception {
    public:

        /**
         * Default Constructor
         */
        ActiveMQException() throw();

        /**
         * Copy Constructor
         */
        ActiveMQException( const ActiveMQException& ex ) throw();

        /**
         * Copy Constructor
         */
        ActiveMQException( const decaf::lang::Exception& ex ) throw();

        /**
         * Constructor - Initializes the file name and line number where
         * this message occured.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param message to report
         * @param list of primitives that are formatted into the message
         */
        ActiveMQException( const char* file, const int lineNumber,
                           const char* msg, ... ) throw();

        virtual ~ActiveMQException() throw();

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         * @return Copy of this Exception object
         */
        virtual ActiveMQException* clone() const;

        /**
         * Assignment operator.
         * @param const reference to another ActiveMQException
         */
        virtual ActiveMQException& operator =( const Exception& ex );

        /**
         * Gets the cause of the error.
         *
         * @return string errors message
         */
        virtual std::string getMessage() const {
            return decaf::lang::Exception::getMessage();
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
            return decaf::lang::Exception::getCause();
        }

        /**
         * Adds a file/line number to the stack trace.
         *
         * @param file
         *      The name of the file calling this method (use __FILE__).
         * @param lineNumber
         *      The line number in the calling file (use __LINE__).
         */
        virtual void setMark( const char* file, const int lineNumber ) {
            decaf::lang::Exception::setMark( file, lineNumber );
        }

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.
         *
         * @return vector containing stack trace strings
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const {
            return decaf::lang::Exception::getStackTrace();
        }

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const {
            decaf::lang::Exception::printStackTrace();
        }

        /**
         * Prints the stack trace to the given output stream.
         *
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const {
            decaf::lang::Exception::printStackTrace( stream );
        }

        /**
         * Gets the stack trace as one contiguous string.
         *
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const {
            return decaf::lang::Exception::getStackTraceString();
        }

   };

}}

#endif /*_ACTIVEMQ_EXCEPTIONS_ACTIVEMQEXCEPTION_H_*/
