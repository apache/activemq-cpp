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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/connector/openwire/commands/ActiveMQMessage.h>
#include <cms/TextMessage.h>
#include <vector>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{
namespace commands{

    class ActiveMQTextMessage : public ActiveMQMessage,
                                public cms::TextMessage
    {
    public:

        const static unsigned char ID_ACTIVEMQTEXTMESSAGE = 28;

    public:

        ActiveMQTextMessage();
        virtual ~ActiveMQTextMessage();

        virtual unsigned char getDataStructureType() const;

    public:   // CMS Message
    
        /**
         * Clonse this message exactly, returns a new instance that the
         * caller is required to delete.
         * @return new copy of this message
         */
        virtual cms::Message* clone(void) const;
        
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
        virtual const char* getCMSCorrelationId(void) const;

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
        virtual const char* getCMSMessageId(void) const;
      
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
        virtual const char* getCMSReplyTo(void) const;
      
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
        virtual const char* getCMSMessageType(void) const;
      
        /**
         * Sets the CMS Message Type for this message
         * @param type - message type value string
         */
        virtual void setCMSMessageType( const std::string& type );

    public:   // CMS TextMessage
    
        /**
         * Gets the message character buffer.
         * @return The message character buffer.
         */
        virtual const char* getText() const throw( cms::CMSException );

        /**
         * Sets the message contents, does not take ownership of the passed
         * char*, but copies it instead.  
         * @param msg The message buffer.
         */
        virtual void setText( const char* msg ) throw( cms::CMSException );

        /**
         * Sets the message contents
         * @param msg The message buffer.
         */
        virtual void setText( const std::string& msg ) throw( cms::CMSException );

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_COMMANDS_ACTIVEMQTEXTMESSAGE_H_*/

