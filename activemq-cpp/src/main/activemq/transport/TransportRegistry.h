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

#ifndef _ACTIVEMQ_TRANSPORT_TRANSPORTREGISTRY_H_
#define _ACTIVEMQ_TRANSPORT_TRANSPORTREGISTRY_H_

#include <activemq/util/Config.h>

#include <string>
#include <vector>
#include <activemq/transport/TransportFactory.h>

#include <decaf/util/StlMap.h>
#include <decaf/util/NoSuchElementException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalArgumentException.h>

namespace activemq {
namespace transport {

    /**
     * Registry of all Transport Factories that are available to the client
     * at runtime.  New Transport's must have a factory registered here before
     * a connection attempt is made.
     *
     * @since 3.0
     */
    class AMQCPP_API TransportRegistry {
    private:

        decaf::util::StlMap<std::string, TransportFactory*> registry;

    private:

        // Hidden Constructor, prevents instantiation
        TransportRegistry();

        // Hidden Copy Constructor
        TransportRegistry( const TransportRegistry& registry );

        // Hidden Assignment operator
        TransportRegistry& operator=( const TransportRegistry& registry );

    public:

        virtual ~TransportRegistry();

        /**
         * Gets a Registered TransportFactory from the Registry and returns it
         * if there is not a registered format factory with the given name an exception
         * is thrown.
         *
         * @param name
         *        The name of the Factory to find in the Registry.
         *
         * @returns the Factory registered under the given name.
         *
         * @throws NoSuchElementException if no factory is registered with that name.
         */
        TransportFactory* findFactory( const std::string& name ) const;

        /**
         * Registers a new TransportFactory with this Registry.  If a Factory with the
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
        void registerFactory( const std::string& name, TransportFactory* factory );

        /**
         * Unregisters the Factory with the given name and deletes that instance of the
         * Factory.
         *
         * @param name
         *        Name of the Factory to unregister and destroy
         */
        void unregisterFactory( const std::string& name );

        /**
         * Removes all Factories and deletes the instances of the Factory objects.
         */
        void unregisterAllFactories();

        /**
         * Retrieves a list of the names of all the Registered Transport's in this
         * Registry.
         *
         * @returns stl vector of strings with all the Transport names registered.
         */
        std::vector<std::string> getTransportNames() const;

    public:  // Static methods

        /**
         * Gets the single instance of the TransportRegistry
         * @return reference to the single instance of this Registry
         */
        static TransportRegistry& getInstance();

    };

}}

#endif /* _ACTIVEMQ_TRANSPORT_TRANSPORTREGISTRY_H_ */
