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

#ifndef _DECAF_UTIL_UUID_H_
#define _DECAF_UTIL_UUID_H_

#include <decaf/util/Config.h>
#include <decaf/util/Comparable.h>
#include <apr_pools.h>
#include <apr_uuid.h>

namespace decaf{
namespace util{

    class UUID : public Comparable<UUID> {
    private:

        // APR Pool to allocate from
        apr_pool_t* pool;

        // APR Uuid Type
        apr_uuid_t* uuid;

    public:

        /**
         * Default Constructor
         */
        UUID();

        virtual ~UUID();

        /**
         * Compare the given UUID to this one
         * @param value - the UUID to compare to
         */
        virtual int compareTo( const UUID& value );

    };

}}

#endif /*_DECAF_UTIL_UUID_H_*/
