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

#ifndef _DECAF_SECURITY_PROVIDER_H_
#define _DECAF_SECURITY_PROVIDER_H_

#include <decaf/util/Config.h>
#include <decaf/util/Set.h>

#include <string>

#include <decaf/security/ProviderService.h>

namespace decaf {
namespace security {

    class ProviderImpl;

    /**
     * This class represents a "provider" for the Decaf Security API, where a provider
     * implements some or all parts of Decaf Security. Services that a provider may implement
     * include:
     *
     *  Algorithms (such as DSA, RSA, MD5 or SHA-1).
     *  Key generation, conversion, and management facilities (such as for algorithm-specific keys).
     *
     * Each provider has a name and a version number, and is configured in each runtime it
     * is installed in.
     *
     * @since 1.0
     */
    class DECAF_API Provider {
    private:

        std::string name;
        double version;
        std::string info;

        ProviderImpl* impl;

    private:

        Provider(const Provider&);
        Provider& operator= (const Provider&);

    protected:

        Provider(const std::string& name, double version, const std::string& info);

    public:

        virtual ~Provider();

    public:

        std::string getName() const {
            return name;
        }

        double getVersion() const {
            return version;
        }

        std::string getInfo() const {
            return info;
        }

        const decaf::util::Set<ProviderService*>& getServices() const;

    protected:

        virtual void initialize() {}

        void addService(ProviderService* service);

    };

}}

#endif /* _DECAF_SECURITY_PROVIDER_H_ */
