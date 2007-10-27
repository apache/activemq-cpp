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
#ifndef _CMS_SESSION_H_
#define _CMS_SESSION_H_

#include <cms/Config.h>
#include <cms/Closeable.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/CMSException.h>

namespace cms
{

    /**
     * A Session object is a single-threaded context for producing and consuming 
     * messages.<br>
     * <br>
     * A session serves several purposes:<br>
     * <br>
     *  - It is a factory for its message producers and consumers.<br>
     *  - It supplies provider-optimized message factories.<br>
     *  - It is a factory for TemporaryTopics and TemporaryQueues.<br>
     *  - It provides a way to create Queue or Topic objects for those clients 
     *    that need to dynamically manipulate provider-specific destination 
     *    names.<br>
     *  - It supports a single series of transactions that combine work spanning 
     *    its producers and consumers into atomic units.<br>
     *  - It defines a serial order for the messages it consumes and the messages 
     *    it produces.<br>
     *  - It retains messages it consumes until they have been acknowledged.<br>
     *  - It serializes execution of message listeners registered with its message 
     *    consumers.<br>
     */
    class CMS_API Session : public Closeable
    {
    public:

        enum AcknowledgeMode
        {
            /**
             * With this acknowledgment mode, the session automatically
             * acknowledges a client's receipt of a message either when
             * the session has successfully returned from a call to receive
             * or when the message listener the session has called to
             * process the message successfully returns.
             */
            AUTO_ACKNOWLEDGE,

            /**
             * With this acknowledgment mode, the session automatically
             * acknowledges a client's receipt of a message either when
             * the session has successfully returned from a call to receive
             * or when the message listener the session has called to
             * process the message successfully returns.  Acknowlegements
             * may be delayed in this mode to increase performance at
             * the cost of the message being redelivered this client fails.
             */
            DUPS_OK_ACKNOWLEDGE,

            /**
             * With this acknowledgment mode, the client acknowledges a
             * consumed message by calling the message's acknowledge method.
             */
            CLIENT_ACKNOWLEDGE,

            /**
             * Messages will be consumed when the transaction commits.
             */
            SESSION_TRANSACTED
                     
        };

    public:

        virtual ~Session() {}

        /**
         * Closes this session as well as any active child consumers or
         * producers.
         * 
         * @throws CMSException
         */
        virtual void close() throw( CMSException ) = 0;
        
        /**
         * Commits all messages done in this transaction and releases any 
         * locks currently held.
         * 
         * @throws CMSException
         */
        virtual void commit() throw ( CMSException ) = 0;

        /**
         * Rollsback all messages done in this transaction and releases any 
         * locks currently held.
         * 
         * @throws CMSException
         */
        virtual void rollback() throw ( CMSException ) = 0;

        /**
         * Creates a MessageConsumer for the specified destination.
         * 
         * @param destination
         *      the Destination that this consumer receiving messages for.
         * @return pointer to a new MessageConsumer that is owned by the 
         *         caller ( caller deletes )
         * @throws CMSException
         */
        virtual MessageConsumer* createConsumer(
            const Destination* destination )
                throw ( CMSException ) = 0;

        /**
         * Creates a MessageConsumer for the specified destination, using a 
         * message selector.
         * 
         * @param destination
         *      the Destination that this consumer receiving messages for.
         * @param selector
         *      the Message Selector to use
         * @return pointer to a new MessageConsumer that is owned by the 
         *         caller ( caller deletes )
         * @throws CMSException
         */
        virtual MessageConsumer* createConsumer( 
            const Destination* destination,
            const std::string& selector )
                throw ( CMSException ) = 0;

        /**
         * Creates a MessageConsumer for the specified destination, using a 
         * message selector.
         * 
         * @param destination
         *      the Destination that this consumer receiving messages for.
         * @param selector
         *      the Message Selector to use
         * @param noLocal
         *      if true, and the destination is a topic, inhibits the 
         *      delivery of messages published by its own connection. The behavior 
         *      for NoLocal is not specified if the destination is a queue. 
         * @return pointer to a new MessageConsumer that is owned by the 
         *         caller ( caller deletes )
         * @throws CMSException
         */
        virtual MessageConsumer* createConsumer( 
            const Destination* destination,
            const std::string& selector,
            bool noLocal )
                throw ( CMSException ) = 0;

