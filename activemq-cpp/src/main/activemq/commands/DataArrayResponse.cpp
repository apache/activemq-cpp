/*
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

#include <activemq/commands/DataArrayResponse.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command code for OpenWire format for DataArrayResponse
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
DataArrayResponse::DataArrayResponse() :
    Response(), data() {

}

////////////////////////////////////////////////////////////////////////////////
DataArrayResponse::~DataArrayResponse() {
}

////////////////////////////////////////////////////////////////////////////////
DataArrayResponse* DataArrayResponse::cloneDataStructure() const {
    std::auto_ptr<DataArrayResponse> dataArrayResponse(new DataArrayResponse());

    // Copy the data from the base class or classes
    dataArrayResponse->copyDataStructure(this);

    return dataArrayResponse.release();
}

////////////////////////////////////////////////////////////////////////////////
void DataArrayResponse::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const DataArrayResponse* srcPtr = dynamic_cast<const DataArrayResponse*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "DataArrayResponse::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    Response::copyDataStructure(src);

    this->setData(srcPtr->getData());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DataArrayResponse::getDataStructureType() const {
    return DataArrayResponse::ID_DATAARRAYRESPONSE;
}

////////////////////////////////////////////////////////////////////////////////
std::string DataArrayResponse::toString() const {

    ostringstream stream;

    stream << "DataArrayResponse { ";
    stream << "Data = ";
    if (this->getData().size() > 0) {
        stream << "[";
        for (size_t idata = 0; idata < this->getData().size(); ++idata) {
            if (this->getData()[idata] != NULL) {
                stream << this->getData()[idata]->toString() << ", ";
            } else {
                stream << "NULL" << ", ";
            }
        }
        stream << "]";
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool DataArrayResponse::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const DataArrayResponse* valuePtr = dynamic_cast<const DataArrayResponse*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    for (size_t idata = 0; idata < this->getData().size(); ++idata) {
        if (this->getData()[idata] != NULL ) {
            if (!this->getData()[idata]->equals( valuePtr->getData()[idata].get())) {
                return false;
            }
        } else if (valuePtr->getData()[idata] != NULL) {
            return false;
        }
    }
    if (!Response::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector< decaf::lang::Pointer<DataStructure> >& DataArrayResponse::getData() const {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
std::vector< decaf::lang::Pointer<DataStructure> >& DataArrayResponse::getData() {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
void DataArrayResponse::setData(const std::vector< decaf::lang::Pointer<DataStructure> >& data) {
    this->data = data;
}

