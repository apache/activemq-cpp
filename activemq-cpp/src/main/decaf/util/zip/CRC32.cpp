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

#include "CRC32.h"

#include <decaf/internal/util/zip/zlib.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::zip;

////////////////////////////////////////////////////////////////////////////////
CRC32::CRC32() : Checksum(), value(0) {
    this->reset();
}

////////////////////////////////////////////////////////////////////////////////
CRC32::~CRC32() {
}

////////////////////////////////////////////////////////////////////////////////
void CRC32::reset() {
    this->value = crc32( (uLong)this->value, NULL, 0 );
}

////////////////////////////////////////////////////////////////////////////////
long long CRC32::getValue() const {
    return this->value;
}

////////////////////////////////////////////////////////////////////////////////
void CRC32::update( const std::vector<unsigned char>& buffer ) {
    this->update( &buffer[0], (int)buffer.size(), 0, (int)buffer.size() );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32::update( const std::vector<unsigned char>& buffer, int offset, int length ) {

    if( offset + length > (int)buffer.size() ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__, "Given offset + length exceeds the length of the buffer." );
    }

    this->update( &buffer[0], (int)buffer.size(), offset, length );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32::update( int byte ) {
    this->value = crc32( (uLong)this->value, (const Bytef*)&byte, 1 );
}

////////////////////////////////////////////////////////////////////////////////
void CRC32::update( const unsigned char* buffer, int size, int offset, int length ) {

    if( offset + length > size ) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__, "Given offset + length exceeds the length of the buffer." );
    }

    this->value = crc32( (uLong)this->value, (const Bytef*)( buffer + offset ), (uInt)length );
}



