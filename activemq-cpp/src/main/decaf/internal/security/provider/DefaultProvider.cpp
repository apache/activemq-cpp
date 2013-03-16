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

#include "DefaultProvider.h"

#include <decaf/lang/Exception.h>

#include <decaf/internal/security/provider/DefaultMessageDigestProviderService.h>
#include <decaf/internal/security/provider/DefaultSecureRandomProviderService.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::security;
using namespace decaf::internal;
using namespace decaf::internal::security;
using namespace decaf::internal::security::provider;

////////////////////////////////////////////////////////////////////////////////
DefaultProvider::DefaultProvider() : Provider("Decaf", 1.0, "Default Decaf Security Provider") {
}

////////////////////////////////////////////////////////////////////////////////
DefaultProvider::~DefaultProvider() {
}

////////////////////////////////////////////////////////////////////////////////
void DefaultProvider::initialize() {
    try {
        this->addService(new DefaultMessageDigestProviderService(this, "MD4"));
        this->addService(new DefaultMessageDigestProviderService(this, "MD5"));
        this->addService(new DefaultMessageDigestProviderService(this, "SHA1"));
        this->addService(new DefaultSecureRandomProviderService(this, "Default"));
    }
    DECAF_CATCHALL_THROW(Exception)
}
