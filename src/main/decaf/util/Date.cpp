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

#include <decaf/util/Date.h>
#include <decaf/util/Config.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/System.h>

using namespace std;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Date::Date(){
    time = System::currentTimeMillis();
}

////////////////////////////////////////////////////////////////////////////////
Date::Date( long long milliseconds ){
    this->time = milliseconds;
}

////////////////////////////////////////////////////////////////////////////////
Date::Date( const Date& source ){
    (*this) = source;
}

////////////////////////////////////////////////////////////////////////////////
Date::~Date() {
}

////////////////////////////////////////////////////////////////////////////////
long long Date::getTime() const{
    return time;
}

////////////////////////////////////////////////////////////////////////////////
void Date::setTime( long long milliseconds ){
    this->time = milliseconds;
}

////////////////////////////////////////////////////////////////////////////////
bool Date::after( const Date& when ) const {
    return time > when.time;
}

////////////////////////////////////////////////////////////////////////////////
bool Date::before( const Date& when ) const {
    return time < when.time;
}

////////////////////////////////////////////////////////////////////////////////
Date& Date::operator= ( const Date& source ) {
    this->time = source.time;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool Date::equals( const Date& when ) const {
    return time == when.time;
}

////////////////////////////////////////////////////////////////////////////////
int Date::compareTo( const Date& value ) const {

    if( this->time < value.time ) {
        return -1;
    } else if( this->time > value.time ) {
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
bool Date::operator==( const Date& value ) const {
    return ( this->time == value.time );
}

////////////////////////////////////////////////////////////////////////////////
bool Date::operator<( const Date& value ) const {
    return ( this->time < value.time );
}
