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

#include "String.h"

#include <decaf/lang/ArrayPointer.h>
#include <decaf/lang/System.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

#include <decaf/internal/util/StringUtils.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace lang {

    class Contents {
    public:

        ArrayPointer<char> value;
        int length;
        int offset;
        int hashCode;

    public:

        /**
         * Contents as empty string.
         */
        Contents() : value(), length(0), offset(0), hashCode(0) {}

        /**
         * Contents created with the given length, the array is length + 1 to add the
         * null terminating character.
         */
        Contents(int length) : value(length + 1), length(length), offset(0), hashCode(0) {
            value[length] = 0;  // Null terminated
        }

        /**
         * Contents is a view of some other String which can either be all or a
         * window allowing for substring methods to not need to copy the contents.
         */
        Contents(int offset, int length, ArrayPointer<char> value) :
            value(value), length(length), offset(offset), hashCode(0) {}
    };

}}

////////////////////////////////////////////////////////////////////////////////
String::String(Contents* content) :
    contents(new Contents(0, content->length, content->value)) {
}

////////////////////////////////////////////////////////////////////////////////
String::String(int offset, int length, const ArrayPointer<char> content) :
    contents(new Contents(offset, length, content)) {
}

////////////////////////////////////////////////////////////////////////////////
String::String() : contents(new Contents()) {
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char value, int count) : contents() {

    if (count < 0) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "count parameter out of Bounds: %d.", count);
    }

    contents = new Contents(count);
    for (int i = 0; i < count; ++i) {
        contents->value[i] = value;
    }
}

////////////////////////////////////////////////////////////////////////////////
String::String(const String& source) : contents(new Contents(*source.contents)) {
}

////////////////////////////////////////////////////////////////////////////////
String::String(const std::string& source) : contents(new Contents((int)source.length())) {

    // load the passed string into the contents value.
    System::arraycopy(source.c_str(), 0, contents->value.get(), 0, source.length());
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array) : contents() {

    if (array == NULL) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL.");
    }

    int size = StringUtils::stringLength(array);

    if (size > 0) {
        this->contents = new Contents(size);
        System::arraycopy(array, 0, contents->value.get(), 0, size);
    } else {
        this->contents = new Contents();
    }
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array, int size) : contents() {

    if (size < 0) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
    }

    if (array == NULL) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL.");
    }

    if (size > 0) {
        this->contents = new Contents(size);
        System::arraycopy(array, 0, contents->value.get(), 0, size);
    } else {
        this->contents = new Contents();
    }
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array, int offset, int length) : contents() {

    int size = StringUtils::stringLength(array);

    if (offset > size || offset < 0) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
    }

    if (length < 0 || length > size - offset) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
    }

    if (array == NULL) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL.");
    }

    if (size > 0 && length > 0) {
        this->contents = new Contents(length);
        System::arraycopy(array, offset, contents->value.get(), 0, length);
    } else {
        this->contents = new Contents();
    }
}

////////////////////////////////////////////////////////////////////////////////
String::String(const char* array, int size, int offset, int length) : contents() {

    if (size < 0) {
        throw IndexOutOfBoundsException(
            __FILE__, __LINE__, "size parameter out of Bounds: %d.", size);
    }

    if (offset > size || offset < 0) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "offset parameter out of Bounds: %d.", offset);
    }

    if (length < 0 || length > size - offset) {
        throw StringIndexOutOfBoundsException(
            __FILE__, __LINE__, "length parameter out of Bounds: %d.", length);
    }

    if (array == NULL) {
        throw NullPointerException(
            __FILE__, __LINE__, "Buffer pointer passed was NULL.");
    }

    if (size > 0 && length > 0) {
        this->contents = new Contents(length);
        System::arraycopy(array, offset, contents->value.get(), 0, length);
    } else {
        this->contents = new Contents();
    }
}

