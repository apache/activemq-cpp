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

#ifndef _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICE_H_
#define _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICE_H_

#include <decaf/util/Config.h>

#include <decaf/util/concurrent/Executor.h>
#include <decaf/util/concurrent/ExecutorService.h>

namespace decaf {
namespace util {
namespace concurrent {

    /**
     * Provides a default implementation for the methods of the ExecutorService
     * interface.  Use this class as a starting point for implementations of custom
     * executor service implementations.
     *
     * @since 1.0
     */
    class DECAF_API AbstractExecutorService : public ExecutorService {
    public:

        AbstractExecutorService();
        virtual ~AbstractExecutorService();

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_ABSTRACTEXECUTORSERVICE_H_ */
