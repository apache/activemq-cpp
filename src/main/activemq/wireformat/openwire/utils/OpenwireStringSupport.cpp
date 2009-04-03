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
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::utils;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
std::string OpenwireStringSupport::readString( decaf::io::DataInputStream& dataIn )
    throw ( decaf::io::IOException ) {

    try {

        short utflen = dataIn.readShort();

        if( utflen > -1 )
        {
            // Let the stream get us all that data.
            std::vector<unsigned char> value;
            value.resize( utflen );
            dataIn.readFully( value );

            unsigned char c = 0;
            int count = 0;

            // x counts the number of 2-byte UTF8 sequences decoded
            int x = 0;

            while( count+x < utflen )
            {
                c = value[count+x];
                switch( c >> 4 )
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        // 1-byte UTF8 encoding: 0xxxxxxx
                        value[count] = c;
                        count++;
                        break;
                    case 12:
                    case 13:
                        // 2-byte UTF8 encoding: 110X XXxx 10xx xxxx
                        // Bits set at 'X' means we have encountered a UTF8 encoded value
                        // greater than 255, which is not supported.
                        if( c & 0x1C ) {
                            throw IOException(
                                __FILE__,
                                __LINE__,
                                "OpenwireStringSupport::readString - Encoding not supported" );
                        }
                        // Place the decoded UTF8 character back into the value array
                        value[count] = ((c & 0x1F) << 6) | (value[count+x+1] & 0x3F);
                        count++;
                        x++;
                        break;
                    case 14:
                    default:
                    {
                        // 3-byte UTF8 encoding: 1110 xxxx  10xx xxxx  10xx xxxx
                        throw IOException(
                            __FILE__,
                            __LINE__,
                            "OpenwireStringSupport::readString - Encoding not supported" );
                    }
                }
            }

            // Let the Compiler give us a string.
            return std::string(reinterpret_cast<const char*>(&value[0]), count);
        }

        return "";
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

            if( str->size() > 65536 ) {

                throw IOException(
                    __FILE__,
                    __LINE__,
                    ( std::string( "OpenwireStringSupport::writeString - Cannot marshall " ) +
                    "string longer than: 65536 characters, supplied string was: " +
                    Integer::toString( (int)str->size() ) + " characters long." ).c_str() );
            }

            unsigned short utflen = 0;
            int count = 0;
            unsigned char c;

            std::string::const_iterator iter = str->begin();

            for(; iter != str->end(); ++iter ) {
                c = *iter;
                if( c < 0x80 ) {
                    utflen++;
                } else {
                    utflen += 2;
                }
            }

            dataOut.writeUnsignedShort( utflen );
            std::vector<unsigned char> byteArr;
            byteArr.resize( utflen );

            for( iter = str->begin(); iter != str->end(); ++iter ) {

                c = *iter;
                if( c < 0x80 ) {
                    byteArr[count++] = (unsigned char)c;
                } else {
                    byteArr[count++] = (unsigned char)( 0xC0 | ( (c >> 6) & 0x1F) );
                    byteArr[count++] = (unsigned char)( 0x80 | ( (c >> 0) & 0x3F) );
                }
            }

            dataOut.write( byteArr );

        } else {
            dataOut.writeShort( (short)-1 );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( Exception, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}
