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

#ifndef _CMS_CMSEXCEPTION_H_
#define _CMS_CMSEXCEPTION_H_

// Includes
#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include <cms/Config.h>

namespace cms{

    class CMSExceptionData;

    /**
     * CMS API Exception that is the base for all exceptions thrown from CMS
     * classes.
     * <p>
     * This class represents an error that has occurred in CMS, providers
     * can wrap provider specific exceptions in this class by setting the
     * cause to an instance of a provider specific exception provided it
     * can be cast to an std::exception.
     * <p>
     * Since the contained cause exception is of type std::exception and the
     * C++ exception class has no clone or copy method defined the contained
     * exception can only be owned by one instance of an CMSException.  To that
     * end the class hands off the exception to each successive copy so care must
     * be taken when handling CMSException instances.
     *
     * @since 1.0
     */
    class CMS_API CMSException : public std::exception {
    private:

        // The actual data that defines this exception.
        CMSExceptionData* data;

    public:

        CMSException();

        CMSException( const CMSException& ex );

        CMSException( const std::string& message );

        CMSException( const std::string& message,
                      const std::exception* cause );

        CMSException( const std::string& message,
                      const std::exception* cause,
                      const std::vector< std::pair< std::string, int> >& stackTrace );

        virtual ~CMSException() throw();

        /**
         * Gets the cause of the error.
         *
         * @return string errors message
         */
        virtual std::string getMessage() const;

        /**
         * Gets the exception that caused this one to be thrown, this allows
         * for chaining of exceptions in the case of a method that throws only
         * a particular exception but wishes to allow for the real causal
         * exception to be passed only in case the caller knows about that
         * type of exception and wishes to respond to it.
         * @returns a const pointer reference to the causal exception, if there
         * was no cause associated with this exception then NULL is returned.
         */
        virtual const std::exception* getCause() const;

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.
         *
         * @return vector containing stack trace strings
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const;

        /**
         * Adds a file/line number to the stack trace.
         * @param file The name of the file calling this method (use __FILE__).
         * @param lineNumber The line number in the calling file (use __LINE__).
         */
        virtual void setMark( const char* file, const int lineNumber );

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const;

        /**
         * Prints the stack trace to the given output stream.
         *
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const;

        /**
         * Gets the stack trace as one contiguous string.
         *
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const;

        /**
         * Overloads the std::exception what() function to return the cause of the exception
         *
         * @return const char pointer to error message
         */
        virtual const char* what() const throw();

    private:

        /**
         * Overridden assignment operator.  We don't allow CMSExceptions to be assigned to one
         * another so this method is hidden.
         *
         * @param other
         *      The CMSException to assign to this instance.
         */
        CMSException& operator= ( const cms::CMSException& ) {
            return *this;
        }

    };

}

#endif /*_CMS_CMSEXCEPTION_H_*/
