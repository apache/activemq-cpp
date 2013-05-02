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

#include "MarshallingSupport.h"

#include <activemq/util/PrimitiveValueNode.h>
#include <activemq/exceptions/ExceptionDefines.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
MarshallingSupport::MarshallingSupport() {
}

////////////////////////////////////////////////////////////////////////////////
MarshallingSupport::~MarshallingSupport() {
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupport::writeString(decaf::io::DataOutputStream& dataOut, const std::string& value) {

    try {

        if ((int)value.length() <= Short::MAX_VALUE / 4) {
            dataOut.write(PrimitiveValueNode::STRING_TYPE);
            MarshallingSupport::writeString16(dataOut, value);
        } else {
            dataOut.write(PrimitiveValueNode::BIG_STRING_TYPE);
            MarshallingSupport::writeString32(dataOut, value);
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupport::writeString16(decaf::io::DataOutputStream& dataOut, const std::string& value) {

    try {

        int strSize = (int) value.length();

        if (strSize > Short::MAX_VALUE) {
            throw IOException(__FILE__, __LINE__, "String size exceeds Short::MAX_VALUE and cannot be sent via Openwire.");
        }

        dataOut.writeShort((short) strSize);
        if (strSize > 0) {
            dataOut.write((unsigned char*) value.c_str(), strSize, 0, strSize);
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
void MarshallingSupport::writeString32(decaf::io::DataOutputStream& dataOut, const std::string& value) {

    try {

        int strSize = (int) value.length();

        if (strSize > Integer::MAX_VALUE) {
            throw IOException(__FILE__, __LINE__, "String size exceeds Integer::MAX_VALUE and cannot be sent via Openwire.");
        }

        dataOut.writeInt(strSize);
        if (strSize > 0) {
            dataOut.write((unsigned char*) value.c_str(), strSize, 0, strSize);
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshallingSupport::readString16(decaf::io::DataInputStream& dataIn) {

    try {
        int utfLength = dataIn.readShort();
        if (utfLength > 0) {

            std::vector<unsigned char> buffer(utfLength);
            dataIn.readFully(&buffer[0], utfLength);
            return std::string((char*) (&buffer[0]), utfLength);
        }
        return "";
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshallingSupport::readString32(decaf::io::DataInputStream& dataIn) {

    try {
        int utfLength = dataIn.readInt();
        if (utfLength > 0) {

            std::vector<unsigned char> buffer(utfLength);
            dataIn.readFully(&buffer[0], utfLength);
            return std::string((char*) (&buffer[0]), utfLength);
        }
        return "";
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshallingSupport::asciiToModifiedUtf8(const std::string& asciiString) {

    try {

        if (asciiString.length() > 0) {

            int utfLength = 0;
            std::size_t length = asciiString.length();

            for (std::size_t i = 0; i < length; ++i) {

                unsigned int charValue = (unsigned char) asciiString.at(i);

                // Written to allow for expansion to wide character strings at some
                // point, as it stands now the value can never be > 255 since the
                // string class returns a single byte char.
                if (charValue > 0 && charValue <= 127) {
                    utfLength++;
                } else if (charValue <= 2047) {
                    utfLength += 2;
                } else {
                    utfLength += 3;
                }
            }

            if (utfLength > Integer::MAX_VALUE) {
                throw UTFDataFormatException(__FILE__, __LINE__,
                        (std::string("MarshallingSupport::asciiToModifiedUtf8 - Cannot marshall ")
                                + "string utf8 encoding longer than: 2^31 bytes, supplied string utf8 encoding was: " + Integer::toString((int) utfLength)
                                + " bytes long.").c_str());
            }

            std::string utfBytes((std::size_t) utfLength, '\0');
            unsigned int utfIndex = 0;

            for (std::size_t i = 0; i < length; i++) {

                unsigned int charValue = (unsigned char) asciiString.at(i);

                // Written to allow for expansion to wide character strings at some
                // point, as it stands now the value can never be > 255 since the
                // string class returns a single byte char.
                if (charValue > 0 && charValue <= 127) {
                    utfBytes[utfIndex++] = (char) charValue;
                } else if (charValue <= 2047) {
                    utfBytes[utfIndex++] = (char) (0xc0 | (0x1f & (charValue >> 6)));
                    utfBytes[utfIndex++] = (char) (0x80 | (0x3f & charValue));
                } else {
                    utfBytes[utfIndex++] = (char) (0xe0 | (0x0f & (charValue >> 12)));
                    utfBytes[utfIndex++] = (char) (0x80 | (0x3f & (charValue >> 6)));
                    utfBytes[utfIndex++] = (char) (0x80 | (0x3f & charValue));
                }
            }

            return utfBytes;
        } else {
            return "";
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::UTFDataFormatException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::UTFDataFormatException)
    AMQ_CATCHALL_THROW(decaf::io::UTFDataFormatException)
}

////////////////////////////////////////////////////////////////////////////////
std::string MarshallingSupport::modifiedUtf8ToAscii(const std::string modifiedUtf8String) {

    try {

        std::size_t utfLength = modifiedUtf8String.length();

        if (utfLength == 0) {
            return "";
        }

        std::vector<unsigned char> result(utfLength);

        std::size_t count = 0;
        std::size_t index = 0;
        unsigned char a = 0;

        while (count < utfLength) {
            if ((result[index] = modifiedUtf8String[count++]) < 0x80) {
                index++;
            } else if (((a = result[index]) & 0xE0) == 0xC0) {
                if (count >= utfLength) {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid UTF-8 encoding found, start of two byte char found at end.");
                }

                unsigned char b = modifiedUtf8String[count++];
                if ((b & 0xC0) != 0x80) {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid UTF-8 encoding found, byte two does not start with 0x80.");
                }

                // 2-byte UTF8 encoding: 110X XXxx 10xx xxxx
                // Bits set at 'X' means we have encountered a UTF8 encoded value
                // greater than 255, which is not supported.
                if (a & 0x1C) {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid 2 byte UTF-8 encoding found, "
                            "This method only supports encoded ASCII values of (0-255).");
                }

                result[index++] = (unsigned char) (((a & 0x1F) << 6) | (b & 0x3F));

            } else if ((a & 0xF0) == 0xE0) {

                if (count + 1 >= utfLength) {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid UTF-8 encoding found, start of three byte char found at end.");
                } else {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid 3 byte UTF-8 encoding found, "
                            "This method only supports encoded ASCII values of (0-255).");
                }

                // If we were to support multibyte strings in the future this would be
                // the remainder of this method decoding logic.
                //
                //int b = modifiedUtf8String[count++];
                //int c = modifiedUtf8String[count++];
                //if( ( ( b & 0xC0 ) != 0x80 ) || ( ( c & 0xC0 ) != 0x80 ) ) {
                //    throw UTFDataFormatException(
                //        __FILE__, __LINE__,
                //        "Invalid UTF-8 encoding found, byte two does not start with 0x80." );
                //}
                //
                //result[inde++] = ( ( a & 0x0F ) << 12 ) |
                //                 ( ( b & 0x3F ) << 6 ) | ( c & 0x3F );

            } else {
                throw UTFDataFormatException(__FILE__, __LINE__, "Invalid UTF-8 encoding found, aborting.");
            }
        }

        return std::string((char*) (&result[0]), index);
    }
    AMQ_CATCH_RETHROW(decaf::io::UTFDataFormatException)
    AMQ_CATCH_EXCEPTION_CONVERT(Exception, decaf::io::UTFDataFormatException)
    AMQ_CATCHALL_THROW(decaf::io::UTFDataFormatException)
}
