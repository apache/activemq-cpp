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

#include "Float.h"

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const float Float::MAX_VALUE = 3.40282347e+38F;
const float Float::MIN_VALUE = 1.17549435e-38F;

////////////////////////////////////////////////////////////////////////////////
Float::Float( float value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Float::Float( double value ) {
    this->value = (float)value;
}

////////////////////////////////////////////////////////////////////////////////
Float::Float( const std::string& value ) throw( exceptions::NumberFormatException ) {
    //TODO this->value = Float::parseFloat( value );
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const Float& f ) const {
    // TODO
    return this->value < f.value ? -1 : this->value == f.value ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
int Float::compareTo( const float& f ) const {
    // TODO
    return this->value < f ? -1 : this->value == f ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
std::string Float::toString() const {
    return ""; //TODO Float::toString( this->value, 10 );
}
