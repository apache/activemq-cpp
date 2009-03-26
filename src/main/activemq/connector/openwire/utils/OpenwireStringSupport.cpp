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

        short utflen = dataIn.readShort();

        if( utflen > -1 ) {

            // Let the stream get us all that data.
            std::vector<unsigned char> value;
            value.resize( utflen );
            dataIn.readFully( value );

            std::vector<unsigned char> byteArr;
            byteArr.resize( utflen );

            int count = 0;
            for( unsigned int i = 0; i < value.size(); ++i ) {

                unsigned int z = (unsigned char) value[i];

                if( (z >= 0x0001) && (z <= 0x007F) )  {
                    byteArr[count++] = (unsigned char)z;
                } else if( (z >= 0x00C0) && (z <= 0x00DF) ) {
                    unsigned int y = (unsigned char) value[++i];
                    byteArr[count++] = (unsigned char)((z-192)*64 + (y-128));
                } else if ((z >= 0x00E0) && (z <= 0x00EF) ) {
                    unsigned int y = (unsigned char) value[++i];
                    unsigned int x = (unsigned char) value[++i];
                    byteArr[count++] = (unsigned char)((z-224)*4096 + (y-128)*64 + (x-128));
                } else if( (z >= 0x00F0) && (z <= 0x00F7) ) {
                    unsigned int y = (unsigned char) value[++i];
                    unsigned int x = (unsigned char) value[++i];
                    unsigned int w = (unsigned char) value[++i];
                    byteArr[count++] = (unsigned char)((z-240)*262144 + (y-128)*4096 + (x-128)*64 + (w-128));
                } else if( (z >= 0x00F8) && (z <= 0x00FB) ) {
                    unsigned int y = (unsigned char) value[++i];
                    unsigned int x = (unsigned char) value[++i];
                    unsigned int w = (unsigned char) value[++i];
                    unsigned int v = (unsigned char) value[++i];
                    byteArr[count++] = (unsigned char)((z-248)*16777216 + (y-128)*262144 + (x-128)*4096 + (w-128)*64 + (v-128));
                } else if( (z >= 0x00FC) && (z <= 0x00FD) ) {
                    unsigned int y = (unsigned char) value[++i];
                    unsigned int x = (unsigned char) value[++i];
                    unsigned int w = (unsigned char) value[++i];
                    unsigned int v = (unsigned char) value[++i];
                    unsigned int u = (unsigned char) value[++i];
                    byteArr[count++] = (unsigned char)((z-252)*1073741824 + (y-128)*16777216 + (x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128));
                }
            }

            // C++ strings need a NULL terminator
            byteArr.push_back( '\0' );

            // Let the Compiler give us a string.
            return reinterpret_cast<const char*>( &byteArr[0] );
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
            unsigned int c, count = 0;

            std::string::const_iterator iter = str->begin();

            for(; iter != str->end(); ++iter ) {
                c = (unsigned char) *iter;
                if( (c >= 0x0001) && (c <= 0x007F) ) {
                    utflen++;
                } else if( c > 0x07FF ) {
                    utflen += 3;
                } else {
                    utflen += 2;
                }
            }

            dataOut.writeUnsignedShort( utflen );
            std::vector<unsigned char> byteArr;
            byteArr.resize( utflen );

            for( iter = str->begin(); iter != str->end(); ++iter ) {

                c = (unsigned char) *iter;
                if( (c >= 0x0001) && (c <= 0x007F) ) {
                    byteArr[count++] = (unsigned char)c;
                } else if( c <= 0x07FF ) {
                    byteArr[count++] = (unsigned char)( 192 + (c / 64));
                    byteArr[count++] = (unsigned char)( 128 + (c % 64));
                } else {
                    byteArr[count++] = (unsigned char)( 224 + (c / 4096));
                    byteArr[count++] = (unsigned char)( 192 + ((c / 64) % 64));
                    byteArr[count++] = (unsigned char)( 128 + (c % 64));
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
