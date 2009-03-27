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

#include "URLEncoder.h"
#include <decaf/lang/Character.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
const std::string URLEncoder::digits = "0123456789ABCDEF";

////////////////////////////////////////////////////////////////////////////////
URLEncoder::URLEncoder() {
}

////////////////////////////////////////////////////////////////////////////////
std::string URLEncoder::encode( const std::string& src ) {

    string encoded = "";
    string allowables = ".-*_";

    for( std::size_t i = 0; i < src.length(); i++ ) {

        char ch = src.at(i);
        if( Character::isLetterOrDigit( ch ) ||
            allowables.find_first_of( ch, 0 ) != std::string::npos ) {

            encoded += ch;
        } else if (ch == ' ') {
            encoded += '+';
        } else {
            encoded += '%';
            encoded += digits.at( (ch & 0xf0) >> 4 );
            encoded += digits.at( ch & 0xf );
        }
    }

    return encoded;
}
