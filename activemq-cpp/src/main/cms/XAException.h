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

#ifndef _CMS_XAEXCEPTION_H_
#define _CMS_XAEXCEPTION_H_

#include <cms/Config.h>
#include <cms/CMSException.h>

namespace cms {

    /**
     * The XAException is thrown by the Resource Manager (RM) to inform the
     * Transaction Manager of an error encountered by the involved transaction.
     *
     * @since 2.3
     */
    class CMS_API XAException : CMSException {
    private:

        int errorCode;

    public:

        /**
         * Code which contains the inclusive lower bound of the rollback error codes
         */
        static const int XA_RBBASE;

        /**
         * Code which means that the rollback occurred for an unspecified reason
         */
        static const int XA_RBROLLBACK;

        /**
         * Code which means that rollback was caused by a communication failure
         */
        static const int XA_RBCOMMFAIL;

        /**
         * Code which means that a failure occurred because a deadlock was detected
         */
        static const int XA_RBDEADLOCK;

        /**
         * Code which means that a condition was detected than implies a violation
         * of the integrity of the resource
         */
        static const int XA_RBINTEGRITY;

        /**
         * Code which means that the Resource Manager rolled back the transaction
         * branch for a reason not separately listed
         */
        static const int XA_RBOTHER;

        /**
         * Code which means that a protocol error occurred in the Resource Manager
         */
        static const int XA_RBPROTO;

        /**
         * Code which means that a transaction branch took too long
         */
        static const int XA_RBTIMEOUT;

        /**
         * Code which means that the caller may retry the transaction branch
         */
        static const int XA_RBTRANSIENT;

        /**
         * Code which contains the inclusive upper bound of the rollback error codes
         */
        static const int XA_RBEND;

        /**
         * Code which means that resumption must occur where the suspension occurred
         */
        static const int XA_NOMIGRATE;

        /**
         * Code which means that the transaction branch may have been heuristically
         * completed
         */
        static const int XA_HEURHAZ;

        /**
         * Code which means that the transaction branch has been heuristically
         * committed
         */
        static const int XA_HEURCOM;

        /**
         * Code which means that the transaction branch has been heuristically
         * rolled back
         */
        static const int XA_HEURRB;

        /**
         * Code which means that the transaction branch has been heuristically
         * committed and rolled back
         */
        static const int XA_HEURMIX;

        /**
         * Code which means that the method returned with no effect and can be
         * reissued
         */
        static const int XA_RETRY;

        /**
         * Code which means that the transaction branch was read only and has been
         * committed
         */
        static const int XA_RDONLY;

        /**
         * Code which means that there is already an asynchronous operation
         * outstanding
         */
        static const int XAER_ASYNC;

        /**
         * Code which means that a Resource Manager error has occurred for the
         * transaction branch
         */
        static const int XAER_RMERR;

        /**
         * Code which means that the XID is not valid
         */
        static const int XAER_NOTA;

        /**
         * Code which means that invalid arguments were supplied
         */
        static const int XAER_INVAL;

        /**
         * Code which means that the method was invoked in an improper context
         */
        static const int XAER_PROTO;

        /**
         * Code which means that the Resource Manager is unavailable
         */
        static const int XAER_RMFAIL;

        /**
         * Code which means that the XID already exists
         */
        static const int XAER_DUPID;

        /**
         * Work is being done by the Resource Manager outside the boundaries of a
         * global transaction.
         */
        static const int XAER_OUTSIDE;

    public:

        XAException();

        XAException( int errorCode );

        XAException( const XAException& ex );

        XAException( const std::string& message );

        XAException( const std::string& message,
                     const std::exception* cause );

        XAException( const std::string& message,
                     const std::exception* cause,
                     const std::vector< std::pair< std::string, int> >& stackTrace );

        virtual ~XAException() throw();

    public:

        /**
         * Sets the error code for this XAException
         *
         * @param errorCode
         *      The error code to assign to this XAException.
         */
        void setErrorCode( int errorCode ) {
            this->errorCode = errorCode;
        }

        /**
         * Gets the error code that was assigned to this XAException.
         *
         * @returns the assigned error code.
         */
        int getErrorCode() const {
            return this->errorCode;
        }

    };

}

#endif /* _CMS_XAEXCEPTION_H_ */
