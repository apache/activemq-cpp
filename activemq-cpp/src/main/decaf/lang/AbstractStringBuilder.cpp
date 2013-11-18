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

#include <decaf/lang/AbstractStringBuilder.h>

#include <decaf/lang/System.h>
#include <decaf/lang/Math.h>
#include <decaf/lang/ArrayPointer.h>
#include <decaf/util/Arrays.h>

#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/ArrayIndexOutOfBoundsException.h>
#include <decaf/lang/exceptions/StringIndexOutOfBoundsException.h>

#include <decaf/internal/util/StringUtils.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
const int AbstractStringBuilder::INITIAL_CAPACITY = 16;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace lang {

    class AbstractStringBuilderImpl {
    public:

        ArrayPointer<char> value;
        int length;
        bool shared;
        int hashCode;

    public:

        /**
         * Contents created with the given length, the array is length + 1 to add the
         * null terminating character.
         */
        AbstractStringBuilderImpl(int capacity) :
            value(capacity + 1), length(0), shared(false), hashCode(0) {}

        /**
         * Contents is a view of some other String which can either be all or a
         * window allowing for substring methods to not need to copy the contents.
         */
        AbstractStringBuilderImpl(int length, ArrayPointer<char> value) :
            value(value), length(length), shared(false), hashCode(0) {}

        void enlargeBuffer(int min) {
            // API calls for length() * 2 + 2 but we need to add one for Null termination.
            int newCount = ((value.length() >> 1) + value.length()) + 3;
            int newCapacity = (min > newCount ? min : newCount) + 1;
            ArrayPointer<char> newData(newCapacity);
            System::arraycopy(value.get(), 0, newData.get(), 0, length);
            value = newData;
            shared = false;
        }

        int capacity() const {
            return this->value.length() - 1;
        }

        // ensure enough room for current length + additional
        void ensureCapacity(int newLength) {
            if (newLength > (value.length() - 1)) {
                enlargeBuffer(newLength);
            }
        }

        void move(int size, int index) {
            int newCount;
            if (value.length() - length - 1 >= size) {
                if (!shared) {
                    // index == impl->length case is no-op
                    System::arraycopy(value.get(), index, value.get(), index + size, length - index);
                    return;
                }
                newCount = value.length();
            } else {
                newCount = Math::max(length + size, value.length() * 2 + 3);
            }

            ArrayPointer<char> newData(newCount);
            System::arraycopy(value.get(), 0, newData.get(), 0, index);
            // index == impl->length case is no-op
            System::arraycopy(value.get(), index, newData.get(), index + size, length - index);
            value = newData;
            shared = false;
        }

        void fixReversedMultibyte(char* string DECAF_UNUSED, int length DECAF_UNUSED) {

            // TODO fix UTF-8 code points that were revered.
//            char* left;
//            char* right;
//            char* right2;
//            char temp;
//
//            // then scan all bytes and reverse each multibyte character
//            for (scanl = scanr = str; temp = *scanr++;) {
//                if ( (temp & 0x80) == 0) // ASCII char
//                    scanl= scanr;
//                else if ( (temp & 0xc0) == 0xc0 ) { // start of multibyte
//                    scanr2= scanr;
//                    switch (scanr - scanl) {
//                        case 4: temp= *scanl, *scanl++= *--scanr, *scanr= temp; // fallthrough
//                        case 3: // fallthrough
//                        case 2: temp= *scanl, *scanl++= *--scanr, *scanr= temp;
//                    }
//                    scanr = scanl = scanr2;
//                }
//            }
        }
    };

}}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::AbstractStringBuilder() :
    impl(new AbstractStringBuilderImpl(INITIAL_CAPACITY)) {
}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::AbstractStringBuilder(int capacity) : impl() {

    if (capacity < 0) {
        throw NegativeArraySizeException(__FILE__, __LINE__, "Capacity cannot be negative");
    }

    impl = new AbstractStringBuilderImpl(capacity);
}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::AbstractStringBuilder(const String& source) :
    impl(new AbstractStringBuilderImpl(source.length() + INITIAL_CAPACITY)) {

    for (int i = 0; i < source.length(); ++i) {
        impl->value[i] = source.charAt(i);
    }

    impl->length = source.length();
}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::AbstractStringBuilder(const std::string& source) :
    impl(new AbstractStringBuilderImpl((int)source.length() + INITIAL_CAPACITY)) {

    for (int i = 0; i < (int) source.length(); ++i) {
        impl->value[i] = source.at(i);
    }

    impl->length = (int) source.length();
}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::AbstractStringBuilder(const CharSequence* source) : impl() {

    if (source == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "CharSequence was NULL");
    }

    std::string src = source->toString();
    int capacity = (int) src.length();

    impl = new AbstractStringBuilderImpl(capacity + INITIAL_CAPACITY);

    for (int i = 0; i < (int) src.length(); ++i) {
        impl->value[i] = src.at(i);
    }

    impl->length = (int) src.length();
}

