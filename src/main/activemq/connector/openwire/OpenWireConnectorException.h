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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOREXCEPTION_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOREXCEPTION_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ConnectorException.h>

namespace activemq{
namespace connector{
namespace openwire{

    /*
     * Signals that an Connector exception of some sort has occurred.
     */
    class AMQCPP_API OpenWireConnectorException : public connector::ConnectorException {
    public:

        OpenWireConnectorException() throw(){}

        OpenWireConnectorException( const exceptions::ActiveMQException& ex ) throw()
            : connector::ConnectorException(){
            *( exceptions::ActiveMQException* )this = ex;
        }

        OpenWireConnectorException( const OpenWireConnectorException& ex ) throw()
            : connector::ConnectorException(){
            *( exceptions::ActiveMQException* )this = ex;
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
        OpenWireConnectorException( const char* file, const int lineNumber,
                                    const std::exception* cause,
                                    const char* msg, ... )
        throw() : connector::ConnectorException( cause )
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
        OpenWireConnectorException( const std::exception* cause )
            throw() : connector::ConnectorException( cause ) {}

        OpenWireConnectorException( const char* file,
                                    const int lineNumber,
                                    const char* msg, ... ) throw()
            : connector::ConnectorException() {

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
        virtual OpenWireConnectorException* clone() const{
            return new OpenWireConnectorException( *this );
        }

        virtual ~OpenWireConnectorException() throw() {}

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOREXCEPTION_H_*/
