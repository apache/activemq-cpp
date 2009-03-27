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

#include "URLDecoder.h"
#include <decaf/lang/Character.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
URLDecoder::URLDecoder() {
}

////////////////////////////////////////////////////////////////////////////////
std::string URLDecoder::decode( const std::string& src ) {

    std::string decoded = "";

    for( size_t i = 0; i < src.length(); ) {
        char c = src.at(i);

        if (c == '+') {
            decoded += ' ';
        } else if (c == '%') {

            do {

                if( i + 2 >= src.length() ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "Incomplete % sequence at: %d.", i );
                }

                int d1 = Character::digit( src.at(i + 1), 16 );
                int d2 = Character::digit( src.at(i + 2), 16 );

                if( d1 == -1 || d2 == -1 ) {
                    throw IllegalArgumentException(
                        __FILE__, __LINE__,
                        "Invalid % sequence (%s) at: %d.",
                        src.substr( i, 3 ).c_str(), i );
                }

                decoded += (char)( ( d1 << 4 ) + d2 );
                i += 3;
            } while( i < src.length() && src.at(i) == '%' );

            continue;
        } else {
            decoded += c;
        }

        i++;
    }

    return decoded;
}
