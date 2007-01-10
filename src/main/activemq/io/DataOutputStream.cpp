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
 
#include "DataOutputStream.h"
#include <activemq/util/Endian.h>

using namespace activemq;
using namespace activemq::io;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
DataOutputStream::DataOutputStream( OutputStream* outputStream, bool own )
 : FilterOutputStream( outputStream, own )
{
    // Init the written count
    written = 0;
}

////////////////////////////////////////////////////////////////////////////////
DataOutputStream::~DataOutputStream()
{
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const unsigned char c ) throw ( IOException ) {
    try {
        outputStream->write( c );
        written++;
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const std::vector<unsigned char>& buffer )
    throw ( IOException ) {

    try {
        
        if( buffer.size() == 0 ){
            // nothing to write.
            return;
        }
        
        outputStream->write( &buffer[0], buffer.size() );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const unsigned char* buffer, int len )
    throw ( IOException ) {

    try {
        outputStream->write( buffer, len );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::write( const unsigned char* buffer, int offset, int len )
    throw ( IOException ) {

    try {
        outputStream->write( buffer+offset, len );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeBoolean( bool value ) throw ( IOException ) {
    try {
        unsigned char ivalue = 0;
        value == true ? ivalue = 1 : ivalue = 0;

        write( ivalue );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeByte( unsigned char value ) throw ( IOException ) {
    try {
        write( value );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeShort( short value ) throw ( IOException ) {
    try {
        writeUnsignedShort( (unsigned short)value );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeUnsignedShort( unsigned short value ) throw ( IOException ) {
    try {
        write( (unsigned char)( (value & 0xFF00) >> 8 ) );
        write( (unsigned char)( (value & 0x00FF) >> 0 ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeChar( char value ) throw ( IOException ) {
    try {
        write( value );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeInt( int value ) throw ( IOException ) {
    try {
        write( (unsigned char)( (value & 0xFF000000) >> 24 ) );
        write( (unsigned char)( (value & 0x00FF0000) >> 16 ) );
        write( (unsigned char)( (value & 0x0000FF00) >> 8 ) );
        write( (unsigned char)( (value & 0x000000FF) >> 0 ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeLong( long long value ) throw ( IOException ) {
    try {
        write( (unsigned char)( (value & 0xFF00000000000000ULL) >> 56 ) );
        write( (unsigned char)( (value & 0x00FF000000000000ULL) >> 48 ) );
        write( (unsigned char)( (value & 0x0000FF0000000000ULL) >> 40 ) );
        write( (unsigned char)( (value & 0x000000FF00000000ULL) >> 32 ) );
        write( (unsigned char)( (value & 0x00000000FF000000ULL) >> 24 ) );
        write( (unsigned char)( (value & 0x0000000000FF0000ULL) >> 16 ) );
        write( (unsigned char)( (value & 0x000000000000FF00ULL) >> 8 ) );
        write( (unsigned char)( (value & 0x00000000000000FFULL) >> 0 ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeFloat( float value ) throw ( IOException ) {
    try {
        this->writeInt( *((unsigned int*)&value) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeDouble( double value ) throw ( IOException ) {
    try {
        writeLong( *((unsigned long long*)&value) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeBytes( const std::string& value ) throw ( IOException ) {
    try {
        // add one so that we write the NULL
        this->write( (const unsigned char*)value.c_str(), value.length() + 1 );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeUTF( const std::string& value ) throw ( IOException ) {
    try {
        this->writeUnsignedShort( value.length() );
        this->write( (const unsigned char*)value.c_str(), value.length() );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}