////////////////////////////////////////////////////////////////////////////////
String::~String() {
    try {
        delete this->contents;
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
String& String::operator= (const String& other) {
    contents->value = other.contents->value;
    contents->length = other.contents->length;
    contents->offset = other.contents->offset;
    contents->hashCode = other.contents->hashCode;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
String& String::operator= (const std::string& other) {

    delete contents;

    if (!other.empty()) {
        int length = (int) other.length();
        contents = new Contents(length);
        System::arraycopy(other.c_str(), 0, contents->value.get(), 0, length);
    } else {
        contents = new Contents();
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
String& String::operator= (const char* other) {

    if (other == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Assignment from NULL not supported.");
    }

    int length = StringUtils::stringLength(other);
    delete contents;
    if (length > 0) {
        contents = new Contents(length);
        System::arraycopy(other, 0, contents->value.get(), 0, length);
    } else {
        contents = new Contents();
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator==(const String& other) const {
    return this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator==(const std::string& other) const {
    return this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator==(const char* other) const {
    return this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator!=(const String& other) const {
    return !this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator!=(const std::string& other) const {
    return !this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator!=(const char* other) const {
    return !this->equals(other);
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator< (const String& other) const {
    return this->compareTo(other) < 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator< (const std::string& other) const {
    return this->compareTo(other) < 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator< (const char* other) const {
    return this->compareTo(other) < 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator<=(const String& other) const {
    return this->compareTo(other) <= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator<=(const std::string& other) const {
    return this->compareTo(other) <= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator<=(const char* other) const {
    return this->compareTo(other) <= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator> (const String& other) const {
    return this->compareTo(other) > 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator> (const std::string& other) const {
    return this->compareTo(other) > 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator> (const char* other) const {
    return this->compareTo(other) > 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator>=(const String& other) const {
    return this->compareTo(other) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator>=(const std::string& other) const {
    return this->compareTo(other) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::operator>=(const char* other) const {
    return this->compareTo(other) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
String String::operator+ (const String& other) const {
    return this->concat(other);
}

////////////////////////////////////////////////////////////////////////////////
String String::operator+ (const std::string& other) const {
    return this->concat(other);
}

////////////////////////////////////////////////////////////////////////////////
String String::operator+ (const char* other) const {
    return this->concat(other);
}

////////////////////////////////////////////////////////////////////////////////
const char* String::c_str() const {

    if (contents->length == 0) {
        return "";
    }

    if (contents->length == contents->value.length() - 1) {
        return (const char*) (contents->value.get() + contents->offset);
    }

    Contents* newContents = new Contents(contents->length);

    System::arraycopy(contents->value.get(), contents->offset,
                      newContents->value.get(), 0, contents->length);

    Contents* oldContents = this->contents;
    this->contents = newContents;
    delete oldContents;

    return contents->value.get();
}

////////////////////////////////////////////////////////////////////////////////
char String::charAt(int index) const {

    try {

        if (index < 0 || index >= this->length()) {
            throw StringIndexOutOfBoundsException(
                __FILE__, __LINE__, "Index given is out of bounds: %d.", index);
        }

        return this->contents->value[this->contents->offset + index];
    }
    DECAF_CATCH_RETHROW(StringIndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(StringIndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
String String::compact() const {

    // Empty String.
    if (contents->value.length() == 0) {
        return *this;
    }

    // Don't do anything if the string is already compact.
    if (contents->value.length() > this->contents->length + 1) {
        return String(contents->value.get(), contents->offset, contents->length);
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int String::compareTo(const String& string) const {

    int o1 = contents->offset;
    int o2 = string.contents->offset;
    int result;

    int end = contents->offset +
        (contents->length < string.contents->length ? contents->length : string.contents->length);

    while (o1 < end) {
        if ((result = contents->value[o1++] - string.contents->value[o2++]) != 0) {
            return result;
        }
    }

    return contents->length - string.contents->length;
}

////////////////////////////////////////////////////////////////////////////////
int String::compareTo(const std::string& string) const {

    int o1 = contents->offset;
    int o2 = 0;
    int result;

    int end = contents->offset +
        (contents->length < (int) string.length() ? contents->length : (int) string.length());

    while (o1 < end) {
        if ((result = contents->value[o1++] - string.at(o2++)) != 0) {
            return result;
        }
    }

    return contents->length - (int)string.length();
}

////////////////////////////////////////////////////////////////////////////////
int String::compareTo(const char* string) const {

    if (string == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Comparison C String cannot be NULL");
    }

    int length = StringUtils::stringLength(string);

    int o1 = contents->offset;
    int o2 = 0;
    int result;

    int end = contents->offset +
        (contents->length < length ? contents->length : length);

    while (o1 < end) {
        if ((result = contents->value[o1++] - string[o2++]) != 0) {
            return result;
        }
    }

    return contents->length - length;
}

////////////////////////////////////////////////////////////////////////////////
int String::compareToIgnoreCase(const String& string) const {

    int o1 = contents->offset;
    int o2 = string.contents->offset;
    int result;

    int end = contents->offset +
        (contents->length < string.contents->length ? contents->length : string.contents->length);
    char c1, c2;

    while (o1 < end) {
        if ((c1 = contents->value[o1++]) == (c2 = string.contents->value[o2++])) {
            continue;
        }
        c1 = Character::toLowerCase(c1);
        c2 = Character::toLowerCase(c2);
        if ((result = c1 - c2) != 0) {
            return result;
        }
    }

    return contents->length - string.contents->length;
}

////////////////////////////////////////////////////////////////////////////////
int String::compareToIgnoreCase(const std::string& string) const {

    int o1 = contents->offset;
    int o2 = 0;
    int result;

    int end = contents->offset +
        (contents->length < (int) string.length() ? contents->length : (int) string.length());
    char c1, c2;

    while (o1 < end) {
        if ((c1 = contents->value[o1++]) == (c2 = string.at(o2++))) {
            continue;
        }
        c1 = Character::toLowerCase(c1);
        c2 = Character::toLowerCase(c2);
        if ((result = c1 - c2) != 0) {
            return result;
        }
    }

    return contents->length - (int) string.length();
}

////////////////////////////////////////////////////////////////////////////////
int String::compareToIgnoreCase(const char* string) const {

    if (string == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Comparison C String cannot be NULL");
    }

    int length = StringUtils::stringLength(string);

    int o1 = contents->offset;
    int o2 = 0;
    int result;

    int end = contents->offset + (contents->length < length ? contents->length : length);
    char c1, c2;

    while (o1 < end) {
        if ((c1 = contents->value[o1++]) == (c2 = string[o2++])) {
            continue;
        }
        c1 = Character::toLowerCase(c1);
        c2 = Character::toLowerCase(c2);
        if ((result = c1 - c2) != 0) {
            return result;
        }
    }

    return contents->length - length;
}

////////////////////////////////////////////////////////////////////////////////
String String::copyValueOf(const char* data) {
    if (data == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String to be copied cannot be NULL");
    }

    return String(data, 0, StringUtils::stringLength(data));
}

////////////////////////////////////////////////////////////////////////////////
String String::copyValueOf(char* data, int start, int length) {
    if (data == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String to be copied cannot be NULL");
    }

    return String(data, start, length);
}

////////////////////////////////////////////////////////////////////////////////
String String::concat(const String& string) const {

    if (string.contents->length == 0) {
        return *this;
    }

    Contents buffer(contents->length + string.contents->length);

    if (contents->length > 0) {
        System::arraycopy(contents->value.get(), contents->offset,
                          buffer.value.get(), 0, contents->length);
    }

    System::arraycopy(string.contents->value.get(), string.contents->offset,
                      buffer.value.get(), contents->length, string.contents->length);

    return String(&buffer);
}

////////////////////////////////////////////////////////////////////////////////
String String::concat(const std::string& string) const {

    if (string.length() == 0) {
        return *this;
    }

    Contents buffer(contents->length + (int) string.length());

    if (contents->length > 0) {
        System::arraycopy(contents->value.get(), contents->offset,
                          buffer.value.get(), 0, contents->length);
    }

    System::arraycopy(string.c_str(), 0, buffer.value.get(), contents->length, string.length());

    return String(&buffer);
}

////////////////////////////////////////////////////////////////////////////////
String String::concat(const char* string) const {

    if (string == NULL) {
        return *this;
    }

    int length = StringUtils::stringLength(string);

    if (length == 0) {
        return *this;
    }

    Contents buffer(contents->length + length);

    if (contents->length > 0) {
        System::arraycopy(contents->value.get(), contents->offset,
                          buffer.value.get(), 0, contents->length);
    }

    System::arraycopy(string, 0, buffer.value.get(), contents->length, length);

    return String(&buffer);
}

////////////////////////////////////////////////////////////////////////////////
bool String::contains(const String& string) const {
    return indexOf(string) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::contains(const std::string& string) const {
    return indexOf(string) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::contains(const char* string) const {
    return indexOf(string) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::endsWith(const String& suffix) const {
    return regionMatches(contents->length - suffix.contents->length, suffix, 0, suffix.contents->length);
}

////////////////////////////////////////////////////////////////////////////////
bool String::equals(const String& other) const {

    if (this == &other) {
        return true;
    }

    if (other.isEmpty() && this->isEmpty()) {
        return true;
    }

    if (other.length() != this->length()) {
        return false;
    }

    // Don't force compute hash code on this instance, if not already done
    // we will just do a straight compare.
    int hashCode = contents->hashCode;
    int otherHashCode = other.contents->hashCode;

    if (hashCode != otherHashCode && hashCode != 0 && otherHashCode != 0) {
        return false;
    }

    for (int i = 0; i < contents->length; ++i) {
        if (contents->value[contents->offset + i] != other.contents->value[other.contents->offset + i]) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::equals(const std::string& other) const {

    if (other.empty() && this->isEmpty()) {
        return true;
    }

    if (other.length() != (std::size_t) this->length()) {
        return false;
    }

    for (int i = 0; i < contents->length; ++i) {
        if (contents->value[contents->offset + i] != (unsigned char) other.at(i)) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::equals(const char* other) const {

    if (other == NULL) {
        return false;
    }

    if (StringUtils::stringLength(other) != this->length()) {
        return false;
    }

    for (int i = 0; i < contents->length; ++i) {
        if (contents->value[contents->offset + i] != (unsigned char) other[i]) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::equalsIgnoreCase(const String& string) const {
    if (this == &string) {
        return true;
    }

    if (contents->length != string.contents->length) {
        return false;
    }

    int offsetThis = contents->offset;
    int offsetOther = string.contents->offset;
    int end = contents->offset + contents->length;

    char c1, c2;
    ArrayPointer<char> target = string.contents->value;

    while (offsetThis < end) {
        if ((c1 = contents->value[offsetThis++]) != (c2 = target[offsetOther++])) {
            // If we add support for multibyte strings we need to check both cases
            // toUpperCase and toLowerCase because of Unicode.
            if (Character::toUpperCase(c1) != Character::toUpperCase(c2)) {
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::equalsIgnoreCase(const std::string& string) const {

    if (contents->length != (int) string.length()) {
        return false;
    }

    int offsetThis = contents->offset;
    int end = contents->offset + contents->length;

    int indexOther = 0;
    char c1, c2;

    while (offsetThis < end) {
        if ((c1 = contents->value[offsetThis++]) != (c2 = string.at(indexOther++))) {
            // If we add support for multibyte strings we need to check both cases
            // toUpperCase and toLowerCase because of Unicode.
            if (Character::toUpperCase(c1) != Character::toUpperCase(c2)) {
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::equalsIgnoreCase(const char* string) const {

    if (string == NULL) {
        return false;
    }

    int stringLen = StringUtils::stringLength(string);

    if (contents->length != stringLen) {
        return false;
    }

    int indexOther = 0;
    int offsetThis = contents->offset;
    int end = contents->offset + contents->length;

    char c1, c2;

    while (offsetThis < end) {
        if ((c1 = contents->value[offsetThis++]) != (c2 = string[indexOther++])) {
            // If we add support for multibyte strings we need to check both cases
            // toUpperCase and toLowerCase because of Unicode.
            if (Character::toUpperCase(c1) != Character::toUpperCase(c2)) {
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int String::findFirstOf(const String& chars) const {
    return findFirstOf(chars, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::findFirstOf(const String& chars, int start) const {
    if (start < contents->length) {
        if (start < 0) {
            start = 0;
        }

        for (int i = contents->offset + start; i < contents->offset + contents->length; i++) {
            char c = contents->value[i];
            if (chars.indexOf(c) != -1) {
                return i;
            }
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::findFirstNotOf(const String& chars) const {
    return findFirstNotOf(chars, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::findFirstNotOf(const String& chars, int start) const {
    if (start < contents->length) {
        if (start < 0) {
            start = 0;
        }

        for (int i = contents->offset + start; i < contents->offset + contents->length; i++) {
            char c = contents->value[i];
            if (chars.indexOf(c) == -1) {
                return i;
            }
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
void String::getChars(int srcBegin, int srcEnd, char* dest, int destSize, int destBegin) const {

    if (srcBegin < 0 || srcBegin > srcEnd || srcEnd > contents->length) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
            "Invalid start or end parameters: %d, %d", srcBegin, srcEnd);
    }

    if (destSize < 0 || destBegin < 0 || (destBegin + (srcEnd - srcBegin)) > destSize) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
            "Invalid destination size or offset parameters: %d, %d", destSize, destBegin);
    }

    if (dest == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Destination pointer was Null");
    }

    // Note: last character not copied!
    System::arraycopy(contents->value.get(), srcBegin + contents->offset,
                      dest, destBegin, srcEnd - srcBegin);
}

////////////////////////////////////////////////////////////////////////////////
void String::getChars(int start, int end, char* buffer, int index) const {
    // NOTE last character not copied!
    System::arraycopy(contents->value.get(), start + contents->offset, buffer, index, end - start);
}

////////////////////////////////////////////////////////////////////////////////
int String::hashCode() const {

    if (contents->hashCode == 0) {
        if (contents->length == 0) {
            return 0;
        }

        int hash = 0;

        for (int i = contents->offset; i < contents->length + contents->offset; i++) {
            hash = contents->value[i] + ((hash << 5) - hash);
        }
        contents->hashCode = hash;
    }
    return contents->hashCode;
}

////////////////////////////////////////////////////////////////////////////////
bool String::isEmpty() const {
    return this->contents->length == 0;
}

////////////////////////////////////////////////////////////////////////////////
bool String::isNullOrEmpty(const char* string) {
    return string == NULL || StringUtils::stringLength(string) == 0;
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(char value) const {
    return indexOf(value, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(char value, int start) const {
    if (start < contents->length) {
        if (start < 0) {
            start = 0;
        }

        for (int i = contents->offset + start; i < contents->offset + contents->length; i++) {
            if (contents->value[i] == value) {
                return i - contents->offset;
            }
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const String& string) const {
    return indexOf(string, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const String& subString, int start) const {

    if (start < 0) {
        start = 0;
    }

    int subCount = subString.contents->length;
    if (subCount > 0) {
        if (subCount + start > contents->length) {
            return -1;
        }

        char* target = subString.contents->value.get();
        int subOffset = subString.contents->offset;

        char firstChar = target[subOffset];
        int end = subOffset + subCount;

        while (true) {
            int i = indexOf(firstChar, start);
            if (i == -1 || subCount + i > contents->length) {
                return -1; // handles subCount > length() || start >= length()
            }

            int o1 = contents->offset + i;
            int o2 = subOffset;

            while (++o2 < end && contents->value[++o1] == target[o2]) {
                // Intentionally empty
            }
            if (o2 == end) {
                return i;
            }
            start = i + 1;
        }
    }

    return start < contents->length ? start : contents->length;
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const std::string& string) const {
    return indexOf(string, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const std::string& subString, int start) const {

    if (start < 0) {
        start = 0;
    }

    int subCount = (int) subString.length();
    if (subCount > 0) {
        if (subCount + start > contents->length) {
            return -1;
        }

        const char* target = subString.c_str();
        char firstChar = target[0];
        int end = subCount;

        while (true) {
            int i = indexOf(firstChar, start);
            if (i == -1 || subCount + i > contents->length) {
                return -1; // handles subCount > length() || start >= length()
            }

            int o1 = contents->offset + i;
            int o2 = 0;

            while (++o2 < end && contents->value[++o1] == target[o2]) {
                // Intentionally empty
            }
            if (o2 == end) {
                return i;
            }
            start = i + 1;
        }
    }

    return start < contents->length ? start : contents->length;
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const char* string) const {
    return indexOf(string, 0);
}

////////////////////////////////////////////////////////////////////////////////
int String::indexOf(const char* subString, int start) const {

    if (start < 0) {
        start = 0;
    }

    if (subString == NULL) {
        return -1;
    }

    int subCount = StringUtils::stringLength(subString);
    if (subCount > 0) {
        if (subCount + start > contents->length) {
            return -1;
        }

        char firstChar = subString[0];
        int end = subCount;

        while (true) {
            int i = indexOf(firstChar, start);
            if (i == -1 || subCount + i > contents->length) {
                return -1; // handles subCount > length() || start >= length()
            }

            int o1 = contents->offset + i;
            int o2 = 0;

            while (++o2 < end && contents->value[++o1] == subString[o2]) {
                // Intentionally empty
            }
            if (o2 == end) {
                return i;
            }
            start = i + 1;
        }
    }

    return start < contents->length ? start : contents->length;
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(char value) const {
    return lastIndexOf(value, contents->length - 1);
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(char value, int start) const {
    if (start >= 0) {
        if (start >= contents->length) {
            start = contents->length - 1;
        }

        for (int i = contents->offset + start; i >= contents->offset; --i) {
            if (contents->value[i] == value) {
                return i - contents->offset;
            }
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const String& string) const {
    // Use length instead of length - 1 so lastIndexOf("") answers length
    return lastIndexOf(string, contents->length);
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const String& subString, int start) const {
    int subCount = subString.contents->length;

    if (subCount <= contents->length && start >= 0) {
        if (subCount > 0) {
            if (start > contents->length - subCount) {
                start = contents->length - subCount;
            }

            // count and subCount are both >= 1
            char* target = subString.contents->value.get();
            int subOffset = subString.contents->offset;
            char firstChar = target[subOffset];
            int end = subOffset + subCount;

            while (true) {
                int i = lastIndexOf(firstChar, start);
                if (i == -1) {
                    return -1;
                }

                int o1 = contents->offset + i;
                int o2 = subOffset;

                while (++o2 < end && contents->value[++o1] == target[o2]) {
                    // Intentionally empty
                }

                if (o2 == end) {
                    return i;
                }
                start = i - 1;
            }
        }
        return start < contents->length ? start : contents->length;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const std::string& string) const {
    // Use length instead of length - 1 so lastIndexOf("") answers length
    return lastIndexOf(string, contents->length);
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const std::string& subString, int start) const {
    int subCount = (int) subString.length();

    if (subCount <= contents->length && start >= 0) {
        if (subCount > 0) {
            if (start > contents->length - subCount) {
                start = contents->length - subCount;
            }

            // count and subCount are both >= 1
            const char* target = subString.c_str();
            char firstChar = target[0];
            int end = subCount;

            while (true) {
                int i = lastIndexOf(firstChar, start);
                if (i == -1) {
                    return -1;
                }

                int o1 = contents->offset + i;
                int o2 = 0;

                while (++o2 < end && contents->value[++o1] == target[o2]) {
                    // Intentionally empty
                }

                if (o2 == end) {
                    return i;
                }
                start = i - 1;
            }
        }
        return start < contents->length ? start : contents->length;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const char* string) const {
    // Use length instead of length - 1 so lastIndexOf("") answers length
    return lastIndexOf(string, contents->length);
}

////////////////////////////////////////////////////////////////////////////////
int String::lastIndexOf(const char* subString, int start) const {

    if (subString == NULL) {
        return -1;
    }

    int subCount = StringUtils::stringLength(subString);

    if (subCount <= contents->length && start >= 0) {
        if (subCount > 0) {
            if (start > contents->length - subCount) {
                start = contents->length - subCount;
            }

            // count and subCount are both >= 1
            char firstChar = subString[0];
            int end = subCount;

            while (true) {
                int i = lastIndexOf(firstChar, start);
                if (i == -1) {
                    return -1;
                }

                int o1 = contents->offset + i;
                int o2 = 0;

                while (++o2 < end && contents->value[++o1] == subString[o2]) {
                    // Intentionally empty
                }

                if (o2 == end) {
                    return i;
                }
                start = i - 1;
            }
        }
        return start < contents->length ? start : contents->length;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int String::length() const {
    return this->contents->length;
}

////////////////////////////////////////////////////////////////////////////////
bool String::regionMatches(int thisStart, const String& string, int start, int length) const {

    if (string.contents->length - start < length || start < 0) {
        return false;
    }

    if (thisStart < 0 || contents->length - thisStart < length) {
        return false;
    }

    if (length <= 0) {
        return true;
    }

    int o1 = contents->offset + thisStart;
    int o2 = string.contents->offset + start;

    for (int i = 0; i < length; ++i) {
        if (contents->value[o1 + i] != string.contents->value[o2 + i]) {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool String::regionMatches(bool ignoreCase, int thisStart, const String& string, int start, int length) const {

    if (!ignoreCase) {
        return regionMatches(thisStart, string, start, length);
    }

    if (thisStart < 0 || length > contents->length - thisStart) {
        return false;
    }
    if (start < 0 || length > string.contents->length - start) {
        return false;
    }

    thisStart += contents->offset;
    start += string.contents->offset;
    int end = thisStart + length;
    char c1, c2;

    while (thisStart < end) {
        if ((c1 = contents->value[thisStart++]) != (c2 = string.contents->value[start++])) {
            // If we add support for multibyte strings we need to check both cases
            // toUpperCase and toLowerCase because of Unicode.
            if (Character::toUpperCase(c1) != Character::toUpperCase(c2)) {
                return false;
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
String String::replace(char oldChar, char newChar) const {

    int index = indexOf(oldChar, 0);
    if (index == -1) {
        return *this;
    }

    Contents buffer(contents->length);

    System::arraycopy(contents->value.get(), contents->offset, buffer.value.get(), 0, contents->length);

    do {
        buffer.value[index++] = newChar;
    } while ((index = indexOf(oldChar, index)) != -1);

    return String(&buffer);
}

////////////////////////////////////////////////////////////////////////////////
bool String::startsWith(const String& prefix) const {
    return regionMatches(0, prefix, 0, prefix.contents->length);
}

////////////////////////////////////////////////////////////////////////////////
bool String::startsWith(const String& prefix, int start) const {
    return regionMatches(start, prefix, 0, prefix.contents->length);
}

////////////////////////////////////////////////////////////////////////////////
String String::substring(int start) const {

    if (start == 0) {
        return *this;
    }

    if (0 <= start && start <= contents->length) {
        return String(contents->offset + start, contents->length - start, contents->value);
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, start);
}

////////////////////////////////////////////////////////////////////////////////
String String::substring(int start, int end) const {

    if (start == 0 && end == contents->length) {
        return *this;
    }

    if (start < 0) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, start);
    } else if (start > end) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, end - start);
    } else if (end > contents->length) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, end);
    }

    // NOTE last character not copied!
    return String(contents->offset + start, end - start, contents->value);
}

////////////////////////////////////////////////////////////////////////////////
CharSequence* String::subSequence(int start DECAF_UNUSED, int end DECAF_UNUSED) const {

    try {

        if (start > end) {
            throw StringIndexOutOfBoundsException(
                __FILE__, __LINE__, "Start index is greater than end index.");
        }

        if (end - start > this->length()) {
            throw StringIndexOutOfBoundsException(
                __FILE__, __LINE__, "Requested Range is greater than the String length.");
        }

        return NULL;
    }
    DECAF_CATCH_RETHROW(IndexOutOfBoundsException)
    DECAF_CATCHALL_THROW(IndexOutOfBoundsException)
}

////////////////////////////////////////////////////////////////////////////////
char* String::toCharArray() const {
    char* buffer = new char[contents->length];
    System::arraycopy((const char*)contents->value.get(), contents->offset, buffer, 0, contents->length);
    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
String String::toLowerCase() const {

    Contents newContents(contents->length);
    int offset = contents->offset;

    for (int i = 0; i < contents->length; ++i) {
        newContents.value[i] = Character::toLowerCase(contents->value[offset + i]);
    }

    return String(&newContents);
}

////////////////////////////////////////////////////////////////////////////////
String String::toUpperCase() const {

    Contents newContents(contents->length);
    int offset = contents->offset;

    for (int i = 0; i < contents->length; ++i) {
        newContents.value[i] = Character::toUpperCase(contents->value[offset + i]);
    }

    return String(&newContents);
}

////////////////////////////////////////////////////////////////////////////////
std::string String::toString() const {

    if (this->contents == NULL) {
        return "null";
    }

    if (this->contents->length == 0) {
        return "";
    }

    return std::string((const char*) contents->value.get() + contents->offset, this->length());
}

////////////////////////////////////////////////////////////////////////////////
String String::trim() const {

    int start = contents->offset;
    int last = contents->offset + contents->length - 1;

    int end = last;

    while ((start <= end) && (contents->value[start] <= 0x20)) {
        start++;
    }

    while ((end >= start) && (contents->value[end] <= 0x20)) {
        end--;
    }

    if (start == contents->offset && end == last) {
        return *this;
    }

    return String(start, end - start + 1, contents->value);
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(bool value) {

    if (value) {
        return String("true");
    }

    return String("false");
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(char value) {
    return String(&value, 1);
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(float value) {
    return String(Float::toString(value));
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(double value) {
    return String(Double::toString(value));
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(short value) {
    return String(Short::toString(value));
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(int value) {
    return String(Integer::toString(value));
}

////////////////////////////////////////////////////////////////////////////////
String String::valueOf(long long value) {
    return String(Long::toString(value));
}

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace lang {

    std::ostream& operator<<(std::ostream &out, const String& target) {

        if (target.isEmpty()) {
            out << "";
        }

        for (int i = 0; i < target.length(); ++i) {
            out << target.charAt(i);
        }

        return out;
    }

    bool operator==(const std::string& left, const String& right) {
        return right.equals(left);
    }

    bool operator==(const char* left, const String& right) {
        return right.equals(left);
    }

    bool operator!=(const std::string& left, const String& right) {
        return !right.equals(left);
    }

    bool operator!=(const char* left, const String& right) {
        return !right.equals(left);
    }

    bool operator< (const std::string& left, const String& right) {
        return right.compareTo(left) > 0;
    }

    bool operator< (const char* left, const String& right) {
        return right.compareTo(left) > 0;
    }

    bool operator<=(const std::string& left, const String& right) {
        return right.compareTo(left) >= 0;
    }

    bool operator<=(const char* left, const String& right) {
        return right.compareTo(left) >= 0;
    }

    bool operator> (const std::string& left, const String& right) {
        return right.compareTo(left) < 0;
    }

    bool operator> (const char* left, const String& right) {
        return right.compareTo(left) < 0;
    }

    bool operator>=(const std::string& left, const String& right) {
        return right.compareTo(left) <= 0;
    }

    bool operator>=(const char* left, const String& right) {
        return right.compareTo(left) <= 0;
    }

}}
