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

#ifndef _CMS_MESSAGECONSUMER_H_
#define _CMS_MESSAGECONSUMER_H_

#include <cms/Config.h>
#include <cms/MessageListener.h>
#include <cms/Message.h>
#include <cms/Closeable.h>

namespace cms{

    /**
     * A client uses a <code>MessageConsumer</code> to received messages
     * from a destination.
     *
     * A client may either synchronously receive a message consumer's messages
     * or have the consumer asynchronously deliver them as they arrive.
     *
     * For synchronous receipt, a client can request the next message from a
     * message consumer using one of its <code>receive</code> methods. There are
     * several variations of <code>receive</code> that allow a client to poll or
     * wait for the next message.
     *
     * For asynchronous delivery, a client can register a
     * <code>MessageListener</code> object with a message consumer. As messages
     * arrive at the message consumer, it delivers them by calling the
     * <code>MessageListener</code>'s <code>onMessage</code> method.
     *
     * When the MessageConsumer's close method is called the method can block while
     * an asynchronous message delivery is in progress or until a <code>receive</code>
     * operation completes.  A blocked consumer in a <code>receive</code> call will
     * return a Null when the close method is called.
     *
     * @see MessageListener
     *
     * @since 1.0
     */
    class CMS_API MessageConsumer : public Closeable {
    public:

        virtual ~MessageConsumer() throw();

        /**
         * Synchronously Receive a Message
         *
         * @return new message which the caller owns and must delete.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Message* receive() = 0;

        /**
         * Synchronously Receive a Message, time out after defined interval.
         * Returns null if nothing read.
         *
         * @return new message which the caller owns and must delete.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Message* receive( int millisecs ) = 0;

        /**
         * Receive a Message, does not wait if there isn't a new message
         * to read, returns NULL if nothing read.
         *
         * @return new message which the caller owns and must delete.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual Message* receiveNoWait() = 0;

        /**
         * Sets the MessageListener that this class will send notifs on
         *
         * @param listener
         *      The listener of messages received by this consumer.
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual void setMessageListener( MessageListener* listener ) = 0;

        /**
         * Gets the MessageListener that this class will send mew Message
         * notification events to.
         *
         * @return The listener of messages received by this consumer
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual MessageListener* getMessageListener() const = 0;

        /**
         * Gets this message consumer's message selector expression.
         *
         * @return This Consumer's selector expression or "".
         *
         * @throws CMSException - If an internal error occurs.
         */
        virtual std::string getMessageSelector() const = 0;

    };

}

#endif /*_CMS_MESSAGECONSUMER_H_*/
