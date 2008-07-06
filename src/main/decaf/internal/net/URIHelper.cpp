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

#include "URIHelper.h"

#include <decaf/lang/Integer.h>
#include <decaf/lang/Character.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
URIHelper::URIHelper() {
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidIP4Word( const std::string& word ) {

    if( word.length() < 1 || word.length() > 3 ) {
        return false;
    }

    for( std::size_t i = 0; i < word.length(); i++ ) {

        if( !Character::isDigit( word.at( i ) ) ) {
            return false;
        }
    }

    if( Integer::parseInt( word ) > 255 ) {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool URIHelper::isValidHexChar( char c ) {

    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}
