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

#ifndef _ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_
#define _ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/transport/mock/ResponseBuilder.h>
#include <activemq/transport/mock/InternalCommandListener.h>
#include <activemq/wireformat/WireFormat.h>

#include <decaf/lang/Thread.h>
#include <decaf/lang/Pointer.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <cms/Message.h>

#include <map>
#include <set>

namespace activemq{
namespace transport{
namespace mock{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * The MockTransport defines a base level Transport class that is intended to
     * be used in place of an a regular protocol Transport suck as TCP.  This
     * Transport assumes that it is the base Transport in the Transports stack, and
     * destroys any Transports that are passed to it in its constructor.
     *
     * This Transport defines an Interface ResponseBuilder which must be implemented
     * by any protocol for which the Transport is used to Emulate.  The Transport
     * hands off all outbound commands to the ResponseBuilder for processing, it is
     * up to the builder to create appropriate responses and schedule any asynchronous
     * messages that might result from a message sent to the Broker.
     */
    class AMQCPP_API MockTransport : public Transport {
    private:

        Pointer<ResponseBuilder> responseBuilder;
        Pointer<wireformat::WireFormat> wireFormat;
        TransportListener* outgoingListener;
        TransportListener* listener;
        decaf::util::concurrent::atomic::AtomicInteger nextCommandId;
        InternalCommandListener internalListener;
        static MockTransport* instance;

        std::string name;

        bool failOnSendMessage;
        int numSentMessageBeforeFail;
        int numSentMessages;
        bool failOnReceiveMessage;
        int numReceivedMessageBeforeFail;
        int numReceivedMessages;
        bool failOnKeepAliveSends;
        int numSentKeepAlivesBeforeFail;
        int numSentKeepAlives;

        bool failOnStart;
        bool failOnStop;
        bool failOnClose;

    private:

        MockTransport( const MockTransport& );
        MockTransport operator= ( const MockTransport& );

    public:

        MockTransport( const Pointer<wireformat::WireFormat>& wireFormat,
                       const Pointer<ResponseBuilder>& responseBuilder );

        virtual ~MockTransport() {}

        static MockTransport* getInstance() {
            return instance;
        }

        /**
         * Fires a Command back through this transport to its registered
         * CommandListener if there is one.
         * @param command - Command to send to the Listener.
         */
        virtual void fireCommand( const Pointer<Command>& command ){
            if( listener != NULL ){
                listener->onCommand( command );
            }
        }

        /**
         * Fires a Exception back through this transport to its registered
         * ExceptionListener if there is one.
         *
         * @param ex
         *      The Exception that will be passed on the the Transport listener.
         */
        virtual void fireException( const exceptions::ActiveMQException& ex ){
            if( listener != NULL ){
                listener->onException( ex );
            }
        }

        /**
         * Sets the ResponseBuilder that this class uses to create Responses to
         * Commands sent.  These are either real Response Objects, or Commands that
         * would have been sent Asynchronously be the Broker.
         * @param responseBuilder - The ResponseBuilder to use from now on.
         */
        void setResponseBuilder( const Pointer<ResponseBuilder>& responseBuilder ){
            this->responseBuilder = responseBuilder;
        }


        /**
         * Sets a Listener that gets notified for every command that would
         * have been sent by this transport to the Broker, this allows a client
         * to verify that its messages are making it to the wire.
         * @param listener - The CommandListener to notify for each message
         */
        virtual void setOutgoingListener( TransportListener* listener ){
            outgoingListener = listener;
        }

        /**
         * Gets the currently set WireFormat
         *
         * @return the current WireFormat object.
         */
        Pointer<wireformat::WireFormat> getWireFormat() const {
            return this->wireFormat;
        }

    public:  // Transport Methods

        virtual void oneway( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command );

        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout );

        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat AMQCPP_UNUSED ) {}

        virtual void setTransportListener( TransportListener* listener ){
            this->listener = listener;
        }

        virtual TransportListener* getTransportListener() const {
            return this->listener;
        }

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual Transport* narrow( const std::type_info& typeId ) {
            if( typeid( *this ) == typeId ) {
                return this;
            }

            return NULL;
        }

        virtual bool isFaultTolerant() const {
            return false;
        }

        virtual bool isConnected() const {
            return true;
        }

        virtual bool isClosed() const {
            return false;
        }

        virtual std::string getRemoteAddress() const {
            return "";
        }

        virtual void reconnect( const decaf::net::URI& uri AMQCPP_UNUSED ) {}

    public:  // Property Getters and Setters

        std::string getName() const {
            return this->name;
        }

        void setName( const std::string& name ) {
            this->name = name;
        }

        bool isFailOnSendMessage() const {
            return this->failOnSendMessage;
        }

        void setFailOnSendMessage( bool value ) {
            this->failOnSendMessage = value;
        }

        int getNumSentMessageBeforeFail() const {
            return this->numSentMessageBeforeFail;
        }

        void setNumSentMessageBeforeFail( int value ) {
            this->numSentMessageBeforeFail = value;
        }

        int getNumSentMessages() const {
            return this->numSentMessages;
        }

        void setNumSentMessages( int value ) {
            this->numSentMessages = value;
        }

        bool isFailOnReceiveMessage() const {
            return this->failOnReceiveMessage;
        }

        void setFailOnReceiveMessage( bool value ) {
            this->failOnReceiveMessage = value;
        }

        int getNumReceivedMessageBeforeFail() const {
            return this->numReceivedMessageBeforeFail;
        }

        void setNumReceivedMessageBeforeFail( int value ) {
            this->numReceivedMessageBeforeFail = value;
        }

        int getNumReceivedMessages() const {
            return this->numReceivedMessages;
        }

        void setNumReceivedMessages( int value ) {
            this->numReceivedMessages = value;
        }

        bool isFailOnKeepAliveSends() const {
            return this->failOnKeepAliveSends;
        }

        void setFailOnKeepAliveSends( bool value ) {
            this->failOnKeepAliveSends = value;
        }

        int getNumSentKeepAlivesBeforeFail() const {
            return this->numSentKeepAlivesBeforeFail;
        }

        void setNumSentKeepAlivesBeforeFail( int value ) {
            this->numSentKeepAlivesBeforeFail = value;
        }

        int getNumSentKeepAlives() const {
            return this->numSentKeepAlives;
        }

        void setNumSentKeepAlives( int value ) {
            this->numSentKeepAlives = value;
        }

        bool isFailOnStart() const {
            return this->failOnReceiveMessage;
        }

        void setFailOnStart( bool value ) {
            this->failOnReceiveMessage = value;
        }

        bool isFailOnStop() const {
            return this->failOnStop;
        }

        void setFailOnStop( bool value ) {
            this->failOnStop = value;
        }

        bool isFailOnClose() const {
            return this->failOnClose;
        }

        void setFailOnClose( bool value ) {
            this->failOnClose = value;
        }

        virtual bool isReconnectSupported() const {
            return false;
        }

        virtual bool isUpdateURIsSupported() const {
            return false;
        }

        virtual void updateURIs( bool rebalance AMQCPP_UNUSED,
                                 const decaf::util::List<decaf::net::URI>& uris AMQCPP_UNUSED ) {
            throw decaf::io::IOException();
        }

    };

}}}

#endif /*_ACTIVEMQ_TANSPORT_MOCK_MOCKTRANSPORT_H_*/
