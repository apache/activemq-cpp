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
#include <activemq/commands/DataResponse.h>
#include <activemq/state/CommandVisitor.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command and marshaling code for OpenWire format for DataResponse
 *
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-core module
 *
 */
////////////////////////////////////////////////////////////////////////////////
DataResponse::DataResponse() {

}

////////////////////////////////////////////////////////////////////////////////
DataResponse::~DataResponse() {

}

////////////////////////////////////////////////////////////////////////////////
DataResponse* DataResponse::cloneDataStructure() const {
    std::auto_ptr<DataResponse> dataResponse( new DataResponse() );

    // Copy the data from the base class or classes
    dataResponse->copyDataStructure( this );

    return dataResponse.release();
}

////////////////////////////////////////////////////////////////////////////////
void DataResponse::copyDataStructure( const DataStructure* src ) {

    // Protect against invalid self assignment.
    if( this == src ) {
        return;
    }

    const DataResponse* srcPtr = dynamic_cast<const DataResponse*>( src );

    if( srcPtr == NULL || src == NULL ) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "DataResponse::copyDataStructure - src is NULL or invalid" );
    }

    // Copy the data of the base class or classes
    Response::copyDataStructure( src );

    this->setData( srcPtr->getData() );
}

////////////////////////////////////////////////////////////////////////////////
unsigned char DataResponse::getDataStructureType() const {
    return DataResponse::ID_DATARESPONSE;
}

////////////////////////////////////////////////////////////////////////////////
std::string DataResponse::toString() const {

    ostringstream stream;

    stream << "Begin Class = DataResponse" << std::endl;
    stream << " Value of DataResponse::ID_DATARESPONSE = 32" << std::endl;
    stream << " Value of Data is Below:" << std::endl;
    if( this->getData() != NULL ) {
        stream << this->getData()->toString() << std::endl;
    } else {
        stream << "   Object is NULL" << std::endl;
    }
    stream << Response::toString();
    stream << "End Class = DataResponse" << std::endl;

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool DataResponse::equals( const DataStructure* value ) const {

    if( this == value ) {
        return true;
    }

    const DataResponse* valuePtr = dynamic_cast<const DataResponse*>( value );

    if( valuePtr == NULL || value == NULL ) {
        return false;
    }

    if( this->getData() != NULL ) {
        if( !this->getData()->equals( valuePtr->getData().get() ) ) {
            return false;
        }
    } else if( valuePtr->getData() != NULL ) {
        return false;
    }
    if( !Response::equals( value ) ) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<DataStructure>& DataResponse::getData() const {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<DataStructure>& DataResponse::getData() {
    return data;
}

////////////////////////////////////////////////////////////////////////////////
void DataResponse::setData( const decaf::lang::Pointer<DataStructure>& data ) {
    this->data = data;
}

