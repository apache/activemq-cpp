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

#ifndef _DECAF_INTERNAL_SECURITY_SERVICEREGISTRY_H_
#define _DECAF_INTERNAL_SECURITY_SERVICEREGISTRY_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace security {
    class Provider;
    class ProviderService;
}
namespace internal {
namespace security {

    class ServiceRegistryImpl;

    /**
     * Serves as a registry for all the Providers for services using the naming
     * format of "ServiceName.Algorithm".
     */
    class DECAF_API ServiceRegistry {
    private:

        ServiceRegistryImpl* impl;

    private:

        ServiceRegistry(const ServiceRegistry&);
        ServiceRegistry& operator=(const ServiceRegistry&);

    public:

        ServiceRegistry();

        virtual ~ServiceRegistry();

        /**
         * Adds the Provider into the registry so that its services can be looked up
         * by the registry clients.
         *
         * @param provider
         *      The instance of the Provider which is to be added to the registry
         */
        void addProvider(const decaf::security::Provider* provider);

        /**
         * Attempts to locate a ProviderService implementation for the named service and
         * return a new instance of the service.  If no service exists for the given name
         * this method returns NULL.
         *
         * @param name
         *      The name of the service to find, format is "serviceName.algorithmName"
         *
         * @returns a caller owned pointer to a new ProviderService for the named service.
         */
        decaf::security::ProviderService* getService(const std::string& name);

    };

}}}

#endif /* _DECAF_INTERNAL_SECURITY_SERVICEREGISTRY_H_ */
