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

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_

#include <string>
#include <string.h>
#include <map>
#include <decaf/util/Properties.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace wireformat{
namespace stomp{

    /**
     * A Stomp-level message frame that encloses all messages
     * to and from the broker.
     */
    class AMQCPP_API StompFrame{
    private:

        // String Name of this command.
        std::string command;

        // Properties of the Stomp Message
        decaf::util::Properties properties;

        // Byte data of Body.
        std::vector<unsigned char> body;

    public:

        /**
         * Default constructor.
         */
        StompFrame(){}

        /**
         * Destruction.
         */
        virtual ~StompFrame() {}

        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual StompFrame* clone() const {
            StompFrame* frame = new StompFrame();
            frame->copy( this );
            return frame;
        }

        /**
         * Copies the contents of the passed Frame to this one
         * @param src - Frame to copy
         */
        virtual void copy( const StompFrame* src ) {

            this->setCommand( src->getCommand() );
            this->properties.copy( &( src->getProperties() ) );
            // Use the Vectors assignment operator.
            this->body = src->getBody();
        }

        /**
         * Sets the command for this stomp frame.
         * @param cmd command The command to be set.
         */
        void setCommand( const std::string& cmd ){
            this->command = cmd;
        }

        /**
         * Accessor for this frame's command field.
         */
        const std::string& getCommand() const{
            return command;
        }

        /**
         * Checks if the given property is present in the Frame.
         *
         * @param name - The name of the property to check for.
         */
        bool hasProperty( const std::string& name ) const {
            return this->properties.hasProperty( name );
        }

        /**
         * Gets a property from this Frame's properties and returns it, or the
         * default value given.
         *
         * @param name - The name of the property to lookup
         * @param fallback - The default value to return if this value isn't set
         *
         * @return string value of the property asked for.
         */
        std::string getProperty( const std::string& name, const std::string& fallback = "" ) const {
            return this->properties.getProperty( name, fallback );
        }

        /**
         * Gets and remove the property specified, if the property is not set, this method
         * returns the empty string.
         *
         * @param name - the Name of the property to get and return.
         */
        std::string removeProperty( const std::string& name ) {
            return this->properties.getProperty( name, "" );
        }

        /**
         * Sets the property given to the value specified in this Frame's Properties
         *
         * @param name - Name of the property.
         * @param value - Value to set the property to.
         */
        void setProperty( const std::string& name, const std::string& value ) {
            this->properties.setProperty( name, value );
        }

        /**
         * Gets access to the header properties for this frame.
         * @return the Properties object owned by this Frame
         */
        decaf::util::Properties& getProperties(){ return properties; }
        const decaf::util::Properties& getProperties() const {
            return properties;
        }

        /**
         * Accessor for the body data of this frame.
         * @return char pointer to body data
         */
        const std::vector<unsigned char>& getBody() const{
            return body;
        }

        /**
         * Non-const version of the body accessor.
         */
        std::vector<unsigned char>& getBody(){
            return body;
        }

        /**
         * Return the number of bytes contained in this frames body
         * @return Body bytes length.
         */
        std::size_t getBodyLength() const{ return body.size(); }

        /**
         * Sets the body data of this frame as a byte sequence.
         * @param bytes The byte buffer to be set in the body.
         * @param numBytes The number of bytes in the buffer.
         */
        void setBody( const unsigned char* bytes, std::size_t numBytes ){

            // Remove old data
            body.clear();

            // Copy data to internal buffer.
            std::back_insert_iterator< std::vector<unsigned char> > iter( body );
            std::copy( bytes, bytes + numBytes, iter );
        }

    };

}}}

#endif /*_ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_*/
