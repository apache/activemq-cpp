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

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::DecafRuntime() {

    // Initializes the APR Runtime from within a library.
    apr_initialize();

    // Create a Global Pool for Threads to use
    apr_pool_create_ex( &aprPool, NULL, NULL, NULL );

}

////////////////////////////////////////////////////////////////////////////////
DecafRuntime::~DecafRuntime() {

    // Destroy the Global Thread Memory Pool
    apr_pool_destroy( aprPool );

    // Cleans up APR data structures.
    apr_terminate();
}

////////////////////////////////////////////////////////////////////////////////
apr_pool_t* DecafRuntime::getGlobalPool() const {
    return this->aprPool;
}

////////////////////////////////////////////////////////////////////////////////
Runtime* Runtime::getRuntime() {

    static DecafRuntime runtime;

    return &runtime;
}
