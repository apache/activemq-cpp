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

#ifndef _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTREGISTRY_H_
#define _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTREGISTRY_H_

#include <activemq/util/Config.h>

#include <string>
#include <vector>

#include <decaf/util/StlMap.h>

namespace activemq {
namespace library {
    class ActiveMQCPP;
}
namespace transport {
namespace discovery {

    class DiscoveryAgentFactory;

    /**
     * Registry of all Discovery Agent Factories that are available to the client
     * at runtime.  New Agents must have a factory registered here before an attempt
     * to create a DiscoveryTansport which uses that agent.
     *
     * @since 3.9.0
     */
    class AMQCPP_API DiscoveryAgentRegistry {
    private:

        decaf::util::StlMap<std::string, DiscoveryAgentFactory*> registry;

    private:

        DiscoveryAgentRegistry();
        DiscoveryAgentRegistry(const DiscoveryAgentRegistry& registry);
        DiscoveryAgentRegistry& operator=(const DiscoveryAgentRegistry& registry);

    public:

        virtual ~DiscoveryAgentRegistry();

        /**
         * Gets a Registered DiscoveryAgentFactory from the Registry and returns it
         * if there is not a registered format factory with the given name an exception
         * is thrown.
         *
         * @param name
         *        The name of the Factory to find in the Registry.
         *
         * @return the Factory registered under the given name.
         *
         * @throws NoSuchElementException if no factory is registered with that name.
         */
        DiscoveryAgentFactory* findFactory(const std::string& name) const;

        /**
         * Registers a new DiscoveryAgentFactory with this Registry.  If a Factory with the
         * given name is already registered it is overwritten with the new one.  Once a
         * factory is added to the Registry its lifetime is controlled by the Registry, it
         * will be deleted once the Registry has been deleted.
         *
         * @param name
         *        The name of the new Factory to register.
         * @param factory
         *        The new Factory to add to the Registry.
         *
         * @throws IllegalArgumentException is name is the empty string.
         * @throws NullPointerException if the Factory is Null.
         */
        void registerFactory(const std::string& name, DiscoveryAgentFactory* factory);

        /**
         * Unregisters the Factory with the given name and deletes that instance of the
         * Factory.
         *
         * @param name
         *        Name of the Factory to unregister and destroy
         */
        void unregisterFactory(const std::string& name);

        /**
         * Removes all Factories and deletes the instances of the Factory objects.
         */
        void unregisterAllFactories();

        /**
         * Retrieves a list of the names of all the Registered Agents in this
         * Registry.
         *
         * @return stl vector of strings with all the Agent names registered.
         */
        std::vector<std::string> getAgentNames() const;

    public:

        /**
         * Gets the single instance of the TransportRegistry
         * @return reference to the single instance of this Registry
         */
        static DiscoveryAgentRegistry& getInstance();

    private:

        static void initialize();
        static void shutdown();

        friend class activemq::library::ActiveMQCPP;

    };

}}}

#endif /* _ACTIVEMQ_TRANSPORT_DISCOVERY_DISCOVERYAGENTREGISTRY_H_ */
