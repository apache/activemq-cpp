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

#include "OpenwireStringSupport.h"

#include <decaf/lang/Integer.h>
#include <activemq/exceptions/ActiveMQException.h>

using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::utils;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
std::string OpenwireStringSupport::readString( decaf::io::DataInputStream& dataIn )
    throw ( decaf::io::IOException ) {

    try {

        int utfLength = dataIn.readInt();

        if( utfLength <= 0 ) {
            return "";
        }

        std::vector<unsigned char> buffer( utfLength );
        std::vector<unsigned char> result( utfLength );

        dataIn.readFully( &buffer[0], 0, utfLength );

        int count = 0;
        int index = 0;
        unsigned char a = 0;

        while( count < utfLength ) {
            if( ( result[index] = buffer[count++] ) < 0x80 ) {
                index++;
            } else if( ( ( a = result[index] ) & 0xE0 ) == 0xC0 ) {
                if( count >= utfLength ) {
                    throw UTFDataFormatException(
                        __FILE__, __LINE__,
                        "Invalid UTF-8 encoding found, start of two byte char found at end.");
                }

                unsigned char b = buffer[count++];
                if( ( b & 0xC0 ) != 0x80 ) {
                    throw UTFDataFormatException(
                        __FILE__, __LINE__,
                        "Invalid UTF-8 encoding found, byte two does not start with 0x80." );
                }

                // 2-byte UTF8 encoding: 110X XXxx 10xx xxxx
                // Bits set at 'X' means we have encountered a UTF8 encoded value
                // greater than 255, which is not supported.
                if( a & 0x1C ) {
                    throw UTFDataFormatException(
                        __FILE__, __LINE__,
                        "Invalid 2 byte UTF-8 encoding found, "
                        "This method only supports encoded ASCII values of (0-255)." );
                }

                result[index++] = ( ( a & 0x1F ) << 6 ) | ( b & 0x3F );

            } else if( ( a & 0xF0 ) == 0xE0 ) {

                if( count + 1 >= utfLength ) {
                    throw UTFDataFormatException(
                        __FILE__, __LINE__,
                        "Invalid UTF-8 encoding found, start of three byte char found at end.");
                } else {
                    throw UTFDataFormatException(
                        __FILE__, __LINE__,
                        "Invalid 3 byte UTF-8 encoding found, "
                        "This method only supports encoded ASCII values of (0-255)." );
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
                throw UTFDataFormatException(
                    __FILE__, __LINE__, "Invalid UTF-8 encoding found, aborting.");
            }
        }

        return std::string( (char*)( &result[0] ), index );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireStringSupport::writeString( decaf::io::DataOutputStream& dataOut,
                                         const std::string* str )
                                         throw ( decaf::io::IOException ) {

    try {

        if( str != NULL ) {

            int utfLength = 0;
            std::size_t length = str->length();

            for( std::size_t i = 0; i < length; ++i ) {

                unsigned int charValue = (unsigned char)str->at( i );

                // Written to allow for expansion to wide character strings at some
                // point, as it stands now the value can never be > 255 since the
                // string class returns a single byte char.
                if( charValue > 0 && charValue <= 127 ) {
                    utfLength++;
                } else if( charValue <= 2047 ) {
                    utfLength += 2;
                } else {
                    utfLength += 3;
                }
            }

            if( utfLength > Integer::MAX_VALUE ) {
                throw UTFDataFormatException(
                    __FILE__, __LINE__,
                    ( std::string( "OpenwireStringSupport::writeString - Cannot marshall " ) +
                    "string utf8 encoding longer than: 2^31 bytes, supplied string utf8 encoding was: " +
                    Integer::toString( (int)utfLength ) + " bytes long." ).c_str() );
            }

            std::vector<unsigned char> utfBytes( (std::size_t)utfLength );
            unsigned int utfIndex = 0;

            for( std::size_t i = 0; i < length; i++ ) {

                unsigned int charValue = (unsigned char)str->at( i );

                // Written to allow for expansion to wide character strings at some
                // point, as it stands now the value can never be > 255 since the
                // string class returns a single byte char.
                if( charValue > 0 && charValue <= 127 ) {
                    utfBytes[utfIndex++] = (unsigned char)charValue;
                } else if( charValue <= 2047 ) {
                    utfBytes[utfIndex++] = (unsigned char)(0xc0 | (0x1f & (charValue >> 6)));
                    utfBytes[utfIndex++] = (unsigned char)(0x80 | (0x3f & charValue));
                } else {
                    utfBytes[utfIndex++] = (unsigned char)(0xe0 | (0x0f & (charValue >> 12)));
                    utfBytes[utfIndex++] = (unsigned char)(0x80 | (0x3f & (charValue >> 6)));
                    utfBytes[utfIndex++] = (unsigned char)(0x80 | (0x3f & charValue));
                }
            }

            dataOut.writeInt( utfLength );
            dataOut.write( &utfBytes[0], 0, utfLength );

        } else {
            dataOut.writeInt( -1 );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}
