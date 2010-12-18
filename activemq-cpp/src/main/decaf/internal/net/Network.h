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

#ifndef _DECAF_INTERNAL_NET_NETWORK_H_
#define _DECAF_INTERNAL_NET_NETWORK_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/Mutex.h>
#include <decaf/internal/util/Resource.h>
#include <decaf/internal/util/GenericResource.h>

namespace decaf {
namespace internal {
namespace net {

    class NetworkData;

    /**
     * Internal class used to manage Networking related resources and hide platform
     * dependent calls from the higher level API.
     *
     * @since 1.0
     */
    class DECAF_API Network {
    private:

        NetworkData* data;

        static Network* networkRuntime;

    private:

        Network( const Network& );
        Network& operator= ( const Network& );

    protected:

        Network();

    public:

        virtual ~Network();

        /**
         * Gets a pointer to the Network Runtime's Lock object, this can be used by
         * Network layer APIs to synchronize around certain actions such as adding
         * a resource to the Network layer, etc.
         *
         * The pointer returned is owned by the Network runtime and should not be
         * deleted or copied by the caller.
         *
         * @returns a pointer to the Network Runtime's single Lock instance.
         */
        decaf::util::concurrent::Mutex* getRuntimeLock();

        /**
         * Adds a Resource to the Network Runtime, this resource will be held by the
         * runtime until the Library shutdown method is called at which time all the
         * Resources held by the Network Runtime are destroyed.
         *
         * @param value
         *      The Resource to add to the Network Runtime.
         *
         * @throw NullPointerException if the Resource value passed is null.
         */
        void addNetworkResource( decaf::internal::util::Resource* value );

        template<typename T>
        void addAsResource( T* value ) {

            util::GenericResource<T>* resource = new util::GenericResource<T>( value );
            this->addNetworkResource( resource );
        }

        /**
         * Register a Runnable to be called when the Network Runtime is shutdown to provide
         * a chance to cleanup any data or references that could cause problems should the
         * Network Runtime be re-initialized.  The Runnable pointer ownership is transfered
         * to the NetworkRuntime to guarantee the timing of resource cleanup.
         *
         * The cleanup tasks are run at a critical time in the Shutdown process and should
         * be as simple as possible and make every attempt to no throw any exceptions.  If an
         * exception is thrown it is ignored and processing of the next task is started.
         *
         * The tasks should not assume that any Network resources are still available and
         * should execute as quickly as possible.
         *
         * @param task
         *      Pointer to a Runnable object that will now be owned by the Network Runtime.
         */
        void addShutdownTask( decaf::lang::Runnable* task );

    public:   // Static methods

        /**
         * Gets the one and only instance of the Network class, if this is called before
         * the Network layer has been initialized or after it has been shutdown then an
         * IllegalStateException is thrown.
         *
         * @return pointer to the Network runtime for the Decaf library.
         */
        static Network* getNetworkRuntime();

        /**
         * Initialize the Networking layer.
         */
        static void initializeNetworking();

        /**
         * Shutdown the Network layer and free any associated resources, classes in the
         * Decaf library that use the networking layer will now fail if used after calling
         * the shutdown method.
         */
        static void shutdownNetworking();

    };

}}}

#endif /* _DECAF_INTERNAL_NET_NETWORK_H_ */
