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

#include "UUIDTest.h"

#include <decaf/lang/Long.h>
#include <decaf/util/UUID.h>
#include <decaf/lang/exceptions/NumberFormatException.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/NumberFormatException.h>

using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
UUIDTest::UUIDTest() {
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testConstructor1() {
    UUID uuid(0xF81D4FAE7DEC11D0LL, 0xA76500A0C91E6BF6LL);
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    CPPUNIT_ASSERT_EQUAL(1, uuid.version());
    CPPUNIT_ASSERT_EQUAL(0x1d07decf81d4faeLL, uuid.timestamp());
    CPPUNIT_ASSERT_EQUAL(130742845922168750LL, uuid.timestamp());
    CPPUNIT_ASSERT_EQUAL(0x2765, uuid.clockSequence());
    CPPUNIT_ASSERT_EQUAL(0xA0C91E6BF6LL, uuid.node());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testGetLeastSignificantBits() {
    UUID uuid(0, 0);
    CPPUNIT_ASSERT_EQUAL(0LL, uuid.getLeastSignificantBits());
    uuid = UUID(0, Long::MIN_VALUE);
    CPPUNIT_ASSERT_EQUAL(Long::MIN_VALUE, uuid.getLeastSignificantBits());
    uuid = UUID(0, Long::MAX_VALUE);
    CPPUNIT_ASSERT_EQUAL(Long::MAX_VALUE, uuid.getLeastSignificantBits());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testGetMostSignificantBits() {
    UUID uuid(0, 0);
    CPPUNIT_ASSERT_EQUAL(0LL, uuid.getMostSignificantBits());
    uuid = UUID(Long::MIN_VALUE, 0LL);
    CPPUNIT_ASSERT_EQUAL(Long::MIN_VALUE, uuid.getMostSignificantBits());
    uuid = UUID(Long::MAX_VALUE, 0LL);
    CPPUNIT_ASSERT_EQUAL(Long::MAX_VALUE, uuid.getMostSignificantBits());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testVersion() {
    UUID uuid(0, 0);
    CPPUNIT_ASSERT_EQUAL(0, uuid.version());
    uuid = UUID(0x0000000000001000LL, 0);
    CPPUNIT_ASSERT_EQUAL(1, uuid.version());
    uuid = UUID(0x0000000000002000LL, 0);
    CPPUNIT_ASSERT_EQUAL(2, uuid.version());
    uuid = UUID(0x0000000000003000LL, 0);
    CPPUNIT_ASSERT_EQUAL(3, uuid.version());
    uuid = UUID(0x0000000000004000LL, 0);
    CPPUNIT_ASSERT_EQUAL(4, uuid.version());
    uuid = UUID(0x0000000000005000LL, 0);
    CPPUNIT_ASSERT_EQUAL(5, uuid.version());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testVariant() {
    UUID uuid(0, 0x0000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0, uuid.variant());
    uuid = UUID(0, 0x7000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0, uuid.variant());
    uuid = UUID(0, 0x3000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0, uuid.variant());
    uuid = UUID(0, 0x1000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0, uuid.variant());

    uuid = UUID(0, 0x8000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    uuid = UUID(0, 0xB000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    uuid = UUID(0, 0xA000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    uuid = UUID(0, 0x9000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());

    uuid = UUID(0, 0xC000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(6, uuid.variant());
    uuid = UUID(0, 0xD000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(6, uuid.variant());

    uuid = UUID(0, 0xE000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(7, uuid.variant());
    uuid = UUID(0, 0xF000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(7, uuid.variant());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testTimestamp() {
    UUID uuid(0x0000000000001000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.timestamp());

    uuid = UUID(0x7777777755551333LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0x333555577777777LL, uuid.timestamp());

    uuid = UUID(0x0000000000000000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.timestamp(),
        UnsupportedOperationException);

    uuid = UUID(0x0000000000002000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.timestamp(),
        UnsupportedOperationException);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testClockSequence() {
    UUID uuid(0x0000000000001000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0x0, uuid.clockSequence());

    uuid = UUID(0x0000000000001000L, 0x8FFF000000000000L);
    CPPUNIT_ASSERT_EQUAL(0x0FFF, uuid.clockSequence());

    uuid = UUID(0x0000000000001000LL, 0xBFFF000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0x3FFF, uuid.clockSequence());

    uuid = UUID(0x0000000000000000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.clockSequence(),
        UnsupportedOperationException);

    uuid = UUID(0x0000000000002000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.clockSequence(),
        UnsupportedOperationException);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testNode() {
    UUID uuid(0x0000000000001000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.node());

    uuid = UUID(0x0000000000001000LL, 0x8000FFFFFFFFFFFFLL);
    CPPUNIT_ASSERT_EQUAL(0xFFFFFFFFFFFFLL, uuid.node());

    uuid = UUID(0x0000000000000000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.node(),
        UnsupportedOperationException);

    uuid = UUID(0x0000000000002000LL, 0x8000000000000000LL);
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an UnsupportedOperationException exception",
        uuid.node(),
        UnsupportedOperationException);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testCompareTo() {
    UUID uuid1(0, 0);
    CPPUNIT_ASSERT_EQUAL(0, uuid1.compareTo(uuid1));
    UUID uuid2(1, 0);
    CPPUNIT_ASSERT_EQUAL(-1, uuid1.compareTo(uuid2));
    CPPUNIT_ASSERT_EQUAL(1, uuid2.compareTo(uuid1));

    uuid2 = UUID(0, 1);
    CPPUNIT_ASSERT_EQUAL(-1, uuid1.compareTo(uuid2));
    CPPUNIT_ASSERT_EQUAL(1, uuid2.compareTo(uuid1));
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testHashCode() {
    UUID uuid(0, 0);
    CPPUNIT_ASSERT_EQUAL(0, uuid.hashCode());
    uuid = UUID(123, 123);
    UUID uuidClone(123, 123);
    CPPUNIT_ASSERT_EQUAL(uuid.hashCode(), uuidClone.hashCode());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testEquals() {
    UUID uuid1(0, 0);
    CPPUNIT_ASSERT(uuid1.equals(uuid1));

    UUID uuid2(0, 0);
    CPPUNIT_ASSERT(uuid1.equals(uuid2));
    CPPUNIT_ASSERT(uuid2.equals(uuid1));

    uuid1 = UUID(0xf81d4fae7dec11d0LL, 0xa76500a0c91e6bf6LL);
    uuid2 = UUID(0xf81d4fae7dec11d0LL, 0xa76500a0c91e6bf6LL);
    CPPUNIT_ASSERT(uuid1.equals(uuid2));
    CPPUNIT_ASSERT(uuid2.equals(uuid1));

    uuid2 = UUID(0xf81d4fae7dec11d0LL, 0xa76500a0c91e6bf7LL);
    CPPUNIT_ASSERT(!uuid1.equals(uuid2));
    CPPUNIT_ASSERT(!uuid2.equals(uuid1));
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testToString() {
    UUID uuid(0xf81d4fae7dec11d0LL, 0xa76500a0c91e6bf6LL);
    std::string actual = uuid.toString();
    CPPUNIT_ASSERT_EQUAL(std::string("f81d4fae-7dec-11d0-a765-00a0c91e6bf6"), actual);

    uuid = UUID(0x0000000000001000L, 0x8000000000000000L);
    actual = uuid.toString();
    CPPUNIT_ASSERT_EQUAL(std::string("00000000-0000-1000-8000-000000000000"), actual);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testRandomUUID() {
    UUID uuid = UUID::randomUUID();
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    CPPUNIT_ASSERT_EQUAL(4, uuid.version());
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testNameUUIDFromBytes() {
    char name[16] = {
        (char) 0x6b, (char) 0xa7, (char) 0xb8, (char) 0x11,
        (char) 0x9d, (char) 0xad, (char) 0x11, (char) 0xd1,
        (char) 0x80, (char) 0xb4, (char) 0x00, (char) 0xc0,
        (char) 0x4f, (char) 0xd4, (char) 0x30, (char) 0xc8 };

    UUID uuid = UUID::nameUUIDFromBytes(&name[0], 16);

    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    CPPUNIT_ASSERT_EQUAL(3, uuid.version());

    CPPUNIT_ASSERT_EQUAL(0xaff565bc2f771745ULL, (unsigned long long) uuid.getLeastSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0x14cdb9b4de013faaLL, uuid.getMostSignificantBits());

    uuid = UUID::nameUUIDFromBytes(std::vector<char>());
    CPPUNIT_ASSERT_EQUAL(2, uuid.variant());
    CPPUNIT_ASSERT_EQUAL(3, uuid.version());

    CPPUNIT_ASSERT_EQUAL(0xa9800998ecf8427eULL, (unsigned long long) uuid.getLeastSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0xd41d8cd98f003204ULL, (unsigned long long) uuid.getMostSignificantBits());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an NullPointerException exception",
        UUID::nameUUIDFromBytes(NULL, 1),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testFromString() {
    UUID actual = UUID::fromString("f81d4fae-7dec-11d0-a765-00a0c91e6bf6");
    UUID expected = UUID(0xf81d4fae7dec11d0LL, 0xa76500a0c91e6bf6LL);
    CPPUNIT_ASSERT(expected.equals(actual));

    CPPUNIT_ASSERT_EQUAL(2, actual.variant());
    CPPUNIT_ASSERT_EQUAL(1, actual.version());
    CPPUNIT_ASSERT_EQUAL(130742845922168750LL, actual.timestamp());
    CPPUNIT_ASSERT_EQUAL(10085, actual.clockSequence());
    CPPUNIT_ASSERT_EQUAL(690568981494LL, actual.node());

    actual = UUID::fromString("00000000-0000-1000-8000-000000000000");
    expected = UUID(0x0000000000001000LL, 0x8000000000000000L);
    CPPUNIT_ASSERT(expected.equals(actual));

    CPPUNIT_ASSERT_EQUAL(2, actual.variant());
    CPPUNIT_ASSERT_EQUAL(1, actual.version());
    CPPUNIT_ASSERT_EQUAL(0LL, actual.timestamp());
    CPPUNIT_ASSERT_EQUAL(0, actual.clockSequence());
    CPPUNIT_ASSERT_EQUAL(0LL, actual.node());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString(""),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("f81d4fae_7dec-11d0-a765-00a0c91e6bf6"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("f81d4fae-7dec_11d0-a765-00a0c91e6bf6"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("f81d4fae-7dec-11d0_a765-00a0c91e6bf6"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("f81d4fae-7dec-11d0-a765_00a0c91e6bf6"),
        IllegalArgumentException);
}

////////////////////////////////////////////////////////////////////////////////
void UUIDTest::testFromStringStringException() {

    UUID uuid = UUID::fromString("0-0-0-0-0");

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("0-0-0-0-"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("00000"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("----"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("-0-0-0-0-0"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("-0-0-0-0"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("-0-0-0-"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("0--0-0-0"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("0-0-0-0-"),
        IllegalArgumentException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("-1-0-0-0-0"),
        IllegalArgumentException);

    uuid = UUID::fromString("123456789-0-0-0-0");
    CPPUNIT_ASSERT_EQUAL(0x2345678900000000LL, uuid.getMostSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.getLeastSignificantBits());

    uuid = UUID::fromString("111123456789-0-0-0-0");
    CPPUNIT_ASSERT_EQUAL(0x2345678900000000LL, uuid.getMostSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.getLeastSignificantBits());

    uuid = UUID::fromString("7fffffffffffffff-0-0-0-0");
    CPPUNIT_ASSERT_EQUAL(0xffffffff00000000ULL, (unsigned long long) uuid.getMostSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.getLeastSignificantBits());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("8000000000000000-0-0-0-0"),
        NumberFormatException);

    uuid = UUID::fromString("7fffffffffffffff-7fffffffffffffff-7fffffffffffffff-0-0");
    CPPUNIT_ASSERT_EQUAL(0xffffffffffffffffULL, (unsigned long long) uuid.getMostSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.getLeastSignificantBits());

    uuid = UUID::fromString("0-0-0-7fffffffffffffff-7fffffffffffffff");
    CPPUNIT_ASSERT_EQUAL(0x0LL, uuid.getMostSignificantBits());
    CPPUNIT_ASSERT_EQUAL(0xffffffffffffffffULL, (unsigned long long) uuid.getLeastSignificantBits());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("0-0-0-8000000000000000-0"),
        NumberFormatException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw an IllegalArgumentException exception",
        UUID::fromString("0-0-0-0-8000000000000000"),
        NumberFormatException);
}