////////////////////////////////////////////////////////////////////////////////
AbstractStringBuilder::~AbstractStringBuilder() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppendNull() {
    int newCount = impl->length + 4;
    if (newCount > impl->capacity()) {
        impl->enlargeBuffer(newCount);
    }

    impl->value[impl->length++] = 'n';
    impl->value[impl->length++] = 'u';
    impl->value[impl->length++] = 'l';
    impl->value[impl->length++] = 'l';
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const char value) {
    int newCount = impl->length + 1;
    if (newCount > impl->capacity()) {
        impl->enlargeBuffer(newCount);
    }

    impl->value[impl->length++] = value;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const char* value) {

    if (value == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String cannot be null, call 'doAppendNull' instead");
    }

    int length = StringUtils::stringLength(value);

    if (length <= 0) {
        return;
    }

    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);
    System::arraycopy(value, 0, impl->value.get(), impl->length, length);
    impl->length = newLength;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const char* value, int offset, int length) {

    if (value == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String cannot be null, call 'doAppendNull' instead");
    }

    int arrayLength = StringUtils::stringLength(value);

    if ((offset | length) < 0 || offset > arrayLength || arrayLength - offset < length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Invalid offset or length value given.");
    }

    if (length <= 0) {
        return;
    }

    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);
    System::arraycopy(value, offset, impl->value.get(), impl->length, length);
    impl->length = newLength;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const CharSequence* value) {

    if (value == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String cannot be null, call 'doAppendNull' instead");
    }

    int length = value->length();

    if (length <= 0) {
        return;
    }

    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);
    for (int i = 0; i < length; ++i) {
        impl->value[impl->length++] = value->charAt(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const CharSequence* value, int start, int end) {

    if (value == NULL) {
        const char* nullString = "null";
        doAppend(nullString, start, end - start);
        return;
    }

    int arrayLength = value->length();

    if ((start | end) < 0 || start > end || end > arrayLength) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Invalid start or end value given.");
    }

    int length = end - start;

    if (length == 0) {
        return;
    }

    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);

    for (int i = start; i < end; ++i) {
        impl->value[impl->length++] = value->charAt(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const std::string& value) {

    int length = (int) value.length();
    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);

    for (int i = 0; i < length; ++i) {
        impl->value[impl->length++] = value.at(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const String& value) {

    int length = value.length();
    int newLength = impl->length + length;
    impl->ensureCapacity(newLength);
    value.getChars(0, length, impl->value.get(), impl->length);
    impl->length = newLength;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doAppend(const AbstractStringBuilder& value) {

    if (value.length() > 0) {
        doAppend(value.impl->value.get(), 0, value.impl->length);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doDeleteRange(int start, int end) {

    // This method is specified not to throw if the end index is >= length(), as
    // long as it's >= start. This means we have to clamp it to impl->length here.
    if (end > impl->length) {
        end = impl->length;
    }

    if (start < 0 || start > impl->length || start > end) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Invalid start index: %d", start);
    }

    // This method is defined to throw only if start > impl->length and start == impl->length is a NO-OP
    // Since 'end' is already a clamped value, that case is handled here.
    if (end == start) {
        return;
    }

    // At this point we know for sure that end > start.
    int length = impl->length - end;
    if (length >= 0) {
        if (!impl->shared) {
            System::arraycopy(impl->value.get(), end, impl->value.get(), start, length);
        } else {
            ArrayPointer<char> newValue(impl->value.length());
            System::arraycopy(impl->value.get(), 0, newValue.get(), 0, start);
            System::arraycopy(impl->value.get(), end, newValue.get(), start, length);
            impl->value = newValue;
            impl->shared = false;
        }
    }
    impl->length -= end - start;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doDeleteCharAt(int index) {
    if (index < 0 || index >= impl->length) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Invalid index: %d", index);
    }

    doDeleteRange(index, index + 1);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, char value) {
    if (index < 0 || index > impl->length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Given index is invalid: %d", index);
    }

    impl->move(1, index);
    impl->value[index] = value;
    impl->length++;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const char* value) {

    if (index < 0 || index > impl->length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Given index is invalid: %d", index);
    }

    if (value == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "C String pointer was NULL");
    }

    int arrayLength = StringUtils::stringLength(value);

    if (arrayLength != 0) {
        impl->move(arrayLength, index);
        System::arraycopy(value, 0, impl->value.get(), index, arrayLength);
        impl->length += arrayLength;
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const String& value) {

    if (index < 0 || index > impl->length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Given index is invalid: %d", index);
    }

    int stringLength = value.length();

    if (stringLength != 0) {
        impl->move(stringLength, index);
        value.getChars(0, stringLength, impl->value.get(), index);
        impl->length += stringLength;
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const std::string& value) {

    if (index < 0 || index > impl->length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Given index is invalid: %d", index);
    }

    int stringLength = (int) value.length();

    if (stringLength != 0) {
        impl->move(stringLength, index);

        for (int i = 0; i < stringLength; ++i) {
            impl->value[index++] = value.at(i);
        }

        impl->length += stringLength;
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const char* value, int offset, int length) {

    if (index >= 0 && index <= impl->length) {

        if (value == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "C string pointer was NULL");
        }

        int arrayLength = StringUtils::stringLength(value);

        // start + length could overflow, start/length maybe MaxInt
        if (offset >= 0 && length >= 0 && length <= arrayLength - offset) {
            if (length != 0) {
                impl->move(length, index);
                System::arraycopy(value, offset, impl->value.get(), index, length);
                impl->length += length;
            }
            return;
        }

        throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
                "Invalid string offsets, offset=%d length=%d but C string length=%d",
                offset, length, arrayLength);
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Index value given was invalid: %d", index);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const CharSequence* value) {

    if (index >= 0 && index <= impl->length) {

        if (value == NULL) {
            throw NullPointerException(__FILE__, __LINE__, "CharSequence pointer was NULL");
        }

        doInsert(index, value->toString());
        return;
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Index value given was invalid: %d", index);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doInsert(int index, const CharSequence* value, int start, int end) {

    if (value == NULL) {
        String nullString("null");
        doInsert(index, nullString.c_str(), start, end - start);
        return;
    }

    if (index >= 0 && index <= impl->length) {

        int arrayLength = value->length();

        if ((start | end) < 0 || start > end || end > arrayLength) {
            throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Invalid start or end value given.");
        }

        int length = end - start;

        if (length == 0) {
            return;
        }

        impl->move(length, index);

        for (int i = start; i < end; ++i) {
            impl->value[index++] = value->charAt(i);
        }

        impl->length += length;
        return;
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Index value given was invalid: %d", index);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doReplace(int start, int end, const String& value) {

    if (start >= 0) {
        if (end > impl->length) {
            end = impl->length;
        }
        if (end > start) {
            int stringLength = value.length();
            int diff = end - start - stringLength;
            if (diff > 0) { // replacing with fewer characters
                if (!impl->shared) {
                    // index == impl->length case is no-op
                    System::arraycopy(impl->value.get(), end, impl->value.get(),
                                      start + stringLength, impl->length - end);
                } else {
                    ArrayPointer<char> newData(impl->value.length());
                    System::arraycopy(impl->value.get(), 0, newData.get(), 0, start);
                    // index == impl->length case is no-op
                    System::arraycopy(impl->value.get(), end, newData.get(),
                                      start + stringLength, impl->length - end);
                    impl->value = newData;
                    impl->shared = false;
                }
            } else if (diff < 0) {
                // replacing with more characters...need some room
                impl->move(-diff, end);
            } else if (impl->shared) {
                impl->value = impl->value.clone();
                impl->shared = false;
            }

            value.getChars(0, stringLength, impl->value.get(), start);
            impl->length -= diff;
            return;
        }

        if (start == end) {
            doInsert(start, value);
            return;
        }
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Index value given was invalid: %d", start);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::doReverse() {

    if (impl->length < 2) {
        return;
    }

    if (!impl->shared) {

        char* original = impl->value.get();
        char* left;
        char* right;
        char temp;

        // Reverse from shared buffer to newly allocated buffer.
        for (left = original, right = original + impl->length; left < right;) {
            temp = *left;
            *(left++) = *(--right),
            *right = temp;
        }

        impl->fixReversedMultibyte(original, impl->length);

    } else {

        // Reverse from shared buffer to newly allocated buffer.
        ArrayPointer<char> newData(impl->value.length());
        for (int i = 0; i < impl->length; i++) {
            newData[impl->length - i] = impl->value[i];
        }

        impl->fixReversedMultibyte(newData.get(), impl->length);

        impl->value = newData;
        impl->shared = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::capacity() const {
    return impl->capacity();
}

////////////////////////////////////////////////////////////////////////////////
char AbstractStringBuilder::charAt(int index) const {
    if (index < 0 || index >= impl->length) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, index);
    }

    return impl->value[index];
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::ensureCapacity(int minCapacity) {
    if (minCapacity > impl->value.length() - 1) {
        impl->enlargeBuffer(minCapacity);
    }
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::getChars(int start, int end, char* dest, int destSize, int destStart) const {

    if (start > impl->length || end > impl->length || start > end) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
            "Invalid range: %d : %d", start, end);
    }

    if (destSize < 0) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Destination size cannot be negative");
    }

    if (destStart < 0) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "Destination start index cannot be negative");
    }

    if ((destStart + (end - start)) > destSize) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
            "Destination array[%d] is not large enough for given copy size: %d", destSize, end - start);
    }

    if (dest == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "Destination array is null");
    }

    System::arraycopy(impl->value.get(), start, dest, destStart, end - start);
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::indexOf(const String& value) const {
    return indexOf(value, 0);
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::indexOf(const String& value, int start) const {
    if (start < 0) {
        start = 0;
    }
    int subCount = value.length();
    if (subCount > 0) {
        if (subCount + start > impl->length) {
            return -1;
        }

        char firstChar = value.charAt(0);
        while (true) {
            int i = start;
            bool found = false;
            for (; i < impl->length; i++) {
                if (impl->value[i] == firstChar) {
                    found = true;
                    break;
                }
            }
            if (!found || subCount + i > impl->length) {
                return -1; // handles subCount > impl->length || start >= impl->length
            }

            int o1 = i;
            int o2 = 0;

            while (++o2 < subCount && impl->value[++o1] == value.charAt(o2)) {
                // Intentionally empty
            }
            if (o2 == subCount) {
                return i;
            }
            start = i + 1;
        }
    }
    return (start < impl->length || start == 0) ? start : impl->length;
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::lastIndexOf(const String& value) const {
    return lastIndexOf(value, impl->length);
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::lastIndexOf(const String& value, int start) const {
    int subCount = value.length();
    if (subCount <= impl->length && start >= 0) {
        if (subCount > 0) {
            if (start > impl->length - subCount) {
                start = impl->length - subCount; // impl->length and subCount are both >= 1
            }

            char firstChar = value.charAt(0);
            while (true) {
                int i = start;
                bool found = false;
                for (; i >= 0; --i) {
                    if (impl->value[i] == firstChar) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return -1;
                }

                int o1 = i;
                int o2 = 0;

                while (++o2 < subCount && impl->value[++o1] == value.charAt(o2)) {
                    // Intentionally empty
                }
                if (o2 == subCount) {
                    return i;
                }
                start = i - 1;
            }
        }

        return start < impl->length ? start : impl->length;
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////
int AbstractStringBuilder::length() const {
    return impl->length;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::setCharAt(int index, char value) {

    if (index < 0) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Index < 0: %d", index);
    }

    if (index > impl->length) {
        throw ArrayIndexOutOfBoundsException(__FILE__, __LINE__, "Index > length(): %d", index);
    }

    if (impl->shared) {
        impl->value = impl->value.clone();
        impl->shared = false;
    }

    impl->value[index] = value;
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::setLength(int length) {

    if (length < 0) {
        throw StringIndexOutOfBoundsException(__FILE__, __LINE__, "length < 0: %d", length);
    }

    if (length > impl->value.length() - 1) {
        impl->enlargeBuffer(length);
    } else {
        if (impl->shared) {
            ArrayPointer<char> newValue(impl->value.length());
            System::arraycopy(impl->value.get(), 0, newValue.get(), 0, impl->length);
            impl->value = newValue;
            impl->shared = false;
        } else {
            if (impl->length < length) {
                Arrays::fill(impl->value.get(), impl->value.length(), impl->length, length, (char) 0);
            }
        }
    }
    impl->length = length;
}

////////////////////////////////////////////////////////////////////////////////
String AbstractStringBuilder::substring(int start) const {

    if (start >= 0 && start <= impl->length) {
        if (start == impl->length) {
            return "";
        }

        // Remove String sharing for more performance
        return String(impl->value.get(), start, impl->length - start);
    }
    throw StringIndexOutOfBoundsException(__FILE__, __LINE__, start);
}

////////////////////////////////////////////////////////////////////////////////
String AbstractStringBuilder::substring(int start, int end) const {

    if (start >= 0 && start <= end && end <= impl->length) {
        if (start == end) {
            return "";
        }

        // Remove String sharing for more performance
        return String(impl->value.get(), start, end - start);
    }

    throw StringIndexOutOfBoundsException(__FILE__, __LINE__,
        "Start [%d] or end [%d] index value are invalid.", start, end);
}

////////////////////////////////////////////////////////////////////////////////
CharSequence* AbstractStringBuilder::subSequence(int start, int end) const {
    return new String(substring(start, end));
}

////////////////////////////////////////////////////////////////////////////////
String AbstractStringBuilder::toString() const {

    if (impl->length == 0) {
        return "";
    }

    int wasted = impl->value.length() - 1 - impl->length;
    if (wasted >= 256 || (wasted >= INITIAL_CAPACITY && wasted >= (impl->length >> 1))) {
        return String(impl->value.get(), 0, impl->length);
    }

    impl->shared = true;

    // TODO optimize so that internal data can be shared with the returned String
    //      and discarded only after a new mutating method call is made.
    //      ensure the shared flag is set once we do this.

    return String(impl->value.get(), 0, impl->length);
}

////////////////////////////////////////////////////////////////////////////////
void AbstractStringBuilder::trimToSize() {
    if (impl->length < (impl->value.length() - 1)) {
        ArrayPointer<char> newValue(impl->length + 1);
        System::arraycopy(impl->value.get(), 0, newValue.get(), 0, impl->length);
        impl->value = newValue;
        impl->shared = false;
    }
}
