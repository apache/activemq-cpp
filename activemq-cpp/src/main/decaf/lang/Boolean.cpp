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

#include "Boolean.h"
#include <sstream>
#include <apr.h>

#include <decaf/internal/util/StringUtils.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
const Boolean Boolean::_FALSE(false);
const Boolean Boolean::_TRUE(true);

////////////////////////////////////////////////////////////////////////////////
Boolean::Boolean(bool value) : value(value) {
}

////////////////////////////////////////////////////////////////////////////////
Boolean::Boolean(const std::string& value) : value(false) {
    this->value = parseBoolean(value);
}

////////////////////////////////////////////////////////////////////////////////
int Boolean::compareTo(const Boolean& b) const {
    if (this->value == b.value) {
        return 0;
    } else if (this->value && !b.value) {
        return 1;
    } else {
        return -1;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Boolean::operator==(const Boolean& b) const {
    return this->value == b.value;
}

////////////////////////////////////////////////////////////////////////////////
bool Boolean::operator<(const Boolean& b) const {
    return this->value < b.value;
}

////////////////////////////////////////////////////////////////////////////////
int Boolean::compareTo(const bool& b) const {
    if (this->value == b) {
        return 0;
    } else if (this->value && !b) {
        return 1;
    } else {
        return -1;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Boolean::operator==(const bool& b) const {
    return this->value == b;
}

////////////////////////////////////////////////////////////////////////////////
bool Boolean::operator<(const bool& b) const {
    return this->value < b;
}

////////////////////////////////////////////////////////////////////////////////
std::string Boolean::toString() const {
    return this->value ? "true" : "false";
}

////////////////////////////////////////////////////////////////////////////////
Boolean Boolean::valueOf(const std::string& value) {
    return Boolean(value);
}

////////////////////////////////////////////////////////////////////////////////
Boolean Boolean::valueOf(bool value) {
    return Boolean(value);
}

////////////////////////////////////////////////////////////////////////////////
bool Boolean::parseBoolean(const std::string& value) {
    return StringUtils::compareIgnoreCase(value.c_str(), "true") == 0;
}

////////////////////////////////////////////////////////////////////////////////
std::string Boolean::toString(bool value) {
    return value ? "true" : "false";
}
