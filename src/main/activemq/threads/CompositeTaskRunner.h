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

#ifndef _ACTIVEMQ_THREADS_COMPOSITETASKRUNNER_H_
#define _ACTIVEMQ_THREADS_COMPOSITETASKRUNNER_H_

#include <activemq/util/Config.h>
#include <activemq/threads/TaskRunner.h>
#include <activemq/threads/CompositeTask.h>
#include <decaf/util/StlSet.h>
#include <decaf/util/LinkedList.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Pointer.h>

namespace activemq {
namespace threads {

    using decaf::lang::Pointer;

    /**
     * A Task Runner that can contain one or more CompositeTasks that are each checked
     * for pending work and run if any is present in the order that the tasks were added.
     *
     * @since 3.0
     */
    class AMQCPP_API CompositeTaskRunner : public activemq::threads::TaskRunner,
                                           public activemq::threads::Task,
                                           public decaf::lang::Runnable {
    private:

        decaf::util::LinkedList<CompositeTask*> tasks;
        decaf::util::concurrent::Mutex mutex;

        Pointer<decaf::lang::Thread> thread;

        bool threadTerminated;
        bool pending;
        bool shutDown;

    private:

        CompositeTaskRunner( const CompositeTaskRunner& );
        CompositeTaskRunner& operator= ( const CompositeTaskRunner& );

    public:

        CompositeTaskRunner();

        virtual ~CompositeTaskRunner();

        /**
         * Adds a new CompositeTask to the Set of Tasks that this class manages.
         * @param task - Pointer to a CompositeTask instance.
         */
        void addTask( CompositeTask* task );

        /**
         * Removes a CompositeTask that was added previously
         * @param task - Pointer to a CompositeTask instance.
         */
        void removeTask( CompositeTask* task );

        /**
         * Shutdown after a timeout, does not guarantee that the task's iterate
         * method has completed and the thread halted.
         *
         * @param timeout - Time in Milliseconds to wait for the task to stop.
         */
        virtual void shutdown( unsigned int timeout );

        /**
         * Shutdown once the task has finished and the TaskRunner's thread has exited.
         */
        virtual void shutdown();

        /**
         * Signal the TaskRunner to wakeup and execute another iteration cycle on
         * the task, the Task instance will be run until its iterate method has
         * returned false indicating it is done.
         */
        virtual void wakeup();

    protected:

        virtual void run();

        virtual bool iterate();

    };

}}

#endif /* _ACTIVEMQ_THREADS_COMPOSITETASKRUNNER_H_ */
