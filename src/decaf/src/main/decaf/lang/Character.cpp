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

#include <decaf/lang/Character.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
Character::Character( char value ) {
    this->value = value;
}

////////////////////////////////////////////////////////////////////////////////
std::string Character::toString() const {
    return string( 1, this->value );
}

////////////////////////////////////////////////////////////////////////////////
int Character::digit( char c, int radix ) {
    if( radix >= MIN_RADIX && radix <= MAX_RADIX ) {
        int result = -1;
        if( '0' <= c && c <= '9' ) {
            result = c - '0';
        } else if( 'a' <= c && c <= 'z' ) {
            result = c - ('a' - 10);
        } else if( 'A' <= c && c <= 'Z' ) {
            result = c - ('A' - 10);
        }
        return result < radix ? result : -1;
    }
    return -1;
}
