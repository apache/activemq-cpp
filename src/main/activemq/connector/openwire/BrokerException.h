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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_BROKEREXCEPTION_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_BROKEREXCEPTION_H_

#include <activemq/util/Config.h>
#include <activemq/connector/openwire/OpenWireConnectorException.h>
#include <activemq/connector/openwire/commands/BrokerError.h>
#include <sstream>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API BrokerException : public OpenWireConnectorException {
    public:

        BrokerException() throw() {}

        BrokerException( const exceptions::ActiveMQException& ex ) throw()
            : OpenWireConnectorException(){
            *( exceptions::ActiveMQException* )this = ex;
        }

        BrokerException( const BrokerException& ex ) throw()
            : OpenWireConnectorException(){
            *( exceptions::ActiveMQException* )this = ex;
        }

        BrokerException( const char* file,
                                    const int lineNumber,
                                    const char* msg, ... ) throw()
          : OpenWireConnectorException() {

            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        BrokerException( const char* file,
                         const int lineNumber,
                         const commands::BrokerError* error ) throw()
          : OpenWireConnectorException() {

            std::ostringstream ostream;
            ostream << "*** BEGIN SERVER-SIDE STACK TRACE ***" << std::endl;
            ostream << "Message: " << error->getMessage() << std::endl;
            ostream << "Cause: " << error->getCause() << std::endl;
            ostream << "Exception Class " << error->getExceptionClass() << std::endl;

            const std::vector<commands::BrokerError::StackTraceElement*>& trace = error->getStackTraceElements();
            for( std::size_t ix=0; ix<trace.size(); ++ix ){
                commands::BrokerError::StackTraceElement* element = trace[ix];
                ostream << "\t[FILE: " << element->FileName << ", LINE: " << element->LineNumber
                    << "] occurred in: " << element->ClassName << "." << element->MethodName << std::endl;
            }

            ostream << "*** END SERVER-SIDE STACK TRACE ***";

            setMessage( ostream.str().c_str() );
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual BrokerException* clone() const{
            return new BrokerException( *this );
        }

        virtual ~BrokerException() throw() {}
    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_BROKEREXCEPTION_H_*/
