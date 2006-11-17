/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>
#include <cms/BytesMessage.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQBytesMessage : public ActiveMQMessage,
                                 public cms::BytesMessage
    {
    public:

        const static unsigned char ID_ACTIVEMQBYTESMESSAGE = 24;

    public:

        ActiveMQBytesMessage();
        virtual ~ActiveMQBytesMessage();

        virtual unsigned char getDataStructureType() const;

        /**
         * Clone this object and return a new instance that the
         * caller now owns, this will be an exact copy of this one
         * @returns new copy of this object.
         */
        virtual DataStructure* cloneDataStructure() const { 
            ActiveMQBytesMessage* message = new ActiveMQBytesMessage();
            message->copyDataStructure( this );            
            return message;
        }

        /**
         * Copy the contents of the passed object into this objects
         * members, overwriting any existing data.
         * @return src - Source Object
         */
        virtual void copyDataStructure( const DataStructure* src ) {
            ActiveMQMessage::copyDataStructure( src );
        }
        
    public:   // CMS Message
    
        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::Message* clone(void) const {
            return dynamic_cast<cms::Message*>( 
                this->cloneDataStructure() );
        }
        
        /**
         * Acknowledges all consumed messages of the session 
         * of this consumed message.
         */
        virtual void acknowledge(void) const throw( cms::CMSException );
      
        /**
         * Retrieves a reference to the properties object owned
         * by this message
         * @return A Properties Object reference
         */
        virtual activemq::util::Properties& getProperties(void);
        virtual const activemq::util::Properties& getProperties(void) const;
      
        /**
         * Get the Correlation Id for this message
         * @return string representation of the correlation Id
         */
        virtual std::string getCMSCorrelationId(void) const;

        /**
         * Sets the Correlation Id used by this message
         * @param correlationId - String representing the correlation id.
         */
        virtual void setCMSCorrelationId( const std::string& correlationId );

        /**
         * Gets the DeliveryMode for this message
         * @return DeliveryMode enumerated value.
         */
        virtual int getCMSDeliveryMode(void) const;

        /**
         * Sets the DeliveryMode for this message
         * @param mode - DeliveryMode enumerated value.
         */
        virtual void setCMSDeliveryMode( int mode );
      
        /**
         * Gets the Destination for this Message, returns a
         * @return Destination object
         */
        virtual const cms::Destination* getCMSDestination(void) const;
      
        /**
         * Sets the Destination for this message
         * @param destination - Destination Object
         */
        virtual void setCMSDestination( const cms::Destination* destination );
      
        /**
         * Gets the Expiration Time for this Message
         * @return time value
         */
        virtual long getCMSExpiration(void) const;
      
        /**
         * Sets the Expiration Time for this message
         * @param expireTime - time value
         */
        virtual void setCMSExpiration( long expireTime );
      
        /**
         * Gets the CMS Message Id for this Message
         * @return time value
         */
        virtual std::string getCMSMessageId(void) const;
      
        /**
         * Sets the CMS Message Id for this message
         * @param id - time value
         */
        virtual void setCMSMessageId( const std::string& id );
      
        /**
         * Gets the Priority Value for this Message
         * @return priority value
         */
        virtual int getCMSPriority(void) const;
      
        /**
         * Sets the Priority Value for this message
         * @param priority - priority value for this message
         */
        virtual void setCMSPriority( int priority );

        /**
         * Gets the Redelivered Flag for this Message
         * @return redelivered value
         */
        virtual bool getCMSRedelivered(void) const;
      
        /**
         * Sets the Redelivered Flag for this message
         * @param redelivered - boolean redelivered value
         */
        virtual void setCMSRedelivered( bool redelivered );

        /**
         * Gets the CMS Reply To Address for this Message
         * @return Reply To Value
         */
        virtual std::string getCMSReplyTo(void) const;
      
        /**
         * Sets the CMS Reply To Address for this message
         * @param id - Reply To value
         */
        virtual void setCMSReplyTo( const std::string& id );

        /**
         * Gets the Time Stamp for this Message
         * @return time stamp value
         */
        virtual long getCMSTimeStamp(void) const;
      
        /**
         * Sets the Time Stamp for this message
         * @param timeStamp - integer time stamp value
         */
        virtual void setCMSTimeStamp( long timeStamp );

        /**
         * Gets the CMS Message Type for this Message
         * @return type value
         */
        virtual std::string getCMSMessageType(void) const;
      
        /**
         * Sets the CMS Message Type for this message
         * @param type - message type value string
         */
        virtual void setCMSMessageType( const std::string& type );

    public:   // CMS BytesMessage
    
        /**
         * sets the bytes given to the message body.  
         * @param Byte Buffer to copy
         * @param Number of bytes in Buffer to copy
         * @throws CMSException
         */
        virtual void setBodyBytes( 
            const unsigned char* buffer, const unsigned long numBytes ) 
                throw( cms::CMSException );
            
        /**
         * Gets the bytes that are contained in this message, user should
         * copy this data into a user allocated buffer.  Call 
         * <code>getBodyLength</code> to determine the number of bytes
         * to expect.
         * @return const pointer to a byte buffer
         */
        virtual const unsigned char* getBodyBytes(void) const;
      
        /**
         * Returns the number of bytes contained in the body of this message.
         * @return number of bytes.
         */
        virtual unsigned long getBodyLength(void) const;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQBYTESMESSAGE_H_*/

