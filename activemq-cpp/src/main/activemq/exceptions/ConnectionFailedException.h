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

#ifndef _ACTIVEMQ_EXCEPTION_CONNECTIONFAILEDEXCEPTION_H_
#define _ACTIVEMQ_EXCEPTION_CONNECTIONFAILEDEXCEPTION_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>

namespace activemq {
namespace exceptions {

    class AMQCPP_API ConnectionFailedException : public exceptions::ActiveMQException {
    public:

        ConnectionFailedException() {}

        ConnectionFailedException( const exceptions::ActiveMQException& ex )
            : exceptions::ActiveMQException() {
            *( exceptions::ActiveMQException* )this = ex;
        }

        ConnectionFailedException( const ConnectionFailedException& ex )
            : exceptions::ActiveMQException() {
            *( exceptions::ActiveMQException* )this = ex;
        }

        ConnectionFailedException( const char* file, const int lineNumber,
                                   const char* msg, ... )
          : exceptions::ActiveMQException() {

            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        virtual ConnectionFailedException* clone() const {
            return new ConnectionFailedException( *this );
        }

        virtual ~ConnectionFailedException() throw() {}

    };

}}

#endif /* _ACTIVEMQ_EXCEPTION_CONNECTIONFAILEDEXCEPTION_H_ */
