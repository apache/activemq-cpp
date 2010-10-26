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

/**
 * Creates a Copy of the given Message instance.
 *
 * @param original
 *      The Message to clone.
 * @param clone
 *      The address of the location to store the new Message instance.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status cloneMessage(CMS_Message* original, CMS_Message** clone);

/**
 * Acknowledge the given Message.
 *
 * @param message
 *      The Message to acknowledge.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status acknowledgeMessage(CMS_Message* message);

/**
 * Clears out the body of the message.  This does not clear the
 * headers or properties.
 *
 * @param message
 *      The Message whose body is to be cleared.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status clearBody(CMS_Message* message);

/**
 * Clears out the message body. Clearing a message's body does not clear
 * its header values or property entries.
 *
 * If this message body was read-only, calling this method leaves the
 * message body in the same state as an empty body in a newly created
 * message.
 *
 * @param message
 *      The Message whose properties are to be cleared.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status clearMessageProperties(CMS_Message* message);

/**
 * Gets a count of the number of properties that are contained in the given Message.
 *
 * @param message
 *      The Message whose property count is to be fetched.
 * @param numProperties
 *      The address where the number of properties value is to be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getNumMessageProperties(CMS_Message* message, int* numProperties);

/*********************************/
/* TODO                          */
/*cms_status getMessagePropertyNames(CMS_Message* message, char** properties, int* size);*/
/*********************************/

/**
 * Indicates whether or not a given property exists in the supplied Message instance.
 * If the property is contained in the Message than the exists value is set to 1 otherwise
 * it is set to zero.
 *
 * @param message
 *      The message that should be queried for the given property name.
 * @param key
 *      The name of the property that the message should be checked for.
 * @param exists
 *      The address to store the boolean result of this operation.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status messagePropertyExists(CMS_Message* message, const char* key, int* exists);

/**
 * Gets a boolean property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageBooleanProperty(CMS_Message* message, const char* key, int* value);

/**
 * Gets a byte property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageByteProperty(CMS_Message* message, const char* key, unsigned char* value);

/**
 * Gets a double property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageDoubleProperty(CMS_Message* message, const char* key, double* value);

/**
 * Gets a float property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageFloatProperty(CMS_Message* message, const char* key, float* value);

/**
 * Gets a int property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageIntProperty(CMS_Message* message, const char* key, int* value);

/**
 * Gets a long property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageLongProperty(CMS_Message* message, const char* key, long long* value);

/**
 * Gets a short property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageShortProperty(CMS_Message* message, const char* key, short* value);

/**
 * Gets a string property.
 *
 * @param message
 *      The message that should be queried for the given property.
 * @param key
 *      The name of the property that should be fetched from the Message instance.
 * @param value
 *      The address to store the value of the requested property.
 * @param size
 *      The size of the character array that is provided to receive the string payload.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageStringProperty(CMS_Message* message, const char* key, char* value, int size);

/**
 * Sets a boolean property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageBooleanProperty(CMS_Message* message, const char* key, int value);

/**
 * Sets a byte property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageByteProperty(CMS_Message* message, const char* key, unsigned char value);

/**
 * Sets a double property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageDoubleProperty(CMS_Message* message, const char* key, double value);

/**
 * Sets a float property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageFloatProperty(CMS_Message* message, const char* key, float value);

/**
 * Sets a int property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageIntProperty(CMS_Message* message, const char* key, int value);

/**
 * Sets a long property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setMessageLongProperty(CMS_Message* message, const char* key, long long value);

/**
 * Sets a short property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setShortProperty(CMS_Message* message, const char* key, short value);

/**
 * Sets a string property.
 *
 * @param message
 *      The message that the property is to be set in.
 * @param key
 *      The name of the property that should be set in the Message instance.
 * @param value
 *      The value to store in the given message property.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status setStringProperty(CMS_Message* message, const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* _CMS_MESSAGE_WRAPPER_H_ */
