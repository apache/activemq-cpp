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
using namespace decaf::net;

////////////////////////////////////////////////////////////////////////////////
const int SocketOptions::SOCKET_OPTION_TCP_NODELAY = 1;
const int SocketOptions::SOCKET_OPTION_BINDADDR = 2;
const int SocketOptions::SOCKET_OPTION_REUSEADDR = 3;
const int SocketOptions::SOCKET_OPTION_BROADCAST = 4;
const int SocketOptions::SOCKET_OPTION_IP_MULTICAST_IF = 5;
const int SocketOptions::SOCKET_OPTION_IP_MULTICAST_LOOP = 6;
const int SocketOptions::SOCKET_OPTION_IP_MULTICAST_IF2 = 7;
const int SocketOptions::SOCKET_OPTION_IP_TOS = 8;
const int SocketOptions::SOCKET_OPTION_LINGER = 9;
const int SocketOptions::SOCKET_OPTION_TIMEOUT = 10;
const int SocketOptions::SOCKET_OPTION_SNDBUF = 11;
const int SocketOptions::SOCKET_OPTION_RCVBUF = 12;
const int SocketOptions::SOCKET_OPTION_KEEPALIVE = 13;
const int SocketOptions::SOCKET_OPTION_OOBINLINE = 14;

////////////////////////////////////////////////////////////////////////////////
SocketOptions::~SocketOptions() {
}
