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

#ifndef _DECAF_INTERNAL_DECAFRUNTIME_H
#define _DECAF_INTERNAL_DECAFRUNTIME_H

#include <decaf/util/Config.h>
#include <decaf/lang/Runtime.h>
#include <decaf/util/concurrent/Mutex.h>

#include <apr_pools.h>

namespace decaf {
namespace internal {

    class RuntimeData;

    /**
     * Handles APR initialization and termination.
     */
    class DECAF_API DecafRuntime : public decaf::lang::Runtime {
    private:

       RuntimeData* runtimeData;

    private:

       DecafRuntime( const DecafRuntime& );
       DecafRuntime& operator= ( const DecafRuntime& );

    public:

        /**
         * Initializes the APR Runtime for a library.
         */
        DecafRuntime();

        /**
         * Terminates the APR Runtime for a library.
         */
        virtual ~DecafRuntime();

        /**
         * Grants access to the Global APR Pool instance that should be
         * used when creating new threads.
         */
        apr_pool_t* getGlobalPool() const;

        /**
         * Gets a pointer to the Decaf Runtime's Global Lock object, this can be used by
         * Decaf APIs to synchronize around certain actions such as adding or acquiring
         * a resource that must be Thread safe.
         *
         * The pointer returned is owned by the Decaf runtime and should not be
         * deleted or copied by the caller.
         *
         * @returns a pointer to the Decaf Runtime's global Lock instance.
         */
        decaf::util::concurrent::Mutex* getGlobalLock();

    };

}}

#endif /*_DECAF_INTERNAL_DECAFRUNTIME_H*/
