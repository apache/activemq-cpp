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

#include "DecafRuntime.h"

#include <apr.h>
#include <apr_general.h>
#include <apr_pools.h>

#include <decaf/lang/System.h>
#include <decaf/lang/Thread.h>
#include <decaf/internal/net/Network.h>
#include <decaf/internal/security/SecurityRuntime.h>
#include <decaf/internal/util/concurrent/Threading.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::security;
using namespace decaf::internal::util::concurrent;
using namespace decaf::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace {
    apr_pool_t* aprPool;
    Mutex* globalLock;
}

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::DecafRuntime() : decaf::lang::Runtime() {
}

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::~DecafRuntime() {
}

////////////////////////////////////////////////////////////////////////////////
apr_pool_t* DecafRuntime::getGlobalPool() const {
    return aprPool;
}

////////////////////////////////////////////////////////////////////////////////
Mutex* DecafRuntime::getGlobalLock() {
    return globalLock;
}

////////////////////////////////////////////////////////////////////////////////
Runtime* Runtime::getRuntime() {
    static DecafRuntime runtime;
    return &runtime;
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::initializeRuntime(int argc, char **argv) {

    // Initializes the APR Runtime from within a library.
    apr_initialize();
    apr_pool_create_ex(&aprPool, NULL, NULL, NULL);

    Runtime::getRuntime();
    Threading::initialize();

    globalLock = new Mutex;

    System::initSystem(argc, argv);
    Network::initializeNetworking();
    SecurityRuntime::initializeSecurity();
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::initializeRuntime() {
    Runtime::initializeRuntime(0, NULL);
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::shutdownRuntime() {

    SecurityRuntime::shutdownSecurity();

    // Shutdown the networking layer before Threading, many network routines need
    // to be thread safe and require Threading primitives.
    Network::shutdownNetworking();

    System::shutdownSystem();

    // This must go away before Threading is shutdown.
    delete globalLock;

    // Threading is the last to by shutdown since most other parts of the Runtime
    // need to make use of Thread primitives.
    Threading::shutdown();

    // Cleans up APR data structures.
    apr_pool_destroy(aprPool);
    apr_terminate();
}
