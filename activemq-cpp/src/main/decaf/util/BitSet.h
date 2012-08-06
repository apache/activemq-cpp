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

#ifndef _DECAF_UTIL_BITSET_H_
#define _DECAF_UTIL_BITSET_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace util {

    /**
     * This class implements a vector of bits that grows as needed. Each component of
     * the bit set has a boolean value. The bits of a BitSet are indexed by nonnegative
     * integers. Individual indexed bits can be examined, set, or cleared. One BitSet
     * may be used to modify the contents of another BitSet through logical AND, logical
     * inclusive OR, and logical exclusive OR operations.
     *
     * By default, all bits in the set initially have the value false.
     *
     * Every bit set has a current size, which is the number of bits of space currently
     * in use by the bit set. Note that the size is related to the implementation of a
     * bit set, so it may change with implementation. The length of a bit set relates to
     * logical length of a bit set and is defined independently of implementation.
     *
     * A BitSet is not safe for multi-threaded use without external synchronization.
     *
     * @since 1.0
     */
    class DECAF_API BitSet {
    private:

        // The actual array of 64 bit bits elements
        unsigned long long* bits;
        int bitsSize;

        // Optimization, when the array is all zero there's no need to traverse
        bool needClear;

        // Lazily maintained actual array length and state.
        mutable int actualArrayLength;
        mutable bool isLengthActual;

    private:

        BitSet(unsigned long long* bits, int bitsSize, bool needClear, int actualArrayLength, bool isLengthActual);

    public:

        /**
         * Creates a new BitSet whose bits are all false.
         */
        BitSet();

        /**
         * Creates a bit set whose initial size is large enough to explicitly represent bits
         * with indices in the range 0 through bitCount-1. All bits are initially false.  If the
         * bitCount is not a multiple of 64 then the count is rounded to the next closest
         * multiple of 64.
         *
         * @param bitCount
         *      The number of bits this BitSet should hold.
         *
         * @throws NegativeArraySizeException if bitCount is negative.
         */
        BitSet(int bitCount);

        /**
         * Copy Constructor
         */
        BitSet(const BitSet& set);

        /**
         * Assignment
         */
        BitSet& operator= (const BitSet& set);

        virtual ~BitSet();

    public:

        /**
         * Performs a logical AND of this target bit set with the argument bit set. This bit
         * set is modified so that each bit in it has the value true if and only if it both
         * initially had the value true and the corresponding bit in the bit set argument
         * also had the value true.
         *
         * @param set
         *      The BitSet to perform this action against.
         */
        void AND(const BitSet& set);

        /**
         * Performs a logical OR of this bit set with the bit set argument. This bit set is
         * modified so that a bit in it has the value true if and only if it either already had
         * the value true or the corresponding bit in the bit set argument has the value true.
         *
         * @param set
         *      The BitSet to perform this action against.
         */
        void OR(const BitSet& set);

        /**
         * Clears all of the bits in this BitSet whose corresponding bit is set in the specified BitSet.
         *
         * @param set
         *      The BitSet to perform this action against.
         */
        void andNot(const BitSet& set);

        /**
         * Returns the number of bits set to true in this BitSet.
         *
         * @returns the number of bits set to true in this BitSet.
         */
        int cardinality();

        /**
         * Sets all of the bits in this BitSet to false.
         */
        void clear();

        /**
         * Sets the bit specified by the index to false.
         *
         * @param index
         *      The index of the bit whose value is to be set to false
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        void clear(int index);

        /**
         * Sets the bits from the specified fromIndex (inclusive) to the specified toIndex
         * (exclusive) to false.
         *
         * @param fromIndex
         *      The index (inclusive) to start setting bits to false.
         * @param toIndex
         *      The index (exclusive) to stop setting bits to false.
         *
         * @throws IndexOutOfBoundsException if fromIndex is negative, or toIndex is negative, or
         *                                   fromIndex is larger than toIndex.
         */
        void clear(int fromIndex, int toIndex);

        /**
         * Compares this object against the specified object. The result is true if and only if
         * is a Bitset object that has exactly the same set of bits set to true as this bit set.
         * That is, for every nonnegative int index k,
         *
         *   set.get(k) == this->get(k)
         *
         * must be true. The current sizes of the two bit sets are not compared.
         *
         * @returns true if the sets are the same, false otherwise.
         */
        bool equals(const BitSet& set) const;

        /**
         * Sets the bit at the specified index to the complement of its current value.
         *
         * @param index
         *      The index of the bit whose value is to be set to its compliment.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        void flip(int index);

        /**
         * Sets each bit from the specified fromIndex (inclusive) to the specified toIndex
         * (exclusive) to the complement of its current value.
         *
         * @param fromIndex
         *      The index (inclusive) to start setting bits to its compliment.
         * @param toIndex
         *      The index (exclusive) to stop setting bits to its compliment.
         *
         * @throws IndexOutOfBoundsException if fromIndex is negative, or toIndex is negative, or
         *                                   fromIndex is larger than toIndex.
         */
        void flip(int fromIndex, int toIndex);

        /**
         * Returns the value of the bit with the specified index. The value is true if the bit
         * with the given index is currently set in this BitSet; otherwise, the result is false.
         *
         * @param index
         *      The index of the bit in question.
         *
         * @returns the value of the bit with the specified index.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        bool get(int index) const;

        /**
         * Returns a new BitSet composed of bits from this BitSet from fromIndex (inclusive) to
         * toIndex (exclusive).
         *
         * @param fromIndex
         *      The index (inclusive) to start at.
         * @param toIndex
         *      The index (exclusive) to stop at.
         *
         * @returns a new BitSet containing the specified values.
         *
         * @throws IndexOutOfBoundsException if fromIndex is negative, or toIndex is negative, or
         *                                   fromIndex is larger than toIndex.
         */
        BitSet get(int fromIndex, int toIndex) const;

        /**
         * Returns true if the specified BitSet has any bits set to true that are also set to
         * true in this BitSet.
         *
         * @param set
         *      BitSet to intersect with.
         *
         * @returns boolean indicating whether this BitSet intersects the specified BitSet.
         */
        bool intersects(const BitSet& set) const;

        /**
         * Returns true if this BitSet contains no bits that are set to true.
         *
         * @returns true if the set is empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * Returns the "logical size" of this BitSet: the index of the highest set bit in the
         * BitSet plus one. Returns zero if the BitSet contains no set bits.
         *
         * @returns the logical size of the BitSet.
         */
        int length() const;

        /**
         * Returns the index of the first bit that is set to false that occurs on or after the
         * specified starting index.
         *
         * @param index
         *      The index to start the search from (inclusive).
         *
         * @returns the index of the next clear bit.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        int nextClearBit(int index) const;

        /**
         * Returns the index of the first bit that is set to true that occurs on or after the
         * specified starting index.
         *
         * @param index
         *      The index to start the search from (inclusive).
         *
         * @returns the index of the next set bit.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        int nextSetBit(int index) const;

        /**
         * Sets the bit at the specified index to true.
         *
         * @param index
         *      The index to set to true.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        void set(int index);

        /**
         * Sets the bit at the specified index to the specified value.
         *
         * @param index
         *      The index to set.
         * @param value
         *      The value to assign to the given bit.
         *
         * @throws IndexOutOfBoundsException if the index value is negative.
         */
        void set(int index, bool value);

        /**
         * Sets the bits from the specified fromIndex (inclusive) to the specified toIndex
         * (exclusive) to true.
         *
         * @param fromIndex
         *      The index (inclusive) to start at.
         * @param toIndex
         *      The index (exclusive) to stop at.
         *
         * @throws IndexOutOfBoundsException if fromIndex is negative, or toIndex is negative, or
         *                                   fromIndex is larger than toIndex.
         */
        void set(int fromIndex, int toIndex);

        /**
         * Sets the bits from the specified fromIndex (inclusive) to the specified toIndex
         * (exclusive) to the value given.
         *
         * @param fromIndex
         *      The index (inclusive) to start at.
         * @param toIndex
         *      The index (exclusive) to stop at.
         * @param value
         *      The boolean value to assign to the target bits.
         *
         * @throws IndexOutOfBoundsException if fromIndex is negative, or toIndex is negative, or
         *                                   fromIndex is larger than toIndex.
         */
        void set(int fromIndex, int toIndex, bool value);

        /**
         * Returns the number of bits of space actually in use by this BitSet to represent bit
         * values. The maximum element in the set is the size - 1st element.
         *
         * @returns the number of bits currently in this bit set.
         */
        int size() const;

        /**
         * Returns a string representation of this bit set. For every index for which this BitSet
         * contains a bit in the set state, the decimal representation of that index is included
         * in the result. Such indices are listed in order from lowest to highest, separated by
         * ", " (a comma and a space) and surrounded by braces, resulting in the usual mathematical
         * notation for a set of integers.
         *
         * @returns string representation of the BitSet.
         */
        std::string toString() const;

        /**
         * Performs a logical XOR of this bit set with the bit set argument. This bit set is modified
         * so that a bit in it has the value true if and only if one of the following statements holds:
         *
         *   * The bit initially has the value true, and the corresponding bit in the argument has
         *     the value false.
         *   * The bit initially has the value false, and the corresponding bit in the argument has
         *     the value true.
         *
         * @param set
         *      The BitSet to use.
         */
        void XOR(const BitSet& set);

    private:

        /**
         * Increase the size of the internal array to accommodate length bits.
         * The new array max index will be a multiple of 64.
         *
         * @param length
         *      the index the new array needs to be able to access.
         */
        void ensureCapacity(int length);

        /**
         * Gets the actual length of the BitSet bit array which is maintained in a
         * sometimes lazy fashion to avoid excessive array traversals.  The actual
         * length is the number of non-zero array elements in the physical array
         * used to back this BitSet.
         *
         * @return the actual array length.
         */
        int getActualArrayLength() const;

    };

}}

#endif /* _DECAF_UTIL_BITSET_H_ */
