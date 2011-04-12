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

#ifndef _CMS_XACONNECTION_H_
#define _CMS_XACONNECTION_H_

#include <cms/Config.h>

#include <cms/CMSException.h>
#include <cms/Connection.h>
#include <cms/XASession.h>

namespace cms {

    /**
     * The XAConnection interface defines an extended Connection type that is used to create
     * XASession objects.  This is an optional interface and CMS providers are allowed to omit
     * an implementation and instead throw an exception from an XAConnectionFactory stub to
     * indicate that XA is not supported.
     *
     * @since 2.3
     */
    class CMS_API XAConnection : public virtual cms::Connection {
    public:

        virtual ~XAConnection() throw();

        /**
         * Creates an XASession object.
         *
         * @returns a newly created XASession instance, caller owns the pointer.
         *
         * @throws CMSException
         *      If the XAConnection object fails to create the XASession instance due to
         *      an internal error.
         */
        virtual XASession* createXASession() = 0;

    };

}

#endif /* _CMS_XACONNECTION_H_ */
