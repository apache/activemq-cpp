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

#include "StandardInputStream.h"

#include <iostream>

#include <apr.h>
#include <apr_general.h>
#include <apr_pools.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::io;

////////////////////////////////////////////////////////////////////////////////
StandardInputStream::StandardInputStream() {
}

////////////////////////////////////////////////////////////////////////////////
StandardInputStream::~StandardInputStream() {
}

////////////////////////////////////////////////////////////////////////////////
std::size_t StandardInputStream::available() const throw ( decaf::io::IOException ) {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char StandardInputStream::read() throw ( decaf::io::IOException ) {

    if( !std::cin.good() ) {
        throw decaf::io::IOException(
            __FILE__, __LINE__,
            "Standard Input Stream in Error State." );
    }

    return std::cin.get();
}

////////////////////////////////////////////////////////////////////////////////
int StandardInputStream::read( unsigned char* buffer,
                               std::size_t offset,
                               std::size_t bufferSize )
    throw ( decaf::io::IOException, decaf::lang::exceptions::NullPointerException ) {

    if( buffer == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__,
            "ByteArrayInputStream::read - Buffer passed is Null" );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t StandardInputStream::skip( std::size_t num )
    throw ( decaf::io::IOException,
            decaf::lang::exceptions::UnsupportedOperationException ) {

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void StandardInputStream::reset() throw ( decaf::io::IOException ) {

    throw decaf::io::IOException(
        __FILE__, __LINE__,
        "Mark and Rest not Supported by this class." );
}
