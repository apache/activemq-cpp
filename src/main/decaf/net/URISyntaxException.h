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

#ifndef _DECAF_NET_URISYNTAXEXCEPTION_H_
#define _DECAF_NET_URISYNTAXEXCEPTION_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace net{

    class DECAF_API URISyntaxException : public lang::Exception {
    private:

        std::string reason;
        std::string input;
		std::size_t index;

    public:

        /**
         * Default Constructor
         */
        URISyntaxException() throw() {
            this->reason = "";
            this->input = "";
            this->index = -1;
        }

        /**
         * Conversion Constructor from some other Exception
         * @param An exception that should become this type of Exception
         */
        URISyntaxException( const Exception& ex ) throw() : Exception() {

            *(Exception*)this = ex;
            this->reason = "";
            this->input = "";
            this->index = -1;
        }

        /**
         * Copy Constructor
         */
        URISyntaxException( const URISyntaxException& ex ) throw() : Exception() {

            *(Exception*)this = ex;
            this->reason = ex.getReason();
            this->input = ex.getInput();
            this->index = ex.getIndex();
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param cause The exception that was the cause for this one to be thrown.
         * @param message to report
         * @param list of primitives that are formatted into the message
         */
        URISyntaxException( const char* file, const int lineNumber,
                            const std::exception* cause,
                            const char* msg, ... )
        throw() : lang::Exception( cause )
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
        URISyntaxException( const std::exception* cause )
            throw() : lang::Exception( cause ) {}

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the message to report, using an
         * optional list of arguments to parse into the message
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param message to report
         * @param list of primitives that are formatted into the message
         */
        URISyntaxException( const char* file, const int lineNumber,
                            const char* msg DECAF_UNUSED ) throw ()
        : Exception()
        {

            this->reason = "<Unknown Reason>";
            this->input = "<No Address Given>";
            this->index = -1;

            const char * message = "Input: %s, Reason it failed: %s";
            this->setMessage( message, input.c_str(), reason.c_str() );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the input string that caused the error
         * and the reason for the error.
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param input uri string
         * @param reason string for the failure.
         * @param list of primitives that are formatted into the message
         */
        URISyntaxException( const char* file, const int lineNumber,
                            const std::string& input,
                            const std::string& reason ) throw () : Exception() {

            this->reason = reason;
            this->input = input;
            this->index = -1;

            const char * message = "Input: %s, Reason it failed: %s";
            this->setMessage( message, input.c_str(), reason.c_str() );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Constructor - Initializes the file name and line number where
         * this message occurred.  Sets the input string that caused the error
         * and the reason for the error.
         * @param file name where exception occurs
         * @param line number where the exception occurred.
         * @param input uri string
         * @param reason string for the failure.
         * @param index in the uri string where the error occured.
         */
        URISyntaxException( const char* file, const int lineNumber,
                            const std::string& input,
                            const std::string& reason,
							std::size_t index ) throw () : Exception() {

            this->reason = reason;
            this->input = input;
            this->index = index;

            const char * message = "Input: %s, Index %d resulted in this error: %s";
            this->setMessage( message, input.c_str(), index, reason.c_str() );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual URISyntaxException* clone() const {
            return new URISyntaxException( *this );
        }

        /**
         * Destructor
         */
        virtual ~URISyntaxException() throw() {}

        /**
         * @returns the Input string that cause this exception or ""
         */
        std::string getInput() const {
            return input;
        }

        /**
         * @returns the Reason given for this failure, or ""
         */
        std::string getReason() const {
            return reason;
        }

        /**
         * @returns the index in the input string where the error occured or -1
         */
		std::size_t getIndex() const {
            return index;
        }

    };

}}

#endif /*_DECAF_NET_URISYNTAXEXCEPTION_H_*/
