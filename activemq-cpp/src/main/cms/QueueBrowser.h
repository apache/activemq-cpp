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

#include <string>
#include <cms/Config.h>
#include <cms/Closeable.h>
#include <cms/Queue.h>
#include <cms/Message.h>
#include <cms/CMSException.h>
#include <cms/MessageEnumeration.h>

namespace cms{

    /**
     * This class implements in interface for browsing the messages in a Queue
     * without removing them.
     *
     * To browse the contents of the Queue the client calls the <code>getEnumeration</code>
     * method to retrieve a new instance of a Queue Enumerator.  The client then calls the
     * hasMoreMessages method of the Enumeration, if it returns true the client can safely
     * call the nextMessage method of the Enumeration instance.
     *
     *      Enumeration* enumeration = queueBrowser->getEnumeration();
     *
     *      while( enumeration->hasMoreMessages() ) {
     *          cms::Message* message = enumeration->nextMessage();
     *
     *          // ... Do something with the Message.
     *
     *          delete message;
     *      }
     *
     *
     * @since 1.1
     */
    class CMS_API QueueBrowser : public Closeable {
    public:

        virtual ~QueueBrowser() throw();

        /**
         * @returns the Queue that this browser is listening on.
         *
         * @throws CMSException if an internal error occurs.
         */
        virtual const Queue* getQueue() const = 0;

        /**
         * @returns the MessageSelector that is used on when this browser was
         * created or empty string if no selector was present.
         *
         * @throws CMSException if an internal error occurs.
         */
        virtual std::string getMessageSelector() const = 0;

        /**
         * Gets a pointer to an Enumeration object for browsing the Messages currently in
         * the Queue in the order that a client would receive them.  The pointer returned is
         * owned by the browser and should not be deleted by the client application.
         *
         * @returns a pointer to a Queue Enumeration, this Pointer is owned by the QueueBrowser
         *          and should not be deleted by the client.
         *
         * @throws CMSException if an internal error occurs.
         */
        virtual cms::MessageEnumeration* getEnumeration() = 0;

    };

}

#endif /*_CMS_QUEUEBROWSER_H_*/
