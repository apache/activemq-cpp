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

#ifndef _ACTIVEMQ_THREADS_TASK_H_
#define _ACTIVEMQ_THREADS_TASK_H_

#include <activemq/util/Config.h>

namespace activemq {
namespace threads {

    /**
     * Represents a unit of work that requires one or more iterations to complete.
     *
     * @since 3.0
     */
    class AMQCPP_API Task {
    public:

        virtual ~Task() {}

        /**
         * Perform one iteration of work, returns true if the task needs
         * to run again to complete or false to indicate that the task is now
         * complete.
         *
         * @return true if the task should be run again or false if the task
         *         has completed and the runner should wait for a wakeup call.
         */
        virtual bool iterate() = 0;

    };

}}

#endif /* _ACTIVEMQ_THREADS_TASK_H_ */
