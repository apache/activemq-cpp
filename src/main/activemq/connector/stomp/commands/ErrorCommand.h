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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_ERRORCOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_ERRORCOMMAND_H_

#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Command.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Message sent from the broker when an error
     * occurs.
     */
    class AMQCPP_API ErrorCommand : public AbstractCommand< transport::Command > {
    public:

        ErrorCommand() :
            AbstractCommand<transport::Command>() {
                initialize( getFrame() );
        }

        ErrorCommand( StompFrame* frame ) :
            AbstractCommand<transport::Command>( frame ) {
                validate( getFrame() );
        }

        virtual ~ErrorCommand() {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new ErrorCommand( getFrame().clone() );
        }

        /**
         * Get the error message
         * @return the error message string
         */
        virtual std::string getErrorMessage() const {
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGE ), "" );
        }

        /**
         * Set the error message
         * @param message the error message string
         */
        virtual void setErrorMessage( const std::string& message ) {
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGE),
                message );
        }

        /**
         * Set the Text associated with this Error
         * @param text Detailed Error Message
         */
        virtual void setErrorDetails( const std::string& text ) {
            setBytes( (unsigned char*)text.c_str(), text.length() + 1 );
        }

        /**
         * Get the Text associated with this Error
         * @return Error Message String
         */
        virtual std::string getErrorDetails() const {

            const std::vector<unsigned char>& bytes = getBytes();
            if( bytes.size() == 0 ){
                return "";
            }

            return std::string((char*)&getBytes()[0]);
        }

    protected:

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::ERROR_CMD ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {
            if((frame.getCommand() ==
                CommandConstants::toString( CommandConstants::ERROR_CMD ) ) &&
               (frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_MESSAGE ) ) ) ) {

                return true;
            }

            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_ERRORCOMMAND_H_*/
