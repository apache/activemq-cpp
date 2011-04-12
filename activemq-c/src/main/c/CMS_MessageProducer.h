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

#include <cms.h>

#ifndef _CMS_MESSAGEPRODUCER_WRAPPER_H_
#define _CMS_MESSAGEPRODUCER_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Given a Session instance, create a new Producer with the default settings.
 *
 * @param session
 *      The Session that is to be used to create the new Producer.
 * @param destination
 *      The Destination that this Producer will publish to.
 * @param producer
 *      The memory location where the newly allocated Producer instance is to be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createProducer(CMS_Session* session, CMS_Destination* destination, CMS_MessageProducer** producer);

/**
 * Given a Message Producer, send the given Message using that Producer.
 *
 * @param producer
 *      The Message Producer to use for this send operation.
 * @param message
 *      The Message to send via the given Message Producer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status producerSendWithDefaults(CMS_MessageProducer* producer, CMS_Message* message);

/**
 * Given a Message Producer, send the given Message using that Producer.
 *
 * @param producer
 *      The Message Producer to use for this send operation.
 * @param message
 *      The Message to send via the given Message Producer.
 * @param timeOut
 *      The timeout for the message.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status producerSendWithTimeOut(CMS_MessageProducer* producer, CMS_Message* message, long long timeOut);

/**
 * Sets the delivery mode used by the given producer.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param mode
 *      The new delivery mode to assign to the Producer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setProducerDeliveryMode(CMS_MessageProducer* producer, int mode);

/**
 * Gets the delivery mode used by the given producer.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param mode
 *      The address where the delivery mode value is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getProducerDeliveryMode(CMS_MessageProducer* producer, int* mode);

/**
 * Sets the value of the Producer's disable Message Id setting.  A value of zero indicates
 * that the disable message Id option is disabled, otherwise its enabled.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param enabled
 *      The new disable message id setting for the given MessageProducer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setProducerDisableMessageID(CMS_MessageProducer* producer, int enabled);

/**
 * Gets the value of the Producer's disable Message Id setting.  A value of zero indicates
 * that the disable message Id option is disabled, otherwise its enabled.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param enabled
 *      The address where the disable message id setting is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getProducerDisableMessageID(CMS_MessageProducer* producer, int* enabled);

/**
 * Sets the value of the Producer's disable time stamp setting.  A value of zero indicates
 * that the disable time stamp option is disabled, otherwise its enabled.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param enabled
 *      The new disable time stamp setting for the given MessageProducer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setProducerDisableMessageTimeStamp(CMS_MessageProducer* producer, int enabled);

/**
 * Gets the value of the Producer's disable time stamps setting.  A value of zero indicates
 * that the disable time stamp option is disabled, otherwise its enabled.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param enabled
 *      The address where the disable time stamp setting is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getProducerDisableMessageTimeStamp(CMS_MessageProducer* producer, int* enabled);

/**
 * Sets the value of the Producer's Message Priority setting.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param priority
 *      The new message priority setting for the given MessageProducer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setProducerPriority(CMS_MessageProducer* producer, int priority);

/**
 * Gets the value of the Producer's Message Priority setting.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param priority
 *      The address where the message priority setting is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getProducerPriority(CMS_MessageProducer* producer, int* priority);

/**
 * Sets the value of the Producer's Message Time to Live setting.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param priority
 *      The new message time to live for the given MessageProducer.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setProducerTimeToLive(CMS_MessageProducer* producer, int timeToLive);

/**
 * Gets the value of the Producer's Message Time to Live setting.
 *
 * @param producer
 *      The Message Producer to use for this operation.
 * @param priority
 *      The address where the message time to live setting is to be written.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getProducerTimeToLive(CMS_MessageProducer* producer, int* timeToLive);

/**
 * Closes the MessageProducer.
 *
 * @param producer
 *      The Producer that is to be closed.
 *
  * @return result code indicating the success or failure of the operation.
 */
cms_status closeProducer(CMS_MessageProducer* producer);

/**
 * Destroys the given Producer instance.
 *
 * @param producer
 *      The Producer that is to be destroyed.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status destroyProducer(CMS_MessageProducer* producer);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_MESSAGEPRODUCER_WRAPPER_H_ */
