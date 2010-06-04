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

#ifndef _DECAF_INTERNAL_SECURITY_SECURERANDOMIMPL_H_
#define _DECAF_INTERNAL_SECURITY_SECURERANDOMIMPL_H_

#include <decaf/util/Config.h>

#include <decaf/security/SecureRandomSpi.h>

namespace decaf {
namespace internal {
namespace security {

    class SRNGData;

    /**
     * Secure Random Number Generator for Windows based platforms that attempts to obtain
     * secure bytes with high entropy from known sources.  If the platform does not have
     * a source of secure bytes then the platform random number generator is used if one
     * exists otherwise the Decaf RNG is used as a last resort.
     *
     * @since 1.0
     */
    class DECAF_API SecureRandomImpl : public decaf::security::SecureRandomSpi {
    private:

        SecureRandomImpl( const SecureRandomImpl& );
        SecureRandomImpl& operator= ( const SecureRandomImpl& );

    private:

        SRNGData* config;

    public:

        SecureRandomImpl();

        virtual ~SecureRandomImpl();

        virtual void providerSetSeed( const unsigned char* seed, int size );

        virtual void providerNextBytes( unsigned char* bytes, int numBytes );

        virtual unsigned char* providerGenerateSeed( int numBytes );

    };

}}}

#endif /* _DECAF_INTERNAL_SECURITY_SECURERANDOMIMPL_H_ */
