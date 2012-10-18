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

#include "ResourceLifecycleManager.h"

#include <decaf/lang/Exception.h>

#include <decaf/util/Iterator.h>

using namespace decaf;
using namespace decaf::internal;
using namespace decaf::internal::util;
using namespace decaf::lang;
using namespace decaf::util;

////////////////////////////////////////////////////////////////////////////////
ResourceLifecycleManager::ResourceLifecycleManager() : resources() {
}

////////////////////////////////////////////////////////////////////////////////
ResourceLifecycleManager::~ResourceLifecycleManager() {
    try {
        this->destroyResources();
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
void ResourceLifecycleManager::addResource(Resource* value) {

    if (value == NULL) {
        return;
    }

    this->resources.add(value);
}

////////////////////////////////////////////////////////////////////////////////
void ResourceLifecycleManager::destroyResources() {

    try {

        std::auto_ptr<Iterator<Resource*> > iterator(this->resources.iterator());

        while (iterator->hasNext()) {
            delete iterator->next();
        }

        this->resources.clear();
    }
    DECAF_CATCH_RETHROW(Exception)
    DECAF_CATCHALL_THROW(Exception)
}
