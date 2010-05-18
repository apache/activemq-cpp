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

#include "OpenSSLServerSocket.h"

#ifdef HAVE_OPENSSL
    #include <openssl/ssl.h>
    #include <openssl/x509.h>
    #include <openssl/x509v3.h>
    #include <openssl/bio.h>
#endif

#include <decaf/net/SocketImpl.h>
#include <decaf/io/IOException.h>
#include <decaf/net/SocketException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IndexOutOfBoundsException.h>
#include <decaf/internal/net/SocketFileDescriptor.h>
#include <decaf/internal/net/ssl/openssl/OpenSSLSocketException.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::net;
//using namespace decaf::net::ssl;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocket::OpenSSLServerSocket() {
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLServerSocket::~OpenSSLServerSocket() {
}

