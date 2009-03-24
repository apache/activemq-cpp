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

#ifndef _ACTIVEMQ_THREADS_COMPOSITETASK_H_
#define _ACTIVEMQ_THREADS_COMPOSITETASK_H_

#include <activemq/util/Config.h>
#include <activemq/threads/Task.h>

namespace activemq {
namespace threads {

    /**
     * Represents a single task that can be part of a set of Tasks that are contained
     * in a <code>CompositeTaskRunner</code>.
     *
     * @since 3.0
     */
    class AMQCPP_API CompositeTask : public activemq::threads::Task {
    public:

        virtual ~CompositeTask() {}

        /**
         * Indicates whether this task has any pending work that needs to be
         * done, if not then it is skipped and the next Task in the
         * CompositeTaskRunner's list of tasks is checked, if none of the tasks
         * have any pending work to do, then the runner can go to sleep until it
         * awakened by a call to <code>wakeup</code>.
         *
         * @since 3.0
         */
        virtual bool isPending() const = 0;

    };

}}

#endif /* _ACTIVEMQ_THREADS_COMPOSITETASK_H_ */
