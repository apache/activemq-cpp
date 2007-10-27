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

#ifndef _CMS_MESSAGE_H_
#define _CMS_MESSAGE_H_

#include <cms/Config.h>
#include <cms/Destination.h>
#include <cms/CMSException.h>
#include <cms/DeliveryMode.h>

namespace cms{
   
    /**
     * Root of all messages.  As in JMS, a message is comprised of
     * 3 parts: CMS-specific headers, user-defined properties, and
     * the body.
     * 
     * @see JMS API
     */
    class CMS_API Message
    {         
    public:
   
        virtual ~Message(){}
      
        /**
         * Clone this message exactly, returns a new instance that the
         * caller is required to delete.
         * 
         * @return new copy of this message
         */
        virtual Message* clone() const = 0;                
        
        /**
         * Acknowledges all consumed messages of the session of this consumed 
         * message.
         * 
         * All consumed CMS messages support the acknowledge method for use 
         * when a client has specified that its CMS session's consumed messages 
         * are to be explicitly acknowledged. By invoking acknowledge on a 
         * consumed message, a client acknowledges all messages consumed by the 
         * session that the message was delivered to.
         * 
         * Calls to acknowledge are ignored for both transacted sessions and 
         * sessions specified to use implicit acknowledgement modes.
         * 
         * A client may individually acknowledge each message as it is consumed, 
         * or it may choose to acknowledge messages as an application-defined 
         * group (which is done by calling acknowledge on the last received 
         * message of the group, thereby acknowledging all messages consumed by 
         * the session.)
         * 
         * Messages that have been received but not acknowledged may be 
         * redelivered.
         */
        virtual void acknowledge() const throw( CMSException ) = 0;
      
        /**
         * Clears out the body of the message.  This does not clear the
         * headers or properties.
         */
        virtual void clearBody() = 0;
        
        /**
         * Clears out the message body. Clearing a message's body does not clear 
         * its header values or property entries.
         * 
         * If this message body was read-only, calling this method leaves the 
         * message body in the same state as an empty body in a newly created 
         * message.
         */
        virtual void clearProperties() = 0;
        
        /**
         * Retrieves the propery names.
         * 
         * @return The complete set of property names currently in this
         * message.
         */
        virtual std::vector<std::string> getPropertyNames() const = 0;
        
        /**
         * Indicates whether or not a given property exists.
         * 
         * @param name 
         *      The name of the property to look up.
         * @return True if the property exists in this message.
         */
        virtual bool propertyExists( const std::string& name ) const = 0;
        
        /**
         * Gets a boolean property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual bool getBooleanProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a byte property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual unsigned char getByteProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a double property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual double getDoubleProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a float property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual float getFloatProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a int property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual int getIntProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a long property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual long long getLongProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a short property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual short getShortProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
            
        /**
         * Gets a string property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @return The value for the named property.
         * @throws CMSException if the property does not exist.
         */
        virtual std::string getStringProperty( const std::string& name ) const 
            throw( CMSException ) = 0;
        
        /**
         * Sets a boolean property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setBooleanProperty( const std::string& name,
            bool value ) throw( CMSException ) = 0;
            
        /**
         * Sets a byte property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setByteProperty( const std::string& name,
            unsigned char value ) throw( CMSException ) = 0;
            
        /**
         * Sets a double property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setDoubleProperty( const std::string& name,
            double value ) throw( CMSException ) = 0;
            
        /**
         * Sets a float property.
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setFloatProperty( const std::string& name,
            float value ) throw( CMSException ) = 0;
            
        /**
         * Sets a int property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setIntProperty( const std::string& name,
            int value ) throw( CMSException ) = 0;
            
        /**
         * Sets a long property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setLongProperty( const std::string& name,
            long long value ) throw( CMSException ) = 0;
            
        /**
         * Sets a short property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setShortProperty( const std::string& name,
            short value ) throw( CMSException ) = 0;
            
        /**
         * Sets a string property.
         * 
         * @param name 
         *      The name of the property to retrieve.
         * @param value 
         *      The value for the named property.
         * @throws CMSException
         */
        virtual void setStringProperty( const std::string& name,
            const std::string& value ) throw( CMSException ) = 0;
      
        /**
         * Gets the correlation ID for the message.
         * 
         * This method is used to return correlation ID values that are either 
         * provider-specific message IDs or application-specific String values.
         * 
         * @return string representation of the correlation Id
         */
        virtual std::string getCMSCorrelationID() const = 0;

