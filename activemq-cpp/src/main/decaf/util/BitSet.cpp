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

#include "BitSet.h"

#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/OutOfMemoryError.h>

#include <decaf/lang/System.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Math.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
namespace {

    static const int OFFSET = 6;
    static const int ELM_SIZE = 1 << OFFSET;
    static const int RIGHT_BITS = ELM_SIZE - 1;

    static unsigned long long TWO_N_ARRAY[64] = {
        0x0000000000000001ULL, 0x0000000000000002ULL, 0x0000000000000004ULL,
        0x0000000000000008ULL, 0x0000000000000010ULL, 0x0000000000000020ULL,
        0x0000000000000040ULL, 0x0000000000000080ULL, 0x0000000000000100ULL,
        0x0000000000000200ULL, 0x0000000000000400ULL, 0x0000000000000800ULL,
        0x0000000000001000ULL, 0x0000000000002000ULL, 0x0000000000004000ULL,
        0x0000000000008000ULL, 0x0000000000010000ULL, 0x0000000000020000ULL,
        0x0000000000040000ULL, 0x0000000000080000ULL, 0x0000000000100000ULL,
        0x0000000000200000ULL, 0x0000000000400000ULL, 0x0000000000800000ULL,
        0x0000000001000000ULL, 0x0000000002000000ULL, 0x0000000004000000ULL,
        0x0000000008000000ULL, 0x0000000010000000ULL, 0x0000000020000000ULL,
        0x0000000040000000ULL, 0x0000000080000000ULL, 0x0000000100000000ULL,
        0x0000000200000000ULL, 0x0000000400000000ULL, 0x0000000800000000ULL,
        0x0000001000000000ULL, 0x0000002000000000ULL, 0x0000004000000000ULL,
        0x0000008000000000ULL, 0x0000010000000000ULL, 0x0000020000000000ULL,
        0x0000040000000000ULL, 0x0000080000000000ULL, 0x0000100000000000ULL,
        0x0000200000000000ULL, 0x0000400000000000ULL, 0x0000800000000000ULL,
        0x0001000000000000ULL, 0x0002000000000000ULL, 0x0004000000000000ULL,
        0x0008000000000000ULL, 0x0010000000000000ULL, 0x0020000000000000ULL,
        0x0040000000000000ULL, 0x0080000000000000ULL, 0x0100000000000000ULL,
        0x0200000000000000ULL, 0x0400000000000000ULL, 0x0800000000000000ULL,
        0x1000000000000000ULL, 0x2000000000000000ULL, 0x4000000000000000ULL,
        0x8000000000000000ULL };

    int pop(unsigned long long x) {
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0f0f0f0f;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return (int) x & 0x0000003f;
    }
}

////////////////////////////////////////////////////////////////////////////////
BitSet::BitSet() : bits(new unsigned long long[1]), bitsSize(1), needClear(false), actualArrayLength(0), isLengthActual(true) {
    bits[0] = 0ULL;
}

////////////////////////////////////////////////////////////////////////////////
BitSet::BitSet(int bitCount) : bits(0), bitsSize(0), needClear(false), actualArrayLength(0), isLengthActual(true) {

    if (bitCount < 0) {
        throw NegativeArraySizeException(__FILE__, __LINE__, "");
    }

    bitsSize = (bitCount >> OFFSET) + ((bitCount & RIGHT_BITS) > 0 ? 1 : 0);
    try {
        bits = new unsigned long long[bitsSize];

        if (bits == NULL) {
            throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
        }

        for (int i = 0; i < bitsSize; ++i) {
            bits[i] = 0ULL;
        }
    } catch (std::bad_alloc& e) {
        throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
    }
}

////////////////////////////////////////////////////////////////////////////////
BitSet::BitSet(unsigned long long* bits, int bitsSize, bool needClear, int actualArrayLength, bool isLengthActual) :
    bits(bits), bitsSize(bitsSize), needClear(needClear), actualArrayLength(actualArrayLength), isLengthActual(isLengthActual) {
}

