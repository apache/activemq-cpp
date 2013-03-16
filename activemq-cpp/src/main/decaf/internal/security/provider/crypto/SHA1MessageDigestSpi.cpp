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

#include "SHA1MessageDigestSpi.h"

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

#define DECAF_SHA1_DIGESTSIZE 20

/* SHA f()-functions */
#define f1(x,y,z) ((x & y) | (~x & z))
#define f2(x,y,z) (x ^ y ^ z)
#define f3(x,y,z) ((x & y) | (x & z) | (y & z))
#define f4(x,y,z) (x ^ y ^ z)

// SHA constants
#define CONST1 0x5a827999
#define CONST2 0x6ed9eba1
#define CONST3 0x8f1bbcdc
#define CONST4 0xca62c1d6

// 32-bit rotate
#define ROT32(x,n) ((x << n) | (x >> (32 - n)))

#define FUNC(n,i) \
    temp = ROT32(A,5) + f##n(B,C,D) + E + W[i] + CONST##n; \
    E = D; D = C; C = ROT32(B,30); B = A; A = temp

#define SHA_BLOCKSIZE 64

}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace security {
namespace provider {
namespace crypto {

    class SHA1MessageDigestSpiImpl {
    public:

        // message digest
        unsigned int state[5];
        // 64-bit bit counts
        unsigned int count[2];
        // SHA data buffer
        unsigned int data[16];
        // unprocessed amount in data form last update
        int local;

    private:

        union endianTest {
            long Long;
            char Char[sizeof(long)];
        };

        static bool isLittleEndian() {
            static union endianTest u;
            u.Long = 1;
            return (u.Char[0] == 1);
        }

        static void maybeReverseBytes(unsigned int* buffer, int count) {

            int i;
            unsigned char ct[4], *cp;

            if (isLittleEndian()) { // do the swap only if it is little endian
                count = (int)(count / 4);
                cp = (unsigned char*) buffer;
                for (i = 0; i < count; ++i) {
                    ct[0] = cp[0];
                    ct[1] = cp[1];
                    ct[2] = cp[2];
                    ct[3] = cp[3];
                    cp[0] = ct[3];
                    cp[1] = ct[2];
                    cp[2] = ct[1];
                    cp[3] = ct[0];
                    cp += sizeof(int);
                }
            }
        }

        static void SHA1Transform(unsigned int state[5], const unsigned int buffer[16]) {
            int i;
            unsigned int temp, A, B, C, D, E, W[80];

            for (i = 0; i < 16; ++i) {
                W[i] = buffer[i];
            }
            for (i = 16; i < 80; ++i) {
                W[i] = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
            }
            A = state[0];
            B = state[1];
            C = state[2];
            D = state[3];
            E = state[4];

            FUNC(1, 0);  FUNC(1, 1);  FUNC(1, 2);  FUNC(1, 3);  FUNC(1, 4);
            FUNC(1, 5);  FUNC(1, 6);  FUNC(1, 7);  FUNC(1, 8);  FUNC(1, 9);
            FUNC(1,10);  FUNC(1,11);  FUNC(1,12);  FUNC(1,13);  FUNC(1,14);
            FUNC(1,15);  FUNC(1,16);  FUNC(1,17);  FUNC(1,18);  FUNC(1,19);

            FUNC(2,20);  FUNC(2,21);  FUNC(2,22);  FUNC(2,23);  FUNC(2,24);
            FUNC(2,25);  FUNC(2,26);  FUNC(2,27);  FUNC(2,28);  FUNC(2,29);
            FUNC(2,30);  FUNC(2,31);  FUNC(2,32);  FUNC(2,33);  FUNC(2,34);
            FUNC(2,35);  FUNC(2,36);  FUNC(2,37);  FUNC(2,38);  FUNC(2,39);

            FUNC(3,40);  FUNC(3,41);  FUNC(3,42);  FUNC(3,43);  FUNC(3,44);
            FUNC(3,45);  FUNC(3,46);  FUNC(3,47);  FUNC(3,48);  FUNC(3,49);
            FUNC(3,50);  FUNC(3,51);  FUNC(3,52);  FUNC(3,53);  FUNC(3,54);
            FUNC(3,55);  FUNC(3,56);  FUNC(3,57);  FUNC(3,58);  FUNC(3,59);

            FUNC(4,60);  FUNC(4,61);  FUNC(4,62);  FUNC(4,63);  FUNC(4,64);
            FUNC(4,65);  FUNC(4,66);  FUNC(4,67);  FUNC(4,68);  FUNC(4,69);
            FUNC(4,70);  FUNC(4,71);  FUNC(4,72);  FUNC(4,73);  FUNC(4,74);
            FUNC(4,75);  FUNC(4,76);  FUNC(4,77);  FUNC(4,78);  FUNC(4,79);

            state[0] += A;
            state[1] += B;
            state[2] += C;
            state[3] += D;
            state[4] += E;
        }

    public:

        SHA1MessageDigestSpiImpl() : state(), count(), data(), local() {
            reset();
        }

        void reset() {
            state[0] = 0x67452301;
            state[1] = 0xefcdab89;
            state[2] = 0x98badcfe;
            state[3] = 0x10325476;
            state[4] = 0xc3d2e1f0;
            count[0] = 0;
            count[0] = 0;
            local = 0;
        }

        void update(const unsigned char* input, int length) {
            int i;

            if ((count[0] + ((unsigned int) length << 3)) < count[0]) {
                ++count[1];
            }
            count[0] += (unsigned int) length << 3;
            count[1] += (unsigned int) length >> 29;
            if (local) {
                i = SHA_BLOCKSIZE - local;
                if (i > length) {
                    i = length;
                }
                memcpy(((unsigned char*) data) + local, input, i);

                length -= i;
                input += i;
                local += i;

                if (local == SHA_BLOCKSIZE) {
                    maybeReverseBytes(data, SHA_BLOCKSIZE);
                    SHA1Transform(state, data);
                } else {
                    return;
                }
            }

            while (length >= SHA_BLOCKSIZE) {
                memcpy(data, input, SHA_BLOCKSIZE);
                input += SHA_BLOCKSIZE;
                length -= SHA_BLOCKSIZE;
                maybeReverseBytes(data, SHA_BLOCKSIZE);
                SHA1Transform(state, data);
            }
            memcpy(data, input, length);
            local = length;
        }

        void finalize(unsigned char digest[DECAF_SHA1_DIGESTSIZE]) {
            int index, i, j;
            unsigned int lo_bit_count, hi_bit_count, k;

            lo_bit_count = count[0];
            hi_bit_count = count[1];
            index = (int) ((lo_bit_count >> 3) & 0x3f);
            ((unsigned char*) data)[index++] = 0x80;

            if (index > SHA_BLOCKSIZE - 8) {
                memset(((unsigned char*) data) + index, 0, SHA_BLOCKSIZE - index);
                maybeReverseBytes(data, SHA_BLOCKSIZE);
                SHA1Transform(state, data);
                memset((unsigned char*) data, 0, SHA_BLOCKSIZE - 8);
            } else {
              memset(((unsigned char*) data) + index, 0, SHA_BLOCKSIZE - 8 - index);
            }

            maybeReverseBytes(data, SHA_BLOCKSIZE);
            data[14] = hi_bit_count;
            data[15] = lo_bit_count;
            SHA1Transform(state, data);

            for (i = 0, j = 0; j < DECAF_SHA1_DIGESTSIZE; i++) {
                k = state[i];
                digest[j++] = (unsigned char) ((k >> 24) & 0xff);
                digest[j++] = (unsigned char) ((k >> 16) & 0xff);
                digest[j++] = (unsigned char) ((k >> 8) & 0xff);
                digest[j++] = (unsigned char) (k & 0xff);
            }
        }
    };

}}}}}

