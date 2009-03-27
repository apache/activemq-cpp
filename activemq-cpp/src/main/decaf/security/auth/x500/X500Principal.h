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

#ifndef _DECAF_SECURITY_AUTH_X500_X500PRINCIPAL_H_
#define _DECAF_SECURITY_AUTH_X500_X500PRINCIPAL_H_

#include <string>
#include <vector>

#include <decaf/security/Principal.h>
#include <decaf/util/Map.h>
#include <decaf/io/InputStream.h>

namespace decaf {
namespace security {
namespace auth {
namespace x500 {

    class X500Principal : public Principal {
    public:
        
        /*X500Principal( unsigned char* name, int offset, int len );
        X500Principal( decaf::io::InputStream& is );
        X500Principal( const std::string& name );
        X500Principal( const std::string& name, 
                const decaf::util::Map<std::string, std::string>& keywordMap );*/
        
        virtual ~X500Principal() {}        
        
        virtual std::string getName() const = 0;
        
        virtual void getEncoded( std::vector<unsigned char>& output ) const = 0;
        
        virtual int hashCode() const = 0;
        
        /*virtual std::string getName( const std::string& format ) const;
        
        virtual std::string getName(const std::string& format, 
                const decaf::util::Map<std::string, std::string>& oldMap );*/                        
    };
    
}}}}

#endif /*_DECAF_SECURITY_AUTH_X500_X500PRINCIPAL_H_*/
