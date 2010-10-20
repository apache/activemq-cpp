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
 * Creates a New Message from the given Session instance.
 *
 * @param session
 *      The Session to use to create the new Message
 * @param message
 *      The address of the location to store the new Message instance.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createMessage(CMS_Session* session, CMS_Message** message);

/**
 * Creates a New Text Message from the given Session instance, if set the value of the
 * body parameter is copied as the body of the Text Message.  The caller retains ownership
 * of the body array and must handle its deallocation.
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
 * Creates a New Bytes Message from the given Session instance.  If set the value of the
 * body parameter is copied as the body of the Bytes Message.  The length parameter defines
 * how many bytes are copied from the body array into the Bytes Message.  The caller retains
 * ownership of the body array and must handle its deallocation.
 *
 * @param session
 *      The Session to use to create the new Message
 * @param message
 *      The address of the location to store the new Message instance.
 * @param body
 *      The bytes that should be copied to the body of the Bytes Message.
 * @param length
 *      The number of bytes contained in the body array.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createBytesMessage(CMS_Session* session, CMS_Message** message, unsigned char* body, int length);

/**
 * Creates a New Map Message from the given Session instance.
 *
 * @param session
 *      The Session to use to create the new Message
 * @param message
 *      The address of the location to store the new Message instance.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createMapMessage(CMS_Session* session, CMS_Message** message);

/**
 * Creates a New Stream Message from the given Session instance.
 *
 * @param session
 *      The Session to use to create the new Message
 * @param message
 *      The address of the location to store the new Message instance.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status createStreamMessage(CMS_Session* session, CMS_Message** message);

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
