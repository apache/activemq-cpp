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

#include "OpenSSLParameters.h"

#include <decaf/lang/exceptions/NullPointerException.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

#include <memory>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

#ifdef HAVE_OPENSSL

////////////////////////////////////////////////////////////////////////////////
OpenSSLParameters::OpenSSLParameters(SSL_CTX* context) : needClientAuth(false),
                                                         wantClientAuth(false),
                                                         useClientMode(true),
                                                         context(context),
                                                         ssl(NULL),
                                                         enabledCipherSuites(),
                                                         enabledProtocols(),
                                                         serverNames() {

    if (context == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "SSL Context was NULL");
    }

    // Create a new SSL instance for this Parameters object, each one needs its own.
    this->ssl = SSL_new(context);
}

#endif

////////////////////////////////////////////////////////////////////////////////
OpenSSLParameters::~OpenSSLParameters() {

    try {
#ifdef HAVE_OPENSSL
        SSL_free(this->ssl);
#endif
    }
    DECAF_CATCH_NOTHROW(Exception)
    DECAF_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLParameters::getSupportedCipherSuites() const {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLParameters::getSupportedProtocols() const {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLParameters::getEnabledCipherSuites() const {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLParameters::setEnabledCipherSuites(const std::vector<std::string>& suites) {
    // Cache the setting for quicker retrieval
    this->enabledCipherSuites = suites;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLParameters::getEnabledProtocols() const {
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLParameters::setEnabledProtocols(const std::vector<std::string>& protocols) {
    // Cache the setting for quicker retrieval
    this->enabledProtocols = protocols;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OpenSSLParameters::getServerNames() const {
    return this->serverNames;
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLParameters::setServerNames(const std::vector<std::string>& serverNames) {
    // Cache the setting for quicker retrieval
    this->serverNames = serverNames;
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLParameters* OpenSSLParameters::clone() const {

#ifdef HAVE_OPENSSL

    std::auto_ptr<OpenSSLParameters> cloned( new OpenSSLParameters( this->context ) );

    cloned->enabledProtocols = this->enabledProtocols;
    cloned->enabledCipherSuites = this->enabledCipherSuites;
    cloned->serverNames = this->serverNames;
    cloned->needClientAuth = this->needClientAuth;
    cloned->wantClientAuth = this->wantClientAuth;
    cloned->useClientMode = this->useClientMode;

    return cloned.release();

#else

    return NULL;

#endif
}
