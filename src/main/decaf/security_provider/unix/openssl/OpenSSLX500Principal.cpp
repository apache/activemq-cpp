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

#include "OpenSSLX500Principal.h"

using namespace decaf::security_provider::unix::openssl;
using namespace decaf::security;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
OpenSSLX500Principal::OpenSSLX500Principal( X509_NAME* name ) {
    
    // Save the X509 name
    this->x509Name = name;
    
    // Indicate that the cached values have not yet been stored.
    cachedNameString = false;
    cachedEncoded = false;
}

////////////////////////////////////////////////////////////////////////////////
bool OpenSSLX500Principal::equals( const Principal& another ) const {

    // Cast the principal to the correct type.
    const OpenSSLX500Principal* other = 
        dynamic_cast<const OpenSSLX500Principal*>(&another);
    if( other == NULL || x509Name == NULL || other->x509Name == NULL ) {
        return false;
    }
        
    // Compare the underlying X509_NAME structures.
    int value = X509_NAME_cmp(x509Name, other->x509Name);
    
    // Return true if they match.
    return (value == 0);
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLX500Principal::getEncoded( X509_NAME* name, 
        std::vector<unsigned char>& output ) {        
    
    // Allocate the out buffer and store the encoded data there.
    char* buffer = NULL;
    int len = i2d_X509_NAME(name, NULL);
    if( len < 0 ) {
        
        // Error - should never get here since we already have a valid certificate.
        return;
    }
    
    // Resize the output buffer and copy over the encoded data.
    output.resize(len);
    for( int ix=0; ix<len; ++ix ) {
        output[ix] = buffer[ix];
    }
    
    // Destroy the allocated buffer.
    delete [] buffer;
}

////////////////////////////////////////////////////////////////////////////////
string OpenSSLX500Principal::toString(X509_NAME* name) const {
    
    string retVal;
    if( name == null ) {
        return retVal;
    }
    
    // Create an in-memory BIO structure to be the target
    // of the conversion.
    BIO* bio = BIO_new( BIO_s_mem() );
    if( bio != NULL ) {
        
        // Print the name to the BIO buffer, using RFC2253 formatting
        if( X509_NAME_print_ex(bio, name, 2, XN_FLAG_RFC2253) ) {

            // Get a pointer to the internal buffer and the length.
            char* chars = NULL;
            size_t len = (size_t)BIO_get_mem_ptr(bio, &chars);
            
            // Add the characters to the output string.
            retVal.append(chars, len);                        
        }        
        
        // Free the BIO structure.
        BIO_free(bio);
    }
    
    // Return the string
    return retVal;
}
