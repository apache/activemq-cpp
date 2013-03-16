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

#include "Engine.h"

#include <decaf/security/SecuritySpi.h>
#include <decaf/security/Provider.h>
#include <decaf/security/ProviderService.h>
#include <decaf/security/NoSuchAlgorithmException.h>

#include <decaf/internal/security/SecurityRuntime.h>
#include <decaf/internal/security/ServiceRegistry.h>

using namespace decaf;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;

////////////////////////////////////////////////////////////////////////////////
Engine::Engine(const std::string& serviceName) : serviceName(serviceName), provider(NULL) {
}

////////////////////////////////////////////////////////////////////////////////
Engine::~Engine() {
}

////////////////////////////////////////////////////////////////////////////////
SecuritySpi* Engine::newInstance(const std::string& algorithmName) {

    if (algorithmName.empty()) {
        throw NoSuchAlgorithmException(__FILE__, __LINE__, "Null algorithm name");
    }

    SecurityRuntime* runtime = SecurityRuntime::getSecurityRuntime();
    SecuritySpi* spi = NULL;

    synchronized(runtime->getRuntimeLock()) {
        std::string providerServiceName = serviceName + "." + algorithmName;

        ServiceRegistry* registry = runtime->getServiceRegistry();
        ProviderService* service = registry->getService(providerServiceName);

        if (service == NULL) {
            throw NoSuchAlgorithmException(__FILE__, __LINE__,
                "%s Implementation not found", providerServiceName.c_str());
        }

        this->provider = service->getProvider();

        spi = service->newInstance();
    }

    return spi;
}
