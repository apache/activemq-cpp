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

#ifndef ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_
#define ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/commands/Command.h>
#include <activemq/transport/TransportListener.h>
#include <decaf/lang/Pointer.h>
#include <typeinfo>

namespace activemq{
namespace transport{

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    /**
     * A filter on the transport layer.  Transport filters implement the Transport
     * interface and optionally delegate calls to another Transport object.
     *
     * @since 1.0
     */
    class AMQCPP_API TransportFilter : public Transport,
                                       public TransportListener {
    protected:

        /**
         * The transport that this filter wraps around.
         */
        Pointer<Transport> next;

        /**
         * Listener of this transport.
         */
        TransportListener* listener;

    protected:

        /**
         * Notify the listener of the thrown Exception.
         * @param ex - the exception to send to listeners
         */
        void fire( const decaf::lang::Exception& ex );

        /**
         * Notify the listener of the new incoming Command.
         * @param command - the command to send to the listener
         */
        void fire( const Pointer<Command>& command );

    private:

        TransportFilter( const TransportFilter& );
        TransportFilter& operator= ( const TransportFilter& );

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         */
        TransportFilter( const Pointer<Transport>& next );

        virtual ~TransportFilter();

    public:  // TransportListener methods

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand( const Pointer<Command>& command );

        /**
         * Event handler for an exception from a command transport.
         * @param ex
         *      The exception to handle.
         */
        virtual void onException( const decaf::lang::Exception& ex );

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted();

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed();

    public:  // Transport Methods.

        virtual void oneway( const Pointer<Command>& command ) {
            next->oneway( command );
        }

        virtual Pointer<Response> request( const Pointer<Command>& command ) {
            return next->request( command );
        }

        virtual Pointer<Response> request( const Pointer<Command>& command, unsigned int timeout ) {
            return next->request( command, timeout );
        }

        virtual void setTransportListener( TransportListener* listener ) {
            this->listener = listener;
        }

        virtual TransportListener* getTransportListener() const {
            return this->listener;
        }

        virtual void setWireFormat( const Pointer<wireformat::WireFormat>& wireFormat ) {
            next->setWireFormat( wireFormat );
        }

        virtual void start();

        virtual void stop();

        virtual void close();

        virtual Transport* narrow( const std::type_info& typeId );

        virtual bool isFaultTolerant() const {
            return next->isFaultTolerant();
        }

        virtual bool isConnected() const {
            return next->isConnected();
        }

        virtual bool isReconnectSupported() const {
            return next->isReconnectSupported();
        }

        virtual bool isUpdateURIsSupported() const {
            return next->isUpdateURIsSupported();
        }

        virtual bool isClosed() const {
            return next->isClosed();
        }

        virtual std::string getRemoteAddress() const {
            return next->getRemoteAddress();
        }

        virtual void reconnect( const decaf::net::URI& uri );

        virtual void updateURIs( bool rebalance, const decaf::util::List<decaf::net::URI>& uris ) {
            next->updateURIs( rebalance, uris );
        }

    };

}}

#endif /*ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_*/
