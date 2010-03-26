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

#include "SocketOptions.h"

using namespace decaf;
using namespace net;

////////////////////////////////////////////////////////////////////////////////
const int SocketOptions::IP_MULTICAST_IF = 16;
const int SocketOptions::IP_MULTICAST_IF2 = 31;
const int SocketOptions::IP_MULTICAST_LOOP = 18;
const int SocketOptions::IP_TOS = 3;
const int SocketOptions::SO_BINDADDR = 15;
const int SocketOptions::SO_BROADCAST = 32;
const int SocketOptions::SO_KEEPALIVE = 8;
const int SocketOptions::SO_LINGER = 128;
const int SocketOptions::SO_OOBINLINE = 4099;
const int SocketOptions::SO_RCVBUF = 4098;
const int SocketOptions::SO_REUSEADDR = 4;
const int SocketOptions::SO_SNDBUF = 4097;
const int SocketOptions::SO_TIMEOUT = 4102;
const int SocketOptions::TCP_NODELAY = 1;

////////////////////////////////////////////////////////////////////////////////
SocketOptions::~SocketOptions() {
}
