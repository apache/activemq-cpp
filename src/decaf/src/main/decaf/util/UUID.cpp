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

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
UUID::UUID( long long mostSigBits, long long leastSigBits ) {

    memcpy( &apr_uuid.data[0], &leastSigBits, sizeof( long long ) );
    memcpy( &apr_uuid.data[sizeof(long long)], &mostSigBits, sizeof(long long ) );
}

////////////////////////////////////////////////////////////////////////////////
UUID::~UUID() {
}

////////////////////////////////////////////////////////////////////////////////
int UUID::compareTo( const UUID& value ) const {
    return apr_strnatcmp( (const char*)this->apr_uuid.data,
                          (const char*)value.apr_uuid.data );
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::equals( const UUID& value ) const {
    return apr_strnatcmp( (const char*)this->apr_uuid.data,
                          (const char*)value.apr_uuid.data ) == 0 ? true : false;
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
    long long result = 0;
    memcpy( &result, &this->apr_uuid.data[0], sizeof(long long) );
    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::getMostSignificantBits() const {
    long long result = 0;
    memcpy( &result, &this->apr_uuid.data[sizeof(long long)], sizeof(long long) );
    return result;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::node() throw ( lang::exceptions::UnsupportedOperationException ) {
    return 0; //TODO
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::timestamp() throw ( lang::exceptions::UnsupportedOperationException ) {
    return 0; //TODO
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::clockSequence() throw ( lang::exceptions::UnsupportedOperationException ) {
    return 0; //TODO
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::variant() throw ( lang::exceptions::UnsupportedOperationException ) {
    return 0; //TODO
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::version() throw ( lang::exceptions::UnsupportedOperationException ) {
    return 0; //TODO
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::randomUUID() {
    return UUID( 0, 0); //TODO
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::nameUUIDFromBytes( const char* name DECAF_UNUSED ) {
    return UUID( 0, 0); //TODO
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::fromString( const std::string& name )
    throw ( lang::exceptions::IllegalArgumentException ){

    UUID result( 0, 0 );

    if( apr_uuid_parse( &(result.apr_uuid), name.c_str() ) != APR_SUCCESS ) {
        throw lang::exceptions::IllegalArgumentException(
            __FILE__, __LINE__,
            "UUID::fromString - Invalid UUID String: ",
            name.c_str() );
    }

    return result;
}
