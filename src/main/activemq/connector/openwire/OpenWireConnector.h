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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_

#include <activemq/connector/Connector.h>

#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/exceptions/IllegalArgumentException.h>

#include <activemq/transport/Transport.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/connector/ProducerInfo.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/connector/ConnectorException.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/core/ActiveMQConstants.h>

#include <activemq/connector/openwire/OpenWireCommandReader.h>
#include <activemq/connector/openwire/OpenWireCommandWriter.h>
#include <activemq/connector/openwire/OpenWireFormat.h>
#include <activemq/connector/openwire/OpenWireFormatNegotiator.h>

#include <activemq/connector/openwire/commands/ConnectionInfo.h>
#include <activemq/connector/openwire/commands/BrokerInfo.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>

namespace activemq{
namespace connector{
namespace openwire{

    class OpenWireConnector :
        public Connector,
        public transport::CommandListener,
        public transport::TransportExceptionListener
    {
    private:

        // Flags the state we are in for connection to broker.
        enum connectionState
        {
            DISCONNECTED,
            CONNECTING,
            CONNECTED
        };

    private:

        /**
         * The transport for sending/receiving commands on the wire.
         */
        transport::Transport* transport;

        /**
         * The OpenWireFormat class that controls Protocal versions and
         * marshalling details.
         */
        OpenWireFormat* wireFormat;

        /**
         * Connection Information for this connection to the Broker
         */
        commands::ConnectionInfo connectionInfo;

        /**
         * Command sent from the Broker with its BrokerInfo
         */
        commands::BrokerInfo* brokerInfo;

        /**
         * Command sent from the Broker with its WireFormatInfo
         */
        commands::WireFormatInfo* brokerWireFormatInfo;

        /**
         * Flag to indicate the start state of the connector.
         */
        connectionState state;

        /**
         * Sync object.
         */
        concurrent::Mutex mutex;

        /**
         * Observer of messages directed at a particular
         * consumer.
         */
        ConsumerMessageListener* messageListener;

        /**
         * Observer of connector exceptions.
         */
        cms::ExceptionListener* exceptionListener;

        /**
         * This Connector's Command Reader
         */
        OpenWireCommandReader reader;

        /**
         * This Connector's Command Writer
         */
        OpenWireCommandWriter writer;

        /**
         * Next avaliable Producer Id
         */
        unsigned int nextProducerId;

        /**
         * Next avaliable Transaction Id
         */
        unsigned int nextTransactionId;

        /**
         * Properties for the connector.
         */
        util::SimpleProperties properties;

    private:

        /**
         * Sends the connect message to the broker and
         * waits for the response.
         */
        void connect(void);

        /**
         * Sends a oneway disconnect message to the broker.
         */
        void disconnect(void);

        /**
         * Fires a consumer message to the observer.
         * @param msg - the message to file
         */
        void fire( ConsumerInfo* consumer, core::ActiveMQMessage* msg ){
            try{
                if( messageListener != NULL ){
                    messageListener->onConsumerMessage(
                        consumer,
                        msg );
                }
            }catch( ... ){/* do nothing*/}
        }

        /**
         * Fires an exception event to the observing object.
         * @param ex - the exception to fire.
         */
        void fire( const exceptions::ActiveMQException& ex ){
            try{
                if( exceptionListener != NULL ){
                    exceptionListener->onException( ex );
                }
            }catch( ... ){/* do nothing*/}
        }

    public:

        /**
         * Constructor for the OpenWire connector.
         * @param transport - the transport object for sending/receiving
         *                    commands on the wire.
         * @param properties - properties for configuring the connector.
         */
        OpenWireConnector( transport::Transport* transport,
                           const util::Properties& properties )
            throw ( exceptions::IllegalArgumentException );

        virtual ~OpenWireConnector();

        /**
         * Starts the service.
         * @throws CMSException
         */
        virtual void start(void) throw( cms::CMSException );

        /**
         * Closes this object and deallocates the appropriate resources.
         * @throws CMSException
         */
        virtual void close(void) throw( cms::CMSException );

        /**
         * Gets the Client Id for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Client Id String
         */
        virtual std::string getClientId(void) const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_CLIENTID ), "" );
        }

