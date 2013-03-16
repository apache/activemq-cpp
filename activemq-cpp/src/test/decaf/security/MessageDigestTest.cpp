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

#include "MessageDigestTest.h"

#include <decaf/lang/Pointer.h>
#include <decaf/lang/Integer.h>
#include <decaf/security/MessageDigest.h>
#include <decaf/security/Provider.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/security/NoSuchAlgorithmException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::security;

////////////////////////////////////////////////////////////////////////////////
MessageDigestTest::MessageDigestTest() {
}

////////////////////////////////////////////////////////////////////////////////
MessageDigestTest::~MessageDigestTest() {
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testGetInstance1() {

    MessageDigest* digest = MessageDigest::getInstance("MD5");

    CPPUNIT_ASSERT(digest != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("MD5"), digest->getAlgorithmName());
    CPPUNIT_ASSERT_EQUAL(std::string("Decaf"), digest->getProvider()->getName());

    delete digest;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testGetInstance2() {

    MessageDigest* digest = MessageDigest::getInstance("MD4");

    CPPUNIT_ASSERT(digest != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("MD4"), digest->getAlgorithmName());
    CPPUNIT_ASSERT_EQUAL(std::string("Decaf"), digest->getProvider()->getName());

    delete digest;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testGetInstance3() {

    MessageDigest* digest = MessageDigest::getInstance("SHA1");

    CPPUNIT_ASSERT(digest != NULL);
    CPPUNIT_ASSERT_EQUAL(std::string("SHA1"), digest->getAlgorithmName());
    CPPUNIT_ASSERT_EQUAL(std::string("Decaf"), digest->getProvider()->getName());

    delete digest;
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testGetInstance4() {
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a NoSuchAlgorithmException",
        MessageDigest::getInstance("MD3"),
        NoSuchAlgorithmException);
}

////////////////////////////////////////////////////////////////////////////////
namespace {

    std::string toString(const std::vector<unsigned char>& digest) {
        std::string result;
        std::vector<unsigned char>::const_iterator iter = digest.begin();
        std::string pad = "00";

        for (; iter != digest.end(); ++iter) {
            std::string hex = Integer::toHexString(*iter & 255);
            result.append(pad.substr(hex.length())).append(hex);
        }
        return result;
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testResults1() {

    Pointer<MessageDigest> digest(MessageDigest::getInstance("MD5"));

    std::string bytes;
    std::vector<unsigned char> result;

    bytes = "";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("d41d8cd98f00b204e9800998ecf8427e"), toString(result));

    bytes = "a";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("0cc175b9c0f1b6a831c399e269772661"), toString(result));

    bytes = "abc";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("900150983cd24fb0d6963f7d28e17f72"), toString(result));

    bytes = "message digest";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("f96b697d7cb7938d525a2f31aaf161d0"), toString(result));

    bytes = "abcdefghijklmnopqrstuvwxyz";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("c3fcd3d76192e4007dfb496cca67e13b"), toString(result));

    bytes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("d174ab98d277d9f5a5611c2c9f419d9f"), toString(result));

    bytes = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("57edf4a22be3c955ac49da2e2107b67a"), toString(result));
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testResults2() {

    Pointer<MessageDigest> digest(MessageDigest::getInstance("MD4"));

    std::string bytes;
    std::vector<unsigned char> result;

    bytes = "";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("31d6cfe0d16ae931b73c59d7e0c089c0"), toString(result));

    bytes = "a";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("bde52cb31de33e46245e05fbdbd6fb24"), toString(result));

    bytes = "abc";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("a448017aaf21d8525fc10ae87aa6729d"), toString(result));

    bytes = "message digest";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("d9130a8164549fe818874806e1c7014b"), toString(result));

    bytes = "abcdefghijklmnopqrstuvwxyz";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("d79e1c308aa5bbcdeea8ed63df412da9"), toString(result));

    bytes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("043f8582f241db351ce627e153e7f0e4"), toString(result));

    bytes = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
    CPPUNIT_ASSERT_EQUAL(std::string("e33b4ddc9c38f2199c3e7b164fcc0536"), toString(result));
}

////////////////////////////////////////////////////////////////////////////////
void MessageDigestTest::testResults3() {

    Pointer<MessageDigest> digest(MessageDigest::getInstance("SHA1"));

    std::string bytes;
    std::vector<unsigned char> result;

// TODO SHA1 not quite right yet
//    bytes = "";
//    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
//    CPPUNIT_ASSERT_EQUAL(std::string("da39a3ee5e6b4b0d3255bfef95601890afd80709"), toString(result));
//
//    bytes = "The quick brown fox jumps over the lazy dog";
//    result = digest->digest((const unsigned char*)bytes.data(), (int)bytes.size());
//    CPPUNIT_ASSERT_EQUAL(std::string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"), toString(result));
}
