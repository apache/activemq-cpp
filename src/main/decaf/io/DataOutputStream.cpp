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

#include <decaf/io/DataOutputStream.h>
#include <decaf/io/UTFDataFormatException.h>
#include <decaf/util/Config.h>
#include <string.h>
#include <stdio.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DataOutputStream::DataOutputStream( OutputStream* outputStream, bool own )
 : FilterOutputStream( outputStream, own ) {
    // Init the written count
    written = 0;
}

////////////////////////////////////////////////////////////////////////////////
DataOutputStream::~DataOutputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const unsigned char c ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        outputStream->write( c );
        written++;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const std::vector<unsigned char>& buffer )
    throw ( IOException ) {

    try {

        if( buffer.size() == 0 ){
            // nothing to write.
            return;
        }

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        outputStream->write( &buffer[0], 0, buffer.size() );
        written += buffer.size();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const unsigned char* buffer,
                              std::size_t offset,
                              std::size_t len )
    throw ( IOException, lang::exceptions::NullPointerException )
{

    try {

        if( len == 0 ) {
            return;
        }

        if( isClosed() ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DataOutputStream::write - passed buffer is Null" );
        }

        outputStream->write( buffer+offset, 0, len );
        written += len;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeBoolean( bool value ) throw ( IOException ) {
    try {
        unsigned char ivalue = 0;
        value == true ? ivalue = 1 : ivalue = 0;

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        outputStream->write( ivalue );
        written++;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeByte( unsigned char value ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        outputStream->write( value );
        written++;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeChar( char value ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        outputStream->write( value );
        written++;
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeShort( short value ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        unsigned char buffer[sizeof(value)];

        buffer[0] = (value & 0xFF00) >> 8;
        buffer[1] = (value & 0x00FF) >> 0;

        outputStream->write( buffer, 0, sizeof(value) );
        written += sizeof( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeUnsignedShort( unsigned short value )
    throw ( IOException )
{
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        unsigned char buffer[sizeof(value)];

        buffer[0] = (value & 0xFF00) >> 8;
        buffer[1] = (value & 0x00FF) >> 0;

        outputStream->write( buffer, 0, sizeof(value) );
        written += sizeof( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeInt( int value ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        unsigned char buffer[sizeof(value)];

        buffer[0] = (value & 0xFF000000) >> 24;
        buffer[1] = (value & 0x00FF0000) >> 16;
        buffer[2] = (value & 0x0000FF00) >> 8;
        buffer[3] = (value & 0x000000FF) >> 0;

        outputStream->write( buffer, 0, sizeof(value) );
        written += sizeof( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeLong( long long value ) throw ( IOException ) {
    try {

        if( outputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataOutputStream::write - Base stream is Null");
        }

        unsigned char buffer[sizeof(value)];

        buffer[0] = (unsigned char)((value & 0xFF00000000000000ULL) >> 56);
        buffer[1] = (unsigned char)((value & 0x00FF000000000000ULL) >> 48);
        buffer[2] = (unsigned char)((value & 0x0000FF0000000000ULL) >> 40);
        buffer[3] = (unsigned char)((value & 0x000000FF00000000ULL) >> 32);
        buffer[4] = (unsigned char)((value & 0x00000000FF000000ULL) >> 24);
        buffer[5] = (unsigned char)((value & 0x0000000000FF0000ULL) >> 16);
        buffer[6] = (unsigned char)((value & 0x000000000000FF00ULL) >> 8);
        buffer[7] = (unsigned char)((value & 0x00000000000000FFULL) >> 0);

        outputStream->write( buffer, 0, sizeof(value) );
        written += sizeof( value );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeFloat( float value ) throw ( IOException ) {
    try {
        unsigned int lvalue = 0;
        memcpy( &lvalue, &value, sizeof( float ) );
        this->writeInt( lvalue );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeDouble( double value ) throw ( IOException ) {
    try {
        unsigned long long lvalue = 0;
        memcpy( &lvalue, &value, sizeof( double ) );
        this->writeLong( lvalue );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeBytes( const std::string& value ) throw ( IOException ) {
    try {

        if( value.length() == 0 ) {
            return;
        }

        // do not add one so that we don't write the NULL
        this->write( (const unsigned char*)value.c_str(), 0, value.length() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeChars( const std::string& value ) throw ( IOException ) {
    try {

        if( value.length() == 0 ) {
            return;
        }

        // add one so that we write the NULL
        this->write( (const unsigned char*)value.c_str(), 0, value.length() + 1 );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeUTF( const std::string& value )
    throw ( IOException, UTFDataFormatException ) {

    try {

        unsigned int utfLength = this->countUTFLength( value );

        if( utfLength > 65535 ) {
            throw UTFDataFormatException(
                __FILE__, __LINE__,
                "Attempted to write a string as UTF-8 whose length is longer "
                "than the supported 65535 bytes" );
        }

        std::size_t length = value.length();
        std::vector<unsigned char> utfBytes( (std::size_t)utfLength );
        unsigned int utfIndex = 0;

        for( std::size_t i = 0; i < length; i++ ) {

            unsigned int charValue = (unsigned char)value.at( i );

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

        this->writeUnsignedShort( (unsigned short)utfLength );
		if( utfLength > 0 ) {
			this->write( &utfBytes[0], 0, utfLength );
		}
    }
    DECAF_CATCH_RETHROW( UTFDataFormatException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned int DataOutputStream::countUTFLength( const std::string& value ) {

    unsigned int utfCount = 0;
    std::size_t length = value.length();

    for( std::size_t i = 0; i < length; ++i ) {

        unsigned int charValue = (unsigned char)value.at( i );

        // Written to allow for expansion to wide character strings at some
        // point, as it stands now the value can never be > 255 since the
        // string class returns a single byte char.
        if( charValue > 0 && charValue <= 127 ) {
            utfCount++;
        } else if( charValue <= 2047 ) {
            utfCount += 2;
        } else {
            utfCount += 3;
        }
    }

    return utfCount;
}
