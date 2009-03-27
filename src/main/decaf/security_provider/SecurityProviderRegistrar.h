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

#ifndef _DECAF_SECURITY_PROVIDER_SECURITYPROVIDERREGISTRAR_H
#define _DECAF_SECURITY_PROVIDER_SECURITYPROVIDERREGISTRAR_H

#include <string>

#include <decaf/security_provider/SecurityProviderMap.h>

namespace decaf{
namespace security_provider{

    /**
     * Registers the passed in provider into the provider map, this class
     * can manage the lifetime of the registered provider (default behaviour).
     */
    class SecurityProviderRegistrar {
    private:
          
        /**
         * Unique name used to identify this provider in the map.
         */
        std::string name;
        
        /**
         * The provider object.
         */
        SecurityProvider* provider;
        
        /**
         * Indicates whether or not the provider will be destroyed
         * when this registrar object is destroyed.
         */
        bool manageLifetime;
        
    public:
   
        /** 
         * Creates a registrar and registers the provider with
         * the provider map.
         * 
         * @param name 
         *      name of the provider to register
         * @param provider 
         *      the provider object
         * @param manageLifetime 
         *      boolean indicating if this object manages the 
         *      lifetime of the factory that is being registered.
         */
        SecurityProviderRegistrar( const std::string& name, 
                                   SecurityProvider* provider,
                                   bool manageLifetime = true ) {       
            // Register it in the map.
            SecurityProviderMap::getInstance()->
                registerSecurityProvider(name, provider);

            // Store for later deletion            
            this->provider        = provider;
            this->manageLifetime = manageLifetime;
            this->name           = name;
        }
      
        /**
         * Unregisters the provider from the provider map and
         * destroys it if <code>manageLifetime</code> is set.
         */
        virtual ~SecurityProviderRegistrar() {
            
            try {
                
                // UnRegister it in the map.
                SecurityProviderMap::getInstance()->
                    unregisterSecurityProvider( name );
            
                if( manageLifetime ) {
                    delete provider;
                }
                
            }
            catch( ... ) {
                // Destructors should not propagate exceptions.
            }
        }
      
        /**
         * get a reference to the factory that this class is holding
         * @return reference to a factory class
         */
        virtual SecurityProvider* getProvider() {
            return provider;
        }    

    }; 
      
}}

#endif /*_DECAF_SECURITY_PROVIDER_SECURITYPROVIDERREGISTRAR_H*/
