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

#include "MessageDigest.h"

#include <decaf/security/MessageDigestSpi.h>
#include <decaf/security/Provider.h>
#include <decaf/security/NoSuchAlgorithmException.h>
#include <decaf/lang/exceptions/CloneNotSupportedException.h>
#include <decaf/internal/security/Engine.h>
#include <decaf/lang/Pointer.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::security;
using namespace decaf::internal::security;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace security {

    class MessageDigestImpl {
    public:
    };
}}

////////////////////////////////////////////////////////////////////////////////
MessageDigest::MessageDigest(const std::string& name) : impl(NULL), spi(), provider(), algorithm(name) {
}

////////////////////////////////////////////////////////////////////////////////
MessageDigest::~MessageDigest() {
    try {
        delete this->spi;
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
MessageDigest* MessageDigest::getInstance(const std::string& algorithm) {

    if (algorithm.empty()) {
        throw NoSuchAlgorithmException(__FILE__, __LINE__, "Supplied algorithm string is empty.");
    }

    static Engine engine("MessageDigest");

    SecuritySpi* newSpi = engine.newInstance(algorithm);

    MessageDigestSpi* spi = dynamic_cast<MessageDigestSpi*>(newSpi);
    if (spi == NULL) {
        delete newSpi;
        throw NoSuchAlgorithmException(__FILE__, __LINE__,
            "Supplied algorithm not a MessageDigest algorithm.");
    }

    MessageDigest* digest = new MessageDigest(algorithm);
    digest->provider = engine.getProvider();
    digest->spi = spi;

    return digest;
}

////////////////////////////////////////////////////////////////////////////////
bool MessageDigest::isEqual(const std::vector<unsigned char>& digesta,
                            const std::vector<unsigned char>& digestb) {
    if (digesta.size() != digestb.size()) {
        return false;
    }

    // No early return is allowed to avoid timing attack.
    // We must not return false until all elements are compared
    // to keep the computing time constant
    bool result = true;
    for (int i = 0; i < (int)digesta.size(); i++) {
        if (digesta[i] != digestb[i]) {
            result = false;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> MessageDigest::digest() {
    return this->spi->engineDigest();
}

////////////////////////////////////////////////////////////////////////////////
int MessageDigest::digest(unsigned char* input, int size, int offset, int length) {
    return this->spi->engineDigest(input, size, offset, length);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> MessageDigest::digest(const unsigned char* input, int size) {
    this->spi->engineUpdate(input, size, 0, size);
    return this->spi->engineDigest();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> MessageDigest::digest(const std::vector<unsigned char>& input) {
    this->spi->engineUpdate(input);
    return this->spi->engineDigest();
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigest::update(unsigned char input) {
    this->spi->engineUpdate(input);
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigest::update(unsigned char* input, int size, int offset, int length) {
    this->spi->engineUpdate(input, size, offset, length);
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigest::update(const std::vector<unsigned char>& input) {
    this->spi->engineUpdate(input);
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigest::update(nio::ByteBuffer& input) {
    this->spi->engineUpdate(input);
}

////////////////////////////////////////////////////////////////////////////////
int MessageDigest::getDigestLength() const {

    if (!this->spi->isCloneable()) {
        return 0;
    }

    return this->spi->engineGetDigestLength();
}

////////////////////////////////////////////////////////////////////////////////
MessageDigest* MessageDigest::clone() {

    if (!this->spi->isCloneable()) {
        throw CloneNotSupportedException(__FILE__, __LINE__,
                "MessageDigestSpi in use not cloneable.");
    }

    MessageDigest* clone = new MessageDigest(this->algorithm);
    clone->provider = this->provider;
    clone->spi = this->spi->clone();

    return clone;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigest::reset() {
    this->spi->engineReset();
}

////////////////////////////////////////////////////////////////////////////////
std::string MessageDigest::toString() const {
    return std::string("MESSAGE DIGEST ") + algorithm;
}
