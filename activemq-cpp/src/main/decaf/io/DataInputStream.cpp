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

#include <decaf/io/DataInputStream.h>

#include <decaf/io/PushbackInputStream.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DataInputStream::DataInputStream(InputStream* inputStream, bool own) :
    FilterInputStream(inputStream, own), buffer() {
}

////////////////////////////////////////////////////////////////////////////////
DataInputStream::~DataInputStream() {
}

////////////////////////////////////////////////////////////////////////////////
bool DataInputStream::readBoolean() {

    try {
        readAllData(buffer, sizeof(char));
        return (bool) (buffer[0] != 0);
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
char DataInputStream::readByte() {

    try {
        readAllData(buffer, sizeof(unsigned char));
        return (char) (buffer[0]);
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DataInputStream::readUnsignedByte() {

    try {
        readAllData(buffer, sizeof(unsigned char));
        return buffer[0];
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
char DataInputStream::readChar() {

    try {
        readAllData(buffer, sizeof(unsigned char));
        return (char) (buffer[0]);
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
short DataInputStream::readShort() {

    try {
        short value = 0;
        readAllData(buffer, sizeof(short));
        value |= (short) (buffer[0] << 8 | buffer[1] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
unsigned short DataInputStream::readUnsignedShort() {

    try {
        unsigned short value = 0;
        readAllData(buffer, sizeof(unsigned short));
        value |= (unsigned short) (buffer[0] << 8 | buffer[1] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
int DataInputStream::readInt() {

    try {
        unsigned int value = 0;
        readAllData(buffer, sizeof(int));
        value |= (buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
double DataInputStream::readDouble() {

    try {
        unsigned long long lvalue = this->readLong();
        double value = 0.0;
        memcpy(&value, &lvalue, sizeof(unsigned long long));
        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
float DataInputStream::readFloat() {

    try {
        unsigned int lvalue = this->readInt();
        float value = 0.0f;
        memcpy(&value, &lvalue, sizeof(unsigned int));
        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long DataInputStream::readLong() {

    try {
        unsigned long long value = 0;
        readAllData(buffer, sizeof(long long));

        // Have to do it this way because on Solaris and Cygwin we get all
        // kinds of warnings when shifting a byte up into a long long.
        unsigned long long byte1 = buffer[0] & 0x00000000000000FFULL;
        unsigned long long byte2 = buffer[1] & 0x00000000000000FFULL;
        unsigned long long byte3 = buffer[2] & 0x00000000000000FFULL;
        unsigned long long byte4 = buffer[3] & 0x00000000000000FFULL;
        unsigned long long byte5 = buffer[4] & 0x00000000000000FFULL;
        unsigned long long byte6 = buffer[5] & 0x00000000000000FFULL;
        unsigned long long byte7 = buffer[6] & 0x00000000000000FFULL;
        unsigned long long byte8 = buffer[7] & 0x00000000000000FFULL;

        value = (byte1 << 56 | byte2 << 48 | byte3 << 40 | byte4 << 32 | byte5 << 24 | byte6 << 16 | byte7 << 8 | byte8 << 0);

        return value;
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string DataInputStream::readString() {

    try {

        if (inputStream == NULL) {
            throw IOException(__FILE__, __LINE__, "DataInputStream::readFully - Base input stream is null");
        }

        int size = 1024;
        std::vector<char> buffer;
        buffer.resize(size);
        int pos = 0;

        while (true) {

            if (inputStream->read((unsigned char*) (&buffer[0]), size, pos, 1) == -1) {
                throw EOFException(__FILE__, __LINE__, "DataInputStream::readString - Reached EOF");
            }

            // if null is found we are done
            if (buffer[pos] == '\0') {
                break;
            }

            // Resize to hold more if we exceed current size
            if (++pos >= size) {
                buffer.resize((size *= 2));
            }
        }

        return &buffer[0];
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string DataInputStream::readUTF() {

    try {

        if (inputStream == NULL) {
            throw IOException(__FILE__, __LINE__, "DataInputStream::readFully - Base input stream is null");
        }

        unsigned short utfLength = readUnsignedShort();
        if (utfLength == 0) {
            return "";
        }

        std::vector<unsigned char> buffer(utfLength);
        std::vector<unsigned char> result(utfLength);

        this->readFully(&buffer[0], utfLength);

        std::size_t count = 0;
        std::size_t index = 0;
        unsigned char a = 0;

        while (count < utfLength) {
            if ((result[index] = buffer[count++]) < 0x80) {
                index++;
            } else if (((a = result[index]) & 0xE0) == 0xC0) {
                if (count >= utfLength) {
                    throw UTFDataFormatException(__FILE__, __LINE__, "Invalid UTF-8 encoding found, start of two byte char found at end.");
                }

                unsigned char b = buffer[count++];
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
                //int b = buffer[count++];
                //int c = buffer[count++];
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
    DECAF_CATCH_RETHROW(UTFDataFormatException)
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStream::readFully(unsigned char* buffer, int size) {

    try {

        if (size == 0) {
            return;
        }

        this->readFully(buffer, size, 0, size);
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
std::string DataInputStream::readLine() {
    try {

        std::string line;
        bool foundTerminator = false;

        while (true) {

            int nextByte = inputStream->read();

            if (nextByte == -1) {

                if (line.length() == 0 && !foundTerminator) {
                    return "";
                }
                return line;

            } else if (nextByte == (unsigned char) '\r') {

                PushbackInputStream* pbStream = dynamic_cast<PushbackInputStream*>(inputStream);

                if (foundTerminator) {

                    if (pbStream == NULL) {
                        throw IOException(__FILE__, __LINE__, "State is not valid, parse failed.");
                    }

                    pbStream->unread((unsigned char) nextByte);
                    return line;
                }

                foundTerminator = true;

                // Have to be able to peek ahead one byte to see if its an newline.
                if (pbStream == NULL) {
                    inputStream = new PushbackInputStream(inputStream, own);
                    own = true;
                }

            } else if (nextByte == (unsigned char) '\n') {

                return line;

            } else {

                if (foundTerminator) {
                    PushbackInputStream* pbStream = dynamic_cast<PushbackInputStream*>(inputStream);

                    if (pbStream == NULL) {
                        throw IOException(__FILE__, __LINE__, "State is not valid, parse failed.");
                    }

                    pbStream->unread((unsigned char) nextByte);
                    return line;
                }

                line += (char) nextByte;
            }
        }
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStream::readFully(unsigned char* buffer, int size, int offset, int length) {

    try {

        if (length == 0) {
            return;
        }

        if (buffer == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Buffer is null");
        }

        if (inputStream == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "Base input stream is null");
        }

        if (size < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
        }

        if (offset > size || offset < 0) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
        }

        if (length < 0 || length > size - offset) {
            throw IndexOutOfBoundsException(__FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
        }

        int n = 0;
        while (n < length) {
            int count = inputStream->read(buffer, length, offset + n, length - n);
            if (count == -1) {
                throw EOFException(__FILE__, __LINE__, "Reached EOF");
            }
            n += count;
        }
    }
    DECAF_CATCH_RETHROW(NullPointerException)
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
long long DataInputStream::skipBytes(long long num) {

    try {

        if (inputStream == NULL) {
            throw IOException(__FILE__, __LINE__, "DataInputStream::readFully - Base input stream is null");
        }

        long long total = 0;
        long long cur = 0;

        while ((total < num) && ((cur = inputStream->skip(num - total)) > 0)) {
            total += cur;
        }

        return total;
    }
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStream::readAllData(unsigned char* buffer, int length) {

    try {

        int n = 0;
        do {
            int count = inputStream->read(buffer, length, n, length - n);
            if (count == -1) {
                throw EOFException(__FILE__, __LINE__, "DataInputStream::readLong - Reached EOF");
            }
            n += count;
        } while (n < length);
    }
    DECAF_CATCH_RETHROW(EOFException)
    DECAF_CATCH_RETHROW(IOException)
    DECAF_CATCHALL_THROW(IOException)
}
