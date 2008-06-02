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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRESESSIONINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRESESSIONINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/connector/openwire/commands/SessionInfo.h>
#include <cms/Session.h>
#include <string>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireSessionInfo : public SessionInfo {
    private:

        // The OpenWire Session Info DataStructure for this Session
        commands::SessionInfo* sessionInfo;

        // Acknowledge Mode of this Session
        cms::Session::AcknowledgeMode ackMode;

        // Internal String used to buffer the ConnectionId
        mutable std::string connectionId;

        // Info for this sessions current transaction
        const TransactionInfo* transaction;

    public:

        OpenWireSessionInfo( Connector* connector ) :
            SessionInfo( connector ) {

            ackMode = cms::Session::AUTO_ACKNOWLEDGE;
            transaction = NULL;
            sessionInfo = NULL;
        }
        
        virtual ~OpenWireSessionInfo() {
            this->close();
            delete sessionInfo;
        }

        /**
         * Gets the Connection Id of the Connection that this consumer is
         * using to receive its messages.
         * @return string value of the connection id
         */
        virtual const std::string& getConnectionId() const{
            if( sessionInfo != NULL ) {
                connectionId = sessionInfo->getSessionId()->getConnectionId();
            }

            return connectionId;
        }

        /**
         * Sets the Connection Id of the Connection that this consumer is
         * using to receive its messages.
         * @param id string value of the connection id
         */
        virtual void setConnectionId( const std::string& id AMQCPP_UNUSED ){
            // Do Nothing here.
        }

        /**
         * Gets the Sessions Id value
         * @return id for this session
         */
        virtual long long getSessionId() const {
            if( sessionInfo != NULL ) {
                return (unsigned int)sessionInfo->getSessionId()->getValue();
            }

            return 0;
        }

        /**
         * Sets the Session Id for this Session
         * @param id integral id value for this session
         */
        virtual void setSessionId( long long id ) {
            if( sessionInfo != NULL ) {
                sessionInfo->getSessionId()->setValue( id );
            }
        }

        /**
         * Sets the Ack Mode of this Session Info object
         * @param ackMode Ack Mode
         */
        virtual void setAckMode( cms::Session::AcknowledgeMode ackMode ) {
            this->ackMode = ackMode;
        }

        /**
         * Gets the Ack Mode of this Session
         * @return Ack Mode
         */
        virtual cms::Session::AcknowledgeMode getAckMode() const {
            return ackMode;
        }

        /**
         * Gets the currently active transaction info, if this session is
         * transacted, returns NULL when not transacted.  You must call
         * getAckMode and see if the session is transacted.
         * @return Transaction Id of current Transaction
         */
        virtual const TransactionInfo* getTransactionInfo() const {
            return transaction;
        }

        /**
         * Sets the current transaction info for this session, this is nit
         * used when the session is not transacted.
         * @param transaction Transaction Id
         */
        virtual void setTransactionInfo( const TransactionInfo* transaction ) {
            this->transaction = transaction;
        }

        /**
         * Gets the OpenWire Session Info object that was used to create
         * this session.
         * @returns The SessionInfo for this Session or NULL if not set.
         */
        virtual const commands::SessionInfo* getSessionInfo() const {
            return this->sessionInfo;
        }
        virtual commands::SessionInfo* getSessionInfo() {
            return this->sessionInfo;
        }

        /**
         * Sets the SessionInfo from OpenWire that was used to create this
         * Session
         * @param sessionInfo - the SessionInfo for this Session.
         */
        virtual void setSessionInfo( commands::SessionInfo* sessionInfo ) {
            this->sessionInfo = sessionInfo;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRESESSIONINFO_H_*/
