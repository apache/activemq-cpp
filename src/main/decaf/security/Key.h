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

#ifndef _DECAF_SECURITY_KEY_H
#define _DECAF_SECURITY_KEY_H

#include <vector>
#include <string>
#include <decaf/util/Config.h>

namespace decaf {
namespace security {

    /**
     * The Key interface is the top-level interface for all keys. It defines the
     * functionality shared by all key objects. All keys have three
     * characteristics:<br>
     * <br>
     * An Algorithm <br>
     * This is the key algorithm for that key. The key algorithm is usually an
     * encryption or asymmetric operation algorithm (such as DSA or RSA), which
     * will work with those algorithms and with related algorithms (such as MD5
     * with RSA, SHA-1 with RSA, Raw DSA, etc.) The name of the algorithm of a
     * key is obtained using the getAlgorithm method.<br>
     * <br>
     * An Encoded Form <br>
     * This is an external encoded form for the key used when a standard
     * representation of the key is needed outside the application, as
     * when transmitting the key to some other party. The key is encoded
     * according to a standard format (such as X.509 SubjectPublicKeyInfo or
     * PKCS#8), and is returned using the getEncoded method. Note: The syntax of
     * the ASN.1 type SubjectPublicKeyInfo is defined as follows:
     * <br>
     * SubjectPublicKeyInfo ::= SEQUENCE { <br>
     *    algorithm AlgorithmIdentifier, <br>
     *    subjectPublicKey BIT STRING } <br>
     * <br>
     * AlgorithmIdentifier ::= SEQUENCE { <br>
     *    algorithm OBJECT IDENTIFIER, <br>
     *    parameters ANY DEFINED BY algorithm OPTIONAL } <br>
     * <br>
     * For more information, see RFC 2459: Internet X.509 Public Key
     * Infrastructure Certificate and CRL Profile. <br>
     * <br>
     * A Format <br>
     * This is the name of the format of the encoded key. It is returned by the
     * getFormat method.
     */
    class DECAF_API Key {

    public:

        virtual ~Key() {}

        /**
         * Returns the standard algorithm name for this key. For example, "DSA"
         * would indicate that this key is a DSA key.
         *
         * @return the name of the algorithm associated with this key.
         */
        virtual std::string getAlgorithm() const = 0;

        /**
         * Provides the key in its primary encoding format, or nothing if this
         * key does not support encoding.
         *
         * @param output
         *      Receives the encoded key, or nothing if the key does not support
         *      encoding.
         */
        virtual void getEncoded( std::vector<unsigned char>& output) const = 0;

        /**
         * Returns the name of the primary encoding format of this key, or
         * an empty string if this key does not support encoding. The primary
         * encoding format is named in terms of the appropriate ASN.1 data
         * format, if an ASN.1 specification for this key exists. For example,
         * the name of the ASN.1 data format for public keys is
         * SubjectPublicKeyInfo, as defined by the X.509 standard; in this
         * case, the returned format is "X.509". Similarly, the name of the
         * ASN.1 data format for private keys is PrivateKeyInfo, as defined by
         * the PKCS #8 standard; in this case, the returned format is "PKCS#8".
         *
         * @return the primary encoding format of the key.
         */
        virtual std::string getFormat() const = 0;
    };
}}

#endif /*_DECAF_SECURITY_KEY_H*/
