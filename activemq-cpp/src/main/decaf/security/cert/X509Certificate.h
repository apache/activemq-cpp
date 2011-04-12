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

#ifndef _DECAF_SECURITY_CERT_X509CERTIFICATE_H_
#define _DECAF_SECURITY_CERT_X509CERTIFICATE_H_

#include <decaf/security/cert/Certificate.h>

#include <decaf/util/Config.h>
#include <decaf/util/Date.h>

namespace decaf {
namespace security {
namespace cert {

    /**
     * Base interface for all identity certificates.
     */
    class DECAF_API X509Certificate : public Certificate {

    public:

        virtual ~X509Certificate() {}

        virtual void checkValidity() const = 0;

        virtual void checkValidity(const decaf::util::Date& date) const = 0;

        virtual int getBasicConstraints() const = 0;

        virtual void getIssuerUniqueID( std::vector<bool>& output ) const = 0;

        virtual const X500Principal* getIssuerX500Principal() const = 0;

        virtual void getKeyUsage( std::vector<unsigned char>& output ) const = 0;

        virtual Date getNotAfter() const = 0;

        virtual Date getNotBefore() const = 0;

        //virtual BigInteger getSerialNumber() const = 0;

        virtual std::string getSigAlgName() const = 0;

        virtual std::string getSigAlgOID() const = 0;

        virtual void getSigAlgParams( std::vector<unsigned char>& output ) const = 0;

        virtual void getSignature( std::vector<unsigned char>& output ) const = 0;

        virtual void getSubjectUniqueID( std::vector<bool>& output ) const = 0;

        virtual const X500Principal* getSubjectX500Principal() const = 0;

        virtual void getTBSCertificate( std::vector<unsigned char>& output ) const = 0;

        virtual int getVersion() const = 0;
    };

}}}

#endif /*_DECAF_SECURITY_CERT_X509CERTIFICATE_H_*/
