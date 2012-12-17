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

#include "BitSetTest.h"

#include <decaf/util/BitSet.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/System.h>

#include <decaf/lang/exceptions/OutOfMemoryError.h>
#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
namespace {

    BitSet eightbs;

    void printBitset(const BitSet& bs) {
        std::cout << std::endl;
        for (int i = bs.size() - 1; i >= 0; i--) {
            if (bs.get(i)) {
                std::cout << "1";
            } else {
                std::cout << "0";
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
BitSetTest::BitSetTest() {
}

////////////////////////////////////////////////////////////////////////////////
BitSetTest::~BitSetTest() {
}


////////////////////////////////////////////////////////////////////////////////
void BitSetTest::setUp() {

    eightbs = BitSet(1);

    for (int i = 0; i < 8; i++) {
        eightbs.set(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testConstructor() {

    BitSet bs;

    // Default size for a BitSet should be 64 elements;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created BitSet of incorrect size", 64, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("New BitSet had invalid string representation",
                                 std::string("{}"), bs.toString());
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testConstructorI() {

    BitSet bs(128);
    // Default size for a BitSet should be 64 elements;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Created BitSet of incorrect size", 128, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("New BitSet had invalid string representation: " + bs.toString(),
                                 std::string("{}"), bs.toString());

    // All BitSets are created with elements of multiples of 64

    BitSet bs1(89);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to round BitSet element size", 128, bs1.size());

    try {
        BitSet bs(-9);
        CPPUNIT_FAIL("Failed to throw exception when creating a new BitSet with negative elements value");
    } catch (NegativeArraySizeException& e) {
        // Correct behavior
    }

    // Regression test for HARMONY-4147
    try {
        BitSet bs(Integer::MAX_VALUE);
    } catch (OutOfMemoryError& e) {
        // Ignore if no room for this size.
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testCopy() {
    BitSet bs(eightbs);
    CPPUNIT_ASSERT_MESSAGE("copy failed to return equal BitSet", bs.equals(eightbs));

    BitSet assigned;
    assigned = eightbs;
    CPPUNIT_ASSERT_MESSAGE("copy failed to return equal BitSet", assigned.equals(eightbs));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testEquals() {
    BitSet bs = eightbs;

    CPPUNIT_ASSERT_MESSAGE("Same BitSet returned false", eightbs.equals(eightbs));
    CPPUNIT_ASSERT_MESSAGE("Identical BitSet returned false", bs.equals(eightbs));
    bs.clear(6);
    CPPUNIT_ASSERT_MESSAGE("Different BitSets returned true", !eightbs.equals(bs));
    // Grow the BitSet
    bs = eightbs;
    bs.set(128);
    CPPUNIT_ASSERT_MESSAGE("Different sized BitSet with higher bit set returned true",
                           !eightbs.equals(bs));
    bs.clear(128);
    CPPUNIT_ASSERT_MESSAGE("Different sized BitSet with higher bits not set returned false",
                           eightbs.equals(bs));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testClear() {
    eightbs.clear();
    for (int i = 0; i < 8; i++) {
        CPPUNIT_ASSERT_MESSAGE("Clear didn't clear bit " + i, !eightbs.get(i));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length", 0, eightbs.length());

    BitSet bs(3400);
    bs.set(0, bs.size() - 1); // ensure all bits are 1's
    bs.set(bs.size() - 1);
    bs.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length", 0, bs.length());
    CPPUNIT_ASSERT_MESSAGE("Test2: isEmpty() returned incorrect value", bs.isEmpty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: cardinality() returned incorrect value", 0,
                                 bs.cardinality());
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testClearI() {

    eightbs.clear(7);
    CPPUNIT_ASSERT_MESSAGE("Failed to clear bit", !eightbs.get(7));

    // Check to see all other bits are still set
    for (int i = 0; i < 7; i++) {
        CPPUNIT_ASSERT_MESSAGE("Clear cleared incorrect bits", eightbs.get(i));
    }

    eightbs.clear(165);
    CPPUNIT_ASSERT_MESSAGE("Failed to clear bit", !eightbs.get(165));

    // Try out of range
    try {
        eightbs.clear(-1);
        CPPUNIT_FAIL("Failed to throw expected out of bounds exception");
    } catch (IndexOutOfBoundsException& e) {
        // Correct behaviour
    }

    BitSet bs(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length,", 0, bs.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong size,", 0, bs.size());
    bs.clear(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length,", 0, bs.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong size,", 0, bs.size());

    bs.clear(60);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length,", 0, bs.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size,", 0, bs.size());

    bs.clear(120);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong size,", 0, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong length,", 0, bs.length());

    bs.set(25);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong size,", 64, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong length,", 26, bs.length());

    bs.clear(80);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong size,", 64, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong length,", 26, bs.length());

    bs.clear(25);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong size,", 64, bs.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong length,", 0, bs.length());

    BitSet bs1;
    try {
        bs1.clear(-1);
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException");
    } catch (IndexOutOfBoundsException& e) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testClearII() {
    BitSet bitset;
    for (int i = 0; i < 20; i++) {
        bitset.set(i);
    }
    bitset.clear(10, 10);

    // pos1 and pos2 are in the same bitset element
    BitSet bs(16);
    int initialSize = bs.size();
    bs.set(0, initialSize);
    bs.clear(5);
    bs.clear(15);
    bs.clear(7, 11);
    for (int i = 0; i < 7; i++) {
        if (i == 5) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        } else {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
        }
    }
    for (int i = 7; i < 11; i++) {
        CPPUNIT_ASSERT_MESSAGE("Failed to clear bit " + Integer::toString(i), !bs.get(i));
    }

    for (int i = 11; i < initialSize; i++) {
        if (i == 15) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        } else {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
        }
    }

    for (int i = initialSize; i < bs.size(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + i, !bs.get(i));
    }

    // pos1 and pos2 is in the same bitset element, boundary testing
    {
        BitSet bs(16);
        initialSize = bs.size();
        bs.set(0, initialSize);
        bs.clear(7, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to grow BitSet", 64, bs.size());
        for (int i = 0; i < 7; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
        }
        for (int i = 7; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to clear bit " + Integer::toString(i), !bs.get(i));
        }
        for (int i = 64; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        // more boundary testing
        BitSet bs(32);
        initialSize = bs.size();
        bs.set(0, initialSize);
        bs.clear(0, 64);
        for (int i = 0; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to clear bit " + Integer::toString(i), !bs.get(i));
        }
        for (int i = 64; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        BitSet bs(32);
        initialSize = bs.size();
        bs.set(0, initialSize);
        bs.clear(0, 65);
        for (int i = 0; i < 65; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to clear bit " + i, !bs.get(i));
        }
        for (int i = 65; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        // pos1 and pos2 are in two sequential bitset elements
        BitSet bs(128);
        initialSize = bs.size();
        bs.set(0, initialSize);
        bs.clear(7);
        bs.clear(110);
        bs.clear(9, 74);
        for (int i = 0; i < 9; i++) {
            if (i == 7) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
            }
        }
        for (int i = 9; i < 74; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to clear bit " + i, !bs.get(i));
        }
        for (int i = 74; i < initialSize; i++) {
            if (i == 110) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
            }
        }
        for (int i = initialSize; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        // pos1 and pos2 are in two non-sequential bitset elements
        BitSet bs(256);
        bs.set(0, 256);
        bs.clear(7);
        bs.clear(255);
        bs.clear(9, 219);
        for (int i = 0; i < 9; i++) {
            if (i == 7) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + Integer::toString(i), bs.get(i));
            }
        }

        for (int i = 9; i < 219; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to clear bit " + i, !bs.get(i));
        }

        for (int i = 219; i < 255; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have cleared bit " + i, bs.get(i));
        }

        for (int i = 255; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + i, !bs.get(i));
        }
    }
    {
        // test illegal args
        BitSet bs(10);
        try {
            bs.clear(-1, 3);
            CPPUNIT_FAIL("Test1: Attempt to flip with  negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // excepted
        }

        try {
            bs.clear(2, -1);
            CPPUNIT_FAIL("Test2: Attempt to flip with negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // excepted
        }

        bs.set(2, 4);
        bs.clear(2, 2);
        CPPUNIT_ASSERT_MESSAGE("Bit got cleared incorrectly ", bs.get(2));
        try {
            bs.clear(4, 2);
            CPPUNIT_FAIL("Test4: Attempt to flip with illegal args CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // excepted
        }
    }
    {
        BitSet bs(0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong size,", 0, bs.size());

        bs.clear(0, 2);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong size,", 0, bs.size());

        bs.clear(60, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size,", 0, bs.size());

        bs.clear(64, 120);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong size,", 0, bs.size());

        bs.set(25);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong length,", 26, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong size,", 64, bs.size());

        bs.clear(60, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong length,", 26, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong size,", 64, bs.size());

        bs.clear(64, 120);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong length,", 26, bs.length());

        bs.clear(80);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test8: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test8: Wrong length,", 26, bs.length());

        bs.clear(25);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test9: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test9: Wrong length,", 0, bs.length());
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testGetI() {

    BitSet bs;
    bs.set(8);
    CPPUNIT_ASSERT_MESSAGE("Get returned true for index out of range", !eightbs.get(99));
    CPPUNIT_ASSERT_MESSAGE("Get returned false for set value", eightbs.get(3));
    CPPUNIT_ASSERT_MESSAGE("Get returned true for a non set value", !bs.get(0));

    try {
        bs.get(-1);
        CPPUNIT_FAIL("Attempt to get at negative index CPPUNIT_FAILed to generate exception");
    } catch (IndexOutOfBoundsException& e) {
        // Correct behaviour
    }

    BitSet bs1;
    CPPUNIT_ASSERT_MESSAGE("Access greater than size", !bs1.get(64));

    BitSet bs2;
    bs2.set(63);
    CPPUNIT_ASSERT_MESSAGE("Test highest bit", bs2.get(63));

    BitSet bs3(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length,", 0, bs3.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong size,", 0, bs3.size());

    bs3.get(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length,", 0, bs3.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong size,", 0, bs3.size());

    bs3.get(70);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length,", 0, bs3.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size,", 0, bs3.size());

    bs3.set(70);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length,", 71, bs3.length());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size,", 128, bs3.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong value,", true, bs3.get(70));

    BitSet bs4;
    try {
        bs4.get(Integer::MIN_VALUE);
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException");
    } catch (IndexOutOfBoundsException& e) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testGetII() {

    BitSet bitset(30);
    bitset.get(3, 3);

    BitSet bs(512);
    BitSet resultbs;

    bs.set(3, 9);
    bs.set(10, 20);
    bs.set(60, 75);
    bs.set(121);
    bs.set(130, 140);

    {
        // pos1 and pos2 are in the same bitset element, at index0
        resultbs = bs.get(3, 6);
        BitSet correctbs(3);
        correctbs.set(0, 3);
        CPPUNIT_ASSERT_MESSAGE("Test1: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // pos1 and pos2 are in the same bitset element, at index 1
        resultbs = bs.get(100, 125);
        BitSet correctbs(25);
        correctbs.set(21);
        CPPUNIT_ASSERT_MESSAGE("Test2: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // pos1 in bitset element at index 0, and pos2 in bitset element at index 1
        resultbs = bs.get(15, 125);
        BitSet correctbs(25);
        correctbs.set(0, 5);
        correctbs.set(45, 60);
        correctbs.set(121 - 15);
        CPPUNIT_ASSERT_MESSAGE("Test3: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // pos1 in bitset element at index 1, and pos2 in bitset element at index 2
        resultbs = bs.get(70, 145);
        BitSet correctbs(75);
        correctbs.set(0, 5);
        correctbs.set(51);
        correctbs.set(60, 70);
        CPPUNIT_ASSERT_MESSAGE("Test4: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // pos1 in bitset element at index 0, and pos2 in bitset element at index 2
        resultbs = bs.get(5, 145);
        BitSet correctbs(140);
        correctbs.set(0, 4);
        correctbs.set(5, 15);
        correctbs.set(55, 70);
        correctbs.set(116);
        correctbs.set(125, 135);
        CPPUNIT_ASSERT_MESSAGE("Test5: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // pos1 in bitset element at index 0, and pos2 in bitset element at index 3
        resultbs = bs.get(5, 250);
        BitSet correctbs(200);
        correctbs.set(0, 4);
        correctbs.set(5, 15);
        correctbs.set(55, 70);
        correctbs.set(116);
        correctbs.set(125, 135);
        CPPUNIT_ASSERT_MESSAGE("Test5: Returned incorrect BitSet", correctbs.equals(resultbs));

        CPPUNIT_ASSERT_MESSAGE("equality principle 1 ", bs.get(0, bs.size()).equals(bs));
    }
    {
        // more tests
        BitSet bs2(129);
        bs2.set(0, 20);
        bs2.set(62, 65);
        bs2.set(121, 123);
        resultbs = bs2.get(1, 124);
        BitSet correctbs(129);
        correctbs.set(0, 19);
        correctbs.set(61, 64);
        correctbs.set(120, 122);
        CPPUNIT_ASSERT_MESSAGE("Test7: Returned incorrect BitSet", correctbs.equals(resultbs));
    }
    {
        // equality principle with some boundary conditions
        BitSet bs2(128);
        bs2.set(2, 20);
        bs2.set(62);
        bs2.set(121, 123);
        bs2.set(127);
        resultbs = bs2.get(0, bs2.size());
        CPPUNIT_ASSERT_MESSAGE("equality principle", bs2.equals(resultbs));
    }
    {
        BitSet bs2(128);
        bs2.set(2, 20);
        bs2.set(62);
        bs2.set(121, 123);
        bs2.set(127);
        bs2.flip(0, 128);
        resultbs = bs2.get(0, bs.size());
        CPPUNIT_ASSERT_MESSAGE("equality principle 3 ", resultbs.equals(bs2));
    }
    {
        BitSet bs(0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong size,", 0, bs.size());

        bs.get(0, 2);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong size,", 0, bs.size());

        bs.get(60, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size,", 0, bs.size());

        bs.get(64, 120);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong length,", 0, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test4: Wrong size,", 0, bs.size());

        bs.set(25);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong length,", 26, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test5: Wrong size,", 64, bs.size());

        bs.get(60, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong length,", 26, bs.length());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test6: Wrong size,", 64, bs.size());

        bs.get(64, 120);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test7: Wrong length,", 26, bs.length());

        bs.get(80);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test8: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test8: Wrong length,", 26, bs.length());

        bs.get(25);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test9: Wrong size,", 64, bs.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test9: Wrong length,", 26, bs.length());
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testFlipI() {
    BitSet bs;
    bs.clear(8);
    bs.clear(9);
    bs.set(10);
    bs.flip(9);
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", !bs.get(8));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", bs.get(9));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", bs.get(10));

    bs.set(8);
    bs.set(9);
    bs.clear(10);
    bs.flip(9);
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", bs.get(8));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", !bs.get(9));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", !bs.get(10));

    try {
        bs.flip(-1);
        CPPUNIT_FAIL("Attempt to flip at negative index CPPUNIT_FAILed to generate exception");
    } catch (IndexOutOfBoundsException& e) {
        // Correct behaviour
    }

    // Try setting a bit on a 64 boundary
    bs.flip(128);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to grow BitSet", 192, bs.size());
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit", bs.get(128));

    BitSet bs1(64);
    for (int i = bs1.size(); --i >= 0;) {
        bs1.flip(i);
        CPPUNIT_ASSERT_MESSAGE("Test1: Incorrectly flipped bit" + Integer::toString(i), bs1.get(i));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect length", i+1, bs1.length());
        for (int j = bs1.size(); --j > i;) {
            CPPUNIT_ASSERT_MESSAGE("Test2: Incorrectly flipped bit" + Integer::toString(j), !bs1.get(j));
        }
        for (int j = i; --j >= 0;) {
            CPPUNIT_ASSERT_MESSAGE("Test3: Incorrectly flipped bit" + Integer::toString(j), !bs1.get(j));
        }
        bs1.flip(i);
    }

    BitSet bs0(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong size", 0, bs0.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length", 0, bs0.length());

    bs0.flip(0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong size", 64, bs0.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length", 1, bs0.length());

    bs0.flip(63);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong size", 64, bs0.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test3: Wrong length", 64, bs0.length());

    eightbs.flip(7);
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 7", !eightbs.get(7));

    // Check to see all other bits are still set
    for (int i = 0; i < 7; i++) {
        CPPUNIT_ASSERT_MESSAGE("Flip flipped incorrect bits", eightbs.get(i));
    }

    eightbs.flip(127);
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 127", eightbs.get(127));

    eightbs.flip(127);
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 127", !eightbs.get(127));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testFlipII() {
    BitSet bitset;
    for (int i = 0; i < 20; i++) {
        bitset.set(i);
    }
    bitset.flip(10, 10);

    // Test for method void java.util.BitSet.flip(int, int)
    // pos1 and pos2 are in the same bitset element
    BitSet bs(16);
    bs.set(7);
    bs.set(10);
    bs.flip(7, 11);
    for (int i = 0; i < 7; i++) {
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + i, !bs.get(i));
    }
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 7", !bs.get(7));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 8", bs.get(8));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 9", bs.get(9));
    CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 10", !bs.get(10));
    for (int i = 11; i < bs.size(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
    }

    {
        // pos1 and pos2 is in the same bitset element, boundry testing
        BitSet bs(16);
        bs.set(7);
        bs.set(10);
        bs.flip(7, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to grow BitSet", 64, bs.size());
        for (int i = 0; i < 7; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 7", !bs.get(7));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 8", bs.get(8));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 9", bs.get(9));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 10", !bs.get(10));
        for (int i = 11; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 64", !bs.get(64));
    }
    {
        // more boundary testing
        BitSet bs(32);
        bs.flip(0, 64);
        for (int i = 0; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 64", !bs.get(64));
    }
    {
        BitSet bs(32);
        bs.flip(0, 65);
        for (int i = 0; i < 65; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 65", !bs.get(65));
    }
    {
        // pos1 and pos2 are in two sequential bitset elements
        BitSet bs(128);
        bs.set(7);
        bs.set(10);
        bs.set(72);
        bs.set(110);
        bs.flip(9, 74);
        for (int i = 0; i < 7; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 7", bs.get(7));
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 8", !bs.get(8));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 9", bs.get(9));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 10", !bs.get(10));
        for (int i = 11; i < 72; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 72", !bs.get(72));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 73", bs.get(73));
        for (int i = 74; i < 110; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 110", bs.get(110));
        for (int i = 111; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        // pos1 and pos2 are in two non-sequential bitset elements
        BitSet bs(256);
        bs.set(7);
        bs.set(10);
        bs.set(72);
        bs.set(110);
        bs.set(181);
        bs.set(220);
        bs.flip(9, 219);
        for (int i = 0; i < 7; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 7", bs.get(7));
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 8", !bs.get(8));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 9", bs.get(9));
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 10", !bs.get(10));
        for (int i = 11; i < 72; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 72", !bs.get(72));
        for (int i = 73; i < 110; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 110", !bs.get(110));
        for (int i = 111; i < 181; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Failed to flip bit 181", !bs.get(181));
        for (int i = 182; i < 219; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to flip bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 219", !bs.get(219));
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 220", bs.get(220));
        for (int i = 221; i < bs.size(); i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), !bs.get(i));
        }
    }
    {
        // test illegal args
        BitSet bs(10);
        try {
            bs.flip(-1, 3);
            CPPUNIT_FAIL("Test1: Attempt to flip with  negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // correct behavior
        }

        try {
            bs.flip(2, -1);
            CPPUNIT_FAIL("Test2: Attempt to flip with negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // correct behavior
        }

        try {
            bs.flip(4, 2);
            CPPUNIT_FAIL("Test4: Attempt to flip with illegal args CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // correct behavior
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testSetI() {

    BitSet bs;
    bs.set(8);
    CPPUNIT_ASSERT_MESSAGE("Failed to set bit", bs.get(8));

    try {
        bs.set(-1);
        CPPUNIT_FAIL("Attempt to set at negative index CPPUNIT_FAILed to generate exception");
    } catch (IndexOutOfBoundsException& e) {
        // Correct behavior
    }

    {
        // Try setting a bit on a 64 boundary
        bs.set(128);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to grow BitSet", 192, bs.size());
        CPPUNIT_ASSERT_MESSAGE("Failed to set bit", bs.get(128));
    }
    {
        BitSet bs(64);
        for (int i = bs.size(); --i >= 0;) {
            bs.set(i);
            CPPUNIT_ASSERT_MESSAGE("Incorrectly set", bs.get(i));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect length", i+1, bs.length());
            for (int j = bs.size(); --j > i;)
                CPPUNIT_ASSERT_MESSAGE("Incorrectly set bit " + Integer::toString(j), !bs.get(j));
            for (int j = i; --j >= 0;)
                CPPUNIT_ASSERT_MESSAGE("Incorrectly set bit " + Integer::toString(j), !bs.get(j));
            bs.clear(i);
        }
    }
    {
        BitSet bs(0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test1: Wrong length", 0, bs.length());
        bs.set(0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test2: Wrong length", 1, bs.length());
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testSetIB() {
    eightbs.set(5, false);
    CPPUNIT_ASSERT_MESSAGE("Should have set bit 5 to true", !eightbs.get(5));

    eightbs.set(5, true);
    CPPUNIT_ASSERT_MESSAGE("Should have set bit 5 to false", eightbs.get(5));

    try {
        BitSet bs;
        bs.set(-2147483648, false);
        CPPUNIT_FAIL("Should throw IndexOutOfBoundsException");
    } catch (IndexOutOfBoundsException& e) {
        // expected
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testSetII() {

    BitSet bitset(30);
    bitset.set(29, 29);

    {
        // pos1 and pos2 are in the same bitset element
        BitSet bs(16);
        bs.set(5);
        bs.set(15);
        bs.set(7, 11);
        for (int i = 0; i < 7; i++) {
            if (i == 5) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit " + Integer::toString(i), !bs.get(i));
            }
        }
        for (int i = 7; i < 11; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to set bit " + Integer::toString(i), bs.get(i));
        }
        for (int i = 11; i < bs.size(); i++) {
            if (i == 15) {
                CPPUNIT_ASSERT_MESSAGE(
                    std::string("Shouldn't have flipped bit ") + Integer::toString(i), bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE(
                    std::string("Shouldn't have set bit ") + Integer::toString(i), !bs.get(i));
            }
        }
    }
    {
        // pos1 and pos2 is in the same bitset element, boundary testing
        BitSet bs(16);
        bs.set(7, 64);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to grow BitSet", 64, bs.size());
        for (int i = 0; i < 7; i++) {
            CPPUNIT_ASSERT_MESSAGE(
                std::string("Shouldn't have set bit ") + Integer::toString(i), !bs.get(i));
        }
        for (int i = 7; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE(
                std::string("Failed to set bit ") + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit 64", !bs.get(64));
    }
    {
        // more boundary testing
        BitSet bs(32);
        bs.set(0, 64);
        for (int i = 0; i < 64; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to set bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit 64", !bs.get(64));
    }
    {
        BitSet bs(32);
        bs.set(0, 65);
        for (int i = 0; i < 65; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to set bit " + Integer::toString(i), bs.get(i));
        }
        CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit 65", !bs.get(65));
    }
    {
        // pos1 and pos2 are in two sequential bitset elements
        BitSet bs(128);
        bs.set(7);
        bs.set(110);
        bs.set(9, 74);
        for (int i = 0; i < 9; i++) {
            if (i == 7) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit " + Integer::toString(i), !bs.get(i));
            }
        }
        for (int i = 9; i < 74; i++) {
            CPPUNIT_ASSERT_MESSAGE("Failed to set bit " + Integer::toString(i), bs.get(i));
        }
        for (int i = 74; i < bs.size(); i++) {
            if (i == 110) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit " + Integer::toString(i), bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit " + Integer::toString(i), !bs.get(i));
            }
        }
    }
    {
        // pos1 and pos2 are in two non-sequential bitset elements
        BitSet bs(256);
        bs.set(7);
        bs.set(255);
        bs.set(9, 219);
        for (int i = 0; i < 9; i++) {
            if (i == 7) {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have set flipped " + Integer::toString(i), bs.get(i));
            } else {
                CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit " + Integer::toString(i), !bs.get(i));
            }
        }

        for (int i = 9; i < 219; i++) {
            CPPUNIT_ASSERT_MESSAGE("CPPUNIT_FAILed to set bit " + Integer::toString(i), bs.get(i));
        }

        for (int i = 219; i < 255; i++) {
            CPPUNIT_ASSERT_MESSAGE("Shouldn't have set bit " + Integer::toString(i), !bs.get(i));
        }

        CPPUNIT_ASSERT_MESSAGE("Shouldn't have flipped bit 255", bs.get(255));
    }
    {
        // test illegal args
        BitSet bs(10);
        try {
            bs.set(-1, 3);
            CPPUNIT_FAIL("Test1: Attempt to flip with  negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // Correct behavior
        }

        try {
            bs.set(2, -1);
            CPPUNIT_FAIL("Test2: Attempt to flip with negative index CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // Correct behavior
        }

        bs.set(2, 2);
        CPPUNIT_ASSERT_MESSAGE("Bit got set incorrectly ", !bs.get(2));

        try {
            bs.set(4, 2);
            CPPUNIT_FAIL("Test4: Attempt to flip with illegal args CPPUNIT_FAILed to generate exception");
        } catch (IndexOutOfBoundsException& e) {
            // Correct behavior
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testSetIIB() {
    eightbs.set(3, 6, false);
    CPPUNIT_ASSERT_MESSAGE("Should have set bits 3, 4, and 5 to false",
                           !eightbs.get(3) && !eightbs.get(4) && !eightbs.get(5));

    eightbs.set(3, 6, true);
    CPPUNIT_ASSERT_MESSAGE("Should have set bits 3, 4, and 5 to true",
                           eightbs.get(3) && eightbs.get(4) && eightbs.get(5));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testNotModified() {
    // BitSet shouldn't be modified by any of the operations below,
    // since the affected bits for these methods are defined as inclusive of
    // pos1, exclusive of pos2.
    try {
        eightbs.flip(0, 0);
        CPPUNIT_ASSERT_MESSAGE("Bit got flipped incorrectly ", eightbs.get(0));

        BitSet bsnew = eightbs.get(2, 2);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet retrieved incorrectly ", 0, bsnew.cardinality());

        eightbs.set(10, 10);
        CPPUNIT_ASSERT_MESSAGE("Bit got set incorrectly ", !eightbs.get(10));

        eightbs.clear(3, 3);
        CPPUNIT_ASSERT_MESSAGE("Bit cleared incorrectly ", eightbs.get(3));
    } catch (IndexOutOfBoundsException& e) {
        CPPUNIT_FAIL("Unexpected IndexOutOfBoundsException when pos1 ==pos2");
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testIntersects() {
    BitSet bs(500);
    bs.set(5);
    bs.set(63);
    bs.set(64);
    bs.set(71, 110);
    bs.set(127, 130);
    bs.set(192);
    bs.set(450);

    BitSet bs2(8);
    CPPUNIT_ASSERT_MESSAGE("Test1: intersects() returned incorrect value", !bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test1: intersects() returned incorrect value", !bs2.intersects(bs));

    bs2.set(4);
    CPPUNIT_ASSERT_MESSAGE("Test2: intersects() returned incorrect value", !bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test2: intersects() returned incorrect value", !bs2.intersects(bs));

    bs2.clear();
    bs2.set(5);
    CPPUNIT_ASSERT_MESSAGE("Test3: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test3: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(63);
    CPPUNIT_ASSERT_MESSAGE("Test4: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test4: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(80);
    CPPUNIT_ASSERT_MESSAGE("Test5: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test5: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(127);
    CPPUNIT_ASSERT_MESSAGE("Test6: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test6: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(192);
    CPPUNIT_ASSERT_MESSAGE("Test7: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test7: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(450);
    CPPUNIT_ASSERT_MESSAGE("Test8: intersects() returned incorrect value", bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test8: intersects() returned incorrect value", bs2.intersects(bs));

    bs2.clear();
    bs2.set(500);
    CPPUNIT_ASSERT_MESSAGE("Test9: intersects() returned incorrect value", !bs.intersects(bs2));
    CPPUNIT_ASSERT_MESSAGE("Test9: intersects() returned incorrect value", !bs2.intersects(bs));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testAnd() {
    BitSet bs(128);

    // Initialize the bottom half of the BitSet
    for (int i = 64; i < 128; i++) {
        bs.set(i);
    }

    eightbs.AND(bs);
    CPPUNIT_ASSERT_MESSAGE("AND CPPUNIT_FAILed to clear bits", !eightbs.equals(bs));
    eightbs.set(3);
    bs.set(3);
    eightbs.AND(bs);
    CPPUNIT_ASSERT_MESSAGE("AND CPPUNIT_FAILed to maintain set bits", bs.get(3));
    bs.AND(eightbs);

    for (int i = 64; i < 128; i++) {
        CPPUNIT_ASSERT_MESSAGE("Failed to clear extra bits in the receiver BitSet", !bs.get(i));
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testAndNot() {

    BitSet bs = eightbs;
    bs.clear(5);
    BitSet bs2;
    bs2.set(2);
    bs2.set(3);
    bs.andNot(bs2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect bitset after andNot",
                                 std::string("{0, 1, 4, 6, 7}"), bs.toString());

    BitSet bs3(0);
    bs3.andNot(bs2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect size", 0, bs3.size());

    {
        BitSet bs(256);
        BitSet bs2(256);
        bs.set(97);
        bs2.set(37);
        bs.andNot(bs2);
        CPPUNIT_ASSERT_MESSAGE("Incorrect value at 97 pos", bs.get(97));
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testOR() {
    {
        BitSet bs(128);
        bs.OR(eightbs);
        for (int i = 0; i < 8; i++) {
            CPPUNIT_ASSERT_MESSAGE("OR CPPUNIT_FAILed to set bits", bs.get(i));
        }
    }
    {
        BitSet bs(0);
        bs.OR(eightbs);
        for (int i = 0; i < 8; i++) {
            CPPUNIT_ASSERT_MESSAGE("OR(0) CPPUNIT_FAILed to set bits", bs.get(i));
        }
    }
    {
        eightbs.clear(5);
        BitSet bs(128);
        bs.OR(eightbs);
        CPPUNIT_ASSERT_MESSAGE("OR set a bit which should be off", !bs.get(5));
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testXOR() {

    {
        BitSet bs = eightbs;
        bs.XOR(eightbs);
        for (int i = 0; i < 8; i++) {
            CPPUNIT_ASSERT_MESSAGE("XOR CPPUNIT_FAILed to clear bits", !bs.get(i));
        }

        bs.XOR(eightbs);
        for (int i = 0; i < 8; i++) {
            CPPUNIT_ASSERT_MESSAGE("XOR CPPUNIT_FAILed to set bits", bs.get(i));
        }
    }
    {
        BitSet bs(0);
        bs.XOR(eightbs);
        for (int i = 0; i < 8; i++) {
            CPPUNIT_ASSERT_MESSAGE("XOR(0) CPPUNIT_FAILed to set bits", bs.get(i));
        }
    }
    {
        BitSet bs;
        bs.set(63);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Test highest bit", std::string("{63}"), bs.toString());
    }
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testSize() {
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect size", 64, eightbs.size());
    eightbs.set(129);
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect size", eightbs.size() >= 129);
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testToString() {
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect string representation",
                                 std::string("{0, 1, 2, 3, 4, 5, 6, 7}"), eightbs.toString());
    eightbs.clear(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect string representation",
                                 std::string("{0, 1, 3, 4, 5, 6, 7}"), eightbs.toString());
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testLength() {
    BitSet bs;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet returned wrong length", 0, bs.length());
    bs.set(5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet returned wrong length", 6, bs.length());
    bs.set(10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet returned wrong length", 11, bs.length());
    bs.set(432);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet returned wrong length", 433, bs.length());
    bs.set(300);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("BitSet returned wrong length", 433, bs.length());
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testNextSetBitI() {
    BitSet bs(500);
    bs.set(5);
    bs.set(32);
    bs.set(63);
    bs.set(64);
    bs.set(71, 110);
    bs.set(127, 130);
    bs.set(193);
    bs.set(450);
    try {
        bs.nextSetBit(-1);
        CPPUNIT_FAIL("Expected IndexOutOfBoundsException for negative index");
    } catch (IndexOutOfBoundsException& e) {
        // correct behavior
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 5, bs.nextSetBit(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 5, bs.nextSetBit(5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 32, bs.nextSetBit(6));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 32, bs.nextSetBit(32));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 63, bs.nextSetBit(33));

    // boundary tests
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 63, bs.nextSetBit(63));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 64, bs.nextSetBit(64));

    // at bitset element 1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 71, bs.nextSetBit(65));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 71, bs.nextSetBit(71));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 72, bs.nextSetBit(72));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 127, bs.nextSetBit(110));

    // boundary tests
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 127, bs.nextSetBit(127));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 128, bs.nextSetBit(128));

    // at bitset element 2
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 193, bs.nextSetBit(130));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 193, bs.nextSetBit(191));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 193, bs.nextSetBit(192));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 193, bs.nextSetBit(193));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 450, bs.nextSetBit(194));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 450, bs.nextSetBit(255));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 450, bs.nextSetBit(256));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", 450, bs.nextSetBit(450));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", -1, bs.nextSetBit(451));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", -1, bs.nextSetBit(511));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", -1, bs.nextSetBit(512));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextSetBit() returned the wrong value", -1, bs.nextSetBit(800));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testNextClearBitI() {
    BitSet bs(500);

    // ensure all the bits from 0 to bs.size() - 1 are set to true
    bs.set(0, bs.size() - 1);
    bs.set(bs.size() - 1);
    bs.clear(5);
    bs.clear(32);
    bs.clear(63);
    bs.clear(64);
    bs.clear(71, 110);
    bs.clear(127, 130);
    bs.clear(193);
    bs.clear(450);
    try {
        bs.nextClearBit(-1);
        CPPUNIT_FAIL("Expected IndexOutOfBoundsException for negative index");
    } catch (IndexOutOfBoundsException& e) {
        // correct behavior
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 5, bs.nextClearBit(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 5, bs.nextClearBit(5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 32, bs.nextClearBit(6));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 32, bs.nextClearBit(32));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 63, bs.nextClearBit(33));

    // boundary tests
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 63, bs .nextClearBit(63));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 64, bs .nextClearBit(64));

    // at bitset element 1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 71, bs .nextClearBit(65));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 71, bs .nextClearBit(71));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 72, bs .nextClearBit(72));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 127, bs .nextClearBit(110));

    // boundary tests
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 127, bs .nextClearBit(127));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 128, bs .nextClearBit(128));

    // at bitset element 2
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 193, bs .nextClearBit(130));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 193, bs .nextClearBit(191));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 193, bs .nextClearBit(192));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 193, bs .nextClearBit(193));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 450, bs .nextClearBit(194));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 450, bs .nextClearBit(255));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 450, bs .nextClearBit(256));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 450, bs .nextClearBit(450));

    // bitset has 1 still the end of bs.size() -1, but calling nextClearBit
    // with any index value after the last true bit should return bs.size()
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 512, bs .nextClearBit(451));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 512, bs .nextClearBit(511));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 512, bs .nextClearBit(512));

    // if the index is larger than bs.size(), nextClearBit should return index
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 513, bs .nextClearBit(513));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("nextClearBit() returned the wrong value", 800, bs .nextClearBit(800));
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testIsEmpty() {
    BitSet bs(500);
    CPPUNIT_ASSERT_MESSAGE("Test: isEmpty() returned wrong value", bs.isEmpty());

    // at bitset element 0
    bs.set(3);
    CPPUNIT_ASSERT_MESSAGE("Test0: isEmpty() returned wrong value", !bs.isEmpty());

    // at bitset element 1
    bs.clear();
    bs.set(12);
    CPPUNIT_ASSERT_MESSAGE("Test1: isEmpty() returned wrong value", !bs.isEmpty());

    // at bitset element 2
    bs.clear();
    bs.set(128);
    CPPUNIT_ASSERT_MESSAGE("Test2: isEmpty() returned wrong value", !bs.isEmpty());

    // boundary testing
    bs.clear();
    bs.set(459);
    CPPUNIT_ASSERT_MESSAGE("Test3: isEmpty() returned wrong value", !bs.isEmpty());

    bs.clear();
    bs.set(511);
    CPPUNIT_ASSERT_MESSAGE("Test4: isEmpty() returned wrong value", !bs.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void BitSetTest::testCardinality() {

    BitSet bs(500);
    bs.set(5);
    bs.set(32);
    bs.set(63);
    bs.set(64);
    bs.set(71, 110);
    bs.set(127, 130);
    bs.set(193);
    bs.set(450);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cardinality() returned wrong value", 48, bs.cardinality());

    bs.flip(0, 500);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cardinality() returned wrong value", 452, bs.cardinality());

    bs.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cardinality() returned wrong value", 0, bs.cardinality());

    bs.set(0, 500);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cardinality() returned wrong value", 500, bs.cardinality());

    bs.clear();
    bs.set(0, 64);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cardinality() returned wrong value", 64, bs.cardinality());
}
