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

#ifndef _DECAF_INTERNAL_SECURITY_PROVIDER_DEFAULTSECURERANDOMPROVIDERSERVICE_H_
#define _DECAF_INTERNAL_SECURITY_PROVIDER_DEFAULTSECURERANDOMPROVIDERSERVICE_H_

#include <decaf/util/Config.h>
#include <decaf/security/ProviderService.h>
#include <string>

namespace decaf {
namespace internal {
namespace security {
namespace provider {

    /**
     * Decaf's Default Secure Random Security provider used to create instances of the
     * built-in Secure Random algorithm SPI classes.
     *
     * @since 1.0
     */
    class DECAF_API DefaultSecureRandomProviderService : public decaf::security::ProviderService {
    private:

        DefaultSecureRandomProviderService(const DefaultSecureRandomProviderService&);
        DefaultSecureRandomProviderService& operator= (const DefaultSecureRandomProviderService&);

    public:

        DefaultSecureRandomProviderService(const decaf::security::Provider* provider,
                                            const std::string& algorithmName);

        virtual ~DefaultSecureRandomProviderService();

        virtual decaf::security::SecuritySpi* newInstance();

    };

}}}}

#endif /* _DECAF_INTERNAL_SECURITY_PROVIDER_DEFAULTSECURERANDOMPROVIDERSERVICE_H_ */
