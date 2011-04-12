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

#ifndef _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETEXCEPTION_H_
#define _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETEXCEPTION_H_

#include <decaf/util/Config.h>

#include <decaf/net/SocketException.h>

namespace decaf {
namespace internal {
namespace net {
namespace ssl {
namespace openssl {

    /**
     * Subclass of the standard SocketException that knows how to produce an error message
     * from the OpenSSL error stack.
     *
     * @since 1.0
     */
    class DECAF_API OpenSSLSocketException : public decaf::net::SocketException {
    public:

        /**
         * Creates an new OpenSSLSocketException with default values.
         */
        OpenSSLSocketException() throw();

        /**
         * Conversion Constructor from some other Exception.
         *
         * @param ex
         *      An Exception object that should become this type of Exception.
         */
        OpenSSLSocketException( const Exception& ex ) throw();

        /**
         * Copy Constructor.
         *
         * @param ex
         *      The OpenSSLSocketException whose values should be copied to this instance.
         */
        OpenSSLSocketException( const OpenSSLSocketException& ex ) throw();

        /**
         * Create a new OpenSSLSocketException and initializes the file name and line number
         * where this message occurred.  Sets the message to report, using an optional list
         * of arguments to parse into the message.
         *
         * @param file
         *      The file name where exception occurs
         * @param lineNumber
         *      The line number where the exception occurred.
         * @param cause
         *      The exception that was the cause for this one to be thrown (can be null).
         * @param msg
         *      The error message to report.
         * @param ...
         *      The list of primitives that are formatted into the message.
         */
        OpenSSLSocketException( const char* file, const int lineNumber,
                                const std::exception* cause,
                                const char* msg, ... ) throw();

        /**
         * Creates a new OpenSSLSocketException with the passed exception set as the
         * cause of this exception.
         *
         * @param cause
         *      Pointer to the exception that caused this one to be thrown, the object
         *      is cloned caller retains ownership.
         */
        OpenSSLSocketException( const std::exception* cause ) throw();

        /**
         * Create a new OpenSSLSocketException and initializes the file name and line number
         * where this message occurred.  Sets the message to report, using an optional list
         * of arguments to parse into the message.
         *
         * @param file
         *      The file name where exception occurs.
         * @param lineNumber
         *      The line number where the exception occurred.
         * @param msg
         *      The error message to report.
         * @param ...
         *      The list of primitives that are formatted into the message
         */
        OpenSSLSocketException( const char* file, const int lineNumber, const char* msg, ... ) throw ();

        /**
         * Create a new OpenSSLSocketException and initializes the file name and line number
         * where this message occurred.  Sets the message to report by getting the complete set
         * of error messages from the OpenSSL error stack and concatenating them into one string.
         *
         * @param file
         *      The file name where exception occurs.
         * @param lineNumber
         *      The line number where the exception occurred.
         */
        OpenSSLSocketException( const char* file, const int lineNumber ) throw();

        /**
         * Clones this exception.  This is useful for cases where you need to preserve
         * the type of the original exception as well as the message.  All subclasses should
         * override this method.
         *
         * @return a new Exception instance that is a copy of this Exception object.
         */
        virtual OpenSSLSocketException* clone() const {
            return new OpenSSLSocketException( *this );
        }

        virtual ~OpenSSLSocketException() throw();

    protected:

        /**
         * Gets and formats an error message string from the OpenSSL error stack.
         *
         * @returns a string containing the complete OpenSSL error string.
         */
        std::string getErrorString() const;

    };

}}}}}

#endif /* _DECAF_INTERNAL_NET_SSL_OPENSSL_OPENSSLSOCKETEXCEPTION_H_ */
