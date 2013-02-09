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

namespace activemq {
namespace transport {

    using decaf::lang::Pointer;
    using activemq::commands::Command;
    using activemq::commands::Response;

    class TransportFilterImpl;

    /**
     * A filter on the transport layer.  Transport filters implement the Transport
     * interface and optionally delegate calls to another Transport object.
     *
     * @since 1.0
     */
    class AMQCPP_API TransportFilter: public Transport, public TransportListener {
    private:

        TransportFilterImpl* impl;

    protected:

        /**
         * The transport that this filter wraps around.
         */
        Pointer<Transport> next;

        /**
         * Listener of this transport.
         */
        TransportListener* listener;

    private:

        TransportFilter(const TransportFilter&);
        TransportFilter& operator=(const TransportFilter&);

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         */
        TransportFilter(const Pointer<Transport> next);

        virtual ~TransportFilter();

        void start();

        void stop();

        void close();

    protected:

        /**
         * Throws an IOException if this filter chain has already been closed.
         */
        void checkClosed() const;

    public:

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand(const Pointer<Command> command);

        /**
         * Event handler for an exception from a command transport.
         * @param ex
         *      The exception to handle.
         */
        virtual void onException(const decaf::lang::Exception& ex);

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted();

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed();

    public:

        virtual void oneway(const Pointer<Command> command) {
            checkClosed();
            next->oneway(command);
        }

        virtual Pointer<FutureResponse> asyncRequest(const Pointer<Command> command,
                                                     const Pointer<ResponseCallback> responseCallback) {
            checkClosed();
            return next->asyncRequest(command, responseCallback);
        }

        virtual Pointer<Response> request(const Pointer<Command> command) {
            checkClosed();
            return next->request(command);
        }

        virtual Pointer<Response> request(const Pointer<Command> command, unsigned int timeout) {
            checkClosed();
            return next->request(command, timeout);
        }

        virtual void setTransportListener(TransportListener* listener) {
            this->listener = listener;
        }

        virtual TransportListener* getTransportListener() const {
            return this->listener;
        }

        virtual Pointer<wireformat::WireFormat> getWireFormat() const;

        virtual void setWireFormat(const Pointer<wireformat::WireFormat> wireFormat);

        virtual Transport* narrow(const std::type_info& typeId);

        virtual bool isFaultTolerant() const {
            return !isClosed() && next->isFaultTolerant();
        }

        virtual bool isConnected() const {
            return !isClosed() && next->isConnected();
        }

        virtual bool isReconnectSupported() const {
            return !isClosed() && next->isReconnectSupported();
        }

        virtual bool isUpdateURIsSupported() const {
            return !isClosed() && next->isUpdateURIsSupported();
        }

        virtual bool isClosed() const;

        virtual std::string getRemoteAddress() const {

            if (isClosed()) {
                return "";
            }

            return next->getRemoteAddress();
        }

        virtual void reconnect(const decaf::net::URI& uri);

        virtual void updateURIs(bool rebalance, const decaf::util::List<decaf::net::URI>& uris) {
            checkClosed();
            next->updateURIs(rebalance, uris);
        }

    protected:

        /**
         * Subclasses can override this method to do their own startup work.  This method
         * will always be called before the next transport in the chain is called in order
         * to allow this transport a chance to initialize required resources.
         */
        virtual void beforeNextIsStarted() {}

        /**
         * Subclasses can override this method to do their own post startup work.  This method
         * will always be called after the doStart() method and the next transport's own start()
         * methods have been successfully run.
         */
        virtual void afterNextIsStarted() {}

        /**
         * Subclasses can override this method to do their own pre-stop work.  This method
         * will always be called before the next transport's own stop() method or this filter's
         * own doStop() method is called.
         */
        virtual void beforeNextIsStopped() {}

        /**
         * Subclasses can override this method to do their own stop work.  This method is
         * always called after all the next transports have been stopped to prevent this
         * transport for destroying resources needed by the lower level transports.
         */
        virtual void afterNextIsStopped() {}

        /**
         * Subclasses can override this method to do their own close work.  This method is
         * always called after all the next transports have been closed to prevent this
         * transport for destroying resources needed by the lower level transports.
         */
        virtual void doClose() {}

    };

}}

#endif /*ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_*/
