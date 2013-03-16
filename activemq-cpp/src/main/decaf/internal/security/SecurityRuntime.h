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

#ifndef _DECAF_INTERNAL_SECURITYRUNTIME_H_
#define _DECAF_INTERNAL_SECURITYRUNTIME_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/Mutex.h>

namespace decaf {
namespace internal {
namespace security {

    class SecurityRuntimeImpl;
    class ServiceRegistry;

    /**
     * Internal class used to manage Security related resources and hide platform
     * dependent calls from the higher level API.
     *
     * @since 1.0
     */
    class DECAF_API SecurityRuntime {
    private:

        SecurityRuntimeImpl* impl;

        static SecurityRuntime* securityRuntime;

    private:

        SecurityRuntime(const SecurityRuntime&);
        SecurityRuntime& operator=(const SecurityRuntime&);

    protected:

        SecurityRuntime();

    public:

        virtual ~SecurityRuntime();

        /**
         * Return the Security Framework's Service Registry.
         *
         * @returns a pointer to the frameworks Service Registry.
         */
        ServiceRegistry* getServiceRegistry();

        /**
         * Gets a pointer to the Security Runtime's Lock object, this can be used by
         * Security layer APIs to synchronize around certain actions such as adding
         * a resource to the Security layer, etc.
         *
         * The pointer returned is owned by the Security runtime and should not be
         * deleted or copied by the caller.
         *
         * @returns a pointer to the Security Runtime's single Lock instance.
         */
        decaf::util::concurrent::Mutex* getRuntimeLock();

    public:   // Static methods

        /**
         * Gets the one and only instance of the Security class, if this is called before
         * the Security layer has been initialized or after it has been shutdown then an
         * IllegalStateException is thrown.
         *
         * @return pointer to the Security runtime for the Decaf library.
         */
        static SecurityRuntime* getSecurityRuntime();

        /**
         * Initialize the Security layer.
         */
        static void initializeSecurity();

        /**
         * Shutdown the Security layer and free any associated resources, classes in the
         * Decaf library that use the Security layer will now fail if used after calling
         * the shutdown method.
         */
        static void shutdownSecurity();

    };

}}}

#endif /* _DECAF_INTERNAL_SECURITY_H_ */
