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

#include "Buffer.h"

#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
Buffer::Buffer( std::size_t capacity ) {
    this->_capacity = capacity;
    this->_limit = capacity;
    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
}

////////////////////////////////////////////////////////////////////////////////
Buffer::Buffer( const Buffer& other ) {

    this->_capacity = other._capacity;
    this->_limit = other._limit;
    this->_position = other._position;
    this->_mark = other._mark;
    this->_markSet = other._markSet;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::position( std::size_t newPosition )
    throw( lang::exceptions::IllegalArgumentException ) {

    if( newPosition > this->_limit ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Buffer::position - New Position is greater than set limit" );
    }

    this->_position = newPosition;
    if( this->_markSet && ( this->_mark > newPosition ) ) {
        this->_mark = 0;
        this->_markSet = false;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::limit( std::size_t newLimit ) throw( IllegalArgumentException ) {

    if( newLimit > this->capacity() ) {
        throw IllegalArgumentException(
            __FILE__, __LINE__,
            "Buffer::limit - new limit is larger than the capacity." );
    }

    this->_limit = newLimit;
    if( this->_position > newLimit ) {
        this->_position = newLimit;
    }

    if( this->_markSet && ( this->_mark > newLimit ) ) {
        this->_mark = 0;
        this->_markSet = false;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::mark() {

    this->_mark = this->_position;
    this->_markSet = true;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::reset() throw ( InvalidMarkException ) {

    if( !this->_markSet ) {
        throw InvalidMarkException(
            __FILE__, __LINE__,
            "Buffer::reset - Buffer has not been Marked." );
    }

    this->_position = this->_mark;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::clear() {

    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    this->_limit = this->capacity();
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::flip() {

    this->_limit = this->_position;
    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
Buffer& Buffer::rewind() {

    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    return *this;
}