        /**
         * Sets the correlation ID for the message.
         * 
         * A client can use the CMSCorrelationID header field to link one 
         * message with another. A typical use is to link a response message 
         * with its request message.
         * 
         * CMSCorrelationID can hold one of the following:
         * 
         *  - A provider-specific message ID
         *  - An application-specific String
         *  - A provider-native byte[] value 
         * 
         * Since each message sent by a CMS provider is assigned a message ID 
         * value, it is convenient to link messages via message ID. All message 
         * ID values must start with the 'ID:' prefix.
         * 
         * In some cases, an application (made up of several clients) needs to 
         * use an application-specific value for linking messages. For instance, 
         * an application may use CMSCorrelationID to hold a value referencing 
         * some external information. Application-specified values must not 
         * start with the 'ID:' prefix; this is reserved for provider-generated 
         * message ID values.
         * 
         * If a provider supports the native concept of correlation ID, a CMS 
         * client may need to assign specific CMSCorrelationID values to match 
         * those expected by clients that do not use the CMS API. A byte[] value 
         * is used for this purpose. CMS providers without native correlation ID 
         * values are not required to support byte[] values. The use of a byte[] 
         * value for CMSCorrelationID is non-portable.
         * 
         * @param correlationId
         *      The message ID of a message being referred to.
         */
        virtual void setCMSCorrelationID( const std::string& correlationId ) = 0;

        /**
         * Gets the DeliveryMode for this message
         * 
         * @return DeliveryMode enumerated value.
         */
        virtual int getCMSDeliveryMode() const = 0;

        /**
         * Sets the DeliveryMode for this message
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the value for a message that has been received.
         * 
         * @param mode
         *      DeliveryMode enumerated value.
         */
        virtual void setCMSDeliveryMode( int mode ) = 0;
      
        /**
         * Gets the Destination object for this message.
         * 
         * The CMSDestination header field contains the destination to which the 
         * message is being sent.
         * 
         * When a message is sent, this field is ignored. After completion of 
         * the send or publish method, the field holds the destination specified 
         * by the method.
         * 
         * When a message is received, its CMSDestination value must be 
         * equivalent to the value assigned when it was sent.
         * 
         * @return Destination object
         */
        virtual const Destination* getCMSDestination() const = 0;
      
        /**
         * Sets the Destination object for this message.
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the value for a message that has been received.
         * 
         * @param destination
         *      Destination Object
         */
        virtual void setCMSDestination( const Destination* destination ) = 0;
      
        /**
         * Gets the message's expiration value.
         * 
         * When a message is sent, the CMSExpiration header field is left 
         * unassigned. After completion of the send or publish method, it holds 
         * the expiration time of the message. This is the sum of the 
         * time-to-live value specified by the client and the GMT at the time 
         * of the send or publish.
         * 
         * If the time-to-live is specified as zero, CMSExpiration is set to 
         * zero to indicate that the message does not expire.
         * 
         * When a message's expiration time is reached, a provider should 
         * discard it. The CMS API does not define any form of notification of 
         * message expiration.
         * 
         * Clients should not receive messages that have expired; however, the 
         * CMS API does not guarantee that this will not happen.
         * 
         * @return the time the message expires, which is the sum of the 
         * time-to-live value specified by the client and the GMT at the time 
         * of the send
         */
        virtual long long getCMSExpiration() const = 0;
      
        /**
         * Sets the message's expiration value.
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the value for a message that has been received.
         * 
         * @param expireTime 
         *      the message's expiration time
         */
        virtual void setCMSExpiration( long long expireTime ) = 0;
      
        /**
         * The CMSMessageID header field contains a value that uniquely 
         * identifies each message sent by a provider. 
         * 
         * When a message is sent, CMSMessageID can be ignored. When the 
         * send or publish method returns, it contains a provider-assigned 
         * value.
         * 
         * A CMSMessageID is a String value that should function as a unique 
         * key for identifying messages in a historical repository. The exact 
         * scope of uniqueness is provider-defined. It should at least cover 
         * all messages for a specific installation of a provider, where an 
         * installation is some connected set of message routers.
         * 
         * All CMSMessageID values must start with the prefix 'ID:'. Uniqueness 
         * of message ID values across different providers is not required.
         * 
         * Since message IDs take some effort to create and increase a 
         * message's size, some CMS providers may be able to optimize message 
         * overhead if they are given a hint that the message ID is not used by 
         * an application. By calling the MessageProducer.setDisableMessageID 
         * method, a CMS client enables this potential optimization for all 
         * messages sent by that message producer. If the CMS provider accepts 
         * this hint, these messages must have the message ID set to null; if 
         * the provider ignores the hint, the message ID must be set to its 
         * normal unique value.
         * 
         * @return provider-assigned message id
         */
        virtual std::string getCMSMessageID() const = 0;
        
        /**
         * Sets the message ID.
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the vaue for a message that has been received.
         * 
         * @param id
         *      the ID of the message
         */
        virtual void setCMSMessageID( const std::string& id ) = 0;
      
        /**
         * Gets the message priority level.
         * 
         * The CMS API defines ten levels of priority value, with 0 as the 
         * lowest priority and 9 as the highest. In addition, clients should 
         * consider priorities 0-4 as gradations of normal priority and 
         * priorities 5-9 as gradations of expedited priority.
         * 
         * The CMS API does not require that a provider strictly implement 
         * priority ordering of messages; however, it should do its best to 
         * deliver expedited messages ahead of normal messages.
         * 
         * @return priority value
         */
        virtual int getCMSPriority() const = 0;
      
