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
#ifndef _ACTIVEMQ_CONNECTOR_STOMPTRANSACTIONINFO_H_
#define _ACTIVEMQ_CONNECTOR_STOMPTRANSACTIONINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/connector/SessionInfo.h>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompTransactionInfo : public TransactionInfo {
    private:

        // Transaction Id
        long long transactionId;

        // Session Info - We do not own this
        const SessionInfo* session;

    public:

        StompTransactionInfo() : TransactionInfo() {

            transactionId = 0;
            session = NULL;
        }

        StompTransactionInfo( Connector* connector ) :
            TransactionInfo( connector ) {

            transactionId = 0;
            session = NULL;
        }

        virtual ~StompTransactionInfo() {
            this->close();
        }

        /**
         * Gets the Transction Id
         * @return long long Id
         */
        virtual long long getTransactionId(void) const {
            return transactionId;
        }

        /**
         * Sets the Transction Id
         * @param id long long Id
         */
        virtual void setTransactionId( long long id ) {
            this->transactionId = id;
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

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMPTRANSACTIONINFO_H_*/
