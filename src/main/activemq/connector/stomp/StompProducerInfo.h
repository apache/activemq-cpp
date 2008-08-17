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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPPRODUCERINFO_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPPRODUCERINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ProducerInfo.h>
#include <cms/Destination.h>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompProducerInfo : public ProducerInfo {
    private:

        // Are Message Ids Disabled
        bool disableMessageIds;

        // Producer Id
        long long producerId;

        // Default Destination
        cms::Destination* destination;

        // Session that this producer is attached to - we do not own this
        const SessionInfo* session;

        // Send timeout, how long to wait for a response before failing.
        unsigned int sendTimeout;

    public:

        StompProducerInfo() : ProducerInfo() {

            this->producerId = 0;
            this->disableMessageIds = false;
            this->session = NULL;
            this->destination = NULL;
            this->sendTimeout = 0;
        }

        StompProducerInfo( Connector* connector ) :
            ProducerInfo( connector ) {

            this->producerId = 0;
            this->disableMessageIds = false;
            this->session = NULL;
            this->destination = NULL;
            this->sendTimeout = 0;
        }

        virtual ~StompProducerInfo(void) {
            this->close();
            delete destination;
        }

        /**
         * Retrieves the default destination that this producer
         * sends its messages to.
         * @return Destionation, owned by this object
         */
        virtual const cms::Destination* getDestination() const {
            return destination;
        }

        /**
         * Sets the Default Destination for this Producer
         * @param destination reference to a destination, copied internally
         */
        virtual void setDestination( const cms::Destination* destination ) {

            // Delete the previous destination if it exists.
            delete this->destination;
            this->destination = NULL;

            if( destination != NULL ) {
                this->destination = destination->clone();
            }
        }

        /**
         * Gets the ID that is assigned to this Producer
         * @return value of the Producer Id.
         */
        virtual long long getProducerId() const {
            return producerId;
        }

        /**
         * Sets the ID that is assigned to this Producer
         * @return id string value of the Producer Id.
         */
        virtual void setProducerId( long long id ) {
            this->producerId = id;
        }

        /**
         * Gets the Session Info that this consumer is attached too
         * @return SessionnInfo pointer
         */
        virtual const SessionInfo* getSessionInfo() const {
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
         * Sets if Message's Produced by this Producer should disable the
         * use of the MessageId field.
         * @param value - true if message ids are disabled
         */
        virtual void setDisableMessageId( bool value ) {
            this->disableMessageIds = value;
        }

        /**
         * Gets if Message's Produced by this Producer should disable the
         * use of the MessageId field.
         * @returns true if message ids are disabled
         */
        virtual bool isDisableMessageId() const {
            return this->disableMessageIds;
        }

        /**
         * Gets the set send timeout for messages from this producer, a value of
         * zero indicates that the Producer should wait forever for a response from
         * the broker on the send.
         * @return default time to wait for broker response to a message being sent.
         */
        virtual unsigned int getSendTimeout() const {
            return this->sendTimeout;
        }

        /**
         * Sets the time to wait for the broker to respond to a message being sent
         * @param timeout - The time to wait for the broker to acknowledge that a
         * message has been received.
         */
        virtual void setSendTimeout( unsigned int timeout ) {
            this->sendTimeout = timeout;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPPRODUCERINFO_H_*/