        /**
         * Gets the Username for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Username String
         */
        virtual std::string getUsername(void) const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_USERNAME ), "" );
        }

        /**
         * Gets the Password for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Password String
         */
        virtual std::string getPassword(void) const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_PASSWORD ), "" );
        }

        /**
         * Gets a reference to the Transport that this connection
         * is using.
         * @param reference to a transport
         * @throws InvalidStateException if the Transport is not set
         */
        virtual transport::Transport& getTransport(void) const
            throw ( exceptions::InvalidStateException ) {

            if( transport == NULL ) {
                throw exceptions::InvalidStateException(
                    __FILE__, __LINE__,
                    "OpenWireConnector::getTransport - "
                    "Invalid State, No Transport.");
            }

            return *transport;
        }

        /**
         * Creates a Session Info object for this connector
         * @param Acknowledgement Mode of the Session
         * @returns Session Info Object
         * @throws ConnectorException
         */
        virtual SessionInfo* createSession(
            cms::Session::AcknowledgeMode ackMode )
                throw( ConnectorException );

        /**
         * Create a Consumer for the given Session
         * @param Destination to Subscribe to.
         * @param Session Information.
         * @param Message Selector
         * @param No Local redelivery indicator
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createConsumer(
            const cms::Destination* destination,
            SessionInfo* session,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException );

        /**
         * Create a Durable Consumer for the given Session
         * @param Topic to Subscribe to.
         * @param Session Information.
         * @param name of the Durable Topic
         * @param Selector
         * @param if set, inhibits the delivery of messages
         *        published by its own connection
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createDurableConsumer(
            const cms::Topic* topic,
            SessionInfo* session,
            const std::string& name,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException );

        /**
         * Create a Consumer for the given Session
         * @param Destination to Subscribe to.
         * @param Session Information.
         * @return Producer Information
         * @throws ConnectorException
         */
        virtual ProducerInfo* createProducer(
            const cms::Destination* destination,
            SessionInfo* session)
                throw ( ConnectorException );

        /**
         * Creates a Topic given a name and session info
         * @param Topic Name
         * @param Session Information
         * @return a newly created Topic Object
         * @throws ConnectorException
         */
        virtual cms::Topic* createTopic( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Queue given a name and session info
         * @param Queue Name
         * @param Session Information
         * @return a newly created Queue Object
         * @throws ConnectorException
         */
        virtual cms::Queue* createQueue( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Temporary Topic given a name and session info
         * @param Temporary Topic Name
         * @param Session Information
         * @return a newly created Temporary Topic Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryTopic* createTemporaryTopic(
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Creates a Temporary Queue given a name and session info
         * @param Temporary Queue Name
         * @param Session Information
         * @return a newly created Temporary Queue Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryQueue* createTemporaryQueue(
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Sends a Message
         * @param The Message to send.
         * @param Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( cms::Message* message, ProducerInfo* producerInfo )
            throw ( ConnectorException );

        /**
         * Sends a set of Messages
         * @param List of Messages to send.
         * @param Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( std::list<cms::Message*>& messages,
                           ProducerInfo* producerInfo )
            throw ( ConnectorException );

        /**
         * Acknowledges a Message
         * @param An ActiveMQMessage to Ack.
         * @throws ConnectorException
         */
        virtual void acknowledge( const SessionInfo* session,
                                  const cms::Message* message,
                                  AckType ackType )
            throw ( ConnectorException );

        /**
         * Starts a new Transaction.
         * @param Session Information
         * @throws ConnectorException
         */
        virtual TransactionInfo* startTransaction(
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Commits a Transaction.
         * @param The Transaction information
         * @param Session Information
         * @throws ConnectorException
         */
        virtual void commit( TransactionInfo* transaction,
                             SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Rolls back a Transaction.
         * @param The Transaction information
         * @param Session Information
         * @throws ConnectorException
         */
        virtual void rollback( TransactionInfo* transaction,
                               SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a new Message.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::Message* createMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new BytesMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::BytesMessage* createBytesMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new TextMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::TextMessage* createTextMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new MapMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::MapMessage* createMapMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Unsubscribe from a givenDurable Subscription
         * @param name of the Subscription
         * @throws ConnectorException
         */
        virtual void unsubscribe( const std::string& name )
            throw ( ConnectorException );

        /**
         * Destroys the given connector resource.
         * @param resource the resource to be destroyed.
         * @throws ConnectorException
         */
        virtual void destroyResource( ConnectorResource* resource )
            throw ( ConnectorException );

        /**
         * Sets the listener of consumer messages.
         * @param listener the observer.
         */
        virtual void setConsumerMessageListener(
            ConsumerMessageListener* listener )
        {
            this->messageListener = listener;
        }

        /**
         * Sets the Listner of exceptions for this connector
         * @param ExceptionListener the observer.
         */
        virtual void setExceptionListener(
            cms::ExceptionListener* listener )
        {
            this->exceptionListener = listener;
        }

    public: // transport::CommandListener

        /**
         * Event handler for the receipt of a non-response command from the
         * transport.
         * @param command the received command object.
         */
        virtual void onCommand( transport::Command* command );

    public: // TransportExceptionListener

        /**
         * Event handler for an exception from a command transport.
         * @param source The source of the exception
         * @param ex The exception.
         */
        virtual void onTransportException(
            transport::Transport* source,
            const exceptions::ActiveMQException& ex );

    private:

        unsigned int getNextProducerId(void);
        unsigned int getNextTransactionId(void);

        // Check for Connected State and Throw an exception if not.
        void enforceConnected( void ) throw ( ConnectorException );

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_*/
