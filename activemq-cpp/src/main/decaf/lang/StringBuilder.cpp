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

#include <decaf/lang/StringBuilder.h>

#include <decaf/lang/exceptions/NegativeArraySizeException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

#include <decaf/lang/StringBuffer.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
StringBuilder::StringBuilder() : AbstractStringBuilder() {
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder::StringBuilder(int capacity) : AbstractStringBuilder(capacity) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder::StringBuilder(const String& source) : AbstractStringBuilder(source) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder::StringBuilder(const CharSequence* source) : AbstractStringBuilder(source) {
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder::~StringBuilder() {
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(bool value) {
    doAppend(value ? "true" : "false");
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(char value) {
    doAppend(value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(short value) {
    // TODO optimize this for direct buffer access and no temporaries.
    doAppend(Short::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(int value) {
    // TODO optimize this for direct buffer access and no temporaries.
    doAppend(Integer::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(long long value) {
    // TODO optimize this for direct buffer access and no temporaries.
    doAppend(Long::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(float value) {
    // TODO optimize this for direct buffer access and no temporaries.
    doAppend(Float::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(double value) {
    // TODO optimize this for direct buffer access and no temporaries.
    doAppend(Double::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const char* value) {
    doAppend(value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const char* value, int offset, int length) {
    doAppend(value, offset, length);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const CharSequence* value) {
    if (value == NULL) {
        doAppendNull();
    } else {
        doAppend(value);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const CharSequence* value, int offset, int length) {
    doAppend(value, offset, length);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const String& value) {
    doAppend(value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::append(const StringBuffer& value) {
    doAppend(value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::deleteRange(int start, int end) {
    doDeleteRange(start, end);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::deleteCharAt(int index) {
    doDeleteCharAt(index);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, char value) {
    doInsert(index, value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, bool value) {
    doInsert(index, (value ? "true" : "false"));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, short value) {
    doInsert(index, Short::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, int value) {
    doInsert(index, Integer::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, long long value) {
    doInsert(index, Long::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, float value) {
    doInsert(index, Float::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, double value) {
    doInsert(index, Double::toString(value));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const char* value) {
    doInsert(index, value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const String& value) {
    doInsert(index, value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const std::string& value) {
    doInsert(index, value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const char* value, int offset, int length) {
    doInsert(index, value, offset, length);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const CharSequence* value) {

    if (value == NULL) {
        doAppendNull();
    } else {
        doInsert(index, value);
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::insert(int index, const CharSequence* value, int offset, int length) {
    doInsert(index, value, offset, length);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
int StringBuilder::length() const {
    return AbstractStringBuilder::length();
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::replace(int start, int end, const String& value) {
    doReplace(start, end, value);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
StringBuilder& StringBuilder::reverse() {
    doReverse();
    return *this;
}
