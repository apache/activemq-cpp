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

#ifndef _CMS_MESSAGE_WRAPPER_H_
#define _CMS_MESSAGE_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a New Text Message from the given Session instance, if set the value of the
 * body parameter is assigned as the body of the Text Message.
 *
 * @param session
 *      The Session to use to create the new Text Message
 * @param message
 *      The address of the location to store the new Message instance.
 * @param body
 *      The text that should be assigned to the body of the Text Message.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createTextMessage(CMS_Session* session, CMS_Message** message, const char* body);

/**
 * Destroy the given Message instance.
 *
 * @param message
 *      The Message to destroy.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status destroyMessage(CMS_Message* message);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_MESSAGE_WRAPPER_H_ */
