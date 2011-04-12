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

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::lang;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
namespace decaf{
namespace internal{

    class RuntimeData {
    public:

        mutable apr_pool_t* aprPool;
        Mutex* lock;

    public:

        RuntimeData() : aprPool(NULL), lock(NULL) {
        }

    };

}}

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::DecafRuntime() : runtimeData(new RuntimeData()) {

    // Initializes the APR Runtime from within a library.
    apr_initialize();

    // Create a Global Pool for Threads to use
    apr_pool_create_ex( &runtimeData->aprPool, NULL, NULL, NULL );

    // Create the global Lock object now that the memory pool exists.
    this->runtimeData->lock = new Mutex;
}

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::~DecafRuntime() {

    try{

        // Destory the Global Lock before we deallocate the memory pool.
        delete this->runtimeData->lock;

        // Destroy the Global Thread Memory Pool
        apr_pool_destroy( this->runtimeData->aprPool );

        // Cleans up APR data structures.
        apr_terminate();

        // Destroy the Runtime Data
        delete this->runtimeData;
    }
    DECAF_CATCH_NOTHROW( Exception )
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
apr_pool_t* DecafRuntime::getGlobalPool() const {
    return this->runtimeData->aprPool;
}

////////////////////////////////////////////////////////////////////////////////
Mutex* DecafRuntime::getGlobalLock() {
    return this->runtimeData->lock;
}

////////////////////////////////////////////////////////////////////////////////
Runtime* Runtime::getRuntime() {

    static DecafRuntime runtime;

    return &runtime;
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::initializeRuntime( int argc, char **argv ) {

    // Do this for now, once we remove APR we can do this in a way that
    // makes more sense.
    Runtime::getRuntime();

    // Initialize any Platform specific Threading primitives
    Thread::initThreading();

    // Initialize the System Class to make things like Properties available.
    System::initSystem( argc, argv );

    // Initialize the Networking layer.
    Network::initializeNetworking();
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::initializeRuntime() {
    Runtime::initializeRuntime( 0, NULL );
}

////////////////////////////////////////////////////////////////////////////////
void Runtime::shutdownRuntime() {

    // Shutdown the networking layer before Threading, many network routines need
    // to be thread safe and require Threading primitives.
    Network::shutdownNetworking();

    // Shutdown the System class so that the Properties and other resources are
    // cleaned up.
    System::shutdownSystem();

    // Threading is the last to by shutdown since most other parts of the Runtime
    // need to make use of Thread primitives.
    Thread::shutdownThreading();
}
