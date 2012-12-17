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

#ifndef _ACTIVEMQ_THREADS_SCHEDULER_H_
#define _ACTIVEMQ_THREADS_SCHEDULER_H_

#include <activemq/util/Config.h>
#include <activemq/util/ServiceSupport.h>

#include <decaf/lang/Runnable.h>
#include <decaf/util/Timer.h>
#include <decaf/util/StlMap.h>
#include <decaf/util/concurrent/Mutex.h>

#include <string>

namespace activemq {
namespace threads {

    /**
     * Scheduler class for use in executing Runnable Tasks either periodically or
     * one time only with optional delay.
     *
     * @since 3.3.0
     */
    class AMQCPP_API Scheduler : public activemq::util::ServiceSupport {
    private:

        decaf::util::concurrent::Mutex mutex;
        std::string name;
        decaf::util::Timer* timer;
        decaf::util::StlMap<decaf::lang::Runnable*, decaf::util::TimerTask*> tasks;

    private:

        Scheduler(const Scheduler&);
        Scheduler& operator= (const Scheduler&);

    public:

        Scheduler(const std::string& name);

        virtual ~Scheduler();

    public:

        void executePeriodically(decaf::lang::Runnable* task, long long period, bool ownsTask = true);

        void schedualPeriodically(decaf::lang::Runnable* task, long long period, bool ownsTask = true);

        void cancel(decaf::lang::Runnable* task);

        void executeAfterDelay(decaf::lang::Runnable* task, long long delay, bool ownsTask = true);

        void shutdown();

    protected:

        virtual void doStart();

        virtual void doStop(activemq::util::ServiceStopper* stopper);

    };

}}

#endif /* _ACTIVEMQ_THREADS_SCHEDULER_H_ */
