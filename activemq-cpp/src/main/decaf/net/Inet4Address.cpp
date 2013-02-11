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

#include "Inet4Address.h"

using namespace decaf;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
Inet4Address::Inet4Address() {
}

////////////////////////////////////////////////////////////////////////////////
Inet4Address::Inet4Address(const unsigned char* ipAddress, int numBytes) :
    InetAddress(ipAddress, numBytes) {
}

////////////////////////////////////////////////////////////////////////////////
Inet4Address::Inet4Address(const std::string& hostname, const unsigned char* ipAddress, int numBytes) :
    InetAddress(hostname, ipAddress, numBytes) {
}

////////////////////////////////////////////////////////////////////////////////
Inet4Address::~Inet4Address() {
}

////////////////////////////////////////////////////////////////////////////////
InetAddress* Inet4Address::clone() const {
    return new Inet4Address(*this);
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isAnyLocalAddress() const {

    for (int ix = 0; ix < 4; ix++) {
        if (this->addressBytes[ix] != 0) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isLoopbackAddress() const {
    return this->addressBytes[0] == 127;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMulticastAddress() const {
    return (this->addressBytes[0] & 0xF0) == 0xE0;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isLinkLocalAddress() const {
    return ((this->addressBytes[0] == 169) && (this->addressBytes[1] == 254));
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isSiteLocalAddress() const {
    return (this->addressBytes[0] == 10) || ((this->addressBytes[0] == 172) && ((this->addressBytes[1] > 15) && (this->addressBytes[1] < 32)))
            || ((this->addressBytes[0] == 192) && (this->addressBytes[1] == 168));
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMCGlobal() const {

    // Check if we have a prefix of 1110
    if (!isMulticastAddress()) {
        return false;
    }

    unsigned int address = InetAddress::bytesToInt(addressBytes.get(), 0);

    // Now check the boundaries of the global space if we have an address
    // that is prefixed by something less than 111000000000000000000001
    // it is not multicast. ( < 224.0.1.0)
    if ((address >> 8) < 0xE00001) {
        return false;
    }

    // Now check the high boundary which is prefixed by 11101110 = 0xEE. If
    // the value is higher than this than it is not MCGlobal ( > 238.255.255.255 )
    if ((address >> 24) > 0xEE) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMCNodeLocal() const {
    // Never true for IPV4
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMCLinkLocal() const {
    return (InetAddress::bytesToInt(addressBytes.get(), 0) >> 8) == 0xE00000;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMCSiteLocal() const {
    return (InetAddress::bytesToInt(addressBytes.get(), 0) >> 16) == 0xEFFF;
}

////////////////////////////////////////////////////////////////////////////////
bool Inet4Address::isMCOrgLocal() const {
    unsigned int prefix = InetAddress::bytesToInt(addressBytes.get(), 0) >> 16;
    return prefix >= 0xEFC0 && prefix <= 0xEFC3;
}
