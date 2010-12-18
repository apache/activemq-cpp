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

#ifndef _ACTIVEMQ_COMMANDS_BROKERERROR_H_
#define _ACTIVEMQ_COMMANDS_BROKERERROR_H_

#include <activemq/util/Config.h>
#include <activemq/commands/BaseCommand.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/Pointer.h>

#include <string>
#include <vector>

namespace activemq{
namespace commands{

    /**
     * This class represents an Exception sent from the Broker.  The Broker
     * sends java Throwables, so we must mimic its structure here.
     */
    class AMQCPP_API BrokerError : public BaseCommand {
    public:

        struct StackTraceElement
        {
            std::string ClassName;
            std::string FileName;
            std::string MethodName;
            int LineNumber;

            StackTraceElement() : ClassName(), FileName(), MethodName(), LineNumber() {}
        };

    private:

        std::string message;
        std::string exceptionClass;
        std::vector< decaf::lang::Pointer<StackTraceElement> > stackTraceElements;
        decaf::lang::Pointer<BrokerError> cause;

    public:

        BrokerError();

        virtual ~BrokerError();

        /**
         * Get the DataStructure Type as defined in CommandTypes.h
         * @return The type of the data structure
         */
        virtual unsigned char getDataStructureType() const {
            return 0;
        }

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual BrokerError* cloneDataStructure() const {

            BrokerError* error = new BrokerError();
            error->copyDataStructure( this );
            return error;
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src );

        /**
         * Allows a Visitor to visit this command and return a response to the
         * command based on the command type being visited.  The command will call
         * the proper processXXX method in the visitor.
         *
         * @return a Response to the visitor being called or NULL if no response.
         */
        virtual decaf::lang::Pointer<commands::Command> visit(
            activemq::state::CommandVisitor* visitor );

        /**
         * Gets the string holding the error message
         * @returns String Message
         */
        virtual const std::string& getMessage() const {
            return message;
        }

        /**
         * Sets the string that contains the error Message
         * @param message - String Error Message
         */
        virtual void setMessage( const std::string& message ) {
            this->message = message;
        }

        /**
         * Gets the string holding the Exception Class name
         * @returns Exception Class name
         */
        virtual const std::string& getExceptionClass() const {
            return exceptionClass;
        }

        /**
         * Sets the string that contains the Exception Class name
         * @param exceptionClass - String Exception Class name
         */
        virtual void setExceptionClass( const std::string& exceptionClass ) {
            this->exceptionClass = exceptionClass;
        }

        /**
         * Gets the Broker Error that caused this exception
         * @returns Broker Error Pointer
         */
        virtual const decaf::lang::Pointer<BrokerError>& getCause() const {
            return cause;
        }

        /**
         * Sets the Broker Error that caused this exception
         * @param cause - Broker Error
         */
        virtual void setCause( const decaf::lang::Pointer<BrokerError>& cause ) {
            this->cause = cause;
        }

        /**
         * Gets the Stack Trace Elements for the Exception
         * @returns Stack Trace Elements
         */
        virtual const std::vector< decaf::lang::Pointer<StackTraceElement> >& getStackTraceElements() const {
            return stackTraceElements;
        }

        /**
         * Sets the Stack Trace Elements for this Exception
         * @param stackTraceElements - Stack Trace Elements
         */
        virtual void setStackTraceElements( const std::vector< decaf::lang::Pointer<StackTraceElement> >& stackTraceElements ) {
            this->stackTraceElements = stackTraceElements;
        }

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_BROKERERROR_H_*/