////////////////////////////////////////////////////////////////////////////////
BitSet::~BitSet() {
    try {
        delete [] bits;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
BitSet::BitSet(const BitSet& set) : bits(0), bitsSize(0), needClear(false), actualArrayLength(0), isLengthActual(true) {

    this->actualArrayLength = set.actualArrayLength;
    this->isLengthActual = set.isLengthActual;
    this->needClear = set.needClear;
    this->bitsSize = set.bitsSize;

    try {
        bits = new unsigned long long[bitsSize];

        if (bits == NULL) {
            throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
        }

        System::arraycopy(set.bits, 0, bits, 0, set.bitsSize);

    } catch (std::bad_alloc& e) {
        throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
    }
}

////////////////////////////////////////////////////////////////////////////////
BitSet& BitSet::operator= (const BitSet& set) {

    delete [] this->bits;

    this->actualArrayLength = set.actualArrayLength;
    this->isLengthActual = set.isLengthActual;
    this->needClear = set.needClear;
    this->bitsSize = set.bitsSize;

    try {
        bits = new unsigned long long[bitsSize];

        if (bits == NULL) {
            throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
        }

        System::arraycopy(set.bits, 0, bits, 0, set.bitsSize);

    } catch (std::bad_alloc& e) {
        throw OutOfMemoryError(__FILE__, __LINE__, "Failed to allocate bit array.");
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::AND(const BitSet& set) {

    unsigned long long* bsBits = set.bits;

    if (!needClear) {
        return;
    }

    int length1 = actualArrayLength, length2 = set.actualArrayLength;
    if (length1 <= length2) {
        for (int i = 0; i < length1; i++) {
            bits[i] &= bsBits[i];
        }
    } else {
        for (int i = 0; i < length2; i++) {
            bits[i] &= bsBits[i];
        }
        for (int i = length2; i < length1; i++) {
            bits[i] = 0;
        }
        actualArrayLength = length2;
    }
    isLengthActual = !((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0));
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::andNot(const BitSet& set) {
    unsigned long long* bsBits = set.bits;

    if (!needClear) {
        return;
    }

    int range = actualArrayLength < set.actualArrayLength ? actualArrayLength : set.actualArrayLength;
    for (int i = 0; i < range; i++) {
        bits[i] &= ~bsBits[i];
    }

    if (actualArrayLength < range) {
        actualArrayLength = range;
    }

    isLengthActual = !((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0));
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::cardinality() {
    if (!needClear) {
        return 0;
    }
    int count = 0;
    int length = bitsSize;
    // FIXME: need to test performance, if still not satisfied, change it to
    // 256-bits table based
    for (int idx = 0; idx < length; idx++) {
        count += pop(bits[idx] & 0xffffffffL);
        count += pop((bits[idx] >> 32));
    }
    return count;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::clear() {
    if (needClear) {
        for (int i = 0; i < bitsSize; i++) {
            bits[i] = 0ULL;
        }
        actualArrayLength = 0;
        isLengthActual = true;
        needClear = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::clear(int index) {

    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    if (!needClear) {
        return;
    }
    int arrayPos = index >> OFFSET;
    if (arrayPos < actualArrayLength) {
        bits[arrayPos] &= ~(TWO_N_ARRAY[index & RIGHT_BITS]);
        if (bits[actualArrayLength - 1] == 0) {
            isLengthActual = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::clear(int fromIndex, int toIndex) {
    if (fromIndex < 0 || toIndex < 0 || toIndex < fromIndex) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Invalid from or to index given");
    }

    if (!needClear) {
        return;
    }

    int last = (actualArrayLength << OFFSET);
    if (fromIndex >= last || fromIndex == toIndex) {
        return;
    }
    if (toIndex > last) {
        toIndex = last;
    }

    int idx1 = fromIndex >> OFFSET;
    int idx2 = (toIndex - 1) >> OFFSET;
    unsigned long long factor1 = (~0ULL) << (fromIndex & RIGHT_BITS);
    int shift = (ELM_SIZE - (toIndex & RIGHT_BITS));
    unsigned long long factor2 = shift < 64 ? (~0ULL) >> shift : (~0ULL);

    if (idx1 == idx2) {
        bits[idx1] &= ~(factor1 & factor2);
    } else {
        bits[idx1] &= ~factor1;
        bits[idx2] &= ~factor2;
        for (int i = idx1 + 1; i < idx2; i++) {
            bits[i] = 0ULL;
        }
    }

    if ((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0)) {
        isLengthActual = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::OR(const BitSet& set) {
    int setActualLen = set.getActualArrayLength();

    if (setActualLen > bitsSize) {
        unsigned long long* tempBits = new unsigned long long[setActualLen];
        System::arraycopy(set.bits, 0, tempBits, 0, set.actualArrayLength);
        for (int i = 0; i < actualArrayLength; i++) {
            tempBits[i] |= bits[i];
        }
        delete [] bits;
        bits = tempBits;
        actualArrayLength = setActualLen;
        isLengthActual = true;
    } else {
        unsigned long long* bsBits = set.bits;
        for (int i = 0; i < setActualLen; i++) {
            bits[i] |= bsBits[i];
        }
        if (setActualLen > actualArrayLength) {
            actualArrayLength = setActualLen;
            isLengthActual = true;
        }
    }
    needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
bool BitSet::equals(const BitSet& set) const {

    if (this == &set) {
        return true;
    }

    unsigned long long* bsBits = set.bits;
    int length1 = this->actualArrayLength;
    int length2 = set.actualArrayLength;
    if (this->isLengthActual && set.isLengthActual && length1 != length2) {
        return false;
    }

    // If one of the BitSets is larger than the other, check to see if
    // any of its extra bits are set. If so return false.
    if (length1 <= length2) {
        for (int i = 0; i < length1; i++) {
            if (bits[i] != bsBits[i]) {
                return false;
            }
        }
        for (int i = length1; i < length2; i++) {
            if (bsBits[i] != 0) {
                return false;
            }
        }
    } else {
        for (int i = 0; i < length2; i++) {
            if (bits[i] != bsBits[i]) {
                return false;
            }
        }
        for (int i = length2; i < length1; i++) {
            if (bits[i] != 0) {
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::flip(int index) {
    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    int len = (index >> OFFSET) + 1;
    if (len > bitsSize) {
        ensureCapacity(len);
    }
    bits[len - 1] ^= TWO_N_ARRAY[index & RIGHT_BITS];
    if (len > actualArrayLength) {
        actualArrayLength = len;
    }
    isLengthActual = !((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0));
    needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::flip(int fromIndex, int toIndex) {
    if (fromIndex < 0 || toIndex < 0 || toIndex < fromIndex) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Invalid from or to index given");
    }

    if (fromIndex == toIndex) {
        return;
    }
    int len2 = ((toIndex - 1) >> OFFSET) + 1;
    if (len2 > bitsSize) {
        ensureCapacity(len2);
    }

    int idx1 = fromIndex >> OFFSET;
    int idx2 = (toIndex - 1) >> OFFSET;
    unsigned long long factor1 = (~0ULL) << (fromIndex & RIGHT_BITS);
    int shift = (ELM_SIZE - (toIndex & RIGHT_BITS));
    unsigned long long factor2 = shift < 64 ? (~0ULL) >> shift : (~0ULL);

    if (idx1 == idx2) {
        bits[idx1] ^= (factor1 & factor2);
    } else {
        bits[idx1] ^= factor1;
        bits[idx2] ^= factor2;
        for (int i = idx1 + 1; i < idx2; i++) {
            bits[i] ^= (~0ULL);
        }
    }
    if (len2 > actualArrayLength) {
        actualArrayLength = len2;
    }
    isLengthActual = !((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0));
    needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
bool BitSet::get(int index) const {

    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    int arrayPos = index >> OFFSET;
    if (arrayPos < actualArrayLength) {
        return (bits[arrayPos] & TWO_N_ARRAY[index & RIGHT_BITS]) != 0;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
BitSet BitSet::get(int fromIndex, int toIndex) const {
    if (fromIndex < 0 || toIndex < 0 || toIndex < fromIndex) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Invalid from or to index given");
    }

    int last = actualArrayLength << OFFSET;
    if (fromIndex >= last || fromIndex == toIndex) {
        return BitSet(0);
    }
    if (toIndex > last) {
        toIndex = last;
    }

    int idx1 = fromIndex >> OFFSET;
    int idx2 = (toIndex - 1) >> OFFSET;
    unsigned long long factor1 = (~0ULL) << (fromIndex & RIGHT_BITS);
    int shift = (ELM_SIZE - (toIndex & RIGHT_BITS));
    unsigned long long factor2 = shift < 64 ? (~0ULL) >> shift : (~0ULL);

    if (idx1 == idx2) {
        unsigned long long result = (bits[idx1] & (factor1 & factor2)) >> (fromIndex % ELM_SIZE);
        if (result == 0) {
            return BitSet(0);
        }

        unsigned long long* newBits = new unsigned long long[1];
        newBits[0] = result;
        return BitSet(newBits, 1, needClear, 1, true);
    }

    int newBitsSize = idx2 - idx1 + 1;
    unsigned long long* newbits = new unsigned long long[newBitsSize];

    // first fill in the first and last indexes in the new bitset
    newbits[0] = bits[idx1] & factor1;
    newbits[newBitsSize - 1] = bits[idx2] & factor2;

    // fill in the in between elements of the new bitset
    for (int i = 1; i < idx2 - idx1; i++) {
        newbits[i] = bits[idx1 + i];
    }

    // shift all the elements in the new bitset to the right by fromIndex % ELM_SIZE
    int numBitsToShift = fromIndex & RIGHT_BITS;
    int actualLen = newBitsSize;
    if (numBitsToShift != 0) {
        for (int i = 0; i < newBitsSize; i++) {
            // shift the current element to the right regardless of sign
            newbits[i] = newbits[i] >> (numBitsToShift);

            // apply the last x bits of newbits[i+1] to the current element
            if (i != newBitsSize - 1) {
                newbits[i] |= newbits[i + 1] << (ELM_SIZE - (numBitsToShift));
            }

            if (newbits[i] != 0) {
                actualLen = i + 1;
            }
        }
    }

    return BitSet(newbits, newBitsSize, needClear, actualLen, newbits[actualLen - 1] != 0);
}

////////////////////////////////////////////////////////////////////////////////
bool BitSet::intersects(const BitSet& set) const {
    unsigned long long* bsBits = set.bits;
    int length1 = actualArrayLength;
    int length2 = set.actualArrayLength;

    if (length1 <= length2) {
        for (int i = 0; i < length1; i++) {
            if ((bits[i] & bsBits[i]) != 0ULL) {
                return true;
            }
        }
    } else {
        for (int i = 0; i < length2; i++) {
            if ((bits[i] & bsBits[i]) != 0ULL) {
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool BitSet::isEmpty() const {
    if (!needClear) {
        return true;
    }

    for (int idx = 0; idx < bitsSize; idx++) {
        if (bits[idx] != 0ULL) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::length() const {

    int idx = actualArrayLength - 1;
    while (idx >= 0 && bits[idx] == 0) {
        --idx;
    }
    actualArrayLength = idx + 1;
    if (idx == -1) {
        return 0;
    }
    int i = ELM_SIZE - 1;
    unsigned long long val = bits[idx];
    while ((val & (TWO_N_ARRAY[i])) == 0 && i > 0) {
        i--;
    }
    return (idx << OFFSET) + i + 1;
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::nextClearBit(int index) const {
    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    int length = actualArrayLength;
    int bssize = length << OFFSET;
    if (index >= bssize) {
        return index;
    }

    int idx = index >> OFFSET;
    // first check in the same bit set element
    if (bits[idx] != (~0ULL)) {
        for (int j = index % ELM_SIZE; j < ELM_SIZE; j++) {
            if (((bits[idx] & (TWO_N_ARRAY[j])) == 0)) {
                return idx * ELM_SIZE + j;
            }
        }
    }
    idx++;
    while (idx < length && bits[idx] == (~0ULL)) {
        idx++;
    }
    if (idx == length) {
        return bssize;
    }

    // we know for sure there is a bit set to true in this element
    // since the bitset value is not 0ULL
    for (int j = 0; j < ELM_SIZE; j++) {
        if (((bits[idx] & (TWO_N_ARRAY[j])) == 0)) {
            return (idx << OFFSET) + j;
        }
    }

    return bssize;
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::nextSetBit(int index) const {
    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    if (index >= actualArrayLength << OFFSET) {
        return -1;
    }

    int idx = index >> OFFSET;
    // first check in the same bit set element
    if (bits[idx] != 0ULL) {
        for (int j = index & RIGHT_BITS; j < ELM_SIZE; j++) {
            if (((bits[idx] & (TWO_N_ARRAY[j])) != 0)) {
                return (idx << OFFSET) + j;
            }
        }
    }

    idx++;
    while (idx < actualArrayLength && bits[idx] == 0ULL) {
        idx++;
    }
    if (idx == actualArrayLength) {
        return -1;
    }

    // we know for sure there is a bit set to true in this element
    // since the bitset value is not 0ULL
    for (int j = 0; j < ELM_SIZE; j++) {
        if (((bits[idx] & (TWO_N_ARRAY[j])) != 0)) {
            return (idx << OFFSET) + j;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::set(int index) {

    if (index < 0) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Index given was negative");
    }

    int len = (index >> OFFSET) + 1;
    if (len > bitsSize) {
        ensureCapacity(len);
    }
    bits[len - 1] |= TWO_N_ARRAY[index & RIGHT_BITS];
    if (len > actualArrayLength) {
        actualArrayLength = len;
        isLengthActual = true;
    }

    this->needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::set(int index, bool value) {

    if (value) {
        set(index);
    } else {
        clear(index);
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::set(int fromIndex, int toIndex) {

    if (fromIndex < 0 || toIndex < 0 || toIndex < fromIndex) {
        throw IndexOutOfBoundsException(__FILE__, __LINE__, "Invalid from or to index given");
    }

    if (fromIndex == toIndex) {
        return;
    }
    int len2 = ((toIndex - 1) >> OFFSET) + 1;
    if (len2 > bitsSize) {
        ensureCapacity(len2);
    }

    int idx1 = fromIndex >> OFFSET;
    int idx2 = (toIndex - 1) >> OFFSET;
    unsigned long long factor1 = (~0ULL) << (fromIndex & RIGHT_BITS);
    int shift = (ELM_SIZE - (toIndex & RIGHT_BITS));
    unsigned long long factor2 = shift < 64 ? (~0ULL) >> shift : (~0ULL);

    if (idx1 == idx2) {
        bits[idx1] |= (factor1 & factor2);
    } else {
        bits[idx1] |= factor1;
        bits[idx2] |= factor2;
        for (int i = idx1 + 1; i < idx2; i++) {
            bits[i] |= (~0ULL);
        }
    }
    if (idx2 + 1 > actualArrayLength) {
        actualArrayLength = idx2 + 1;
        isLengthActual = true;
    }

    needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::set(int fromIndex, int toIndex, bool value) {

    if (value) {
        set(fromIndex, toIndex);
    } else {
        clear(fromIndex, toIndex);
    }
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::size() const {
    return bitsSize << OFFSET;
}

////////////////////////////////////////////////////////////////////////////////
std::string BitSet::toString() const {
    std::string sb;
    int bitCount = 0;

    sb.append("{");
    bool comma = false;
    for (int i = 0; i < bitsSize; i++) {
        if (bits[i] == 0) {
            bitCount += ELM_SIZE;
            continue;
        }
        for (int j = 0; j < ELM_SIZE; j++) {
            if (((bits[i] & (TWO_N_ARRAY[j])) != 0)) {
                if (comma) {
                    sb.append(", ");
                }
                sb.append(Integer::toString(bitCount));
                comma = true;
            }
            bitCount++;
        }
    }
    sb.append("}");
    return sb;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::XOR(const BitSet& set) {
    int setActualLength = set.getActualArrayLength();
    if (setActualLength > bitsSize) {
        unsigned long long* tempBits = new unsigned long long[setActualLength];
        System::arraycopy(set.bits, 0, tempBits, 0, set.actualArrayLength);
        for (int i = 0; i < actualArrayLength; i++) {
            tempBits[i] ^= bits[i];
        }
        delete [] bits;
        bits = tempBits;
        actualArrayLength = setActualLength;
        isLengthActual = !((actualArrayLength > 0) && (bits[actualArrayLength - 1] == 0));
    } else {
        unsigned long long* bsBits = set.bits;
        for (int i = 0; i < setActualLength; i++) {
            bits[i] ^= bsBits[i];
        }
        if (setActualLength > actualArrayLength) {
            actualArrayLength = setActualLength;
            isLengthActual = true;
        }
    }

    this->needClear = true;
}

////////////////////////////////////////////////////////////////////////////////
void BitSet::ensureCapacity(int length) {
    int newSize = Math::max(length, bitsSize * 2);
    unsigned long long* tempBits = new unsigned long long[newSize];
    System::arraycopy(bits, 0, tempBits, 0, this->actualArrayLength);
    for (int i = this->actualArrayLength; i < newSize; ++i) {
        tempBits[i] = 0ULL;
    }
    delete [] bits;
    bits = tempBits;
    bitsSize = newSize;
}

////////////////////////////////////////////////////////////////////////////////
int BitSet::getActualArrayLength() const {
    if (isLengthActual) {
        return actualArrayLength;
    }
    int idx = actualArrayLength - 1;
    while (idx >= 0 && bits[idx] == 0) {
        --idx;
    }
    actualArrayLength = idx + 1;
    isLengthActual = true;
    return actualArrayLength;
}
