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

#include "SSLServerSocket.h"

#include <decaf/io/IOException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

using namespace decaf;
using namespace decaf::net;
using namespace decaf::net::ssl;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
SSLServerSocket::SSLServerSocket() {
}

////////////////////////////////////////////////////////////////////////////////
SSLServerSocket::SSLServerSocket( int port ) : ServerSocket( port ) {
}

////////////////////////////////////////////////////////////////////////////////
SSLServerSocket::SSLServerSocket( int port, int backlog ) : ServerSocket( port, backlog ) {
}

////////////////////////////////////////////////////////////////////////////////
SSLServerSocket::SSLServerSocket( int port, int backlog, const InetAddress* address ) : ServerSocket( port, backlog, address ) {
}

////////////////////////////////////////////////////////////////////////////////
SSLServerSocket::~SSLServerSocket() {
}
