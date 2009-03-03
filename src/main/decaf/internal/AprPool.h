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

#ifndef _DECAF_INTERNAL_APRPOOL_H_
#define _DECAF_INTERNAL_APRPOOL_H_

#include <decaf/util/Config.h>

#include <apr_pools.h>

namespace decaf{
namespace internal{

    /**
     * Wraps an APR pool object so that classes in decaf can create a static
     * member for use in static methods where apr function calls that need a pool
     * are made.
     */
    class DECAF_API AprPool {
    private:

        /**
         * Internal APR pool
         */
        mutable apr_pool_t* aprPool;

    private:

        AprPool( const AprPool& pool );
        AprPool& operator= ( const AprPool& pool );

    public:

        AprPool();
        virtual ~AprPool();

        /**
         * Gets the internal APR Pool.
         * @returns the internal APR pool
         */
        apr_pool_t* getAprPool() const;

        /**
         * Clears data that was allocated by this pool.  Users should call this
         * after getting the data from the APR functions and copying it to
         * someplace safe.
         */
        void cleanup();

        /**
         * Gets a pointer to the Global APR Pool used for the Application
         * @return pointer to the global APR Pool
         */
        static apr_pool_t* getGlobalPool();

    private:

        /**
         * Allocates the pool if it isn't already allocated.
         */
        void allocatePool() const;

        /**
         * Destroys the pool if it has been allocated.
         */
        void destroyPool();

    };

}}

#endif /*_DECAF_INTERNAL_APRPOOL_H_*/
