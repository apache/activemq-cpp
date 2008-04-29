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

#include "SecurityProviderMap.h"

using namespace decaf::security_provider;

////////////////////////////////////////////////////////////////////////////////
SecurityProviderMap* SecurityProviderMap::getInstance()
{
    // Static instance of this Map, create here so that one will
    // always exist, the one and only Connector Map.      
    static SecurityProviderMap instance;
    
    return &instance;
} 

////////////////////////////////////////////////////////////////////////////////
void SecurityProviderMap::registerSecurityProvider( const std::string& name, 
                                                    SecurityProvider* provider )
{
    providerMap[name] = provider;
}

////////////////////////////////////////////////////////////////////////////////
void SecurityProviderMap::unregisterSecurityProvider( const std::string& name )
{
    providerMap.erase( name );
}

////////////////////////////////////////////////////////////////////////////////
SecurityProvider* SecurityProviderMap::lookup( const std::string& name ) 
{
    std::map<std::string, SecurityProvider*>::const_iterator itr = 
        providerMap.find( name );

    if( itr != providerMap.end() )
    {
        return itr->second;
    }

    // Didn't find it, return nothing, not a single thing.
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
std::size_t SecurityProviderMap::getFactoryNames(
   std::vector<std::string>& factoryList )
{
    std::map<std::string, SecurityProvider*>::const_iterator itr =
        providerMap.begin();
      
    for( ; itr != providerMap.end(); ++itr )
    {
        providerMap.insert( factoryList.end(), itr->first );
    }
      
    return providerMap.size();
}
