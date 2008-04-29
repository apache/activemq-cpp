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

#include "UUID.h"
#include <apr_strings.h>
#include <apr_md5.h>
#include <decaf/lang/exceptions/RuntimeException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
UUID::UUID( long long mostSigBits, long long leastSigBits ) {

    memcpy( &apr_uuid.data[0], &mostSigBits, sizeof( long long ) );
    memcpy( &apr_uuid.data[sizeof(long long)], &leastSigBits, sizeof(long long ) );

    this->mostSigBits = mostSigBits;
    this->leastSigBits = leastSigBits;

    // Version indicator, set when a UUID is generated
    this->uuidVersion = (int)( mostSigBits & 0x000000000000F000LL ) >> 12;
}

////////////////////////////////////////////////////////////////////////////////
UUID::~UUID() {
}

////////////////////////////////////////////////////////////////////////////////
int UUID::compareTo( const UUID& value ) const {
    return apr_strnatcmp( this->toString().c_str(), value.toString().c_str() );
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::equals( const UUID& value ) const {
    return this->getMostSignificantBits() == value.getMostSignificantBits() &&
           this->getLeastSignificantBits() == value.getLeastSignificantBits();
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::operator==( const UUID& value ) const {
    return this->equals( value );
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::operator<( const UUID& value ) const {
    return this->compareTo( value ) == -1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
std::string UUID::toString() const {
    char buffer[APR_UUID_FORMATTED_LENGTH+1] = {0};
    apr_uuid_format( &buffer[0], &apr_uuid );
    return &buffer[0];
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::getLeastSignificantBits() const {
    return this->leastSigBits;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::getMostSignificantBits() const {
    long long result = 0;
    memcpy( &result, &this->apr_uuid.data[sizeof(long long)], sizeof(long long) );
    return this->mostSigBits;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::node() throw ( lang::exceptions::UnsupportedOperationException ) {

    if( this->version() != 1 ) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__,
            "UUID::node - Only a Version 1 UUID supports this operation." );
    }

    return ( this->leastSigBits & 0x0000FFFFFFFFFFFFULL );
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::timestamp() throw ( lang::exceptions::UnsupportedOperationException ) {

    if( this->version() != 1 ) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__,
            "UUID::node - Only a Version 1 UUID supports this operation." );
    }

    // Mask out the version and shift values around to make time.
    long long timeLow  = ( mostSigBits & 0xFFFFFFFF00000000ULL) >> 32;
    long long timeMid  = ( mostSigBits & 0x00000000FFFF0000ULL) << 16;
    long long timeHigh = ( mostSigBits & 0x0000000000000FFFULL) << 48;

    return timeLow | timeMid | timeHigh;
}

////////////////////////////////////////////////////////////////////////////////
int UUID::clockSequence() throw ( lang::exceptions::UnsupportedOperationException ) {

    if( this->version() != 1 ) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__,
            "UUID::node - Only a Version 1 UUID supports this operation." );
    }

    return (int)( ( this->leastSigBits & 0x3FFF000000000000ULL ) >> 48 );
}

////////////////////////////////////////////////////////////////////////////////
int UUID::variant() throw ( lang::exceptions::UnsupportedOperationException ) {

    // determine variant field
    if( ( this->leastSigBits & 0x8000000000000000ULL ) == 0 ) {
        // MSB0 not set, NCS backwards compatibility variant
        return 0;
    } else if( ( this->leastSigBits & 0x4000000000000000ULL ) != 0 ) {
        // MSB1 set, either MS reserved or future reserved
        return (int)( ( this->leastSigBits & 0xE000000000000000ULL ) >> 61 );
    }

    // MSB1 not set, RFC 4122 variant
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
int UUID::version() throw ( lang::exceptions::UnsupportedOperationException ) {
    return this->uuidVersion;
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::randomUUID() {

    apr_uuid_t temp;
    // Generate some random bytes.
    apr_uuid_get( &temp );

    long long msb = 0;
    long long lsb = 0;

    msb = (temp.data[0] & 0xFFLL) << 56;
    msb |= (temp.data[1] & 0xFFLL) << 48;
    msb |= (temp.data[2] & 0xFFLL) << 40;
    msb |= (temp.data[3] & 0xFFLL) << 32;
    msb |= (temp.data[4] & 0xFFLL) << 24;
    msb |= (temp.data[5] & 0xFFLL) << 16;
    msb |= (temp.data[6] & 0x0FLL) << 8;
    msb |= (0x4LL << 12); // set the version to 4
    msb |= (temp.data[7] & 0xFFLL);

    lsb = (temp.data[8] & 0x3FLL) << 56;
    lsb |= (0x2LL << 62); // set the variant to bits 01
    lsb |= (temp.data[9] & 0xFFLL) << 48;
    lsb |= (temp.data[10] & 0xFFLL) << 40;
    lsb |= (temp.data[11] & 0xFFLL) << 32;
    lsb |= (temp.data[12] & 0xFFLL) << 24;
    lsb |= (temp.data[13] & 0xFFLL) << 16;
    lsb |= (temp.data[14] & 0xFFLL) << 8;
    lsb |= (temp.data[15] & 0xFFLL);

    return UUID( msb, lsb );
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::nameUUIDFromBytes( const std::vector<char>& name ) {
    return UUID::nameUUIDFromBytes( &name[0], name.size() );
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::nameUUIDFromBytes( const char* name, std::size_t size ) {

    apr_uuid_t temp;

    if( apr_md5( &temp.data[0], name, size ) != APR_SUCCESS ) {
        throw exceptions::RuntimeException(
            __FILE__, __LINE__,
            "UUID::nameUUIDFromBytes - Failed to run MD5 encoder." );
    }

    long long msb = 0;
    long long lsb = 0;

    msb = (temp.data[0] & 0xFFLL) << 56;
    msb |= (temp.data[1] & 0xFFLL) << 48;
    msb |= (temp.data[2] & 0xFFLL) << 40;
    msb |= (temp.data[3] & 0xFFLL) << 32;
    msb |= (temp.data[4] & 0xFFLL) << 24;
    msb |= (temp.data[5] & 0xFFLL) << 16;
    msb |= (temp.data[6] & 0x0FLL) << 8;
    msb |= (0x3LL << 12); // set the version to 3
    msb |= (temp.data[7] & 0xFFLL);

    lsb = (temp.data[8] & 0x3FLL) << 56;
    lsb |= (0x2LL << 62); // set the variant to bits 01
    lsb |= (temp.data[9] & 0xFFLL) << 48;
    lsb |= (temp.data[10] & 0xFFLL) << 40;
    lsb |= (temp.data[11] & 0xFFLL) << 32;
    lsb |= (temp.data[12] & 0xFFLL) << 24;
    lsb |= (temp.data[13] & 0xFFLL) << 16;
    lsb |= (temp.data[14] & 0xFFLL) << 8;
    lsb |= (temp.data[15] & 0xFFLL);

    return UUID( msb, lsb );
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::fromString( const std::string& name )
    throw ( lang::exceptions::IllegalArgumentException ){

    apr_uuid_t temp;

    if( apr_uuid_parse( &temp, name.c_str() ) != APR_SUCCESS ) {
        throw lang::exceptions::IllegalArgumentException(
            __FILE__, __LINE__,
            "UUID::fromString - Invalid UUID String: ",
            name.c_str() );
    }

    long long mostSigBits = 0;
    long long leastSigBits = 0;

    // Extract to data from the uuid data
    memcpy( &mostSigBits, &temp.data[0], sizeof(long long) );
    memcpy( &leastSigBits, &temp.data[sizeof(long long)], sizeof(long long) );

    return UUID( mostSigBits, leastSigBits );
}
