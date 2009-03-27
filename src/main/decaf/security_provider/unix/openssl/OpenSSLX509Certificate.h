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

#ifndef _DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX509CERTIFICATE_H_
#define _DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX509CERTIFICATE_H_

#include <decaf/security/cert/X509Certificate.h>

namespace decaf {
namespace security_provider {
namespace unix {
namespace openssl {

    class OpenSSLX509Certificate : public decaf::security::cert::X509Certificate {
    public:
        
        virtual ~OpenSSLX509Certificate();
        
        /**
         * Compares the encoded form of the two certificates.
         * 
         * @param cert
         *      The certificate to be tested for equality with this certificate.
         * @return true if the given certificate is equal to this certificate.
         */
        virtual bool equals( const Certificate& cert ) const = 0;
        
        /**
         * Provides the encoded form of this certificate.
         * 
         * @param output
         *      Receives the encoded form of this certificate.
         * @throws CertificateEncodingException if an encoding error occurs
         */
        virtual void getEncoded( std::vector<unsigned char>& output ) const 
            throw ( CertificateEncodingException ) = 0;
        
        /**
         * Returns the type of this certificate
         * 
         * @return the type of this certificate
         */
        virtual std::string getType() const = 0;
        
        /**
         * Gets the public key of this certificate.
         * 
         * @return the public key
         */
        virtual PublicKey* getPublicKey() = 0;
        
        /**
         * Gets the public key of this certificate.
         * 
         * @return the public key
         */
        virtual const PublicKey* getPublicKey() const = 0;
        
        /**
         * Verifies that this certificate was signed with the private key
         * that corresponds to the specified public key.
         * 
         * @param publicKey
         *      The public key used to carry out the validation.
         * @throws NoSuchAlgorithmException - on unsupported signature algorithms. 
         * @throws InvalidKeyException - on incorrect key. 
         * @throws NoSuchProviderException - if there's no default provider. 
         * @throws SignatureException - on signature errors. 
         * @throws CertificateException - on encoding errors.
         */
        virtual void verify( const PublicKey& publicKey ) const 
            throw( NoSuchAlgorithmException, 
                   InvalidKeyException, 
                   NoSuchProviderException, 
                   SignatureException, 
                   CertificateException) = 0;
        
        /**
         * Verifies that this certificate was signed with the private key
         * that corresponds to the specified public key.  Uses the verification
         * engine of the specified provider.
         * 
         * @param publicKey
         *      The public key used to carry out the validation.
         * @param sigProvider
         *      The name of the signature provider
         * @throws NoSuchAlgorithmException - on unsupported signature algorithms. 
         * @throws InvalidKeyException - on incorrect key. 
         * @throws NoSuchProviderException - if there's no default provider. 
         * @throws SignatureException - on signature errors. 
         * @throws CertificateException - on encoding errors.
         */
        virtual void verify( const PublicKey& publicKey, 
                             const std::string& sigProvider ) const 
                    throw( NoSuchAlgorithmException, 
                           InvalidKeyException, 
                           NoSuchProviderException, 
                           SignatureException, 
                           CertificateException) = 0;
        
        /**
         * Returns a string representation of this certificate.
         * 
         * @return a string representation of this certificate
         */
        virtual std::string toString() const = 0;
                
        virtual void checkValidity() const 
                    throw(CertificateExpiredException, CertificateNotYetValidException) = 0;
                
        virtual void checkValidity(const decaf::util::Date& date) const 
                    throw(CertificateExpiredException, CertificateNotYetValidException) = 0;                    
        
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
        
        virtual void getTBSCertificate( std::vector<unsigned char>& output ) const 
            throw( CertificateEncodingException ) = 0;
        
        virtual int getVersion() const = 0;
    };
    
}}}}

#endif /*_DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX509CERTIFICATE_H_*/
