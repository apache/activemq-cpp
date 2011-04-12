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

#ifndef _CMS_TEXTMESSAGE_WRAPPER_H_
#define _CMS_TEXTMESSAGE_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Gets the string value contained in a TextMessage body.
 *
 * @param message
 *      The message that the text is retreived from.
 * @param dest
 *      The address to store the value of the message text.
 * @param size
 *      The size of the character array that is provided to receive the string payload.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageText(CMS_Message* message, char* dest, int size);

/**
 * Sets a value of the String body of the given Text Message.  If the given message is
 * not a cms::TextMessage than an error code is returned.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param value
 *      The value to store in the given message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageText(CMS_Message* message, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_TEXTMESSAGE_WRAPPER_H_ */
