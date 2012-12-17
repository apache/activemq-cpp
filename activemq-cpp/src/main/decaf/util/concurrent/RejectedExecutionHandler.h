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

#ifndef _DECAF_UTIL_CONCURRENT_REJECTEDEXECUTIONHANDLER_H_
#define _DECAF_UTIL_CONCURRENT_REJECTEDEXECUTIONHANDLER_H_

#include <decaf/util/Config.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/RejectedExecutionException.h>

namespace decaf {
namespace util {
namespace concurrent {

    class ThreadPoolExecutor;

    /**
     * A handler for tasks that cannot be executed by a {@link ThreadPoolExecutor}.
     *
     * @since 1.0
     */
    class DECAF_API RejectedExecutionHandler {
    public:

        RejectedExecutionHandler();
        virtual ~RejectedExecutionHandler();

        /**
         * Method that may be invoked by a {@link ThreadPoolExecutor} when
         * {@link ThreadPoolExecutor#execute execute} cannot accept a
         * task.  This may occur when no more threads or queue slots are
         * available because their bounds would be exceeded, or upon
         * shutdown of the Executor.
         *
         * <p>In the absence of other alternatives, the method may throw
         * an {@link RejectedExecutionException}, which will be propagated to
         * the caller of {@link ThreadPoolExecutor#execute execute}.
         *
         * @param r
         *      The pointer to the runnable task requested to be executed.
         * @param executor
         *      The pointer to the executor attempting to execute this task.
         *
         * @throws RejectedExecutionException if there is no remedy.
         */
        virtual void rejectedExecution( decaf::lang::Runnable* r, ThreadPoolExecutor* executer ) = 0;

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_REJECTEDEXECUTIONHANDLER_H_ */
