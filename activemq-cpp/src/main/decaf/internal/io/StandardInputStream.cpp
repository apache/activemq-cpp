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
int StandardInputStream::available() const {
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
int StandardInputStream::doReadByte() {

    if( !std::cin.good() ) {
        throw decaf::io::IOException(
            __FILE__, __LINE__,
            "Standard Input Stream in Error State." );
    }

    return std::cin.get();
}
