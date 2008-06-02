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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRETRANSACTIONINFO_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRETRANSACTIONINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/connector/openwire/commands/TransactionInfo.h>
#include <activemq/connector/SessionInfo.h>

namespace activemq{
namespace connector{
namespace openwire{

    class AMQCPP_API OpenWireTransactionInfo : public TransactionInfo {
    private:

        // OpenWire Tramsaction Info Command
        commands::TransactionInfo* transactionInfo;

        // Session Info - We do not own this
        const SessionInfo* session;

    public:

        OpenWireTransactionInfo( Connector* connector ) :
            TransactionInfo( connector ) {

            transactionInfo = NULL;
            session = NULL;
        }

        virtual ~OpenWireTransactionInfo() {
            this->close();
            delete transactionInfo;
        }

        /**
         * Gets the Transction Id
         * @return integral vlaue of the Id
         */
        virtual long long getTransactionId(void) const {
            if( transactionInfo != NULL ) {
                return ( dynamic_cast<commands::LocalTransactionId*>(
                    transactionInfo->getTransactionId() ) )->getValue();
            }
            return 0;
        }

        /**
         * Sets the Transction Id
         * @param id integral value of the Id
         */
        virtual void setTransactionId( long long id ) {
            if( transactionInfo != NULL ) {
                ( dynamic_cast<commands::LocalTransactionId*>(
                    this->transactionInfo->getTransactionId() ) )->setValue( id );
            }
        }

        /**
         * Gets the Session Info that this Transction is attached too
         * @return SessionnInfo pointer
         */
        virtual const SessionInfo* getSessionInfo(void) const {
            return session;
        }

        /**
         * Gets the Session Info that this Transction is attached too
         * @return session SessionnInfo pointer
         */
        virtual void setSessionInfo( const SessionInfo* session ) {
            this->session = session;
        }

        /**
         * Gets the OpenWire TransactionInfo object that was used to create
         * this session.
         * @returns The TransactionInfo for this Session or NULL if not set.
         */
        virtual const commands::TransactionInfo* getTransactionInfo() const {
            return this->transactionInfo;
        }
        virtual commands::TransactionInfo* getTransactionInfo() {
            return this->transactionInfo;
        }

        /**
         * Sets the TransactionInfo from OpenWire that was used to create this
         * Session
         * @param transactionInfo - the TransactionInfo for this Session.
         */
        virtual void setTransactionInfo( commands::TransactionInfo* transactionInfo ) {
            delete this->transactionInfo;
            this->transactionInfo = transactionInfo;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRETRANSACTIONINFO_H_*/
