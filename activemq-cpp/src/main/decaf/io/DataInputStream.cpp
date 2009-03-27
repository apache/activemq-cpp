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

using namespace std;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
DataInputStream::DataInputStream( InputStream* inputStream, bool own )
 : FilterInputStream( inputStream, own ) {}

////////////////////////////////////////////////////////////////////////////////
DataInputStream::~DataInputStream() {}

////////////////////////////////////////////////////////////////////////////////
int DataInputStream::read( std::vector<unsigned char>& buffer )
    throw ( io::IOException ) {

    try {
        return this->read( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int DataInputStream::read( unsigned char* buffer,
                           std::size_t offset,
                           std::size_t length )
    throw ( IOException, NullPointerException ) {

    try {

        if( length == 0 ) {
            return 0;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DataInputStream::read - Buffer is null" );
        }

        if( inputStream == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Base input stream is null" );
        }

        return inputStream->read( &buffer[offset], 0, length );
    }
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
bool DataInputStream::readBoolean()
    throw( IOException, EOFException ) {

    try {
        unsigned char value = 0;
        readAllData( &value, sizeof(value) );
        return (bool)( value != 0 );
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
char DataInputStream::readByte()
    throw ( IOException, EOFException ) {

    try {
        unsigned char value = 0;
        readAllData( &value, sizeof(value) );
        return (char)( value );
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DataInputStream::readUnsignedByte()
    throw ( IOException, EOFException ) {

    try {
        unsigned char value = 0;
        readAllData( &value, sizeof(value) );
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
char DataInputStream::readChar() throw ( IOException, EOFException ) {
    try {
        unsigned char value = 0;
        readAllData( &value, sizeof(value) );
        return (char)( value );
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
short DataInputStream::readShort() throw ( io::IOException, io::EOFException ) {
    try {
        short value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        readAllData( buffer, sizeof(value) );
        value |= (buffer[0] << 8 | buffer[1] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
unsigned short DataInputStream::readUnsignedShort()
    throw ( io::IOException, io::EOFException ) {
    try {
        unsigned short value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        readAllData( buffer, sizeof(value) );
        value |= (buffer[0] << 8 | buffer[1] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
int DataInputStream::readInt() throw ( io::IOException, io::EOFException ) {
    try {
        unsigned int value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        readAllData( buffer, sizeof(value) );
        value |= (buffer[0] << 24 | buffer[1] << 16 |
                  buffer[2] << 8 | buffer[3] << 0);
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
double DataInputStream::readDouble() throw ( io::IOException, io::EOFException ) {
    try {
        unsigned long long lvalue = this->readLong();
        double value = 0.0;
        memcpy( &value, &lvalue, sizeof( unsigned long long ) );
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
float DataInputStream::readFloat() throw ( io::IOException, io::EOFException ) {
    try {
        unsigned int lvalue = this->readInt();
        float value = 0.0f;
        memcpy( &value, &lvalue, sizeof( unsigned int ) );
        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
long long DataInputStream::readLong()
    throw ( io::IOException, io::EOFException ) {
    try {
        unsigned long long value = 0;
        unsigned char buffer[sizeof(value)] = {0};
        readAllData( buffer, sizeof(value) );

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

        value = ( byte1 << 56 | byte2 << 48 | byte3 << 40 | byte4 << 32 |
                  byte5 << 24 | byte6 << 16 | byte7 << 8  | byte8 << 0 );

        return value;
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::string DataInputStream::readString()
    throw ( io::IOException, io::EOFException ) {
    try {

        if( inputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Base input stream is null" );
        }

        size_t size = 1024;
        std::vector<char> buffer;
        buffer.resize( size );
        size_t pos = 0;

        while( true ) {

            if( inputStream->read( (unsigned char*)( &buffer[pos] ), 0, 1 ) == -1 ) {
                throw EOFException(
                    __FILE__, __LINE__,
                    "DataInputStream::readString - Reached EOF" );
            }

            // if null is found we are done
            if( buffer[pos] == '\0' ){
                break;
            }

            // Resize to hold more if we exceed current size
            if( ++pos > size ) {
                buffer.resize( (size *= 2) );
            }
        }

        return &buffer[0];
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::string DataInputStream::readUTF()
    throw ( io::IOException, io::EOFException ) {
    try {

        if( inputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Base input stream is null" );
        }

        std::vector<unsigned char> buffer;
        unsigned short length = readUnsignedShort();
        buffer.resize(length + 1);  // Add one for a null charactor.

        std::size_t n = 0;
        while( n < length ) {
            int count = inputStream->read( &buffer[n], 0, (length - n) );
            if( count == -1 ) {
                throw EOFException(
                    __FILE__, __LINE__,
                    "DataInputStream::readUTF - Reached EOF" );
            }
            n += count;
        }

        return (char*)&buffer[0];
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStream::readFully( std::vector< unsigned char >& buffer )
    throw ( io::IOException, io::EOFException ) {
    try {
        if( buffer.empty() ) {
            return;
        }
        this->readFully( &buffer[0], 0, buffer.size() );
    }
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataInputStream::readFully( unsigned char* buffer,
                                 std::size_t offset,
                                 std::size_t length )
    throw ( io::IOException,
            io::EOFException,
            lang::exceptions::NullPointerException )
{
    try {

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Buffer is null" );
        }

        if( inputStream == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Base input stream is null" );
        }

        std::size_t n = 0;
        while( n < length ) {
            int count = inputStream->read( &buffer[offset + n], 0, (length - n) );
            if( count == -1 ) {
                throw EOFException(
                    __FILE__, __LINE__,
                    "DataInputStream::readFully - Reached EOF" );
            }
            n += count;
        }
    }
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( EOFException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
std::size_t DataInputStream::skip( std::size_t num )
    throw( io::IOException, lang::exceptions::UnsupportedOperationException ) {
    try {

        if( inputStream == NULL ) {
            throw IOException(
                __FILE__, __LINE__,
                "DataInputStream::readFully - Base input stream is null" );
        }

        std::size_t total = 0;
        std::size_t cur = 0;

        while( ( total < num ) &&
               ( ( cur = inputStream->skip( num-total ) ) > 0 ) ) {
            total += cur;
        }

        return total;
    }
    DECAF_CATCH_RETHROW( UnsupportedOperationException )
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}
