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

#ifndef _ACTIVE_TRANSPORT_FAILOVER_BACKUPTRANSPORT_H_
#define _ACTIVE_TRANSPORT_FAILOVER_BACKUPTRANSPORT_H_

#include <activemq/util/Config.h>

#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <decaf/net/URI.h>

namespace activemq {
namespace transport {
namespace failover {

    class FailoverTransport;

    class AMQCPP_API BackupTransport : public TransportExceptionListener {
    private:

        // The parent Failover Transport
        FailoverTransport* failover;

        // The Transport this one is managing.
        Transport* transport;

        // The URI of this Backup
        URI uri;

        // Indicates that the contained transport is not valid any longer.
        bool disposed;

    public:

        BackupTransport( FailoverTransport* failover );
        virtual ~BackupTransport();

        /**
         * Gets the URI assigned to this Backup
         * @return the assigned URI
         */
        URI getURI() const {
            return this->uri;
        }

        /**
         * Sets the URI assigned to this Transport.
         */
        void setURI( const URI& uri ) {
            this->uri = uri;
        }

        /**
         * Gets the currently held transport
         * @returns pointer to the held transport or NULL if not set.
         */
        Transport* getTransport() {
            return transport;
        }

        /**
         * Sets the held transport, if not NULL then start to listen for exceptions
         * from the held transport.
         *
         * @param transport
         *        The transport to hold.
         */
        void setTransport( Transport* transport ) {
            this->transport = transport;

            if( this->transport != NULL ) {
                this->transport->setTransportExceptionListener( this );
            }
        }

        /**
         * Event handler for an exception from a command transport.
         * @param source The source of the exception
         * @param ex The exception.
         */
        virtual void onTransportException( transport::Transport* source,
                                           const decaf::lang::Exception& ex );

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted();

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed();

    };

}}}

#endif /* _ACTIVE_TRANSPORT_FAILOVER_BACKUPTRANSPORT_H_ */
