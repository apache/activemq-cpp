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

#ifndef _DECAF_INTERNAL_SECURITY_ENGINE_H_
#define _DECAF_INTERNAL_SECURITY_ENGINE_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace security {
    class Provider;
    class SecuritySpi;
}
namespace internal {
namespace security {

    /**
     * The Engine class serves as a convenience class for classes in the
     * Decaf Security package.  An engine can be created for a given service
     * type, "MessageDigest" for instance and reused to create different
     * algorithms for that type.  The Engine class takes care of the details
     * of looking up a ProviderService in the Security Runtime, using correct
     * locking and exception handling so that the higher level classes don't
     * need to implement that logic over again.
     */
    class DECAF_API Engine {
    private:

        std::string serviceName;
        const decaf::security::Provider* provider;

    private:

        Engine(const Engine&);
        Engine operator= (const Engine&);

    public:

        Engine(const std::string& serviceName);

        virtual ~Engine();

        /**
         * Returns the name of the service type that this Engine will be a
         * builder of SecuritySpi instances for.
         *
         * @returns the service class name of this engine, e.g. MessageDigest.
         */
        std::string getServiceName() const {
            return this->serviceName;
        }

        /**
         * Returns the Provider associated with this Engine.
         *
         * The pointer returned by this method remains the property of the Security framework
         * and should be deleted by the calling application at any time.
         *
         * @return the provider associated with this MessageDigest.
         */
        const decaf::security::Provider* getProvider() const {
            return this->provider;
        }

        /**
         * Return a new instance of the SercuritySpi implementation that is named by
         * this engine's serviceName and the passed algorithmName.
         *
         * @returns a new instance of the SecuritySpi provided by serviceName.algorithmName
         */
        decaf::security::SecuritySpi* newInstance(const std::string& algorithmName);

    };

}}}

#endif /* _DECAF_INTERNAL_SECURITY_ENGINE_H_ */
