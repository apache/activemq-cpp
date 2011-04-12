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

#include <CMS_Message.h>
#include <CMS_BytesMessage.h>

#include <Config.h>
#include <types/CMS_Types.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status getMessageBodyLength(CMS_Message* message, int* length) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && length != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );
        *length = bytesMessage->getBodyLength();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status bytesMessageReset(CMS_Message* message) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->reset();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readBooleanFromBytesMessage(CMS_Message* message, int* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readBoolean();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeBooleanToBytesMessage(CMS_Message* message, int value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeBoolean((bool)value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readByteFromBytesMessage(CMS_Message* message, unsigned char* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readByte();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeByteToBytesMessage(CMS_Message* message, unsigned char value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeByte(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readCharFromBytesMessage(CMS_Message* message, char* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readChar();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeCharToBytesMessage(CMS_Message* message, char value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeChar(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readFloatFromBytesMessage(CMS_Message* message, float* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readFloat();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeFloatToBytesMessage(CMS_Message* message, float value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeFloat(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readDoubleFromBytesMessage(CMS_Message* message, double* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readDouble();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeDoubleToBytesMessage(CMS_Message* message, double value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeDouble(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readShortFromBytesMessage(CMS_Message* message, short* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readShort();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeShortToBytesMessage(CMS_Message* message, short value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeShort(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readIntFromBytesMessage(CMS_Message* message, int* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readInt();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeIntToBytesMessage(CMS_Message* message, int value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeInt(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readLongFromBytesMessage(CMS_Message* message, long long* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            *value = bytesMessage->readLong();
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeLongToBytesMessage(CMS_Message* message, long long value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL) {

        if( message->type != CMS_BYTES_MESSAGE ) {
            return CMS_INVALID_MESSAGE_TYPE;
        }

        cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

        try{
            bytesMessage->writeLong(value);
        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readBytesFromBytesMessage(CMS_Message* message, unsigned char* value, int size) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            int readCount = bytesMessage->readBytes(value, size);

            if( readCount == -1 ) {
                result = CMS_INCOMPLETE_READ;
            }

        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeBytesToBytesMessage(CMS_Message* message, const unsigned char* value, int offset, int length) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            bytesMessage->writeBytes(value, offset, length);

        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readStringFromBytesMessage(CMS_Message* message, char* value, int size) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            std::string str = bytesMessage->readString();

            if(!str.empty()) {

                std::size_t pos = 0;
                for(; pos < str.size() && pos < (std::size_t)size - 1; ++pos) {
                    value[pos] = str.at(pos);
                }

                value[pos] = '\0';
            } else {
                value[0] = '\0';
            }

        } catch(cms::MessageFormatException& ex) {
            value[0] = '\0';
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            value[0] = '\0';
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            value[0] = '\0';
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeStringToBytesMessage(CMS_Message* message, const char* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            if(strlen(value) > 0) {
                bytesMessage->writeString(value);
            }

        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status readUTFFromBytesMessage(CMS_Message* message, char* value, int size) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            std::string str = bytesMessage->readUTF();

            if(!str.empty()) {

                std::size_t pos = 0;
                for(; pos < str.size() && pos < (std::size_t)size - 1; ++pos) {
                    value[pos] = str.at(pos);
                }

                value[pos] = '\0';
            } else {
                value[0] = '\0';
            }

        } catch(cms::MessageFormatException& ex) {
            value[0] = '\0';
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotReadableException& ex) {
            value[0] = '\0';
            result = CMS_MESSAGE_NOT_READABLE;
        } catch(...) {
            value[0] = '\0';
            result = CMS_ERROR;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status writeUTFToBytesMessage(CMS_Message* message, const char* value) {

    cms_status result = CMS_SUCCESS;

    if(message != NULL && value != NULL) {

        try{

            if( message->type != CMS_BYTES_MESSAGE ) {
                return CMS_INVALID_MESSAGE_TYPE;
            }

            cms::BytesMessage* bytesMessage = dynamic_cast<cms::BytesMessage*>( message->message );

            if(strlen(value) > 0) {
                bytesMessage->writeUTF(value);
            }

        } catch(cms::MessageFormatException& ex) {
            result = CMS_MESSAGE_FORMAT_ERROR;
        } catch(cms::MessageNotWriteableException& ex) {
            result = CMS_MESSAGE_NOT_WRITABLE;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
