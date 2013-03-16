/*
 * This is work is derived from material Copyright RSA Data Security, Inc.
 *
 * The RSA copyright statement and Licence for that original material is
 * included below. This is followed by the Apache copyright statement and
 * licence for the modifications made to that material.
 */

/* MD5C.C - RSA Data Security, Inc., MD5 message-digest algorithm
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
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

/*
 * The encode() routine uses much code obtained from the FreeBSD 3.0
 * MD5 crypt() function, which is licenced as follows:
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@login.dknet.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include "MD5MessageDigestSpi.h"

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

#define DECAF_MD5_DIGESTSIZE 16

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static const unsigned char PADDING[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// F, G, H and I are basic MD5 functions.
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// ROTATE_LEFT rotates x left n bits.
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace security {
namespace provider {
namespace crypto {

    class MD5MessageDigestSpiImpl {
    public:

        // state (ABCD)
        apr_uint32_t state[4];
        // number of bits, modulo 2^64 (lsb first)
        unsigned int count[2];
        // Digest Input buffer
        unsigned char buffer[64];

    public:

        MD5MessageDigestSpiImpl() : state(), count(), buffer() {
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

        // MD5 block update operation. Continues an MD5 message-digest
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
                MD5Transform(state, buffer);

                for (i = partLen; i + 63 < inputLen; i += 64) {
                    MD5Transform(state, &input[i]);
                }

                idx = 0;
            } else {
                i = 0;
            }

            // Buffer remaining input
            memcpy(&buffer[idx], &input[i], inputLen - i);
        }

        // MD5 finalization. Ends an MD5 message-digest operation, writing the
        // the message digest and then reset.
        void finalize(unsigned char digest[DECAF_MD5_DIGESTSIZE]) {
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
            encode(digest, state, DECAF_MD5_DIGESTSIZE);

            reset();
        }

    private:

        // MD5 basic transformation. Transforms state based on block.
        static void MD5Transform(unsigned int state[4], const unsigned char block[64]) {
            unsigned int a = state[0];
            unsigned int b = state[1];
            unsigned int c = state[2];
            unsigned int d = state[3];
            unsigned int x[DECAF_MD5_DIGESTSIZE];

            decode(x, block, 64);

            /* Round 1 */
            FF(a, b, c, d, x[0],  S11, 0xd76aa478); /* 1 */
            FF(d, a, b, c, x[1],  S12, 0xe8c7b756); /* 2 */
            FF(c, d, a, b, x[2],  S13, 0x242070db); /* 3 */
            FF(b, c, d, a, x[3],  S14, 0xc1bdceee); /* 4 */
            FF(a, b, c, d, x[4],  S11, 0xf57c0faf); /* 5 */
            FF(d, a, b, c, x[5],  S12, 0x4787c62a); /* 6 */
            FF(c, d, a, b, x[6],  S13, 0xa8304613); /* 7 */
            FF(b, c, d, a, x[7],  S14, 0xfd469501); /* 8 */
            FF(a, b, c, d, x[8],  S11, 0x698098d8); /* 9 */
            FF(d, a, b, c, x[9],  S12, 0x8b44f7af); /* 10 */
            FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
            FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
            FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
            FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
            FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
            FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

            /* Round 2 */
            GG(a, b, c, d, x[1],  S21, 0xf61e2562); /* 17 */
            GG(d, a, b, c, x[6],  S22, 0xc040b340); /* 18 */
            GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
            GG(b, c, d, a, x[0],  S24, 0xe9b6c7aa); /* 20 */
            GG(a, b, c, d, x[5],  S21, 0xd62f105d); /* 21 */
            GG(d, a, b, c, x[10], S22, 0x2441453);  /* 22 */
            GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
            GG(b, c, d, a, x[4],  S24, 0xe7d3fbc8); /* 24 */
            GG(a, b, c, d, x[9],  S21, 0x21e1cde6); /* 25 */
            GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
            GG(c, d, a, b, x[3],  S23, 0xf4d50d87); /* 27 */
            GG(b, c, d, a, x[8],  S24, 0x455a14ed); /* 28 */
            GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
            GG(d, a, b, c, x[2],  S22, 0xfcefa3f8); /* 30 */
            GG(c, d, a, b, x[7],  S23, 0x676f02d9); /* 31 */
            GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

            /* Round 3 */
            HH(a, b, c, d, x[5],  S31, 0xfffa3942); /* 33 */
            HH(d, a, b, c, x[8],  S32, 0x8771f681); /* 34 */
            HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
            HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
            HH(a, b, c, d, x[1],  S31, 0xa4beea44); /* 37 */
            HH(d, a, b, c, x[4],  S32, 0x4bdecfa9); /* 38 */
            HH(c, d, a, b, x[7],  S33, 0xf6bb4b60); /* 39 */
            HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
            HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
            HH(d, a, b, c, x[0],  S32, 0xeaa127fa); /* 42 */
            HH(c, d, a, b, x[3],  S33, 0xd4ef3085); /* 43 */
            HH(b, c, d, a, x[6],  S34, 0x4881d05);  /* 44 */
            HH(a, b, c, d, x[9],  S31, 0xd9d4d039); /* 45 */
            HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
            HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
            HH(b, c, d, a, x[2],  S34, 0xc4ac5665); /* 48 */

            /* Round 4 */
            II(a, b, c, d, x[0],  S41, 0xf4292244); /* 49 */
            II(d, a, b, c, x[7],  S42, 0x432aff97); /* 50 */
            II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
            II(b, c, d, a, x[5],  S44, 0xfc93a039); /* 52 */
            II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
            II(d, a, b, c, x[3],  S42, 0x8f0ccc92); /* 54 */
            II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
            II(b, c, d, a, x[1],  S44, 0x85845dd1); /* 56 */
            II(a, b, c, d, x[8],  S41, 0x6fa87e4f); /* 57 */
            II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
            II(c, d, a, b, x[6],  S43, 0xa3014314); /* 59 */
            II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
            II(a, b, c, d, x[4],  S41, 0xf7537e82); /* 61 */
            II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
            II(c, d, a, b, x[2],  S43, 0x2ad7d2bb); /* 63 */
            II(b, c, d, a, x[9],  S44, 0xeb86d391); /* 64 */

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
MD5MessageDigestSpi::MD5MessageDigestSpi() : MessageDigestSpi(), impl(new MD5MessageDigestSpiImpl) {
}

