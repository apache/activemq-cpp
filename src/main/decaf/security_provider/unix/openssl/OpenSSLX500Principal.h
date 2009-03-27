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

#ifndef _DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX500PRINCIPAL_H_
#define _DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX500PRINCIPAL_H_

#include <decaf/security/auth/x500/X500Principal.h>

#include <openssl/x509.h>

namespace decaf {
namespace security_provider {
namespace unix {
namespace openssl {

    /**
     * The <code>OpenSSLX500Principal</code> wraps around an OpenSSL 
     * <code>X509_NAME</code> structure.  It does not, however, control the 
     * lifetime of the structure.
     */
    class OpenSSLX500Principal : public X500Principal {
        
    private:
        
        /**
         * The underlying X509 name structure.
         */
        X509_NAME* x509Name;
        
        /**
         * Indicates whether or not the name string has been
         * cached.
         */
        mutable bool cachedNameString;
        
        /**
         * Cached string representation of the X509 name.
         */
        mutable std::string nameString;
        
        /**
         * Indicates whether or not the encoded buffer has been
         * cached.
         */
        mutable bool cachedEncoded;
        
        /**
         * Cached encoded representation of teh X509 name.
         */
        mutable std::vector<unsigned char> encoded;
        
    public:
        
        /**
         * Constructor.  Saves the internal X509 name and caches the
         * string representation of the name.
         * 
         * @param name
         *      The underlying X509 name structure.
         */
        OpenSSLX500Principal( X509_NAME* name );
        
        /**
         * Destructor.  Does nothing.
         */
        virtual ~OpenSSLX500Principal() {}
        
        /**
         * Accessor to the underlying X509 name structure.
         */
        virtual X509_NAME* getX509Name() {
            return x509Name;
        }
        
        /**
         * Compares two principals to see if they are the same.
         * 
         * @param another
         *      A principal to be tested for equality to this one.
         * @return true if the given principal is equivalent to this one.
         */
        virtual bool equals( const Principal& another ) const;
            
        /**
         * Returns the distinguished name string using the RFC2253 formatting.
         * 
         * @return the RFC2253 formatted distinguished name string.
         */
        virtual std::string getName() const {
            
            // If we haven't cached the name yet - do it now.
            if( !cachedNameString ) {
                nameString = toString(x509Name);
                cachedNameString = true;
            }
            
            return nameString;
        }
        
        /**
         * Serializes the distinguished name to its ASN.1 DER encoded form.
         * 
         * @param output
         *      Receives the distinguished name in ASN.1 DER encoded form.
         */
        virtual void getEncoded( std::vector<unsigned char>& output ) const {
            
            // If we haven't cached the encoded buffer yet, do it now.
            if( !cachedEncoded ) {
                getEncoded(x509Name, encoded);
                cachedEncoded = true;
            }
            
            // Copy the encoded buffer to the output buffer.
            output = encoded;
        }
        
        /**
         * Serializes the given distinguished name to its ASN.1 DER encoded 
         * form.
         * 
         * @param name
         *      the X509 name structure to be encoded.
         * @param output
         *      Receives the distinguished name in ASN.1 DER encoded form.
         */
        static void getEncoded( X509_NAME* name, 
                std::vector<unsigned char>& output );
                
        /**
         * Converts the given name to a string using the RFC2253 formatting.
         * @param name
         *      the X509 name structure to be formatted.
         * @return the RFC2253 formatted name string.
         */
        static std::string toString( X509_NAME* name ) const;
        
    };

}}}}

#endif /*_DECAF_SECURITY_PROVIDER_UNIX_OPENSSL_OPENSSLX500PRINCIPAL_H_*/
