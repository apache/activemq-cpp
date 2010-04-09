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

#include "InetAddress.h"

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const unsigned char InetAddress::loopbackBytes[4] = { 127, 0, 0, 1 };
const unsigned char InetAddress::anyBytes[4] = { 0, 0, 0, 0 };

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress() {
}

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress( unsigned char* ipAddress DECAF_UNUSED, int numBytes DECAF_UNUSED ) {

    if( ipAddress == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "InetAddress constructor called with null address array." );
    }

    if( numBytes < 0 ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__, "Number of bytes value is invalid: %d", numBytes );
    }


}

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress( const std::string& hostname DECAF_UNUSED, unsigned char* ipAddress DECAF_UNUSED, int numBytes DECAF_UNUSED ) {

}

////////////////////////////////////////////////////////////////////////////////
InetAddress::~InetAddress() {
}

////////////////////////////////////////////////////////////////////////////////
ArrayPointer<unsigned char> InetAddress::getAddress() const {

    return ArrayPointer<unsigned char>();
}

////////////////////////////////////////////////////////////////////////////////
std::string InetAddress::getHostAddress() const {

    return "";
}
