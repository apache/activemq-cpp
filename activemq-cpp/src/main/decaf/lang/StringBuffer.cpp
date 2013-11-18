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

#include <decaf/lang/StringBuffer.h>

#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <decaf/lang/StringBuilder.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer() : AbstractStringBuilder() {
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(int capacity) : AbstractStringBuilder(capacity) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(const String& source) : AbstractStringBuilder(source) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(const CharSequence* source) : AbstractStringBuilder(source) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer::~StringBuffer() {
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::capacity() const {
    int capacity = 0;

    synchronized(&this->mutex) {
        capacity = AbstractStringBuilder::capacity();
    }

    return capacity;
}

////////////////////////////////////////////////////////////////////////////////
char StringBuffer::charAt(int index) const {

    char result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::charAt(index);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void StringBuffer::ensureCapacity(int minCapacity) {
    synchronized(&this->mutex) {
        AbstractStringBuilder::ensureCapacity(minCapacity);
    }
}

////////////////////////////////////////////////////////////////////////////////
void StringBuffer::getChars(int start, int end, char* dst, int dstSize, int dstStart) const {
    synchronized(&this->mutex) {
        AbstractStringBuilder::getChars(start, end, dst, dstSize, dstStart);
    }
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::indexOf(const String& value) const {

    int result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::indexOf(value);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::indexOf(const String& value, int start) const {

    int result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::indexOf(value, start);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::lastIndexOf(const String& value) const {

    int result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::lastIndexOf(value);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::lastIndexOf(const String& value, int start) const {

    int result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::lastIndexOf(value, start);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
int StringBuffer::length() const {

    int result = 0;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::length();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void StringBuffer::setLength(int length) {
    synchronized(&this->mutex) {
        AbstractStringBuilder::setLength(length);
    }
}

////////////////////////////////////////////////////////////////////////////////
void StringBuffer::setCharAt(int index, char value) {
    synchronized(&this->mutex) {
        AbstractStringBuilder::setCharAt(index, value);
    }
}

////////////////////////////////////////////////////////////////////////////////
String StringBuffer::substring(int start) const {

    String result;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::substring(start);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
String StringBuffer::substring(int start, int end) const {

    String result;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::substring(start, end);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
CharSequence* StringBuffer::subSequence(int start, int end) const {

    CharSequence* result = NULL;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::subSequence(start, end);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
String StringBuffer::toString() const {

    String result;

    synchronized(&this->mutex) {
        result = AbstractStringBuilder::toString();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
void StringBuffer::trimToSize() {
    synchronized(&this->mutex) {
        AbstractStringBuilder::trimToSize();
    }
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(bool value) {
    synchronized(&this->mutex) {
        doAppend(value ? "true" : "false");
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(char value) {
    synchronized(&this->mutex) {
        doAppend(value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(short value) {
    // TODO optimize this for direct buffer access and no temporaries.
    synchronized(&this->mutex) {
        doAppend(Short::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(int value) {
    // TODO optimize this for direct buffer access and no temporaries.
    synchronized(&this->mutex) {
        doAppend(Integer::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(long long value) {
    // TODO optimize this for direct buffer access and no temporaries.
    synchronized(&this->mutex) {
        doAppend(Long::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(float value) {
    // TODO optimize this for direct buffer access and no temporaries.
    synchronized(&this->mutex) {
        doAppend(Float::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(double value) {
    // TODO optimize this for direct buffer access and no temporaries.
    synchronized(&this->mutex) {
        doAppend(Double::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const char* value) {
    synchronized(&this->mutex) {
        doAppend(value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const char* value, int offset, int length) {
    synchronized(&this->mutex) {
        doAppend(value, offset, length);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const CharSequence* value) {
    synchronized(&this->mutex) {
        if (value == NULL) {
            doAppendNull();
        } else {
            doAppend(value);
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const CharSequence* value, int offset, int length) {
    synchronized(&this->mutex) {
        doAppend(value, offset, length);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const String& value) {
    synchronized(&this->mutex) {
        doAppend(value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::append(const StringBuilder& value) {
    synchronized(&this->mutex) {
        doAppend(value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::deleteRange(int start, int end) {
    synchronized(&this->mutex) {
        doDeleteRange(start, end);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::deleteCharAt(int index) {
    synchronized(&this->mutex) {
        doDeleteCharAt(index);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, char value) {
    synchronized(&this->mutex) {
        doInsert(index, value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, bool value) {
    synchronized(&this->mutex) {
        doInsert(index, (value ? "true" : "false"));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, short value) {
    synchronized(&this->mutex) {
        doInsert(index, Short::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, int value) {
    synchronized(&this->mutex) {
        doInsert(index, Integer::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, long long value) {
    synchronized(&this->mutex) {
        doInsert(index, Long::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, float value) {
    synchronized(&this->mutex) {
        doInsert(index, Float::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, double value) {
    synchronized(&this->mutex) {
        doInsert(index, Double::toString(value));
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const char* value) {
    synchronized(&this->mutex) {
        doInsert(index, value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const String& value) {
    synchronized(&this->mutex) {
        doInsert(index, value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const std::string& value) {
    synchronized(&this->mutex) {
        doInsert(index, value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const char* value, int offset, int length) {
    synchronized(&this->mutex) {
        doInsert(index, value, offset, length);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const CharSequence* value) {

    synchronized(&this->mutex) {
        if (value == NULL) {
            doAppendNull();
        } else {
            doInsert(index, value);
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::insert(int index, const CharSequence* value, int offset, int length) {
    synchronized(&this->mutex) {
        doInsert(index, value, offset, length);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::replace(int start, int end, const String& value) {
    synchronized(&this->mutex) {
        doReplace(start, end, value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuffer& StringBuffer::reverse() {
    synchronized(&this->mutex) {
        doReverse();
    }
    return *this;
}
