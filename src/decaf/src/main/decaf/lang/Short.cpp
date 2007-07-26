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

#include "Short.h"

using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Short::Short( short value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
Short::Short( const std::string& value ) {
    this->value = value.size(); //Short::parseShort( value );
}

////////////////////////////////////////////////////////////////////////////////
int Short::compareTo( const Short& b ) const {
    return value == b.value ? 0 : ( value > b.value ? 1 : -1 );
}
