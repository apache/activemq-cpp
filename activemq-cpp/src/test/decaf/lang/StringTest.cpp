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

#include "StringTest.h"

#include <decaf/lang/String.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>

#include <decaf/util/Arrays.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StringTest::StringTest() {
}

////////////////////////////////////////////////////////////////////////////////
StringTest::~StringTest() {
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testDefaultConstructor() {
    String test;
    CPPUNIT_ASSERT_MESSAGE("Default string should equal empty", test == "");

    CPPUNIT_ASSERT(test.length() == 0);
    CPPUNIT_ASSERT(test.isEmpty() == true);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(1),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorStdString() {

    std::string stdString("ABCDE");

    String test(stdString);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and std::string should be equal", test == stdString);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorCString() {

    const char* cstring("ABCDE");

    String test(cstring);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and C string should be equal", test == cstring);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NullPointerException",
        String((const char*)NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorCStringWithSize() {

    const char* cstring("ABCDEF");
    const char* expected("ABCDE");

    String test(cstring, 5);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and C string should be equal", test == expected);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NullPointerException",
        String((const char*)NULL, 10),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, -1),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorCStringOffsetAndLength() {

    const char* cstring("1ABCDEF");
    const char* expected("ABCDE");

    String test(cstring, 1, 5);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and C string should be equal", test == expected);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NullPointerException",
        String((const char*)NULL, 1, 20),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, -1, 5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, 1, -5),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorCStringSizeOffsetAndLength() {

    const char* cstring("1ABCDEF");
    const char* expected("ABCDE");

    String test(cstring, 7, 1, 5);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and C string should be equal", test == expected);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an NullPointerException",
        String((const char*)NULL, 7, 1, 4),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, -1, 0, 5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, 7, -1, 5),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String(cstring, 7, 1, -5),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorString() {

    String original("ABCDE");

    String test(original);

    CPPUNIT_ASSERT(test.length() == 5);
    CPPUNIT_ASSERT(test.isEmpty() == false);

    CPPUNIT_ASSERT_MESSAGE("String and std::string should be equal", test == original);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        test.charAt(5),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConstructorCharFill() {

    String expected("AAAAA");
    String input('A', 5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("String fill failed", expected, input);

    CPPUNIT_ASSERT_MESSAGE("String should be empty", String('A', 0).isEmpty());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an IndexOutOfBoundsException",
        String('A', -1),
        IndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testAssignmentString() {

    String transient;
    String input("HelloWorld");
    String hello("Hello");
    String world("World");

    const String expected("World");

    transient = input;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", transient, input);
    transient = hello;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", transient, hello);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", input, String("HelloWorld"));
    transient = world;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", transient, world);

    String toEmpty("ABCDEF");
    toEmpty = String("");
    CPPUNIT_ASSERT_MESSAGE("String did not get set to empty", toEmpty.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testAssignmentStdString() {

    String transient;
    String input("HelloWorld");
    std::string hello("Hello");
    std::string world("World");

    const String expected("World");

    transient = input;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(input));
    transient = hello;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(hello));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", input, String("HelloWorld"));
    transient = world;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(world));

    String toEmpty("ABCDEF");
    toEmpty = std::string("");
    CPPUNIT_ASSERT_MESSAGE("String did not get set to empty", toEmpty.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testAssignmentCString() {

    String transient;
    String input("HelloWorld");
    const char* hello = "Hello";
    const char* world = "World";

    const String expected("World");

    transient = input;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(input));
    transient = hello;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(hello));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("String assignment failed", input, String("HelloWorld"));
    transient = world;
    CPPUNIT_ASSERT_MESSAGE("String assignment failed", transient.equals(world));

    String toEmpty("ABCDEF");
    toEmpty = "";
    CPPUNIT_ASSERT_MESSAGE("String did not get set to empty", toEmpty.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompact() {

    const String input("HelloWorld");
    const String expected("World");

    CPPUNIT_ASSERT_MESSAGE("Incorrect substring returned", expected.equals(input.substring(5)));
    CPPUNIT_ASSERT_MESSAGE("not identical", expected.substring(0) == expected);

    String subStr = input.substring(5);
    CPPUNIT_ASSERT_MESSAGE("wrong length returned.", subStr.length() == 5);
    String compacted = subStr.compact();
    CPPUNIT_ASSERT_MESSAGE("wrong length returned.", compacted.length() == 5);
    CPPUNIT_ASSERT_MESSAGE("Incorrect compacted string returned", expected.equals(compacted));

    String empty;
    empty = empty.compact();
    CPPUNIT_ASSERT_MESSAGE("wrong length returned.", empty.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIsEmpty() {

    String hw("HelloWorld");
    CPPUNIT_ASSERT_MESSAGE("String should not be empty", !hw.isEmpty());

    String empty;
    CPPUNIT_ASSERT_MESSAGE("String should be empty", empty.isEmpty());

    hw = String("");
    CPPUNIT_ASSERT_MESSAGE("String should be empty", hw.isEmpty());
    hw = "A";
    CPPUNIT_ASSERT_MESSAGE("String should not be empty", !hw.isEmpty());
    hw = std::string("");
    CPPUNIT_ASSERT_MESSAGE("String should be empty", hw.isEmpty());
    hw = "A";
    CPPUNIT_ASSERT_MESSAGE("String should not be empty", !hw.isEmpty());
    hw = "";
    CPPUNIT_ASSERT_MESSAGE("String should be empty", hw.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testHashCode() {

    String hw("HelloWorld");

    int hwHashCode = 0;
    const int hwLength = hw.length();
    int powerOfThirtyOne = 1;

    for (int counter = hwLength - 1; counter >= 0; counter--) {
        hwHashCode += (int) hw.charAt(counter) * powerOfThirtyOne;
        powerOfThirtyOne *= 31;
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("String did not hash to correct value", hwHashCode, hw.hashCode());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("The empty string \"\" did not hash to zero", 0, String().hashCode());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Calculated wrong string hashcode", -1933545242, String("Harmony").hashCode());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testSubstring1() {

    const String input("HelloWorld");
    const String expected("World");

    CPPUNIT_ASSERT_MESSAGE("Incorrect substring returned", expected.equals(input.substring(5)));
    CPPUNIT_ASSERT_MESSAGE("not identical", expected.substring(0) == expected);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testSubstring2() {

    const String input("HelloWorld");
    const String expected("Hello");

    CPPUNIT_ASSERT_MESSAGE("Incorrect substring returned", input.substring(0, 5).equals("Hello"));
    CPPUNIT_ASSERT_MESSAGE("Incorrect substring returned", input.substring(5, 10).equals("World"));
    CPPUNIT_ASSERT_MESSAGE("not identical", input.substring(0, input.length()) == input);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testRegionMatches() {

    const String input1("HelloWorld");
    const String input2("HelloWorld");
    const String bogusString("xxcedkedkleiorem lvvwr e''' 3r3r 23r");

    CPPUNIT_ASSERT_MESSAGE("identical regions failed comparison",
                           input1.regionMatches(2, input2, 2, 5));

    CPPUNIT_ASSERT_MESSAGE("Different regions returned true",
                           !input1.regionMatches(2, bogusString, 2, 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testRegionMatchesCaseSensitive() {

    const String input1("HelloWorld");
    const String input2("HelloWorld");
    String bogusString = "xxcedkedkleiorem lvvwr e''' 3r3r 23r";

    CPPUNIT_ASSERT_MESSAGE("identical regions failed comparison", input1.regionMatches(
                           false, 2, input2, 2, 5));
    CPPUNIT_ASSERT_MESSAGE("identical regions failed comparison with different cases",
                           input1.regionMatches(true, 2, input2, 2, 5));
    CPPUNIT_ASSERT_MESSAGE("Different regions returned true",
                           !input1.regionMatches(true, 2, bogusString, 2, 5));
    CPPUNIT_ASSERT_MESSAGE("identical regions failed comparison with different cases",
                           input1.regionMatches(false, 2, input2, 2, 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testStartsWith() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.startsWith("Hello"));
    CPPUNIT_ASSERT_MESSAGE("Found incorrect string", !input.startsWith("T"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testStartsWithI() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.startsWith("World", 5));
    CPPUNIT_ASSERT_MESSAGE("Found incorrect string", !input.startsWith("Hello", 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testSubstringExceptions() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        input.substring(-1, 1),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        input.substring(4, 1),
        StringIndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown an StringIndexOutOfBoundsException",
        input.substring(0, 100),
        StringIndexOutOfBoundsException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testTrim() {
    const String input(" HelloWorld ");
    const String expected("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Incorrect string returned", input.trim().equals(expected));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testToString() {
    const String input("HelloWorld");
    const std::string helloworld("HelloWorld");
    const std::string expected("World");

    String substring = input.substring(5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect string returned", helloworld, input.toString());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect string returned", expected, substring.toString());
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testToCharArray() {
    String input("ABCDE");
    char* result = input.toCharArray();

    for (int i = 0; i < input.length(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned incorrect char aray", input.charAt(i) == result[i]);
    }

    delete [] result;
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCStr() {

    String input("ABCDE");
    const char* result = input.c_str();

    for (int i = 0; i < input.length(); i++) {
        CPPUNIT_ASSERT_MESSAGE("Returned incorrect char aray", input.charAt(i) == result[i]);
    }

    std::string empty("");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid string returned", empty, std::string(String().c_str()));

    const String hw("HelloWorld");
    String substr = hw.substring(5);
    String world = "World";

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid string returned",
                                 std::string(world.c_str()), std::string(substr.c_str()));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEndsWith() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed to find ending String", input.endsWith(String("ld")));
    CPPUNIT_ASSERT_MESSAGE("Failed to not find ending String", !input.endsWith(String("lo")));

    CPPUNIT_ASSERT_MESSAGE("Failed to find ending std::string", input.endsWith(std::string("ld")));
    CPPUNIT_ASSERT_MESSAGE("Failed to not find ending std::string", !input.endsWith(std::string("lo")));

    CPPUNIT_ASSERT_MESSAGE("Failed to find ending C string", input.endsWith("ld"));
    CPPUNIT_ASSERT_MESSAGE("Failed to not find ending C string", !input.endsWith("lo"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEquals() {

    String lower = "helloworld";
    String lower2 = "helloworld";
    String upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned equal to uc", !lower.equals(upper));
    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to lc", lower.equals(lower));
    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to lc", lower.equals(lower2));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEqualsCString() {

    String lower = "helloworld";
    const char* lower2 = "helloworld";
    const char* upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned equal to uc", !lower.equals(upper));
    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to lc", lower.equals(lower2));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEqualsStdString() {

    String lower = "helloworld";
    std::string lower2 = "helloworld";
    std::string upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned equal to uc", !lower.equals(upper));
    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to lc", lower.equals(lower2));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEqualsIgnoreCase() {

    String lower = "helloworld";
    String upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to uc", lower.equalsIgnoreCase(upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEqualsIgnoreCaseCString() {

    String lower = "helloworld";
    const char* upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to uc", lower.equalsIgnoreCase(upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testEqualsIgnoreCaseStdString() {

    String lower = "helloworld";
    std::string upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("lc version returned unequal to uc", lower.equalsIgnoreCase(upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testFindFirstOf() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL(0, input.findFirstOf("H"));
    CPPUNIT_ASSERT_EQUAL(-1, input.findFirstOf("z"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testFindFirstOf2() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL(0, input.findFirstOf("H", 0));
    CPPUNIT_ASSERT_EQUAL(0, input.findFirstOf("H", -1));
    CPPUNIT_ASSERT_EQUAL(-1, input.findFirstOf("H", 1));
    CPPUNIT_ASSERT_EQUAL(-1, input.findFirstOf("H", 25));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testFindFirstNotOf() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL(1, input.findFirstNotOf("H"));
    CPPUNIT_ASSERT_EQUAL(0, input.findFirstNotOf("z"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testFindFirstNotOf2() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL(5, input.findFirstNotOf("Hello", 5));
    CPPUNIT_ASSERT_EQUAL(0, input.findFirstNotOf("z", -1));
    CPPUNIT_ASSERT_EQUAL(1, input.findFirstNotOf("H", 1));
    CPPUNIT_ASSERT_EQUAL(-1, input.findFirstNotOf("H", 25));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfChar() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid index returned", 1, input.indexOf('e'));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid index returned", -1, input.indexOf('q'));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfChar2() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid character index returned", 5, input.indexOf('W', 2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid index returned", -1, input.indexOf('q', 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid index returned", 1, input.indexOf('e', -1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid index returned", -1, input.indexOf('H', 10));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfString() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf(String("World")) > 0);
    CPPUNIT_ASSERT_MESSAGE("Failed to find string", !(input.indexOf(String("ZZ")) > 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfString2() {

    const String input("HelloWorld");
    const String hello("Hello");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf(String("World"), 0) > 0);
    CPPUNIT_ASSERT_MESSAGE("Found string outside index", !(input.indexOf(String("Hello"), 6) > 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid negative starting position",
                                 0, hello.indexOf(String(""), -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", 5, hello.indexOf(String(""), 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong for empty in empty", 0, String("").indexOf(String(""), 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfStdString() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf(std::string("World")) > 0);
    CPPUNIT_ASSERT_MESSAGE("Failed to find string", !(input.indexOf(std::string("ZZ")) > 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfStdString2() {

    const String input("HelloWorld");
    const String hello("Hello");

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf(std::string("World"), 0) > 0);
    CPPUNIT_ASSERT_MESSAGE("Found string outside index", !(input.indexOf(std::string("Hello"), 6) > 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid negative starting position",
                                 0, hello.indexOf(std::string(""), -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", 5, hello.indexOf(std::string(""), 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong for empty in empty", 0, String("").indexOf(std::string(""), 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfCString() {
    const String input("HelloWorld");
    const char* nullString = NULL;

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf("World") > 0);
    CPPUNIT_ASSERT_MESSAGE("Failed to find string", !(input.indexOf("ZZ") > 0));
    CPPUNIT_ASSERT_MESSAGE("Failed to return correct code", !(input.indexOf(nullString) > 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIndexOfCString2() {

    const String input("HelloWorld");
    const String hello("Hello");
    const char* nullString = NULL;

    CPPUNIT_ASSERT_MESSAGE("Failed to find string", input.indexOf("World", 0) > 0);
    CPPUNIT_ASSERT_MESSAGE("Found string outside index", !(input.indexOf("Hello", 6) > 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid negative starting position",
                                 0, hello.indexOf("", -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", 5, hello.indexOf("", 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong for empty in empty", 0, String("").indexOf("", 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", -1, hello.indexOf(nullString, 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfChar() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to return correct index", 5, input.lastIndexOf('W'));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned index for non-existent char", -1, input.lastIndexOf('Z'));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfChar2() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to return correct index", 5, input.lastIndexOf('W', 6));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned index for char out of specified range",
                                 -1, input.lastIndexOf('W', 4));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned index for non-existent char",
                                 -1, input.lastIndexOf('Z', 9));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfString() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf(String("World")));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Found String outside of index",
                                 -1, input.lastIndexOf(String("HeKKKKKKKK")));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", input.length(), input.lastIndexOf(String()));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfString2() {
    const String input("HelloWorld");
    const String hello("Hello");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf(String("World"), 9));
    int result = input.lastIndexOf(String("Hello"), 2);
    CPPUNIT_ASSERT_MESSAGE("Found String outside of index: " + Integer::toString(result), result == 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", -1, hello.lastIndexOf(String(""), -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid large starting position",
                                 5, hello.lastIndexOf(String(""), 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfStdString() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf(std::string("World")));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Found String outside of index",
                                 -1, input.lastIndexOf(std::string("HeKKKKKKKK")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfStdString2() {
    const String input("HelloWorld");
    const String hello("Hello");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf(std::string("World"), 9));
    int result = input.lastIndexOf(std::string("Hello"), 2);
    CPPUNIT_ASSERT_MESSAGE("Found String outside of index: " + Integer::toString(result), result == 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", -1, hello.lastIndexOf(std::string(""), -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid large starting position",
                                 5, hello.lastIndexOf(std::string(""), 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfCString() {
    const String input("HelloWorld");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf("World"));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Found String outside of index", -1, input.lastIndexOf("HeKKKKKKKK"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testLastIndexOfCString2() {
    const String input("HelloWorld");
    const String hello("Hello");
    const char* nullString = NULL;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", -1, input.lastIndexOf(nullString, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect index", 5, input.lastIndexOf("World", 9));
    int result = input.lastIndexOf("Hello", 2);
    CPPUNIT_ASSERT_MESSAGE("Found String outside of index: " + Integer::toString(result), result == 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Reported wrong error code", -1, hello.lastIndexOf("", -5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Did not accept valid large starting position", 5, hello.lastIndexOf("", 5));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testToLowerCase() {

    String lower = "helloworld";
    String upper = "HELLOWORLD";

    upper.toLowerCase();

    CPPUNIT_ASSERT_MESSAGE("toLowerCase case conversion did not succeed",
                           upper.toLowerCase().equals(lower));
    CPPUNIT_ASSERT_MESSAGE("toLowerCase case non-conversion did not succeed",
                           lower.toLowerCase().equals(lower));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testToUpperCase() {

    String lower = "helloworld";
    String upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("toUpperCase case conversion did not succeed",
                           lower.toUpperCase().equals(upper));
    CPPUNIT_ASSERT_MESSAGE("toUpperCase case non-conversion did not succeed",
                           upper.toUpperCase().equals(upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testReplaceCharChar() {

    const String input("HelloWorld");
    const String expected("HezzoWorzd");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed replace", String("HezzoWorzd"), input.replace('l', 'z'));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testContainsString() {

    String s = "abcdefghijklmnopqrstuvwxyz";
    CPPUNIT_ASSERT(s.contains(String("abc")));
    CPPUNIT_ASSERT(s.contains(String("def")));
    CPPUNIT_ASSERT(!s.contains(String("ac")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testContainsStdString() {

    String s = "abcdefghijklmnopqrstuvwxyz";
    CPPUNIT_ASSERT(s.contains(std::string("abc")));
    CPPUNIT_ASSERT(s.contains(std::string("def")));
    CPPUNIT_ASSERT(!s.contains(std::string("ac")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testContainsCString() {

    String s = "abcdefghijklmnopqrstuvwxyz";
    CPPUNIT_ASSERT(s.contains("abc"));
    CPPUNIT_ASSERT(s.contains("def"));
    CPPUNIT_ASSERT(!s.contains("ac"));
    CPPUNIT_ASSERT(!s.contains((const char*) NULL));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConcatString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const String world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", expected, hello.concat(world));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", hello, hello.concat(String("")));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", String(""), String("").concat(String("")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConcatStdString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const std::string world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", expected, hello.concat(world));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", hello, hello.concat(std::string("")));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", String(""), String("").concat(std::string("")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testConcatCString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const char* world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", expected, hello.concat(world));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", hello, hello.concat(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed Concat", String(""), String("").concat(""));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                           String("aaaaab").compareTo(String("aaaaac")) < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareTo(String("aaaaac")));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareTo(String("aaaaab")) > 0);
    CPPUNIT_ASSERT_MESSAGE("Considered case to not be of importance",
                           !(String("A").compareTo(String("a")) == 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToStdString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                           String("aaaaab").compareTo(std::string("aaaaac")) < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareTo(std::string("aaaaac")));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareTo(std::string("aaaaab")) > 0);
    CPPUNIT_ASSERT_MESSAGE("Considered case to not be of importance",
                           !(String("A").compareTo(std::string("a")) == 0));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToCString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                           String("aaaaab").compareTo("aaaaac") < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareTo("aaaaac"));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareTo("aaaaab") > 0);
    CPPUNIT_ASSERT_MESSAGE("Considered case to not be of importance",
                           !(String("A").compareTo("a") == 0));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        String("").compareTo((const char*) NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToIgnoreCaseString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                          String("aaaaab").compareToIgnoreCase(String("aaaaac")) < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareToIgnoreCase(String("aaaaac")));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareToIgnoreCase(String("aaaaab")) > 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Considered case to not be of importance",
                                 0, String("A").compareToIgnoreCase(String("a")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToIgnoreCaseStdString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                          String("aaaaab").compareToIgnoreCase(std::string("aaaaac")) < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareToIgnoreCase(std::string("aaaaac")));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareToIgnoreCase(std::string("aaaaab")) > 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Considered case to not be of importance",
                                 0, String("A").compareToIgnoreCase(std::string("a")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testCompareToIgnoreCaseCString() {

    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first < second",
                          String("aaaaab").compareToIgnoreCase("aaaaac") < 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Returned incorrect value for first = second",
                                 0, String("aaaaac").compareToIgnoreCase("aaaaac"));
    CPPUNIT_ASSERT_MESSAGE("Returned incorrect value for first > second",
                           String("aaaaac").compareToIgnoreCase("aaaaab") > 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Considered case to not be of importance",
                                 0, String("A").compareToIgnoreCase("a"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        String("").compareTo((const char*) NULL),
        NullPointerException);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testIsNullOrEmpty() {

    CPPUNIT_ASSERT_MESSAGE("Failed to detect NULL", String::isNullOrEmpty((const char*) NULL));
    CPPUNIT_ASSERT_MESSAGE("Failed to detect empty", String::isNullOrEmpty(""));
    CPPUNIT_ASSERT_MESSAGE("Failed to detect non-empty", !String::isNullOrEmpty("abcd"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorEqualsString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input == String("HelloWorld"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input == String("HolloWorld")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorEqualsStdString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input == std::string("HelloWorld"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input == std::string("HolloWorld")));

    // Test comparison with lhs as std::string
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", std::string("HelloWorld") == input);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorEqualsCString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input == "HelloWorld");
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input == "HolloWorld"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input == NULL));

    // Test comparison with lhs as C String
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", "HelloWorld" == input);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorNotEqualsString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input != String("HelloWorzd"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input != String("HelloWorld")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorNotEqualsStdString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input != std::string("HelloWorzd"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input != std::string("HelloWorld")));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorNotEqualsCString() {

    const String input("HelloWorld");

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input != "HelloWorzd");
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(input != "HelloWorld"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", input != NULL);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorLessString() {

    String upper = "HELLOWORLD";
    String lower = "helloworld";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", upper < lower);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(upper < upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorLessStdString() {

    String upper = "HELLOWORLD";
    std::string lower = "helloworld";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", upper < lower);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(upper < std::string("HELLOWORLD")));

    // test lhs as std::string
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", std::string("aaab") < String("aaac"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", std::string("aaab") <= String("aaab"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorLessCString() {

    String upper = "HELLOWORLD";
    const char* lower = "helloworld";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", upper < lower);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(upper < "HELLOWORLD"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        (upper < NULL),
        NullPointerException);

    // test lhs as std::string
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", "aaab" < String("aaac"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", "aaab" <= String("aaab"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorGreaterString() {

    String upper = "HELLOWORLD";
    String lower = "helloworld";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", lower > upper);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(upper > upper));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorGreaterStdString() {

    std::string upper = "HELLOWORLD";
    String lower = "helloworld";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", lower > upper);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(lower > std::string("helloworld")));

    // test lhs as std::string
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", std::string("aaac") > String("aaab"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", std::string("aaac") >= String("aaac"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorGreaterCString() {

    String lower = "helloworld";
    const char* upper = "HELLOWORLD";

    CPPUNIT_ASSERT_MESSAGE("Failed comparison", lower > upper);
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", !(lower > "helloworld"));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        (lower < NULL),
        NullPointerException);

    // test lhs as C string
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", "aaac" > String("aaab"));
    CPPUNIT_ASSERT_MESSAGE("Failed comparison", "aaac" >= String("aaac"));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorPlusString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const String world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", expected, hello + world);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", hello, hello + String(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", String(""), String("") + String(""));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorPlusStdString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const std::string world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", expected, hello + world);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", hello, hello + std::string(""));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", String(""), String("") + std::string(""));
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testOperatorPlusCString() {

    const String expected("HelloWorld");
    const String hello("Hello");
    const char* world("World");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", expected, hello + world);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", hello, hello + "");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", String(""), String("") + "");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed operator+ ", String(""), String("") + NULL);
}

////////////////////////////////////////////////////////////////////////////////
void StringTest::testGetChars() {

    String hello("Hello World");
    char* buffer = new char[10];
    Arrays::fill(buffer, 10, '\0');

    hello.getChars(0, 5, buffer, 10, 0);
    CPPUNIT_ASSERT_EQUAL(String("Hello"), String(buffer));

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(-1, 1, buffer, 10, 0),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(1, -1, buffer, 10, 0),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(0, 1, buffer, 10, -1),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(1, 1, buffer, -1, 0),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a NullPointerException",
        hello.getChars(0, 2, (char*) NULL, 10, 0),
        NullPointerException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(15, 1, buffer, 10, 0),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(0, 12, buffer, 10, 0),
        IndexOutOfBoundsException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should have thrown a IndexOutOfBoundsException",
        hello.getChars(2, 10, buffer, 10, 4),
        IndexOutOfBoundsException);

    delete [] buffer;
}
