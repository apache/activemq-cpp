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

#ifndef _DECAF_INTERNAL_UTIL_RESOURCE_H_
#define _DECAF_INTERNAL_UTIL_RESOURCE_H_

#include <decaf/util/Config.h>

namespace decaf {
namespace internal {
namespace util {

    /**
     * Interface for all Managed Resources in Decaf, these objects are added to the
     * Runtime System and are destroyed at shutdown.
     *
     * @since 1.0
     */
    class DECAF_API Resource {
    public:

        virtual ~Resource();

    };

}}}

#endif /* _DECAF_INTERNAL_UTIL_RESOURCE_H_ */