////////////////////////////////////////////////////////////////////////////////
SHA1MessageDigestSpi::SHA1MessageDigestSpi() : MessageDigestSpi(), impl(new SHA1MessageDigestSpiImpl) {
}

////////////////////////////////////////////////////////////////////////////////
SHA1MessageDigestSpi::~SHA1MessageDigestSpi() {
    delete this->impl;
}

////////////////////////////////////////////////////////////////////////////////
MessageDigestSpi* SHA1MessageDigestSpi::clone() {
    SHA1MessageDigestSpi* clone = new SHA1MessageDigestSpi;

    System::arraycopy(clone->impl->state, 0, this->impl->state, 0, 5);
    System::arraycopy(clone->impl->count, 0, this->impl->count, 0, 2);
    System::arraycopy(clone->impl->data, 0, this->impl->data, 0, 16);
    clone->impl->local = this->impl->local;

    return clone;
}

////////////////////////////////////////////////////////////////////////////////
int SHA1MessageDigestSpi::engineGetDigestLength() {
    return DECAF_SHA1_DIGESTSIZE;
}

////////////////////////////////////////////////////////////////////////////////
void SHA1MessageDigestSpi::engineUpdate(unsigned char input) {
    this->impl->update(&input, 1);
}

////////////////////////////////////////////////////////////////////////////////
void SHA1MessageDigestSpi::engineUpdate(const unsigned char* input, int size, int offset, int length) {

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
void SHA1MessageDigestSpi::engineUpdate(const std::vector<unsigned char>& input) {

    if (input.empty()) {
        return;
    }

    this->impl->update(&input[0], (int)input.size());
}

////////////////////////////////////////////////////////////////////////////////
void SHA1MessageDigestSpi::engineUpdate(decaf::nio::ByteBuffer& input) {

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
void SHA1MessageDigestSpi::engineReset() {
    this->impl->reset();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> SHA1MessageDigestSpi::engineDigest() {

    std::vector<unsigned char> buffer;
    buffer.resize(DECAF_SHA1_DIGESTSIZE);
    this->impl->finalize(&buffer[0]);

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
int SHA1MessageDigestSpi::engineDigest(unsigned char* buffer, int size, int offset, int length) {

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
