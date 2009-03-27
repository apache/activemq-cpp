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

#ifndef _DECAF_SECURITY_PROVIDER_SECURITYPROVIDERMAP_H
#define _DECAF_SECURITY_PROVIDER_SECURITYPROVIDERMAP_H

#include <map>
#include <vector>
#include <string>

#include <decaf/lang/Exception.h>

namespace activemq{
namespace connector{

    /**
     * Lookup Map for Connector Factories.  Use the Connector name to
     * find the associated factory.  This class does not take ownership
     * of the stored factories, they must be deallocated somewhere.
     */
    class SecurityProviderMap
    {
    private:
   
        /**
         * Map of Factories
         */
        std::map< std::string, SecurityProvider* > providerMap;
                
        /**
         * Hidden Contrustor, prevents instantiation
         */
        SecurityProviderMap() {};
      
        /**
         * Hidden Destructor.
         */
        virtual ~SecurityProviderMap() {};
 
        /**
         * Hidden Copy Constructore
         */
        SecurityProviderMap( const ConnectorFactoryMap& factoryMap );
      
        /**
         * Hidden Assignment operator
         */
        SecurityProviderMap operator=( const ConnectorFactoryMap& factoryMap );
        
    public:
      
        /**
         * Gets a singleton instance of this class.
         */
        static SecurityProviderMap* getInstance();

        /**
         * Registers a new provider with this map
         * 
         * @param name 
         *      A name to associate the provider with
         * @param provider 
         *      the provider object to store in the map.
        */
        void registerSecurityProvider( const std::string& name, 
                                       SecurityProvider* provider );
      
        /**
         * Unregisters a provider from this map
         * 
         * @param name 
         *      the name of the provider to remove
         */
        void unregisterSecurityProvider( const std::string& name );

        /**
         * Lookup the named provider in the Map
         * 
         * @param name 
         *      the provider name to lookup
         * @return the provider assciated with the name, or NULL
         */
        SecurityProvider* lookup( const std::string& name );
      
        /**
         * Fetch a list of provider names that this Map contains
         * 
         * @param providers 
         *      A vector object to receive the list
         * @returns count of providers.
         */
        std::size_t getSecurityProviderNames( std::vector< std::string >& providers );
      
    };

}}

#endif /*_DECAF_SECURITY_PROVIDER_SECURITYPROVIDERMAP_H*/
