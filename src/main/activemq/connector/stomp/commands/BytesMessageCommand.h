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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_

#include <cms/BytesMessage.h>
#include <activemq/connector/stomp/commands/StompMessage.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/exceptions/IllegalStateException.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Implements the interface for a cms::BytesMessage.  Uses the template
     * class StompMessage to implement all cms::Message type functionality
     * and implements the BytesMessage interface here.
     */    
    class BytesMessageCommand : public StompMessage< cms::BytesMessage >
    {
    private:
    
        /**
         * Flag that indicates what state the stream is in.  If true, the
         * message may only be read from.  If false, the message may only be
         * written to.
         */
        bool readOnly;
        
    public:

        BytesMessageCommand() :
        StompMessage< cms::BytesMessage >() {
            initialize( getFrame() );
            readOnly = false;    
        }
        
        BytesMessageCommand( StompFrame* frame ) : 
        StompMessage< cms::BytesMessage >( frame ) {
            validate( getFrame() );
            readOnly = false;
        }
        
    	virtual ~BytesMessageCommand() {}

        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody(){
            
            // Invoke base class's version.
            StompMessage< cms::BytesMessage >::clearBody();
            
            // Set the stream in write only mode.
            readOnly = false;
        }
        
        /**
         * Puts the message body in read-only mode and repositions the stream 
         * of bytes to the beginning.
         * @throws CMSException
         */
        virtual void reset() throw ( cms::CMSException ){
            readOnly = true;
        }
        
        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::Message* clone() const {
            StompFrame* frame = getFrame().clone();
            
            return new BytesMessageCommand( frame );
        }   

        /**
         * sets the bytes given to the message body.  
         * @param buffer Byte Buffer to copy
         * @param numBytes Number of bytes in Buffer to copy
         * @throws CMSException
         */
        virtual void setBodyBytes( const unsigned char* buffer, 
                                   const unsigned long long numBytes ) 
            throw( cms::CMSException ) {
            
            if( readOnly ){
                throw exceptions::IllegalStateException( __FILE__, __LINE__, 
                    "message is in read-only mode and cannot be written to" );
            }
            
            this->setBytes( buffer, numBytes );
        }
        
        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call 
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes() const {
            const std::vector<unsigned char>& bytes = getBytes();
            if( bytes.size() == 0 ){
                return NULL;
            }
            
            return &this->getBytes()[0];
        }
      
        /**
         * Returns the number of bytes contained in the body of this message.
         * @return number of bytes.
         */
        virtual unsigned long long getBodyLength() const {
            return this->getNumBytes();
        }               
        
    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_BYTESMESSAGECOMMAND_H_*/
