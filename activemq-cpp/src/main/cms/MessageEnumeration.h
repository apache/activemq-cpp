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

#ifndef _CMS_MESSAGEENUMERATION_H_
#define _CMS_MESSAGEENUMERATION_H_

#include <cms/Config.h>
#include <cms/Message.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * Defines an object that enumerates a collection of Messages.  The client calls
     * the hasMoreMessages method to determine if a Message is available.  If a Message
     * is available the client calls the nextMessage method to retrieve that Message,
     * calling nextMessage when a Message is not available results in an exception.
     *
     * @since 2.1
     */
    class CMS_API MessageEnumeration {
    public:

        virtual ~MessageEnumeration() throw();

        /**
         * Returns true if there are more Message in the Browser that can be retrieved
         * via the <code>nextMessage</code> method.  If this method returns false and the
         * <code>nextMessage</code> method is called then an Exception will be thrown.
         *
         * @return true if more Message's are available in the Browser.
         */
        virtual bool hasMoreMessages() = 0;

        /**
         * Returns the Next Message in the Queue if one is present, if no more Message's are
         * available then an Exception is thrown.  If a Message object pointer is returned then
         * that object becomes the property of the caller and must be deleted by the caller
         * when finished.
         *
         * @return The next Message in the Queue.
         *
         * @throws CMSException if no more Message's currently in the Queue.
         */
        virtual cms::Message* nextMessage() = 0;

    };
}

#endif /* _CMS_MESSAGEENUMERATION_H_ */
