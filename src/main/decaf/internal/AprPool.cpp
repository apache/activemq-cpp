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

#include "AprPool.h"
#include <decaf/internal/DecafRuntime.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::internal;

////////////////////////////////////////////////////////////////////////////////
AprPool::AprPool() {
    aprPool = NULL;
}

////////////////////////////////////////////////////////////////////////////////
AprPool::~AprPool() {

    // Destroy the pool if it was allocated.
    destroyPool();
}

////////////////////////////////////////////////////////////////////////////////
void AprPool::allocatePool() const {

    if( aprPool == NULL ) {
        apr_pool_create_unmanaged_ex( &aprPool, NULL, NULL );
    }
}

////////////////////////////////////////////////////////////////////////////////
void AprPool::destroyPool() {

    if( aprPool != NULL ) {
        apr_pool_destroy( aprPool );
    }

    aprPool = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void AprPool::cleanup() {

    if( aprPool != NULL ) {
        apr_pool_clear( aprPool );
    }
}

////////////////////////////////////////////////////////////////////////////////
apr_pool_t* AprPool::getAprPool() const {

    // Ensure there is a Runtime instance created.
    Runtime::getRuntime();

    // Ensure that the pool has been allocated.
    allocatePool();

    return aprPool;
}

////////////////////////////////////////////////////////////////////////////////
apr_pool_t* AprPool::getGlobalPool() {

    // Ensure there is a Runtime instance created so we have a global pool
    DecafRuntime* runtime = dynamic_cast<DecafRuntime*>( Runtime::getRuntime() );
    return runtime->getGlobalPool();
}
