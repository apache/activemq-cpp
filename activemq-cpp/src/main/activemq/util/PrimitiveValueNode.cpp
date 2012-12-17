/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PrimitiveValueNode.h"

#include <activemq/util/PrimitiveList.h>
#include <activemq/util/PrimitiveMap.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/LinkedList.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using namespace std;
using namespace activemq;
using namespace activemq::util;

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode() : valueType(NULL_TYPE), value() {
    memset(&value, 0, sizeof(value));
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(bool value) : valueType(NULL_TYPE), value() {
    this->setBool(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(unsigned char value) : valueType(NULL_TYPE), value() {
    this->setByte(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(char value) : valueType(NULL_TYPE), value() {
    this->setChar(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(short value) : valueType(NULL_TYPE), value() {
    this->setShort(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(int value) : valueType(NULL_TYPE), value() {
    this->setInt(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(long long value) : valueType(NULL_TYPE), value() {
    this->setLong(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(float value) : valueType(NULL_TYPE), value() {
    this->setFloat(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(double value) : valueType(NULL_TYPE), value() {
    this->setDouble(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const char* value) : valueType(NULL_TYPE), value() {
    if (value != NULL) {
        this->setString(string(value));
    }
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const std::string& value) : valueType(NULL_TYPE), value() {
    this->setString(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const std::vector<unsigned char>& value) : valueType(NULL_TYPE), value() {
    this->setByteArray(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const decaf::util::List<PrimitiveValueNode>& value) : valueType(NULL_TYPE), value() {
    this->setList(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const decaf::util::Map<std::string, PrimitiveValueNode>& value) : valueType(NULL_TYPE), value() {
    this->setMap(value);
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode::PrimitiveValueNode(const PrimitiveValueNode& node) : valueType(NULL_TYPE), value() {
    (*this) = node;
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveValueNode& PrimitiveValueNode::operator =(const PrimitiveValueNode& node) {
    clear();
    this->setValue(node.getValue(), node.getType());
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool PrimitiveValueNode::operator==(const PrimitiveValueNode& node) const {

    if (valueType != node.valueType) {
        return false;
    }

    if (valueType == BOOLEAN_TYPE && value.boolValue == node.value.boolValue) {
        return true;
    } else if (valueType == BYTE_TYPE && value.byteValue == node.value.byteValue) {
        return true;
    } else if (valueType == CHAR_TYPE && value.charValue == node.value.charValue) {
        return true;
    } else if (valueType == SHORT_TYPE && value.shortValue == node.value.shortValue) {
        return true;
    } else if (valueType == INTEGER_TYPE && value.intValue == node.value.intValue) {
        return true;
    } else if (valueType == LONG_TYPE && value.longValue == node.value.longValue) {
        return true;
    } else if (valueType == DOUBLE_TYPE && value.doubleValue == node.value.doubleValue) {
        return true;
    } else if (valueType == FLOAT_TYPE && value.floatValue == node.value.floatValue) {
        return true;
    } else if (valueType == STRING_TYPE && *value.stringValue == *node.value.stringValue) {
        return true;
    } else if (valueType == BYTE_ARRAY_TYPE && *value.byteArrayValue == *node.value.byteArrayValue) {
        return true;
    } else if (valueType == LIST_TYPE && value.listValue->equals(*node.value.listValue)) {
        return true;
    } else if (valueType == MAP_TYPE && value.mapValue->equals(*node.value.mapValue)) {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::clear() {

    if (valueType == STRING_TYPE && value.stringValue != NULL) {
        delete value.stringValue;
    } else if (valueType == BYTE_ARRAY_TYPE && value.byteArrayValue != NULL) {
        delete value.byteArrayValue;
    } else if (valueType == LIST_TYPE && value.listValue != NULL) {
        delete value.listValue;
    } else if (valueType == MAP_TYPE && value.mapValue != NULL) {
        delete value.mapValue;
    }

    valueType = NULL_TYPE;
    memset(&value, 0, sizeof(value));
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setValue(const PrimitiveValue& value, PrimitiveType valueType) {

    if (valueType == BOOLEAN_TYPE) {
        this->setBool(value.boolValue);
    } else if (valueType == BYTE_TYPE) {
        this->setByte(value.byteValue);
    } else if (valueType == CHAR_TYPE) {
        this->setChar(value.charValue);
    } else if (valueType == SHORT_TYPE) {
        this->setShort(value.shortValue);
    } else if (valueType == INTEGER_TYPE) {
        this->setInt(value.intValue);
    } else if (valueType == LONG_TYPE) {
        this->setLong(value.longValue);
    } else if (valueType == DOUBLE_TYPE) {
        this->setDouble(value.doubleValue);
    } else if (valueType == FLOAT_TYPE) {
        this->setFloat(value.floatValue);
    } else if (valueType == STRING_TYPE || valueType == BIG_STRING_TYPE) {
        this->setString(*value.stringValue);
    } else if (valueType == BYTE_ARRAY_TYPE) {
        this->setByteArray(*value.byteArrayValue);
    } else if (valueType == LIST_TYPE) {
        this->setList(*value.listValue);
    } else if (valueType == MAP_TYPE) {
        this->setMap(*value.mapValue);
    } else {
        this->clear();
    }
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setBool(bool lvalue) {
    clear();
    valueType = BOOLEAN_TYPE;
    value.boolValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
bool PrimitiveValueNode::getBool() const {

    if (valueType != BOOLEAN_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not BOOLEAN_TYPE");
    }

    return value.boolValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setByte(unsigned char lvalue) {
    clear();
    valueType = BYTE_TYPE;
    value.byteValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char PrimitiveValueNode::getByte() const {

    if (valueType != BYTE_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not BYTE_TYPE");
    }

    return value.byteValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setChar(char lvalue) {
    clear();
    valueType = CHAR_TYPE;
    value.charValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
char PrimitiveValueNode::getChar() const {

    if (valueType != CHAR_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not CHAR_TYPE");
    }

    return value.charValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setShort(short lvalue) {
    clear();
    valueType = SHORT_TYPE;
    value.shortValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
short PrimitiveValueNode::getShort() const {

    if (valueType != SHORT_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not SHORT_TYPE");
    }

    return value.shortValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setInt(int lvalue) {
    clear();
    valueType = INTEGER_TYPE;
    value.intValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
int PrimitiveValueNode::getInt() const {

    if (valueType != INTEGER_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not INTEGER_TYPE");
    }

    return value.intValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setLong(long long lvalue) {
    clear();
    valueType = LONG_TYPE;
    value.longValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
long long PrimitiveValueNode::getLong() const {

    if (valueType != LONG_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not LONG_TYPE");
    }

    return value.longValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setDouble(double lvalue) {
    clear();
    valueType = DOUBLE_TYPE;
    value.doubleValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
double PrimitiveValueNode::getDouble() const {

    if (valueType != DOUBLE_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not DOUBLE_TYPE");
    }

    return value.doubleValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setFloat(float lvalue) {
    clear();
    valueType = FLOAT_TYPE;
    value.floatValue = lvalue;
}

////////////////////////////////////////////////////////////////////////////////
float PrimitiveValueNode::getFloat() const {

    if (valueType != FLOAT_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not FLOAT_TYPE");
    }

    return value.floatValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setString(const std::string& lvalue) {
    clear();
    valueType = STRING_TYPE;
    value.stringValue = new std::string(lvalue);
}

////////////////////////////////////////////////////////////////////////////////
std::string PrimitiveValueNode::getString() const {

    if (valueType != STRING_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not STRING_TYPE");
    }

    if (value.stringValue == NULL) {
        return std::string();
    }

    return *value.stringValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setByteArray(const std::vector<unsigned char>& lvalue) {
    clear();
    valueType = BYTE_ARRAY_TYPE;
    value.byteArrayValue = new std::vector<unsigned char>(lvalue);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> PrimitiveValueNode::getByteArray() const {

    if (valueType != BYTE_ARRAY_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not BYTE_ARRAY_TYPE");
    }

    if (value.byteArrayValue == NULL) {
        return std::vector<unsigned char>();
    }

    return *value.byteArrayValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setList(const decaf::util::List<PrimitiveValueNode>& lvalue) {
    clear();
    valueType = LIST_TYPE;
    value.listValue = new decaf::util::LinkedList<PrimitiveValueNode>();
    value.listValue->copy(lvalue);
}

////////////////////////////////////////////////////////////////////////////////
const decaf::util::List<PrimitiveValueNode>& PrimitiveValueNode::getList() const {

    if (valueType != LIST_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not LIST_TYPE");
    }

    if (value.listValue == NULL) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not set but an element was placed in the Map");
    }

    return *value.listValue;
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveValueNode::setMap(const decaf::util::Map<std::string, PrimitiveValueNode>& lvalue) {

    clear();
    valueType = MAP_TYPE;
    value.mapValue = new decaf::util::StlMap<std::string, PrimitiveValueNode>(lvalue);
}

////////////////////////////////////////////////////////////////////////////////
const decaf::util::Map<std::string, PrimitiveValueNode>& PrimitiveValueNode::getMap() const {

    if (valueType != MAP_TYPE) {
        throw decaf::util::NoSuchElementException(__FILE__, __LINE__, "PrimitiveValue is not MAP_TYPE");
    }

    if (value.mapValue == NULL) {
        throw decaf::lang::exceptions::NullPointerException(__FILE__, __LINE__, "PrimitiveValue is not set but an element was placed in the Map");
    }

    return *value.mapValue;
}

////////////////////////////////////////////////////////////////////////////////
std::string PrimitiveValueNode::toString() const {
    std::ostringstream stream;

    if (valueType == BOOLEAN_TYPE) {
        stream << std::boolalpha << value.boolValue;
    } else if (valueType == BYTE_TYPE) {
        stream << value.byteValue;
    } else if (valueType == CHAR_TYPE) {
        stream << value.charValue;
    } else if (valueType == SHORT_TYPE) {
        stream << value.shortValue;
    } else if (valueType == INTEGER_TYPE) {
        stream << value.intValue;
    } else if (valueType == LONG_TYPE) {
        stream << value.longValue;
    } else if (valueType == DOUBLE_TYPE) {
        stream << value.doubleValue;
    } else if (valueType == FLOAT_TYPE) {
        stream << value.floatValue;
    } else if (valueType == STRING_TYPE || valueType == BIG_STRING_TYPE) {
        stream << *value.stringValue;
    } else if (valueType == BYTE_ARRAY_TYPE) {
        std::vector<unsigned char>::const_iterator iter = value.byteArrayValue->begin();
        for (; iter != value.byteArrayValue->end(); ++iter) {
            stream << '[' << (int) (*iter) << ']';
        }
    } else if (valueType == LIST_TYPE) {
        stream << PrimitiveList(*value.listValue).toString();
    } else if (valueType == MAP_TYPE) {
        stream << PrimitiveMap(*value.mapValue).toString();
    }
    return stream.str();
}
