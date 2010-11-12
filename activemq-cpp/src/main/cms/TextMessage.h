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

#ifndef _CMS_TEXTMESSAGE_H_
#define _CMS_TEXTMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>
#include <cms/CMSException.h>
#include <cms/MessageNotWriteableException.h>

namespace cms{

    /**
     * Interface for a text message.
     *
     * A TextMessage can contain any Text based pay load such as an XML Document or
     * other Text based document.
     *
     * Like all Messages, a TextMessage is received in Read-Only mode, any attempt to write
     * to the Message will result in a MessageNotWritableException being thrown until the
     * <code>clearBody</code> method is called which will erase the contents and place the
     * message back in a read / write mode.
     *
     * @since 1.0
     */
    class CMS_API TextMessage : public Message{
    public:

        virtual ~TextMessage() throw();

        /**
         * Gets the message character buffer.
         *
         * @return The message character buffer.
         *
         * @throws CMSException - if an internal error occurs.
         */
        virtual std::string getText() const = 0;

        /**
         * Sets the message contents, does not take ownership of the passed
         * char*, but copies it instead.
         *
         * @param msg
         *      The message buffer.
         *
         * @throws CMSException - if an internal error occurs.
         * @throws MessageNotWriteableException - if the message is in read-only mode..
         */
        virtual void setText( const char* msg ) = 0;

        /**
         * Sets the message contents
         *
         * @param msg
         *      The message buffer.
         *
         * @throws CMSException - if an internal error occurs.
         * @throws MessageNotWriteableException - if the message is in read-only mode..
         */
        virtual void setText( const std::string& msg ) = 0;

    };
}

#endif /*_CMS_TEXTMESSAGE_H_*/
