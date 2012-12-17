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

#ifndef _CMS_OBJECTMESSAGE_H_
#define _CMS_OBJECTMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>

namespace cms{

    /**
     * Place holder for interaction with JMS systems that support Java, the C++
     * client is not responsible for deserializing the contained Object.  The Object
     * can be accessed in its serialized form as a vector of bytes which allows for
     * bridging of message systems.
     *
     * serialized <code>ObjectMessage</code>s.
     *
     * @since 1.0
     */
    class CMS_API ObjectMessage : public Message {
    public:

        virtual ~ObjectMessage();

        /**
         * Sets the payload bytes the represent the Object being transmitted.
         *
         * @param bytes
         *      The byte array that contains the serialized object.
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotWriteableException - if the Message is in Read-only Mode.
         */
        virtual void setObjectBytes(const std::vector<unsigned char>& bytes) = 0;

        /**
         * Returns the byte array containing the serialized form of the transmitted Object.
         *
         * @returns a byte vector containing the serialized Object.
         *
         * @throws CMSException - if the operation fails due to an internal error.
         * @throws MessageNotReadableException - if the message is in write only mode.
         */
        virtual std::vector<unsigned char> getObjectBytes() const = 0;

    };

}

#endif /*_CMS_OBJECTMESSAGE_H_*/
