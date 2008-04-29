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

#ifndef _DECAF_NIO_BUFFEROVERFLOWEXCEPTION_H_
#define _DECAF_NIO_BUFFEROVERFLOWEXCEPTION_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace nio{

    class DECAF_API BufferOverflowException : public lang::Exception {
    public:

        /**
         * Default Constructor
         */
        BufferOverflowException() throw() {}

        /**
         * Copy Constructor
         * @param ex the exception to copy
         */
        BufferOverflowException( const lang::Exception& ex ) throw()
        : lang::Exception() {
            *(lang::Exception*)this = ex;
        }

        /**
         * Copy Constructor
         * @param ex the exception to copy, which is an instance of this type
         */
        BufferOverflowException( const BufferOverflowException& ex ) throw()
        : lang::Exception() {
            *(lang::Exception*)this = ex;
        }

        /**
         * Consturctor
         * @param file name of the file were the exception occured.
         * @param lineNumber line where the exception occured
         * @param msg the message that was generated
         */
        BufferOverflowException( const char* file, const int lineNumber,
                                 const char* msg, ... ) throw()
        : lang::Exception() {

            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual BufferOverflowException* clone() const {
            return new BufferOverflowException( *this );
        }

        virtual ~BufferOverflowException() throw() {}

    };

}}

#endif /*_DECAF_NIO_BUFFEROVERFLOWEXCEPTION_H_*/
