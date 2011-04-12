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

#ifndef _ACTIVEMQ_THREADS_SCHEDULERTIMERTASK_H_
#define _ACTIVEMQ_THREADS_SCHEDULERTIMERTASK_H_

#include <activemq/util/Config.h>

#include <decaf/util/TimerTask.h>
#include <decaf/lang/Runnable.h>

namespace activemq {
namespace threads {

    /**
     * Extension of the Decaf TimerTask that adds a Runnable instance which is
     * the target of this task.
     *
     * @since 3.3.0
     */
    class AMQCPP_API SchedulerTimerTask : public decaf::util::TimerTask {
    private:

        decaf::lang::Runnable* task;
        bool ownsTask;

    private:

        SchedulerTimerTask(const SchedulerTimerTask&);
        SchedulerTimerTask& operator= (const SchedulerTimerTask&);

    public:

        SchedulerTimerTask(decaf::lang::Runnable* task, bool ownsTask = true);

        virtual ~SchedulerTimerTask();

        virtual void run();
    };

}}

#endif /* _ACTIVEMQ_THREADS_SCHEDULERTIMERTASK_H_ */