        /**
         * Sets the Priority Value for this message
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the value for a message that has been received.
         * 
         * @param priority
         *      priority value for this message
         */
        virtual void setCMSPriority( int priority ) = 0;

        /**
         * Gets an indication of whether this message is being redelivered.
         * 
         * If a client receives a message with the CMSRedelivered field set, it 
         * is likely, but not guaranteed, that this message was delivered 
         * earlier but that its receipt was not acknowledged at that time.
         * 
         * @return true if this message is being redelivered
         */
        virtual bool getCMSRedelivered() const = 0;
      
        /**
         * Specifies whether this message is being redelivered.
         * 
         * This field is set at the time the message is delivered. This method 
         * can be used to change the value for a message that has been received.
         * 
         * @param redelivered
         *      boolean redelivered value
         */
        virtual void setCMSRedelivered( bool redelivered ) = 0;

        /**
         * Gets the Destination object to which a reply to this message should 
         * be sent.
         * 
         * @return Destination to which to send a response to this message
         */
        virtual const cms::Destination* getCMSReplyTo() const = 0;
      
        /**
         * Sets the Destination object to which a reply to this message should 
         * be sent.
         * 
         * The CMSReplyTo header field contains the destination where a reply 
         * to the current message should be sent. If it is null, no reply is 
         * expected. The destination may be either a Queue object or a Topic 
         * object.
         * 
         * Messages sent with a null CMSReplyTo value may be a notification of 
         * some event, or they may just be some data the sender thinks is of 
         * interest.
         * 
         * Messages with a CMSReplyTo value typically expect a response. A 
         * response is optional; it is up to the client to decide. These 
         * messages are called requests. A message sent in response to a 
         * request is called a reply.
         * 
         * In some cases a client may wish to match a request it sent earlier 
         * with a reply it has just received. The client can use the 
         * CMSCorrelationID header field for this purpose.
         * 
         * @param destination
         *      Destination to which to send a response to this message
         */
        virtual void setCMSReplyTo( const cms::Destination* destination ) = 0;

        /**
         * Gets the message timestamp.
         * 
         * The CMSTimestamp header field contains the time a message was handed 
         * off to a provider to be sent. It is not the time the message was 
         * actually transmitted, because the actual send may occur later due to 
         * transactions or other client-side queueing of messages.
         * 
         * When a message is sent, CMSTimestamp is ignored. When the send or 
         * publish method returns, it contains a time value somewhere in the 
         * interval between the call and the return. The value is in the format 
         * of a normal millis time value in the Java programming language.
         * 
         * Since timestamps take some effort to create and increase a message's 
         * size, some CMS providers may be able to optimize message overhead if 
         * they are given a hint that the timestamp is not used by an 
         * application. By calling the MessageProducer.setDisableMessageTimestamp 
         * method, a CMS client enables this potential optimization for all 
         * messages sent by that message producer. If the CMS provider accepts 
         * this hint, these messages must have the timestamp set to zero; if the 
         * provider ignores the hint, the timestamp must be set to its normal 
         * value.
         * 
         * @return the message timestamp
         */
        virtual long long getCMSTimestamp() const = 0;
      
        /**
         * Sets the message timestamp.
         * 
         * CMS providers set this field when a message is sent. This method can 
         * be used to change the value for a message that has been received.
         * 
         * @param timeStamp
         *      integer time stamp value
         */
        virtual void setCMSTimestamp( long long timeStamp ) = 0;

        /**
         * Gets the message type identifier supplied by the client when the 
         * message was sent.
         * 
         * @return the message type
         * @see setCMSType
         */
        virtual std::string getCMSType() const = 0;
      
        /**
         * Sets the message type.
         * 
         * Some CMS providers use a message repository that contains the 
         * definitions of messages sent by applications. The CMSType header 
         * field may reference a message's definition in the provider's 
         * repository.
         * 
         * The CMS API does not define a standard message definition repository, 
         * nor does it define a naming policy for the definitions it contains.
         * 
         * Some messaging systems require that a message type definition for 
         * each application message be created and that each message specify its 
         * type. In order to work with such CMS providers, CMS clients should 
         * assign a value to CMSType, whether the application makes use of it or 
         * not. This ensures that the field is properly set for those providers 
         * that require it.
         * 
         * To ensure portability, CMS clients should use symbolic values for 
         * CMSType that can be configured at installation time to the values 
         * defined in the current provider's message repository. If string 
         * literals are used, they may not be valid type names for some CMS 
         * providers.
         * 
         * @param type 
         *      the message type
         * @see getCMSType
         */
        virtual void setCMSType( const std::string& type ) = 0;
        
    };
}

#endif /*_CMS_MESSAGE_H_*/
