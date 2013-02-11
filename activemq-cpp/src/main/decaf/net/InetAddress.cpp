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

#include <decaf/lang/System.h>
#include <decaf/lang/Byte.h>
#include <decaf/net/Inet4Address.h>
#include <decaf/net/Inet6Address.h>
#include <decaf/net/UnknownHostException.h>
#include <decaf/lang/exceptions/RuntimeException.h>

#include <apr_network_io.h>
#include <decaf/internal/AprPool.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;

////////////////////////////////////////////////////////////////////////////////
const unsigned char InetAddress::loopbackBytes[4] = { 127, 0, 0, 1 };
const unsigned char InetAddress::anyBytes[4] = { 0, 0, 0, 0 };

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress() : hostname(), reached(false), addressBytes() {
}

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress(const unsigned char* ipAddress, int numBytes) :
        hostname(), reached(false), addressBytes() {

    if (ipAddress == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "InetAddress constructor called with null address array.");
    }

    if (numBytes < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Number of bytes value is invalid: %d", numBytes);
    }

    unsigned char* copy = new unsigned char[numBytes];
    System::arraycopy(ipAddress, 0, copy, 0, numBytes);
    this->addressBytes.reset(copy, numBytes);
}

////////////////////////////////////////////////////////////////////////////////
InetAddress::InetAddress(const std::string& hostname, const unsigned char* ipAddress, int numBytes) :
        hostname(hostname), reached(false), addressBytes() {

    if (ipAddress == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "InetAddress constructor called with null address array.");
    }

    if (numBytes < 0) {
        throw IllegalArgumentException(__FILE__, __LINE__, "Number of bytes value is invalid: %d", numBytes);
    }

    unsigned char* copy = new unsigned char[numBytes];
    System::arraycopy(ipAddress, 0, copy, 0, numBytes);
    this->addressBytes.reset(copy, numBytes);
}

////////////////////////////////////////////////////////////////////////////////
InetAddress::~InetAddress() {
}

////////////////////////////////////////////////////////////////////////////////
InetAddress* InetAddress::clone() const {
    return new InetAddress(*this);
}

////////////////////////////////////////////////////////////////////////////////
ArrayPointer<unsigned char> InetAddress::getAddress() const {
    return this->addressBytes.clone();
}

////////////////////////////////////////////////////////////////////////////////
std::string InetAddress::getHostAddress() const {

    std::string address;

    for (int ix = 0; ix < this->addressBytes.length(); ix++) {
        address.append(Byte::toString(addressBytes[ix]));

        if (ix < this->addressBytes.length() - 1) {
            address.append(".");
        }
    }
    return address;
}

////////////////////////////////////////////////////////////////////////////////
std::string InetAddress::getHostName() const {

    if (!this->hostname.empty()) {
        return this->hostname;
    }

    return this->getHostAddress();
}

////////////////////////////////////////////////////////////////////////////////
std::string InetAddress::toString() const {
    return getHostName() + " / " + getHostAddress();
}

////////////////////////////////////////////////////////////////////////////////
InetAddress InetAddress::getByAddress(const std::string& hostname, const unsigned char* bytes, int numBytes) {

    if (numBytes == 4) {
        return Inet4Address(hostname, bytes, numBytes);
    } else if (numBytes == 16) {
        return Inet6Address(hostname, bytes, numBytes);
    } else {
        throw UnknownHostException(__FILE__, __LINE__, "Number of Bytes passed was invalid: %d", numBytes);
    }
}

////////////////////////////////////////////////////////////////////////////////
InetAddress InetAddress::getByAddress(const unsigned char* bytes, int numBytes) {

    if (numBytes == 4) {
        return Inet4Address(bytes, numBytes);
    } else if (numBytes == 16) {
        return Inet6Address(bytes, numBytes);
    } else {
        throw UnknownHostException(__FILE__, __LINE__, "Number of Bytes passed was invalid: %d", numBytes);
    }
}

////////////////////////////////////////////////////////////////////////////////
InetAddress InetAddress::getLocalHost() {

    char hostname[APRMAXHOSTLEN + 1] = { 0 };

    try {

        AprPool pool;
        apr_status_t result = APR_SUCCESS;

        try {

            result = apr_gethostname(hostname, APRMAXHOSTLEN + 1, pool.getAprPool());

            if (result != APR_SUCCESS) {
                return getLoopbackAddress();
            }

        } catch (...) {
        }

        apr_sockaddr_t* address = NULL;
        result = apr_sockaddr_info_get(&address, hostname, APR_UNSPEC, 0, APR_IPV4_ADDR_OK, pool.getAprPool());

        if (result != APR_SUCCESS) {
            throw UnknownHostException(__FILE__, __LINE__, "Could not resolve the IP Address of this host.");
        }

        if (address->family == APR_INET) {
            return Inet4Address(hostname, (const unsigned char*) address->ipaddr_ptr, address->ipaddr_len);
        } else {
            return Inet6Address(hostname, (const unsigned char*) address->ipaddr_ptr, address->ipaddr_len);
        }
    }
    DECAF_CATCH_RETHROW( UnknownHostException)
    DECAF_CATCH_EXCEPTION_CONVERT( Exception, UnknownHostException)
    DECAF_CATCHALL_THROW( UnknownHostException)
}

////////////////////////////////////////////////////////////////////////////////
unsigned int InetAddress::bytesToInt(const unsigned char* bytes, int start) {

    // First mask the byte with 255, as when a negative
    // signed byte converts to an integer, it has bits
    // on in the first 3 bytes, we are only concerned
    // about the right-most 8 bits.
    // Then shift the rightmost byte to align with its
    // position in the integer.
    int value = ((bytes[start + 3] & 255)) | ((bytes[start + 2] & 255) << 8) | ((bytes[start + 1] & 255) << 16) | ((bytes[start] & 255) << 24);

    return value;
}

////////////////////////////////////////////////////////////////////////////////
InetAddress InetAddress::getAnyAddress() {
    return Inet4Address("localhost", InetAddress::loopbackBytes, 4);
}

////////////////////////////////////////////////////////////////////////////////
InetAddress InetAddress::getLoopbackAddress() {
    return Inet4Address(InetAddress::anyBytes, 4);
}
