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

#include "PrimitiveList.h"

#include <stdio.h>
#include <string.h>

using namespace activemq;
using namespace activemq::util;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList() : converter() {
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList( const decaf::util::List<PrimitiveValueNode>& src )
  : LinkedList<PrimitiveValueNode>( src ), converter(){
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::PrimitiveList( const PrimitiveList& src )
  : LinkedList<PrimitiveValueNode>( src ), converter() {
}

////////////////////////////////////////////////////////////////////////////////
PrimitiveList::~PrimitiveList() {
    clear();
}

////////////////////////////////////////////////////////////////////////////////
std::string PrimitiveList::toString() const {

    ostringstream stream;

    stream << "Begin Class PrimitiveList:" << std::endl;

    for (int i = 0; i < this->size(); ++i) {
        stream << "list[" << i << "] = " << this->get(i).toString() << std::endl;
    }

    stream << "End Class PrimitiveList:" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool PrimitiveList::getBool(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<bool>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setBool(int index, bool value) {

    PrimitiveValueNode node;
    node.setBool(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
unsigned char PrimitiveList::getByte(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<unsigned char>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setByte(int index, unsigned char value) {

    PrimitiveValueNode node;
    node.setByte(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
char PrimitiveList::getChar(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<char>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setChar(int index, char value) {

    PrimitiveValueNode node;
    node.setChar(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
short PrimitiveList::getShort(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<short>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setShort(int index, short value) {

    PrimitiveValueNode node;
    node.setShort(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
int PrimitiveList::getInt(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<int>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setInt(int index, int value) {

    PrimitiveValueNode node;
    node.setInt(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
long long PrimitiveList::getLong(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<long long>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setLong(int index, long long value) {

    PrimitiveValueNode node;
    node.setLong(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
double PrimitiveList::getDouble(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<double>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setDouble(int index, double value) {

    PrimitiveValueNode node;
    node.setDouble(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
float PrimitiveList::getFloat(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<float>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setFloat(int index, float value) {

    PrimitiveValueNode node;
    node.setFloat(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
string PrimitiveList::getString(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<std::string>(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setString(int index, const string& value) {

    PrimitiveValueNode node;
    node.setString(value);

    this->set(index, node);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned char> PrimitiveList::getByteArray(int index) const {

    PrimitiveValueNode node = this->get(index);
    return converter.convert<std::vector<unsigned char> >(node);
}

////////////////////////////////////////////////////////////////////////////////
void PrimitiveList::setByteArray(int index, const std::vector<unsigned char>& value) {

    PrimitiveValueNode node;
    node.setByteArray(value);

    this->set(index, node);
}
