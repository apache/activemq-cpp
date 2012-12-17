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

#ifndef _CMS_MESSAGETRANSFORMER_H_
#define _CMS_MESSAGETRANSFORMER_H_

#include <cms/Config.h>

namespace cms {

    class MessageProducer;
    class MessageConsumer;
    class Message;
    class Session;

    /**
     * Provides an interface for clients to transform cms::Message objects inside the
     * CMS MessageProducer and MessageConsumer objects before the message's are sent or
     * received.
     *
     * @since 3.0
     */
    class MessageTransformer {
    public:

        virtual ~MessageTransformer();

        /**
         * Transforms the given message inside the producer before it is sent to the CMS bus.
         *
         * The contract of this method is that the resulting transformed message pointer is set if and
         * only if the method actually creates a new cms::Message object, otherwise it must always be
         * set to NULL.  The return value indicates whether a transformation took place and indicates
         * that the resulting transformed cms::Message pointer will need to be deleted once the
         * producer has sent the cms::Message on to the CMS bus.
         *
         * @param session
         *      The Session used to create the target MessageProducer for this transformation.
         * @param producer
         *      The MessageProducer instance that is going to handle sending the transformed Message.
         * @param message
         *      The CMS Message object that is to be transformed by this method.
         * @param transformed
         *      A pointer to the location in memory where the newly transformed Message has been allocated.
         *
         * @returns true if the MessageProducer should handle deleting the transformed Message once sent.
         *
         * @throws cms::CMSException if an error occurs during the transform operation.
         */
        virtual bool producerTransform(cms::Session* session, cms::MessageProducer* producer, cms::Message* message, cms::Message** transformed) = 0;

        /**
         * Transforms the given message inside the consumer before it is dispatched to the client code.
         *
         * The contract of this method is that the resulting transformed message pointer is set if and
         * only if the method actually creates a new cms::Message object, otherwise it must always be
         * set to NULL.   The return value indicates whether a transformation took place and indicates
         * that the resulting transformed cms::Message pointer will need to be deleted once the
         * consumer completed message dispatch.
         *
         * @param session
         *      The Session used to create the target MessageConsumer for this transformation.
         * @param consumer
         *      The MessageConsumer instance that is going to handle dispatching the transformed Message.
         * @param message
         *      The CMS Message object that is to be transformed by this method.
         * @param transformed
         *      A pointer to the location in memory where the newly transformed Message has been allocated.
         *
         * @returns true if the MessageConsumer should handle deleting the transformed Message once sent.
         *
         * @throws cms::CMSException if an error occurs during the transform operation.
         */
        virtual bool consumerTransform(cms::Session* session, cms::MessageConsumer* consumer, cms::Message* message, cms::Message** transformed) = 0;

    };

}

#endif /* _CMS_MESSAGETRANSFORMER_H_ */
