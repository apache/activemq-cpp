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

#ifndef CMSBYTESMESSAGE_H_
#define CMSBYTESMESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Gets the number of bytes contained in this BytesMessage.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param length
 *      The number of bytes contained in the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status getMessageBodyLength(CMS_Message* message, int* length);

/**
 * Puts the message body in read-only mode and repositions the stream
 * of bytes to the beginning.
 *
 * @param message
 *      The CMS Message instance to operate on.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status resetBytesMessage(CMS_Message* message);

/**
 * Reads a boolean value from the Bytes Message Body (reads one byte).
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readBooleanFromBytesMessage(CMS_Message* message, int* value);

/**
 * Write a boolean value to the Bytes Message body as one byte (either zero or one).
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeBooleanToBytesMessage(CMS_Message* message, int value);

/**
 * Reads a byte from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readByteFromBytesMessage(CMS_Message* message, unsigned char* value);

/**
 * Writes a byte to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeByteToBytesMessage(CMS_Message* message, unsigned char value);

/**
 * Reads a char from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readCharFromBytesMessage(CMS_Message* message, char* value);

/**
 * Writes a char to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeCharToBytesMessage(CMS_Message* message, char value);

/**
 * Reads a float from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readFloatFromBytesMessage(CMS_Message* message, float* value);

/**
 * Writes a float to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeFloatToBytesMessage(CMS_Message* message, float value);

/**
 * Reads a double from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readDoubleFromBytesMessage(CMS_Message* message, double* value);

/**
 * Writes a double to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeDoubleToBytesMessage(CMS_Message* message, double value);

/**
 * Reads a short from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readShortFromBytesMessage(CMS_Message* message, short* value);

/**
 * Writes a short to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeShortToBytesMessage(CMS_Message* message, short value);

/**
 * Reads a int from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readIntFromBytesMessage(CMS_Message* message, int* value);

/**
 * Writes a int to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeIntToBytesMessage(CMS_Message* message, int value);

/**
 * Reads a long long from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readLongFromBytesMessage(CMS_Message* message, long long* value);

/**
 * Writes a long long to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeLongToBytesMessage(CMS_Message* message, long long value);

/**
 * Reads raw bytes from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 * @param size
 *      The size of the passed in string buffer, only that amount will be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readBytesFromBytesMessage(CMS_Message* message, unsigned char* value, int size);

/**
 * Writes raw bytes string to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 * @param offset
 *      The position in the passed buffer to start writing from.
 * @param length
 *      The number of bytes to write to the Message.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeBytesToBytesMessage(CMS_Message* message, const unsigned char* value, int offset, int length);

/**
 * Reads a null terminated ASCII string from the Bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 * @param size
 *      The size of the passed in string buffer, only that amount will be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readStringFromBytesMessage(CMS_Message* message, char* value, int size);

/**
 * Writes a null terminated ASCII string to the bytes message stream.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeStringToBytesMessage(CMS_Message* message, const char* value);

/**
 * Reads a null terminated UTF-8 string from the Bytes message stream and decodes it.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      Pointer to the location to store the read value.
 * @param size
 *      The size of the passed in string buffer, only that amount will be stored.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status readUTFFromBytesMessage(CMS_Message* message, char* value, int size);

/**
 * Writes a null terminated UTF-8 string to the bytes message stream encoding it first.
 *
 * @param message
 *      The CMS Message instance to operate on.
 * @param value
 *      The value to write to the Message body.
 *
 * @return result code indicating the success or failure of the operation.
 */
cms_status writeUTFToBytesMessage(CMS_Message* message, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* CMSBYTESMESSAGE_H_ */
