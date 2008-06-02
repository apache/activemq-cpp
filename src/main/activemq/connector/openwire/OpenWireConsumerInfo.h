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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONSUMERINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONSUMERINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/connector/openwire/commands/ConsumerInfo.h>
#include <cms/Destination.h>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireConsumerInfo : public ConsumerInfo {
    private:

        // OpenWire's Consumer Info Command
        commands::ConsumerInfo* consumerInfo;

        // Session Info - We do not own this
        const SessionInfo* session;

        // Has this consumer been started.
        bool started;

    public:

        OpenWireConsumerInfo( Connector* connector ) :
            ConsumerInfo( connector ) {

            this->session = NULL;
            this->started = false;
        }

        virtual ~OpenWireConsumerInfo() {
            this->close();
            delete consumerInfo;
        }

        /**
         * Gets this message consumer's message selector expression.
         * @return This Consumer's selector expression or "".
         */
        virtual const std::string& getMessageSelector() const {
            static std::string selector = "";

            if( consumerInfo != NULL ) {
                selector = consumerInfo->getSelector();
            }

            return selector;
        }

        /**
         * Sets this message consumer's message selector expression.
         * @param selector This Consumer's selector expression or "".
         */
        virtual void setMessageSelector( const std::string& selector ) {
            if( consumerInfo != NULL ){
                this->consumerInfo->setSelector( selector );
            }
        }

        /**
         * Gets the ID that is assigned to this consumer
         * @return value of the Consumer Id.
         */
        virtual long long getConsumerId() const {
            if( consumerInfo != NULL ) {
                return (unsigned int)
                    this->consumerInfo->getConsumerId()->getValue();
            }

            return 0;
        }

        /**
         * Sets the ID that is assigned to this consumer
         * @param id value of the Consumer Id.
         */
        virtual void setConsumerId( long long int id ) {
            if( this->consumerInfo != NULL ) {
                this->consumerInfo->getConsumerId()->setValue( id );
            }
        }

        /**
         * Gets the Destination that this Consumer is subscribed on
         * @return Destination this consumer is attached to
         */
        virtual const cms::Destination* getDestination() const {
            if( this->consumerInfo != NULL ) {
                return this->consumerInfo->getDestination()->getCMSDestination();
            }

            return NULL;
        }

        /**
         * Sets the destination that this Consumer is listening on
         * @param destination Destination this consumer is attached to
         */
        virtual void setDestination( const cms::Destination* destination ) {
            if( consumerInfo != NULL ) {
                this->consumerInfo->setDestination(
                    dynamic_cast<commands::ActiveMQDestination*>(
                        destination->clone() ) );
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
         * @param session SessionnInfo for this consumer
         */
        virtual void setSessionInfo( const SessionInfo* session ) {
            this->session = session;
        }

        /**
         * Gets the OpenWire Consumer Info object that was used to create
         * this Consumer.
         * @returns The ConsumerInfo for this Session or NULL if not set.
         */
        virtual const commands::ConsumerInfo* getConsumerInfo() const {
            return this->consumerInfo;
        }
        virtual commands::ConsumerInfo* getConsumerInfo() {
            return this->consumerInfo;
        }

        /**
         * Sets the ConsumerInfo from OpenWire that was used to create this
         * Consumer
         * @param consumerInfo - the ConsumerInfo for this Session.
         */
        virtual void setConsumerInfo( commands::ConsumerInfo* consumerInfo ) {
            this->consumerInfo = consumerInfo;
        }

        /**
         * @returns if this Consumer has been started already
         */
        virtual bool isStarted() const {
            return this->started;
        }

        /**
         * Sets if the consumer is started or not
         * @param started - True if Consumer is started
         */
        virtual void setStarted( bool started ) {
            this->started = started;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONSUMERINFO_H_*/
