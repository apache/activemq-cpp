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

#include <activemq/util/Config.h>
#include <activemq/connector/ProducerInfo.h>
#include <activemq/commands/ProducerInfo.h>
#include <cms/Destination.h>
#include <activemq/util/MemoryUsage.h>
#include <memory>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireProducerInfo : public ProducerInfo {
    private:

        // Are Message Ids Disabled
        bool disableMessageIds;

        // OpenWire ProducerInfo Command
        commands::ProducerInfo* producerInfo;

        // Session that this producer is attached to - we do not own this
        const SessionInfo* session;

        // Send timeout, how long to wait for a response before failing.
        unsigned int sendTimeout;

        // Memory Usage if we are using Producer Window Sizes
        std::auto_ptr<activemq::util::MemoryUsage> usage;

    public:

        OpenWireProducerInfo( Connector* connector ) :
            ProducerInfo( connector ) {

            this->disableMessageIds = false;
            this->producerInfo = NULL;
            this->session = NULL;
            this->sendTimeout = 0;
        }

        virtual ~OpenWireProducerInfo() {
            this->close();
            delete producerInfo;
        }

        /**
         * Retrieves the default destination that this producer
         * sends its messages to.
         * @return Destionation, owned by this object
         */
        virtual const cms::Destination* getDestination() const {
            if( this->producerInfo != NULL ) {
                return this->producerInfo->getDestination()->getCMSDestination();
            }

            return NULL;
        }

        /**
         * Sets the Default Destination for this Producer
         * @param destination reference to a destination, copied internally
         */
        virtual void setDestination( const cms::Destination* destination ) {
            if( this->producerInfo != NULL ) {

                this->producerInfo->setDestination(
                    dynamic_cast<commands::ActiveMQDestination*>(
                        destination->clone() ) );
            }
        }

        /**
         * Gets the ID that is assigned to this Producer
         * @return value of the Producer Id.
         */
        virtual long long getProducerId() const {
            if( this->producerInfo != NULL ) {
                return (unsigned int)this->producerInfo->getProducerId()->getValue();
            }

            return 0;
        }

        /**
         * Sets the ID that is assigned to this Producer
         * @return id string value of the Producer Id.
         */
        virtual void setProducerId( long long id ) {
            if( this->producerInfo != NULL ) {
                this->producerInfo->getProducerId()->setValue( id );
            }
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

            if( this->producerInfo != NULL && this->producerInfo->getWindowSize() > 0 ) {
                this->usage.reset( new activemq::util::MemoryUsage(
                    this->producerInfo->getWindowSize() ) );
            }
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

        /**
         * Queries if this Producer is tracking memory usage, returns true if so.
         * @return true if this producer is tracking memory usage.
         */
        virtual bool isUsageTrackingEnabled() const {
            return this->usage.get() != NULL;
        }

        /**
         * Enqueues more usage on this producer's current Memory usage, if there
         * is not enough space, then this call blocks until there is room for the
         * requested space.  If Memory Usage tracking is not enabled then this method
         * does nothing.
         *
         * @param usage - Size in bytes of the usage to enqueue
         */
        virtual void enqueUsage( unsigned int usage ) {
            if( this->usage.get() != NULL ) {
                this->usage->enqueueUsage( usage );
            }
        }

        /**
         * Frees up a given amount of usage from the usage being tracked for this
         * Producer.  If a previous call to enqueueUsage was blocked and enough space
         * is freed for it to continue then it will be woken up.  If memory usage
         * tracking is not enabled then this method has no effect.
         *
         * @param usage - Size in bytes of the usage to enqueue
         */
        virtual void decreaseUsage( unsigned int usage ) {
            if( this->usage.get() != NULL ) {
                this->usage->decreaseUsage( usage );
            }
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIREPRODUCERINFO_H_*/
