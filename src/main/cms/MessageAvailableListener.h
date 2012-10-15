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

#ifndef _CMS_MESSAGEAVAILABLELISTENER_H_
#define _CMS_MESSAGEAVAILABLELISTENER_H_

#include <cms/Config.h>

namespace cms {

    class MessageConsumer;

    /**
     * A listener interface similar to the MessageListener interface.  This listener
     * is notified by its associated MessageConsumer that a new Message is available
     * for processing when the consumer is in sync consumption mode.  A consumer with
     * a registered MessageListener will not use this listener.
     */
    class CMS_API MessageAvailableListener {
    public:

        virtual ~MessageAvailableListener();

        /**
         * Indicates that a new Message has become available for consumption
         * from the provided consumer, a call to receiveNoWait should return
         * the message immediately.
         *
         * @param consumer
         *      The consumer that now has a message queued for consumption.
         */
        virtual void onMessageAvailable(cms::MessageConsumer* consumer) = 0;

    };

}
#endif /* _CMS_MESSAGEAVAILABLELISTENER_H_ */
