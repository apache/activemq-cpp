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

#ifndef ACTIVEMQ_CONNECTOR_STOMP_STOMPFRAMEWRAPPER_H_
#define ACTIVEMQ_CONNECTOR_STOMP_STOMPFRAMEWRAPPER_H_
 
#include <string>
#include <string.h>
#include <map>
#include <activemq/util/SimpleProperties.h>

namespace activemq{
namespace connector{
namespace stomp{

    /**
     * A Stomp-level message frame that encloses all messages
     * to and from the broker.
     */
    class StompFrame{           
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
            frame->command = command;
            frame->properties = properties;
            frame->body = body;
            return frame;
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
         * Gets access to the header properties for this frame.
         * @return the Properties object owned by this Frame
         */
        util::Properties& getProperties(){ return properties; }
        const util::Properties& getProperties() const { 
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
        unsigned long long getBodyLength() const{ return (unsigned long long)body.size(); }
        
        /**
         * Sets the body data of this frame as a byte sequence.
         * @param bytes The byte buffer to be set in the body.
         * @param numBytes The number of bytes in the buffer.
         */
        void setBody( const unsigned char* bytes, unsigned long long numBytes ){
            
            // Remove old data
            body.clear();
           
            // Copy data to internal buffer.
            for( long long ix = 0; ix < numBytes; ++ix )
            {
                body.push_back(bytes[ix]);
            }
        }   
        
    private:

        // String Name of this command.
        std::string command;

        // Properties of the Stomp Message
        util::SimpleProperties properties;

        // Byte data of Body.
        std::vector<unsigned char> body;         
    };
    
}}}

#endif /*ACTIVEMQ_CONNECTOR_STOMP_STOMPFRAMEWRAPPER_H_*/
