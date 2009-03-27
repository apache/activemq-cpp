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

#include "ByteArrayPerspective.h"

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::nio;
using namespace decaf::internal::util;

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( std::size_t capacity ) : ByteArrayAdapter( capacity ){
    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( unsigned char* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( char* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( double* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( float* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( long long* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( int* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}

////////////////////////////////////////////////////////////////////////////////
ByteArrayPerspective::ByteArrayPerspective( short* array, std::size_t capacity, bool own )
    throw( lang::exceptions::NullPointerException ) :
        ByteArrayAdapter( array, capacity, own ) {

    this->references = 1;
}
