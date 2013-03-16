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

#ifndef _DECAF_SECURITY_PROVIDERSERVICE_H_
#define _DECAF_SECURITY_PROVIDERSERVICE_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace security {

    class Provider;
    class SecuritySpi;

    class DECAF_API ProviderService {
    private:

        const Provider* provider;
        std::string type;
        std::string algorithm;

    public:

        ProviderService(const Provider* provider, const std::string& type, const std::string& algorithm);

        virtual ~ProviderService();

        /**
         * Gets the type of service this ProviderService instance supports.
         *
         * @return type name of the service this ProviderService supports.
         */
        std::string getType() const {
            return this->type;
        }

        /**
         * Gets the algorithm name that this ProviderService supplies for its
         * service type.
         *
         * @return the algorithm this ProviderService supports.
         */
        std::string getAlgorithm() const {
            return this->algorithm;
        }

        /**
         * Returns a pointer to the Provider that owns this ProviderService.
         *
         * The returned pointer is owned by the Security framework an should not be
         * deleted by the caller at any time.
         *
         * @return pointer to the security provider that owns this service.
         */
        const Provider* getProvider() const {
            return provider;
        }

        /**
         * Return a new instance of the implementation described by this service. The
         * security provider framework uses this method to construct implementations.
         * Applications will typically not need to call it.
         *
         * @returns a new instance of the SecuritySpi provided by this ProviderService.
         */
        virtual SecuritySpi* newInstance() = 0;

        /**
         * Return a String representation of this service.  The format of this string is
         * always, "type.algorithm"
         *
         * @returns string describing this ProviderService.
         */
        std::string toString() const {
            return getType() + "." + getAlgorithm();
        }

    };

}}

#endif /* _DECAF_SECURITY_PROVIDERSERVICE_H_ */
