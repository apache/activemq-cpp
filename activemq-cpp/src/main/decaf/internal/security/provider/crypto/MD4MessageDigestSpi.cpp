/*
 * This is work is derived from material Copyright RSA Data Security, Inc.
 *
 * The RSA copyright statement and Licence for that original material is
 * included below. This is followed by the Apache copyright statement and
 * licence for the modifications made to that material.
 *
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD4 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 *
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD4 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 *
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */

/* Licensed to the Apache Software Foundation (ASF) under one or more
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

#include "MD4MessageDigestSpi.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <decaf/lang/System.h>
#include <decaf/security/DigestException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;
using namespace decaf::internal::security::provider;
using namespace decaf::internal::security::provider::crypto;

////////////////////////////////////////////////////////////////////////////////
namespace {

#define DECAF_MD4_DIGESTSIZE 16

#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

static const unsigned char PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// F, G and I are basic MD4 functions.
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

// ROTATE_LEFT rotates x left n bits.
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// FF, GG and HH are transformations for rounds 1, 2 and 3 */
// Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s) { \
    (a) += F ((b), (c), (d)) + (x); \
    (a) = ROTATE_LEFT ((a), (s)); \
}
#define GG(a, b, c, d, x, s) { \
    (a) += G ((b), (c), (d)) + (x) + (apr_uint32_t)0x5a827999; \
    (a) = ROTATE_LEFT ((a), (s)); \
}
#define HH(a, b, c, d, x, s) { \
    (a) += H ((b), (c), (d)) + (x) + (apr_uint32_t)0x6ed9eba1; \
    (a) = ROTATE_LEFT ((a), (s)); \
}

}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace security {
namespace provider {
namespace crypto {

    class MD4MessageDigestSpiImpl {
    public:

        // state (ABCD)
        apr_uint32_t state[4];
        // number of bits, modulo 2^64 (lsb first)
        unsigned int count[2];
        // Digest Input buffer
        unsigned char buffer[64];

    public:

        MD4MessageDigestSpiImpl() : state(), count(), buffer() {
            reset();
        }

        void reset() {
            count[0] = count[1] = 0;

            // Load magic initialization constants.
            state[0] = 0x67452301;
            state[1] = 0xefcdab89;
            state[2] = 0x98badcfe;
            state[3] = 0x10325476;

            memset(buffer, 0, 64);
        }

        // MD4 block update operation. Continues an MD4 message-digest
        // operation, processing another message block, and updating the
        // context.
        void update(const unsigned char* vinput, int inputLen) {
            const unsigned char *input = vinput;
            int i;
            int idx;
            int partLen;

            // Compute number of bytes mod 64
            idx = (unsigned int)((count[0] >> 3) & 0x3F);

            // Update number of bits
            if ((count[0] += ((unsigned int)inputLen << 3)) < ((unsigned int)inputLen << 3)) {
                count[1]++;
            }

            count[1] += (unsigned int)inputLen >> 29;

            partLen = 64 - idx;

            // Transform as many times as possible.
            if (inputLen >= partLen) {
                memcpy(&buffer[idx], input, partLen);
                MD4Transform(state, buffer);

                for (i = partLen; i + 63 < inputLen; i += 64) {
                    MD4Transform(state, &input[i]);
                }

                idx = 0;
            } else {
                i = 0;
            }

            // Buffer remaining input
            memcpy(&buffer[idx], &input[i], inputLen - i);
        }

        // MD4 finalization. Ends an MD4 message-digest operation, writing the
        // the message digest and then reset.
        void finalize(unsigned char digest[DECAF_MD4_DIGESTSIZE]) {
            unsigned char bits[8];
            unsigned int idx;
            unsigned int padLen;

            // Save number of bits
            encode(bits, count, 8);

            // Pad out to 56 mod 64.
            idx = (unsigned int)((count[0] >> 3) & 0x3f);
            padLen = (idx < 56) ? (56 - idx) : (120 - idx);
            update(PADDING, padLen);

            // Append length (before padding)
            update(bits, 8);

            // Store state in digest
            encode(digest, state, DECAF_MD4_DIGESTSIZE);

            reset();
        }

    private:

        // MD4 basic transformation. Transforms state based on block.
        static void MD4Transform(unsigned int state[4], const unsigned char block[64]) {
            unsigned int a = state[0];
            unsigned int b = state[1];
            unsigned int c = state[2];
            unsigned int d = state[3];
            unsigned int x[DECAF_MD4_DIGESTSIZE];

            decode(x, block, 64);

            /* Round 1 */
            FF (a, b, c, d, x[ 0], S11); /* 1 */
            FF (d, a, b, c, x[ 1], S12); /* 2 */
            FF (c, d, a, b, x[ 2], S13); /* 3 */
            FF (b, c, d, a, x[ 3], S14); /* 4 */
            FF (a, b, c, d, x[ 4], S11); /* 5 */
            FF (d, a, b, c, x[ 5], S12); /* 6 */
            FF (c, d, a, b, x[ 6], S13); /* 7 */
            FF (b, c, d, a, x[ 7], S14); /* 8 */
            FF (a, b, c, d, x[ 8], S11); /* 9 */
            FF (d, a, b, c, x[ 9], S12); /* 10 */
            FF (c, d, a, b, x[10], S13); /* 11 */
            FF (b, c, d, a, x[11], S14); /* 12 */
            FF (a, b, c, d, x[12], S11); /* 13 */
            FF (d, a, b, c, x[13], S12); /* 14 */
            FF (c, d, a, b, x[14], S13); /* 15 */
            FF (b, c, d, a, x[15], S14); /* 16 */

            /* Round 2 */
            GG (a, b, c, d, x[ 0], S21); /* 17 */
            GG (d, a, b, c, x[ 4], S22); /* 18 */
            GG (c, d, a, b, x[ 8], S23); /* 19 */
            GG (b, c, d, a, x[12], S24); /* 20 */
            GG (a, b, c, d, x[ 1], S21); /* 21 */
            GG (d, a, b, c, x[ 5], S22); /* 22 */
            GG (c, d, a, b, x[ 9], S23); /* 23 */
            GG (b, c, d, a, x[13], S24); /* 24 */
            GG (a, b, c, d, x[ 2], S21); /* 25 */
            GG (d, a, b, c, x[ 6], S22); /* 26 */
            GG (c, d, a, b, x[10], S23); /* 27 */
            GG (b, c, d, a, x[14], S24); /* 28 */
            GG (a, b, c, d, x[ 3], S21); /* 29 */
            GG (d, a, b, c, x[ 7], S22); /* 30 */
            GG (c, d, a, b, x[11], S23); /* 31 */
            GG (b, c, d, a, x[15], S24); /* 32 */

            /* Round 3 */
            HH (a, b, c, d, x[ 0], S31); /* 33 */
            HH (d, a, b, c, x[ 8], S32); /* 34 */
            HH (c, d, a, b, x[ 4], S33); /* 35 */
            HH (b, c, d, a, x[12], S34); /* 36 */
            HH (a, b, c, d, x[ 2], S31); /* 37 */
            HH (d, a, b, c, x[10], S32); /* 38 */
            HH (c, d, a, b, x[ 6], S33); /* 39 */
            HH (b, c, d, a, x[14], S34); /* 40 */
            HH (a, b, c, d, x[ 1], S31); /* 41 */
            HH (d, a, b, c, x[ 9], S32); /* 42 */
            HH (c, d, a, b, x[ 5], S33); /* 43 */
            HH (b, c, d, a, x[13], S34); /* 44 */
            HH (a, b, c, d, x[ 3], S31); /* 45 */
            HH (d, a, b, c, x[11], S32); /* 46 */
            HH (c, d, a, b, x[ 7], S33); /* 47 */
            HH (b, c, d, a, x[15], S34); /* 48 */

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;

            // clear any sensitive information.
            memset(x, 0, sizeof(x));
        }

        static void decode(unsigned int* output, const unsigned char* input, unsigned int length) {
            unsigned int i, j;

            for (i = 0, j = 0; j < length; i++, j += 4)
                output[i] = ((unsigned int)input[j])             |
                            (((unsigned int)input[j + 1]) << 8)  |
                            (((unsigned int)input[j + 2]) << 16) |
                            (((unsigned int)input[j + 3]) << 24);
        }

        static void encode(unsigned char* output, const unsigned int* input, unsigned int length) {
            unsigned int i, j;
            unsigned int k;

            for (i = 0, j = 0; j < length; i++, j += 4) {
                k = input[i];
                output[j]     = (unsigned char)(k & 0xff);
                output[j + 1] = (unsigned char)((k >> 8) & 0xff);
                output[j + 2] = (unsigned char)((k >> 16) & 0xff);
                output[j + 3] = (unsigned char)((k >> 24) & 0xff);
            }
        }
    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
MD4MessageDigestSpi::MD4MessageDigestSpi() : MessageDigestSpi(), impl(new MD4MessageDigestSpiImpl) {
}

////////////////////////////////////////////////////////////////////////////////
MD4MessageDigestSpi::~MD4MessageDigestSpi() {
    delete this->impl;
}

////////////////////////////////////////////////////////////////////////////////
MessageDigestSpi* MD4MessageDigestSpi::clone() {
    MD4MessageDigestSpi* clone = new MD4MessageDigestSpi;

    System::arraycopy(clone->impl->state, 0, this->impl->state, 0, 4);
    System::arraycopy(clone->impl->count, 0, this->impl->count, 0, 2);
    System::arraycopy(clone->impl->buffer, 0, this->impl->buffer, 0, 64);

    return clone;
}

////////////////////////////////////////////////////////////////////////////////
int MD4MessageDigestSpi::engineGetDigestLength() {
    return DECAF_MD4_DIGESTSIZE;
}

////////////////////////////////////////////////////////////////////////////////
void MD4MessageDigestSpi::engineUpdate(unsigned char input) {
    this->impl->update(&input, 1);
}

////////////////////////////////////////////////////////////////////////////////
void MD4MessageDigestSpi::engineUpdate(const unsigned char* input, int size, int offset, int length) {

    if (input == NULL && size > 0) {
        throw DigestException(__FILE__, __LINE__, "Null buffer parameter.");
    }

    if (size <= 0) {
        return;
    }

    if (offset < 0 || length < 0) {
        engineReset();
        throw DigestException(__FILE__, __LINE__, "Incorrect offset or length value.");
    }

    if (offset + length > size) {
        engineReset();
        throw DigestException(__FILE__, __LINE__, "Incorrect offset or length value.");
    }

    this->impl->update(&input[offset], length);
}

////////////////////////////////////////////////////////////////////////////////
void MD4MessageDigestSpi::engineUpdate(const std::vector<unsigned char>& input) {

    if (input.empty()) {
        return;
    }

    this->impl->update(&input[0], (int)input.size());
}

////////////////////////////////////////////////////////////////////////////////
void MD4MessageDigestSpi::engineUpdate(decaf::nio::ByteBuffer& input) {

    if (!input.hasRemaining()) {
        return;
    }
    unsigned char* temp;
    if (input.hasArray()) {
        temp = input.array();
        int offset = input.arrayOffset();
        int position = input.position();
        int limit = input.limit();
        engineUpdate(temp, limit - position, offset + position, limit - position);
        input.position(limit);
    } else {
        int length = input.limit() - input.position();
        temp = new unsigned char[length];
        input.get(temp, length, 0, length);
        engineUpdate(temp, length, 0, length);
        delete temp;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MD4MessageDigestSpi::engineReset() {
    this->impl->reset();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> MD4MessageDigestSpi::engineDigest() {

    std::vector<unsigned char> buffer;
    buffer.resize(DECAF_MD4_DIGESTSIZE);
    this->impl->finalize(&buffer[0]);

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
int MD4MessageDigestSpi::engineDigest(unsigned char* buffer, int size, int offset, int length) {

    if (buffer == NULL) {
        engineReset();
        throw DigestException(__FILE__, __LINE__, "Null buffer parameter.");
    }

    if (size < engineGetDigestLength()) {
        engineReset();
        throw DigestException(__FILE__, __LINE__,
            "The value of size parameter is less than the actual digest length.");
    }

    if (length < engineGetDigestLength()) {
        engineReset();
        throw DigestException(__FILE__, __LINE__,
            "The value of length parameter is less than the actual digest length.");
    }

    if (offset < 0) {
        engineReset();
        throw DigestException(__FILE__, __LINE__, "Invalid negative offset.");
    }

    if (offset + length > size) {
        engineReset();
        throw DigestException(__FILE__, __LINE__, "Incorrect offset or length value.");
    }

    std::vector<unsigned char> temp = engineDigest();
    if (length < (int)temp.size()) {
        throw DigestException(__FILE__, __LINE__,
            "The value of length parameter is less than the actual digest length..");
    }

    System::arraycopy(&temp[0], 0, buffer, offset, temp.size());
    return (int)temp.size();
}
