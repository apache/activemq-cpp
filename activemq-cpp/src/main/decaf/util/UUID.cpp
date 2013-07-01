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
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <decaf/lang/Pointer.h>
#include <decaf/lang/Long.h>
#include <decaf/internal/util/StringUtils.h>
#include <decaf/lang/exceptions/RuntimeException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/security/SecureRandom.h>
#include <decaf/security/MessageDigest.h>
#include <decaf/security/NoSuchAlgorithmException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::internal::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::security;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace util {

    class UUIDImpl {
    private:

        UUIDImpl(const UUIDImpl&);
        UUIDImpl& operator= (const UUIDImpl&);

    public:

        unsigned long long mostSigBits;
        unsigned long long leastSigBits;

        int variant;
        int version;
        long long timestamp;
        int clockSequence;
        long long node;
        int hash;

        UUIDImpl() : mostSigBits(0), leastSigBits(0), variant(0),
                     version(0), timestamp(0), clockSequence(0), node(0), hash(0) {
        }

        UUIDImpl(long long mostSigBits, long long leastSigBits) :
            mostSigBits(mostSigBits), leastSigBits(leastSigBits), variant(0),
            version(0), timestamp(0), clockSequence(0), node(0), hash(0) {

            init();
        }

    private:

        void init() {

            // setup hash field
            int msbHash = (int) (mostSigBits ^ (mostSigBits >> 32));
            int lsbHash = (int) (leastSigBits ^ (leastSigBits >> 32));
            hash = msbHash ^ lsbHash;

            // setup variant field
            if ((leastSigBits & 0x8000000000000000ULL) == 0) {
                // MSB0 not set, NCS backwards compatibility variant
                variant = 0;
            } else if ((leastSigBits & 0x4000000000000000ULL) != 0) {
                // MSB1 set, either MS reserved or future reserved
                variant = (int) ((leastSigBits & 0xE000000000000000ULL) >> 61);
            } else {
                // MSB1 not set, RFC 4122 variant
                variant = 2;
            }

            // setup version field
            version = (int) ((mostSigBits & 0x000000000000F000ULL) >> 12);

            if (variant != 2 && version != 1) {
                return;
            }

            // setup timestamp field
            long long timeLow = (mostSigBits & 0xFFFFFFFF00000000ULL) >> 32;
            long long timeMid = (mostSigBits & 0x00000000FFFF0000ULL) << 16;
            long long timeHigh = (mostSigBits & 0x0000000000000FFFULL) << 48;
            timestamp = timeLow | timeMid | timeHigh;

            // setup clock sequence field
            clockSequence = (int) ((leastSigBits & 0x3FFF000000000000ULL) >> 48);

            // setup node field
            node = (leastSigBits & 0x0000FFFFFFFFFFFFULL);
        }

    public:

        std::string toString() const {
            std::string builder;
            std::string msbStr = Long::toHexString(mostSigBits);
            if (msbStr.length() < 16) {
                int diff = 16 - (int)msbStr.length();
                for (int i = 0; i < diff; i++) {
                    builder.append("0");
                }
            }
            builder.append(msbStr);
            builder.insert(8, "-");
            builder.insert(13, "-");
            builder.append("-");
            std::string lsbStr = Long::toHexString(leastSigBits);
            if (lsbStr.length() < 16) {
                int diff = 16 - (int)lsbStr.length();
                for (int i = 0; i < diff; i++) {
                    builder.append("0");
                }
            }
            builder.append(lsbStr);
            builder.insert(23, "-");
            return builder;
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
UUID::UUID(long long mostSigBits, long long leastSigBits) : uuid(new UUIDImpl(mostSigBits, leastSigBits)) {

}

////////////////////////////////////////////////////////////////////////////////
UUID::UUID(const UUID& source) :
    uuid(new UUIDImpl(source.getMostSignificantBits(), source.getLeastSignificantBits())) {
}

////////////////////////////////////////////////////////////////////////////////
UUID& UUID::operator= (const UUID& source) {
    delete this->uuid;
    this->uuid = new UUIDImpl(source.getMostSignificantBits(), source.getLeastSignificantBits());
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
UUID::~UUID() {
    delete this->uuid;
}

////////////////////////////////////////////////////////////////////////////////
int UUID::compareTo(const UUID& value) const {
    return StringUtils::compare(this->toString().c_str(), value.toString().c_str());
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::equals(const UUID& value) const {
    return this->getMostSignificantBits() == value.getMostSignificantBits() &&
           this->getLeastSignificantBits() == value.getLeastSignificantBits();
}

////////////////////////////////////////////////////////////////////////////////
int UUID::hashCode() const {
    return this->uuid->hash;
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::operator==(const UUID& value) const {
    return this->equals(value);
}

////////////////////////////////////////////////////////////////////////////////
bool UUID::operator<(const UUID& value) const {
    return this->compareTo(value) == -1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
std::string UUID::toString() const {
    return this->uuid->toString();
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::getLeastSignificantBits() const {
    return this->uuid->leastSigBits;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::getMostSignificantBits() const {
    return this->uuid->mostSigBits;
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::node() {

    if (this->version() != 1) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__, "UUID::node - Only a Version 1 UUID supports this operation.");
    }

    return (this->uuid->leastSigBits & 0x0000FFFFFFFFFFFFULL);
}

////////////////////////////////////////////////////////////////////////////////
long long UUID::timestamp() {

    if (this->version() != 1) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__, "UUID::node - Only a Version 1 UUID supports this operation.");
    }

    // Mask out the version and shift values around to make time.
    long long timeLow = (uuid->mostSigBits & 0xFFFFFFFF00000000ULL) >> 32;
    long long timeMid = (uuid->mostSigBits & 0x00000000FFFF0000ULL) << 16;
    long long timeHigh = (uuid->mostSigBits & 0x0000000000000FFFULL) << 48;

    return timeLow | timeMid | timeHigh;
}

////////////////////////////////////////////////////////////////////////////////
int UUID::clockSequence() {

    if (this->version() != 1) {
        throw exceptions::UnsupportedOperationException(
            __FILE__, __LINE__, "UUID::node - Only a Version 1 UUID supports this operation.");
    }

    return (int) ((this->uuid->leastSigBits & 0x3FFF000000000000ULL) >> 48);
}

////////////////////////////////////////////////////////////////////////////////
int UUID::variant() {

    // determine variant field
    if ((this->uuid->leastSigBits & 0x8000000000000000ULL) == 0) {
        // MSB0 not set, NCS backwards compatibility variant
        return 0;
    } else if ((this->uuid->leastSigBits & 0x4000000000000000ULL) != 0) {
        // MSB1 set, either MS reserved or future reserved
        return (int) ((this->uuid->leastSigBits & 0xE000000000000000ULL) >> 61);
    }

    // MSB1 not set, RFC 4122 variant
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
int UUID::version() {
    return this->uuid->version;
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::randomUUID() {

    unsigned char data[16];
    SecureRandom secRand;
    secRand.nextBytes(data, 16);

    long long msb = 0;
    long long lsb = 0;

    msb = (data[0] & 0xFFLL) << 56;
    msb |= (data[1] & 0xFFLL) << 48;
    msb |= (data[2] & 0xFFLL) << 40;
    msb |= (data[3] & 0xFFLL) << 32;
    msb |= (data[4] & 0xFFLL) << 24;
    msb |= (data[5] & 0xFFLL) << 16;
    msb |= (data[6] & 0x0FLL) << 8;
    msb |= (0x4LL << 12); // set the version to 4
    msb |= (data[7] & 0xFFLL);

    lsb = (data[8] & 0x3FLL) << 56;
    lsb |= (0x2LL << 62); // set the variant to bits 01
    lsb |= (data[9] & 0xFFLL) << 48;
    lsb |= (data[10] & 0xFFLL) << 40;
    lsb |= (data[11] & 0xFFLL) << 32;
    lsb |= (data[12] & 0xFFLL) << 24;
    lsb |= (data[13] & 0xFFLL) << 16;
    lsb |= (data[14] & 0xFFLL) << 8;
    lsb |= (data[15] & 0xFFLL);

    return UUID(msb, lsb);
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::nameUUIDFromBytes(const std::vector<char>& name) {
    if (!name.empty()) {
        return UUID::nameUUIDFromBytes(&name[0], (int)name.size());
    } else {
        return UUID::nameUUIDFromBytes(NULL, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::nameUUIDFromBytes(const char* name, int size) {

    if (name == NULL && size > 0) {
        throw NullPointerException(__FILE__, __LINE__, "UUID name buffer was NULL");
    }

    std::vector<unsigned char> hash;

    try {
        Pointer<MessageDigest> digest(MessageDigest::getInstance("MD5"));
        hash = digest->digest((unsigned char*)name, size);
    } catch (NoSuchAlgorithmException& ex) {
        throw exceptions::RuntimeException(
            __FILE__, __LINE__, "UUID::nameUUIDFromBytes - Failed to run MD5 encoder.");
    }

    long long msb = 0;
    long long lsb = 0;

    msb = (hash[0] & 0xFFLL) << 56;
    msb |= (hash[1] & 0xFFLL) << 48;
    msb |= (hash[2] & 0xFFLL) << 40;
    msb |= (hash[3] & 0xFFLL) << 32;
    msb |= (hash[4] & 0xFFLL) << 24;
    msb |= (hash[5] & 0xFFLL) << 16;
    msb |= (hash[6] & 0x0FLL) << 8;
    msb |= (0x3LL << 12); // set the version to 3
    msb |= (hash[7] & 0xFFLL);

    lsb = (hash[8] & 0x3FLL) << 56;
    lsb |= (0x2LL << 62); // set the variant to bits 01
    lsb |= (hash[9] & 0xFFLL) << 48;
    lsb |= (hash[10] & 0xFFLL) << 40;
    lsb |= (hash[11] & 0xFFLL) << 32;
    lsb |= (hash[12] & 0xFFLL) << 24;
    lsb |= (hash[13] & 0xFFLL) << 16;
    lsb |= (hash[14] & 0xFFLL) << 8;
    lsb |= (hash[15] & 0xFFLL);

    return UUID(msb, lsb);
}

////////////////////////////////////////////////////////////////////////////////
UUID UUID::fromString(const std::string& uuid) {

    static const int POS_SIZE = 5;

    int position[5];
    int lastPosition = 1;
    int startPosition = 0;

    int i = 0;
    for (; i < POS_SIZE && lastPosition > 0; i++) {
        position[i] = (int)uuid.find("-", startPosition);
        lastPosition = position[i];
        startPosition = position[i] + 1;
    }

    // should have and only can have four "-" in UUID
    if (i != POS_SIZE || lastPosition != -1) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Invalid UUID string %s", uuid.c_str());
    }

    if (position[0] == 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Invalid UUID string %s", uuid.c_str());
    } else if (((position[1] - position[0]) - 1) <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Invalid UUID string %s", uuid.c_str());
    } else if (((position[2] - position[1]) - 1) <= 0) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Invalid UUID string %s", uuid.c_str());
    } else if ((position[3] + 1) >= (int)uuid.length()) {
        throw IllegalArgumentException(__FILE__, __LINE__,
            "Invalid UUID string %s", uuid.c_str());
    }

    long long m1 = Long::parseLong(uuid.substr(0, position[0]), 16);
    long long m2 = Long::parseLong(uuid.substr(position[0] + 1, (position[1] - position[0]) - 1), 16);
    long long m3 = Long::parseLong(uuid.substr(position[1] + 1, (position[2] - position[1]) - 1), 16);

    long long lsb1 = Long::parseLong(uuid.substr(position[2] + 1, (position[3] - position[2]) - 1), 16);
    long long lsb2 = Long::parseLong(uuid.substr(position[3] + 1), 16);

    long long msb = (m1 << 32) | (m2 << 16) | m3;
    long long lsb = (lsb1 << 48) | lsb2;

    return UUID(msb, lsb);
}
