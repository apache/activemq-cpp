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

#ifndef _CMS_XID_H_
#define _CMS_XID_H_

#include <cms/Config.h>

#include <cms/CMSException.h>

namespace cms {

    /**
     * An interface which provides a mapping for the X/Open XID transaction
     * identifier structure. The Xid interface is used by the Transaction Manager
     * and the Resource managers. It is not typically used by application programs
     * directly but the application developer must define a mechanism to map the
     * calls and structures used by the Transaction Manager API in use into the
     * format used by the CMS XA interfaces.
     *
     * @since 2.3
     */
    class CMS_API Xid {
    private:

        Xid( const Xid& );
        Xid& operator= ( const Xid& );

    public:

        /**
         * The maximum number of bytes which will be copied into the array passed to
         * getGlobaltransactionId().
         */
        static const int MAXGTRIDSIZE;

        /**
         * The maximum number of bytes which will be copied into the array that is passed
         * to getBranchQualifier().
         */
        static const int MAXBQUALSIZE;

    public:

        Xid();

        virtual ~Xid();

        /**
         * Creates a Copy of this Xid instance that contains the same id values.
         *
         * @returns a new Xid instance that is equal to this one when compared.
         */
        virtual Xid* clone() const = 0;

        /**
         * Compares this Xid to another and returns true if they are the same.
         *
         * @returns true if both Xid's represent that same id value.
         */
        virtual bool equals( const Xid* other ) const = 0;

        /**
         * Gets the transaction branch qualifier component of the XID.  The value of this
         * Xid's branch qualifier is copied into the buffer provided.  If the size of the
         * provided buffer is not large enough to accommodate the branch qualifier then
         * no copy is performed and the method returns -1.
         *
         * @param buffer
         *      The location in memory to copy the qualifier bytes to.
         * @param size
         *      The size of the buffer provided.
         *
         * @return the number of bytes copied into the buffer, or -1 if the buffer
         *         provided was not large enough.
         *
         * @throws XAException if the size parameter is less than zero or buffer is NULL.
         */
        virtual int getBranchQualifier( unsigned char* buffer, int size ) const = 0;

        /**
         * Gets the format identifier component of the XID.
         *
         * @return an integer containing the format identifier. 0 means the OSI CCR
         *         format.
         */
        virtual int getFormatId() const = 0;

        /**
         * Gets the global transaction id component of the XID.  The value of this
         * Xid's transaction id is copied into the buffer provided.  If the size of the
         * provided buffer is not large enough to accommodate the transaction id then
         * no copy is performed and the method returns -1.
         *
         * @param buffer
         *      The location in memory to copy the transaction id bytes to.
         * @param size
         *      The size of the buffer provided.
         *
         * @return the number of bytes copied into the buffer, or -1 if the buffer
         *         provided was not large enough.
         *
         * @throws XAException if the size parameter is less than zero or buffer is NULL.
         */
        virtual int getGlobalTransactionId( unsigned char* buffer, int size ) const = 0;

    };

}

#endif /* _CMS_XID_H_ */