        /**
         * Creates a durable subscriber to the specified topic, using a 
         * message selector
         * 
         * @param destination
         *      the topic to subscribe to
         * @param name
         *      The name used to identify the subscription
         * @param selector
         *      the Message Selector to use
         * @param noLocal
         *      if true, and the destination is a topic, inhibits the 
         *      delivery of messages published by its own connection. The behavior 
         *      for NoLocal is not specified if the destination is a queue. 
         * @return pointer to a new durable MessageConsumer that is owned by 
         *         the caller ( caller deletes )
         * @throws CMSException
         */
        virtual MessageConsumer* createDurableConsumer(
            const Topic* destination,
            const std::string& name,
            const std::string& selector,
            bool noLocal = false )
                throw ( CMSException ) = 0;

        /**
         * Creates a MessageProducer to send messages to the specified 
         * destination.
         * 
         * @param destination
         *      the Destination to send on
         * @return New MessageProducer that is owned by the caller.
         * @throws CMSException
         */
        virtual MessageProducer* createProducer( const Destination* destination )
            throw ( CMSException ) = 0;

        /**
         * Creates a queue identity given a Queue name.
         * 
         * @param queueName
         *      the name of the new Queue
         * @return new Queue pointer that is owned by the caller.
         * @throws CMSException
         */
        virtual Queue* createQueue( const std::string& queueName )
            throw ( CMSException ) = 0;

        /**
         * Creates a topic identity given a Queue name.
         * 
         * @param topicName
         *      the name of the new Topic
         * @return new Topic pointer that is owned by the caller.
         * @throws CMSException
         */
        virtual Topic* createTopic( const std::string& topicName )
            throw ( CMSException ) = 0;

        /**
         * Creates a TemporaryQueue object.
         * 
         * @return new TemporaryQueue pointer that is owned by the caller.
         * @throws CMSException
         */
        virtual TemporaryQueue* createTemporaryQueue()
            throw ( CMSException ) = 0;

        /**
         * Creates a TemporaryTopic object.
         * 
         * @throws CMSException
         */
        virtual TemporaryTopic* createTemporaryTopic()
            throw ( CMSException ) = 0;

        /**
         * Creates a new Message
         * 
         * @throws CMSException
         */
        virtual Message* createMessage() 
            throw ( CMSException ) = 0;

        /**
         * Creates a BytesMessage
         * 
         * @throws CMSException
         */
        virtual BytesMessage* createBytesMessage() 
            throw ( CMSException) = 0;

        /**
         * Creates a BytesMessage and sets the paylod to the passed value
         * 
         * @param bytes
         *      an array of bytes to set in the message
         * @param bytesSize
         *      the size of the bytes array, or number of bytes to use
         * @throws CMSException
         */
        virtual BytesMessage* createBytesMessage(
            const unsigned char* bytes,
            std::size_t bytesSize ) 
                throw ( CMSException) = 0;

        /**
         * Creates a new TextMessage
         * 
         * @throws CMSException
         */
        virtual TextMessage* createTextMessage() 
            throw ( CMSException ) = 0;

        /**
         * Creates a new TextMessage and set the text to the value given
         * 
         * @param text
         *      the initial text for the message
         * @throws CMSException
         */
        virtual TextMessage* createTextMessage( const std::string& text ) 
            throw ( CMSException ) = 0;

        /**
         * Creates a new MapMessage
         * 
         * @throws CMSException
         */
        virtual MapMessage* createMapMessage() 
            throw ( CMSException ) = 0;

        /**
         * Returns the acknowledgement mode of the session.
         * 
         * @return the Sessions Acknowledge Mode
         */
        virtual AcknowledgeMode getAcknowledgeMode() const = 0;

        /**
         * Gets if the Sessions is a Transacted Session
         * 
         * @return transacted true - false.
         */
        virtual bool isTransacted() const = 0;
        
        /**
         * Unsubscribes a durable subscription that has been created by a 
         * client.
         * 
         * This method deletes the state being maintained on behalf of the 
         * subscriber by its provider.  It is erroneous for a client to delete a 
         * durable subscription while there is an active MessageConsumer or 
         * Subscriber for the subscription, or while a consumed message is 
         * part of a pending transaction or has not been acknowledged in the 
         * session.
         * 
         * @param name 
         *      The name used to identify this subscription
         * @throws CMSException
         */
        virtual void unsubscribe( const std::string& name ) 
            throw ( CMSException ) = 0;

    };

}

#endif /*_CMS_SESSION_H_*/