////////////////////////////////////////////////////////////////////////////////
MD5MessageDigestSpi::~MD5MessageDigestSpi() {
    delete this->impl;
}

////////////////////////////////////////////////////////////////////////////////
MessageDigestSpi* MD5MessageDigestSpi::clone() {
    MD5MessageDigestSpi* clone = new MD5MessageDigestSpi;

    System::arraycopy(clone->impl->state, 0, this->impl->state, 0, 4);
    System::arraycopy(clone->impl->count, 0, this->impl->count, 0, 2);
    System::arraycopy(clone->impl->buffer, 0, this->impl->buffer, 0, 64);

    return clone;
}

////////////////////////////////////////////////////////////////////////////////
int MD5MessageDigestSpi::engineGetDigestLength() {
    return DECAF_MD5_DIGESTSIZE;
}

////////////////////////////////////////////////////////////////////////////////
void MD5MessageDigestSpi::engineUpdate(unsigned char input) {
    this->impl->update(&input, 1);
}

////////////////////////////////////////////////////////////////////////////////
void MD5MessageDigestSpi::engineUpdate(const unsigned char* input, int size, int offset, int length) {

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
void MD5MessageDigestSpi::engineUpdate(const std::vector<unsigned char>& input) {

    if (input.empty()) {
        return;
    }

    this->impl->update(&input[0], (int)input.size());
}

////////////////////////////////////////////////////////////////////////////////
void MD5MessageDigestSpi::engineUpdate(decaf::nio::ByteBuffer& input) {

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
void MD5MessageDigestSpi::engineReset() {
    this->impl->reset();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> MD5MessageDigestSpi::engineDigest() {

    std::vector<unsigned char> buffer;
    buffer.resize(DECAF_MD5_DIGESTSIZE);
    this->impl->finalize(&buffer[0]);

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
int MD5MessageDigestSpi::engineDigest(unsigned char* buffer, int size, int offset, int length) {

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
