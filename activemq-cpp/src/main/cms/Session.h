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
#include <cms/StreamMessage.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/QueueBrowser.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * A Session object is a single-threaded context for producing and consuming
     * messages.
     *
     * A session serves several purposes:
     *
     *  - It is a factory for its message producers and consumers.
     *  - It supplies provider-optimized message factories.
     *  - It is a factory for TemporaryTopics and TemporaryQueues.
     *  - It provides a way to create Queue or Topic objects for those clients
     *    that need to dynamically manipulate provider-specific destination
     *    names.
     *  - It supports a single series of transactions that combine work spanning
     *    its producers and consumers into atomic units.
     *  - It defines a serial order for the messages it consumes and the messages
     *    it produces.
     *  - It retains messages it consumes until they have been acknowledged.
     *  - It serializes execution of message listeners registered with its message
     *    consumers.
     *
     * A session can create and service multiple message producers and consumers.
     *
     * One typical use is to have a thread block on a synchronous MessageConsumer until
     * a message arrives. The thread may then use one or more of the Session's
     * MessageProducers.
     *
     * If a client desires to have one thread produce messages while others consume
     * them, the client should use a separate session for its producing thread.
     *
     * Certain rules apply to a session's <code>close</code> method and are detailed
     * below.
     *
     *  - There is no need to close the producers and consumers of a closed session.
     *  - The close call will block until a receive call or message listener in progress
     *    has completed. A blocked message consumer receive call returns null when this
     *    session is closed.
     *  - Closing a transacted session must roll back the transaction in progress.
     *  - The close method is the only Session method that can be called concurrently.
     *  - Invoking any other Session method on a closed session must throw an
     *    IllegalStateException. Closing a closed session must not throw  any exceptions.
     *
     * <B>Transacted Sessions</B>
     *
     * When a Session is created it can be set to operate in a Transaction based mode.  Each
     * Session then operates in a single transaction for all Producers and Consumers of that
     * Session.  Messages sent and received within a Transaction are grouped into an atomic
     * unit that is committed or rolled back together.
     *
     * For a MessageProducer this implies that all messages sent by the producer are not sent
     * to the Provider unit the commit call is made.  Rolling back the Transaction results in
     * all produced Messages being dropped.
     *
     * For a MessageConsumer this implies that all received messages are not Acknowledged until
     * the Commit call is made.  Rolling back the Transaction results in all Consumed Message
     * being redelivered to the client, the Provider may allow configuration that limits the
     * Maximum number of redeliveries for a Message.
     *
     * @since 1.0
     */
    class CMS_API Session : public Closeable {
    public:

        enum AcknowledgeMode {

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
             * process the message successfully returns.  Acknowledgments
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
            SESSION_TRANSACTED,

            /**
             * Message will be acknowledged individually.  Normally the acks sent
             * acknowledge the given message and all messages received before it, this
             * mode only acknowledges one message.
             */
            INDIVIDUAL_ACKNOWLEDGE

        };

    public:

        virtual ~Session() throw();

        /**
         * Closes this session as well as any active child consumers or
         * producers.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual void close() = 0;

        /**
         * Commits all messages done in this transaction and releases any
         * locks currently held.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws IllegalStateException - if the method is not called by a transacted session.
         */
        virtual void commit() = 0;

        /**
         * Rolls back all messages done in this transaction and releases any
         * locks currently held.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws IllegalStateException - if the method is not called by a transacted session.
         */
        virtual void rollback() = 0;

        /**
         * Stops message delivery in this session, and restarts message delivery with the
         * oldest unacknowledged message.
         *
         * All consumers deliver messages in a serial order. Acknowledging a received message
         * automatically acknowledges all messages that have been delivered to the client.
         *
         * Restarting a session causes it to take the following actions:
         *
         *  - Stop message delivery
         *  - Mark all messages that might have been delivered but not acknowledged
         *    as "redelivered"
         *  - Restart the delivery sequence including all unacknowledged messages that had
         *    been previously delivered.  Redelivered messages do not have to be delivered in
         *    exactly their original delivery order.
         *
         * @throws CMSException - if the CMS provider fails to stop and restart message
         *                        delivery due to some internal error.
         * @throws IllegalStateException - if the method is called by a transacted session.
         */
        virtual void recover() = 0;

        /**
         * Creates a MessageConsumer for the specified destination.
         *
         * @param destination
         *      the Destination that this consumer receiving messages for.
         * @return pointer to a new MessageConsumer that is owned by the
         *         caller ( caller deletes )
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if an invalid destination is specified.
         */
        virtual MessageConsumer* createConsumer( const Destination* destination ) = 0;

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
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if an invalid destination is specified.
         * @throws InvalidSelectorException - if the message selector is invalid.
         */
        virtual MessageConsumer* createConsumer( const Destination* destination,
                                                 const std::string& selector ) = 0;

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
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if an invalid destination is specified.
         * @throws InvalidSelectorException - if the message selector is invalid.
         */
        virtual MessageConsumer* createConsumer( const Destination* destination,
                                                 const std::string& selector,
                                                 bool noLocal ) = 0;

        /**
         * Creates a durable subscriber to the specified topic, using a Message
         * selector.  Sessions that create durable consumers must use the same client Id
         * as was used the last time the subscription was created in order to receive
         * all messages that were delivered while the client was offline.
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
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if an invalid destination is specified.
         * @throws InvalidSelectorException - if the message selector is invalid.
         */
        virtual MessageConsumer* createDurableConsumer( const Topic* destination,
                                                        const std::string& name,
                                                        const std::string& selector,
                                                        bool noLocal = false ) = 0;

        /**
         * Creates a MessageProducer to send messages to the specified
         * destination.
         *
         * @param destination
         *      the Destination to send on
         * @return New MessageProducer that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if an invalid destination is specified.
         */
        virtual MessageProducer* createProducer( const Destination* destination = NULL ) = 0;

        /**
         * Creates a new QueueBrowser to peek at Messages on the given Queue.
         *
         * @param queue
         *      the Queue to browse
         * @return New QueueBrowser that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if the destination given is invalid.
         */
        virtual QueueBrowser* createBrowser( const cms::Queue* queue ) = 0;

        /**
         * Creates a new QueueBrowser to peek at Messages on the given Queue.
         *
         * @param queue
         *      the Queue to browse
         * @param selector
         *      the Message selector to filter which messages are browsed.
         * @return New QueueBrowser that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         * @throws InvalidDestinationException - if the destination given is invalid.
         */
        virtual QueueBrowser* createBrowser( const cms::Queue* queue, const std::string& selector ) = 0;

        /**
         * Creates a queue identity given a Queue name.
         *
         * @param queueName
         *      the name of the new Queue
         * @return new Queue pointer that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Queue* createQueue( const std::string& queueName ) = 0;

        /**
         * Creates a topic identity given a Queue name.
         *
         * @param topicName
         *      the name of the new Topic
         * @return new Topic pointer that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Topic* createTopic( const std::string& topicName ) = 0;

        /**
         * Creates a TemporaryQueue object.
         *
         * @return new TemporaryQueue pointer that is owned by the caller.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual TemporaryQueue* createTemporaryQueue() = 0;

        /**
         * Creates a TemporaryTopic object.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual TemporaryTopic* createTemporaryTopic() = 0;

        /**
         * Creates a new Message
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Message* createMessage() = 0;

        /**
         * Creates a BytesMessage
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual BytesMessage* createBytesMessage() = 0;

        /**
         * Creates a BytesMessage and sets the payload to the passed value
         *
         * @param bytes
         *      an array of bytes to set in the message
         * @param bytesSize
         *      the size of the bytes array, or number of bytes to use
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual BytesMessage* createBytesMessage( const unsigned char* bytes, int bytesSize ) = 0;

        /**
         * Creates a new StreamMessage
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual StreamMessage* createStreamMessage() = 0;

        /**
         * Creates a new TextMessage
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual TextMessage* createTextMessage() = 0;

        /**
         * Creates a new TextMessage and set the text to the value given
         *
         * @param text
         *      the initial text for the message
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual TextMessage* createTextMessage( const std::string& text ) = 0;

        /**
         * Creates a new MapMessage
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual MapMessage* createMapMessage() = 0;

        /**
         * Returns the acknowledgment mode of the session.
         *
         * @return the Sessions Acknowledge Mode
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual AcknowledgeMode getAcknowledgeMode() const = 0;

        /**
         * Gets if the Sessions is a Transacted Session
         *
         * @return transacted true - false.
         *
         * @throws CMSException - If an internal error occurs.
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
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual void unsubscribe( const std::string& name ) = 0;

    };

}

#endif /*_CMS_SESSION_H_*/
