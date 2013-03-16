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

#ifndef _DECAF_INTERNAL_SECURITY_PROVIDER_CRYPTO_MD5MESSAGEDIGESTSPI_H_
#define _DECAF_INTERNAL_SECURITY_PROVIDER_CRYPTO_MD5MESSAGEDIGESTSPI_H_

#include <decaf/util/Config.h>

#include <decaf/security/MessageDigestSpi.h>

namespace decaf {
namespace internal {
namespace security {
namespace provider {
namespace crypto {

    class MD5MessageDigestSpiImpl;

    /**
     * MD5 MessageDigestSpi
     *
     * @since 1.0
     */
    class DECAF_API MD5MessageDigestSpi : public decaf::security::MessageDigestSpi {
    private:

        MD5MessageDigestSpi(const MD5MessageDigestSpi&);
        MD5MessageDigestSpi& operator= (const MD5MessageDigestSpi&);

        MD5MessageDigestSpiImpl* impl;

    public:

        MD5MessageDigestSpi();

        virtual ~MD5MessageDigestSpi();

    public:

        virtual bool isCloneable() const {
            return true;
        }

        virtual MessageDigestSpi* clone();

        virtual int engineGetDigestLength();

        virtual void engineUpdate(unsigned char input);

        virtual void engineUpdate(const unsigned char* input, int size, int offset, int length);

        virtual void engineReset();

        virtual void engineUpdate(const std::vector<unsigned char>& input);

        virtual void engineUpdate(decaf::nio::ByteBuffer& input);

        virtual std::vector<unsigned char> engineDigest();

        virtual int engineDigest(unsigned char* buffer, int size, int offset, int length);

    };

}}}}}

#endif /* _DECAF_INTERNAL_SECURITY_PROVIDER_CRYPTO_MD5MESSAGEDIGESTSPI_H_ */
