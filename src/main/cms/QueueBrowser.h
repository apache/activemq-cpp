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

#ifndef _CMS_QUEUEBROWSER_H_
#define _CMS_QUEUEBROWSER_H_

#include <vector>
#include <cms/Config.h>
#include <cms/Closeable.h>
#include <cms/Queue.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * This class implements in interface for browsing the messages in a Queue
     * without removing them.
     */
    class CMS_API QueueBrowser : public Closeable {
    public:

        virtual ~QueueBrowser() {}

        /**
         * @returns the Queue that this browser is listening on.
         * @throws CMSException if an internal error occurs.
         */
        virtual const Queue* getQueue() const throw ( cms::CMSException ) = 0;

        /**
         * @returns the MessageSelector that is used on when this browser was
         * created or empty string if no selector was present.
         * @throws CMSException if an internal error occurs.
         */
        virtual std::string getMessageSelector() const throw ( cms::CMSExceptio ) = 0;

        /**
         * Gets an enumeration for browsing the current queue messages in the
         * order they would be received.
         * @returns an stl vector for browsing the messages.
         * @throws CMSException if an internal error occurs.
         */
        virtual void std::vector<cms::Message*> getEnumeration() const throw ( cms::CMSException ) = 0;

    };

}

#endif /*_CMS_QUEUEBROWSER_H_*/
