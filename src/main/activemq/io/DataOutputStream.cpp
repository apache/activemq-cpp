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
        ivalue = Endian::byteSwap( ivalue );
        write( ( unsigned char* )&ivalue, sizeof( ivalue ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeByte( unsigned char value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeShort( short value ) throw ( IOException ) {
    writeUnsignedShort( (unsigned short)value );
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeUnsignedShort( unsigned short value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeChar( char value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( ( unsigned char )value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeInt( int value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( ( unsigned int )value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeLong( long long value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( ( unsigned long long )value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeFloat( float value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( value );
        write( ( unsigned char* )&value, sizeof( value ) );
    }
    AMQ_CATCH_RETHROW( IOException )
    AMQ_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void DataOutputStream::writeDouble( double value ) throw ( IOException ) {
    try {
        value = Endian::byteSwap( value );
        write( ( unsigned char* )&value, sizeof( value ) );
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
