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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREPRODUCERINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREPRODUCERINFO_H_

#include <activemq/connector/ProducerInfo.h>
#include <activemq/connector/openwire/commands/ProducerInfo.h>
#include <cms/Destination.h>

namespace activemq{
namespace connector{
namespace openwire{

    class OpenWireProducerInfo : public ProducerInfo {

    private:

        // OpenWire ProducerInfo Command
        commands::ProducerInfo* producerInfo;

        // Producer Id
        unsigned int producerId;

        // Default Destination
        cms::Destination* destination;

        // Session that this producer is attached to - we do not own this
        const SessionInfo* session;

    public:

        OpenWireProducerInfo() {
            destination = NULL;
            producerId = 0;
            session = NULL;
        }

        virtual ~OpenWireProducerInfo() {
            delete destination;
        }

        /**
         * Retrieves the default destination that this producer
         * sends its messages to.
         * @return Destionation, owned by this object
         */
        virtual const cms::Destination& getDestination(void) const {
            return *destination;
        }

        /**
         * Sets the Default Destination for this Producer
         * @param destination reference to a destination, copied internally
         */
        virtual void setDestination( const cms::Destination& destination ) {
            this->destination = destination.clone();
        }

        /**
         * Gets the ID that is assigned to this Producer
         * @return value of the Producer Id.
         */
        virtual unsigned int getProducerId(void) const {
            return producerId;
        }

        /**
         * Sets the ID that is assigned to this Producer
         * @return id string value of the Producer Id.
         */
        virtual void setProducerId( const unsigned int id ) {
            this->producerId = id;
        }

        /**
         * Gets the Session Info that this consumer is attached too
         * @return SessionnInfo pointer
         */
        virtual const SessionInfo* getSessionInfo(void) const {
            return session;
        }

        /**
         * Gets the Session Info that this consumer is attached too
         * @return session SessionnInfo pointer
         */
        virtual void setSessionInfo( const SessionInfo* session ) {
            this->session = session;
        }

        /**
         * Gets the OpenWire ProducerInfo object that was used to create
         * this session.
         * @returns The ProducerInfo for this Session or NULL if not set.
         */
        virtual const commands::ProducerInfo* getProducerInfo() const {
            return this->producerInfo;
        }
        virtual commands::ProducerInfo* getProducerInfo() {
            return this->producerInfo;
        }

        /**
         * Sets the ProducerInfo from OpenWire that was used to create this
         * Session
         * @param producerInfo - the ProducerInfo for this Session.
         */
        virtual void setProducerInfo( commands::ProducerInfo* producerInfo ) {
            this->producerInfo = producerInfo;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREPRODUCERINFO_H_*/
