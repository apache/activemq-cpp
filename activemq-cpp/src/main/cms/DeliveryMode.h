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

#ifndef _CMS_DELIVERYMODE_H_
#define _CMS_DELIVERYMODE_H_

#include <cms/Config.h>

namespace cms{

    /**
     * This is an Abstract class whose purpose is to provide a container
     * for the delivery mode enumeration for CMS messages.
     * <p>
     * When a client sends a <code>cms::Message</code> it can mark the Message as either
     * Persistent or Non-Persistent.  If the client feels that the Message cannot be lost in
     * transit it should mark it as Persistent, otherwise if it is allowable for a Message to
     * occasionally be lost it can mark it as Non-Persistent.  This allows the Provider to
     * balance make tradeoffs between balance and Message throughput.
     * <p>
     * The DeliveryMode covers only the transport of the Message for sending client to its
     * destination and doesn't apply to the receiving Message consumer.  The receiving
     * Consumer can drop Message's based on configuration such as memory limits or Message
     * filtering.
     * <p>
     * A message is guaranteed to be delivered once and only once by a CMS provider if the
     * delivery mode of the message is PERSISTENT and the configuration of the Message
     * consumer allows for it.
     *
     * @since 1.0
     */
    class CMS_API DeliveryMode {
    public:

        virtual ~DeliveryMode();

        /**
         * Enumeration values for Message Delivery Mode
         */
        enum DELIVERY_MODE {
            PERSISTENT = 0,
            NON_PERSISTENT = 1
        };

    };

}

#endif /*DELIVERYMODE_H_*/
