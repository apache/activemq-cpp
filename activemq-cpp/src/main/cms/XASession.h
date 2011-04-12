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

#ifndef _CMS_XASESSION_H_
#define _CMS_XASESSION_H_

#include <cms/Config.h>

#include <cms/Session.h>
#include <cms/XAResource.h>

namespace cms {

    /**
     * The XASession interface extends the capability of Session by adding access to a CMS provider's
     * support for the operating inside an XA Transaction (optional). This support takes the form of
     * a cms::XAResource object. The functionality of this object closely resembles that defined by
     * the standard X/Open XA Resource interface.
     *
     * An application controls the transactional assignment of an XASession by obtaining its XAResource.
     * It uses the XAResource to assign the session to a transaction, prepare and commit work on the
     * transaction, and so on.
     *
     * An XAResource provides some fairly sophisticated facilities for interleaving work on multiple
     * transactions, recovering a list of transactions in progress, and so on. A XA aware CMS provider
     * must fully implement this functionality.
     *
     * The XASession instance will behave much like a normal cms::Session however some methods will
     * will not operate as normal, any call to Session::commit, or Session::rollback will result in
     * a CMSException being thrown.  Also when not inside an XA transaction the MessageConsumer will
     * operate as if it were in the AutoAcknowlege mode.
     *
     * The XASession interface is optional. CMS providers are not required to support this interface.
     * This interface is for use by CMS providers to support transactional environments. Client programs
     * are strongly encouraged to use the transactional support available in their environment, rather
     * than use these XA interfaces directly.
     *
     * @since 2.3
     */
    class CMS_API XASession : public virtual cms::Session {
    public:

        virtual ~XASession() throw();

        /**
         * Returns the XA resource associated with this Session to the caller.
         *
         * The client can use the provided XA resource to interact with the XA Transaction
         * Manager in use in the client application.
         *
         * @returns an XAResouce instance to the caller, the caller does not own this
         *          pointer and should not delete it.
         */
        virtual XAResource* getXAResource() const = 0;

    };

}

#endif /* _CMS_XASESSION_H_ */
