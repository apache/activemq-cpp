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
cms_status producerDefaultSend(CMS_MessageProducer* producer, CMS_Message* message);

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
