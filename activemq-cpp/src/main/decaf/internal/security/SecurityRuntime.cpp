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

#include "SecurityRuntime.h"

#include <decaf/lang/exceptions/IllegalStateException.h>
#include <decaf/internal/security/ServiceRegistry.h>
#include <decaf/internal/security/provider/DefaultProvider.h>

using namespace decaf;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;
using namespace decaf::internal::security::provider;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;

////////////////////////////////////////////////////////////////////////////////
SecurityRuntime* SecurityRuntime::securityRuntime = NULL;

////////////////////////////////////////////////////////////////////////////////
namespace decaf {
namespace internal {
namespace security {

    class SecurityRuntimeImpl {
    private:

        SecurityRuntimeImpl(const SecurityRuntimeImpl&);
        SecurityRuntimeImpl& operator=(const SecurityRuntimeImpl&);

    public:

        Mutex lock;
        ServiceRegistry* registry;
        Provider* defaultProvider;

        SecurityRuntimeImpl() : lock(), registry(), defaultProvider() {
            registry = new ServiceRegistry();
        }

        ~SecurityRuntimeImpl() {
            delete registry;
            delete defaultProvider;
        }
    };

}}}

////////////////////////////////////////////////////////////////////////////////
SecurityRuntime::SecurityRuntime() : impl(new SecurityRuntimeImpl) {
}

////////////////////////////////////////////////////////////////////////////////
SecurityRuntime::~SecurityRuntime() {
    try {
        delete this->impl;
    }
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
SecurityRuntime* SecurityRuntime::getSecurityRuntime() {

    if( SecurityRuntime::securityRuntime == NULL ) {
        throw IllegalStateException(
            __FILE__, __LINE__, "SecurityRuntime Runtime is not Initialized." );
    }

    return SecurityRuntime::securityRuntime;
}

////////////////////////////////////////////////////////////////////////////////
Mutex* SecurityRuntime::getRuntimeLock() {
    return &(this->impl->lock);
}

////////////////////////////////////////////////////////////////////////////////
ServiceRegistry* SecurityRuntime::getServiceRegistry() {
    return this->impl->registry;
}

////////////////////////////////////////////////////////////////////////////////
void SecurityRuntime::initializeSecurity() {
    SecurityRuntime::securityRuntime = new SecurityRuntime();

    DefaultProvider* defaultProvider = new DefaultProvider();
    securityRuntime->impl->defaultProvider = defaultProvider;
    defaultProvider->initialize();
    securityRuntime->impl->registry->addProvider(defaultProvider);
}

////////////////////////////////////////////////////////////////////////////////
void SecurityRuntime::shutdownSecurity() {
    delete SecurityRuntime::securityRuntime;
}
