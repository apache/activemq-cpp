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
#ifndef _ACTIVEMQ_CONNECTOR_STOMP_MARSHALL_MARSHALEXCEPTION_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_MARSHALL_MARSHALEXCEPTION_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace marshal{

    /*
     * Signals that an problem occurred during marshalling.
     */
    class AMQCPP_API MarshalException : public exceptions::ActiveMQException {
    public:

        /**
         * Default Constructor
         */
        MarshalException() throw() {}

        /**
         * Copy Constructor
         * @param ex the exception to copy
         */
        MarshalException( const exceptions::ActiveMQException& ex ) throw()
        : exceptions::ActiveMQException() {
            *(ActiveMQException*)this = ex;
        }

        /**
         * Copy Constructor
         * @param ex the exception to copy, which is an instance of this type
         */
        MarshalException( const MarshalException& ex ) throw()
        : exceptions::ActiveMQException() {
            *(exceptions::ActiveMQException*)this = ex;
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
        MarshalException( const char* file, const int lineNumber,
                          const std::exception* cause,
                          const char* msg, ... )
        throw() : exceptions::ActiveMQException( cause )
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
        MarshalException( const std::exception* cause )
            throw() : exceptions::ActiveMQException( cause ) {}

        /**
         * Constructor
         * @param file name of the file were the exception occurred.
         * @param lineNumber line where the exception occurred
         * @param msg the message that was generated
         */
        MarshalException( const char* file,
                          const int lineNumber,
                          const char* msg, ...) throw()
         :  exceptions::ActiveMQException() {

            va_list vargs ;
            va_start(vargs, msg) ;
            buildMessage(msg, vargs) ;

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual MarshalException* clone() const{
            return new MarshalException( *this );
        }

        virtual ~MarshalException() throw() {}

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_MARSHAL_MARSHALLEXCEPTION_H_*/
